/*
 * File:   bootmain.c
 * Author: Sirapob-ASUSTUF
 *
 * Created on April 6, 2025, 8:39 PM
 */

#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <avr/wdt.h>

#define BAUD 9600
#define UBRR_VALUE 51
#define EEPROM_BOOT_COUNTER_ADDR 0x10
#define START_MARKER '<'
#define END_MARKER   '>'

#define FLASH_TASK_START  0x2800
#define FLASH_TASK_LIMIT  0x7000
#define SPM_PAGESIZE      128
#define EEPROM_ADDR_FLASH_PTR 0x00

__attribute__((section(".noinit")))
static uint16_t currentFlashAddress;

uint32_t read_flash_ptr_from_eeprom() {
    uint32_t addr = 0;
    addr |= eeprom_read_byte((uint8_t*)(EEPROM_ADDR_FLASH_PTR + 0));
    addr |= ((uint32_t)eeprom_read_byte((uint8_t*)(EEPROM_ADDR_FLASH_PTR + 1))) << 8;
    addr |= ((uint32_t)eeprom_read_byte((uint8_t*)(EEPROM_ADDR_FLASH_PTR + 2))) << 16;
    addr |= ((uint32_t)eeprom_read_byte((uint8_t*)(EEPROM_ADDR_FLASH_PTR + 3))) << 24;
    return addr;
}

void write_flash_ptr_to_eeprom(uint32_t addr) {
    eeprom_busy_wait();
    eeprom_update_byte((uint8_t*)(EEPROM_ADDR_FLASH_PTR + 0), addr & 0xFF);
    eeprom_update_byte((uint8_t*)(EEPROM_ADDR_FLASH_PTR + 1), (addr >> 8) & 0xFF);
    eeprom_update_byte((uint8_t*)(EEPROM_ADDR_FLASH_PTR + 2), (addr >> 16) & 0xFF);
    eeprom_update_byte((uint8_t*)(EEPROM_ADDR_FLASH_PTR + 3), (addr >> 24) & 0xFF);
}

void uart_init() {
    UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
    UBRR0L = (unsigned char)(UBRR_VALUE);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable RX and TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
    while (UCSR0A & (1 << RXC0)) (void)UDR0;
}

void uart_transmit(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void uart_transmit_string(const char *str) {
    char c;
    while ((c = pgm_read_byte(str++))) {
        uart_transmit(c);
    }
}

void uart_transmit_string_R(const char *str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

void uart_transmit_hex(uint8_t value) {
    char hex[3];
    hex[0] = "0123456789ABCDEF"[value >> 4];  // Extract high nibble
    hex[1] = "0123456789ABCDEF"[value & 0x0F]; // Extract low nibble
    hex[2] = '\0';  // Null terminator
    uart_transmit_string_R(hex);
}

unsigned char uart_receive() {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

unsigned char uart_receive_timeout(uint16_t timeout_ms) {
    uint16_t count = 0;
    while (!(UCSR0A & (1 << RXC0))) {
        _delay_ms(1);
        if (++count >= timeout_ms) return 0xFF;  // timeout marker
    }
    return UDR0;
}

void send_flash_address(uint32_t address) {
    uart_transmit_string("<ADDR:");
    for (int i = 3; i >= 0; i--) {
        uint8_t byte = (address >> (8 * i)) & 0xFF;
        uart_transmit("0123456789ABCDEF"[byte >> 4]);
        uart_transmit("0123456789ABCDEF"[byte & 0x0F]);
    }
    uart_transmit('>');
}

void jump_to_rtos() {
    uart_transmit_string("Jumping to RTOS...\n");
    void (*rtos_start)(void) = (void *)0x0000;
    rtos_start();
}

void write_flash_page(uint32_t pageAddr, uint8_t *data) {
    uint16_t i;
    uint16_t word;

    eeprom_busy_wait();
    boot_page_erase(pageAddr);
    boot_spm_busy_wait();

    for (i = 0; i < SPM_PAGESIZE; i += 2) {
        word = data[i] | (data[i+1] << 8);
        boot_page_fill(pageAddr + i, word);
    }

    boot_page_write(pageAddr);
    boot_spm_busy_wait();
    boot_rww_enable();
}

void process_task(uint8_t *data, uint16_t size) {
    uint32_t addr = currentFlashAddress;
    while (size > 0) {
        if ((addr + SPM_PAGESIZE) >= FLASH_TASK_LIMIT) break;
        
        static char buffer[SPM_PAGESIZE + 16];  // Give extra room for <TASK:>
        uint16_t chunk = (size > SPM_PAGESIZE) ? SPM_PAGESIZE : size;

        memset(buffer, 0xFF, SPM_PAGESIZE);
        memcpy(buffer, data, chunk);

        write_flash_page(addr, (uint8_t *)buffer);

        addr += SPM_PAGESIZE;
        data += chunk;
        size -= chunk;
    }

    send_flash_address(addr - SPM_PAGESIZE);
    currentFlashAddress = addr;
    write_flash_ptr_to_eeprom(currentFlashAddress);

}

void read_packet() {
    static char buffer[128];
    char tempBuffer[16] = {0};
    uint16_t index = 0;
    uint8_t b;

    uart_transmit_string(PSTR("<TASK> or <RUN>\n"));
    // uart_transmit_string(PSTR("Waiting for '<'...\n"));

    // Flush UART
    while (UCSR0A & (1 << RXC0)) (void)UDR0;

    // Wait for '<' with non-blocking LED blink
    uint16_t blinkTimer = 0;
    while (1) {
        b = uart_receive_timeout(10);
        if (b == START_MARKER) break;
        if (b == 0xFF) {
            blinkTimer += 10;
            if (blinkTimer >= 500) {
                PORTB ^= (1 << PB1);  // Blink
                blinkTimer = 0;
            }
            continue;
        }
    }
    PORTB &= ~(1 << PB1); // LED OFF

    // Read until '>'
    index = 0;
    while (index < sizeof(buffer) - 1) {
        b = uart_receive_timeout(200);
        if (b == 0xFF) continue;
        if (b == END_MARKER) break;
        buffer[index++] = b;

        // Optional debug
        // uart_transmit('['); uart_transmit(b); uart_transmit(']');
    }
    buffer[index] = '\0';

    // Strip trailing \r or \n
    while (index > 0 && (buffer[index - 1] == '\r' || buffer[index - 1] == '\n')) {
        buffer[--index] = '\0';
    }

    // Copy to tempBuffer safely
    uint8_t copyLen = (index < sizeof(tempBuffer) - 1) ? index : sizeof(tempBuffer) - 1;
    for (uint8_t i = 0; i < copyLen; ++i) tempBuffer[i] = buffer[i];
    tempBuffer[copyLen] = '\0';

    // Debug: ASCII summary
    /*
    uart_transmit_string(PSTR("CMD: "));
    uart_transmit_string_R(tempBuffer[0] ? tempBuffer : "(empty)");
    uart_transmit('\n');

    uart_transmit_string(PSTR("BYTES: "));
    for (uint8_t i = 0; i < copyLen; ++i) {
        uart_transmit('[');
        uart_transmit("0123456789ABCDEF"[(uint8_t)tempBuffer[i] >> 4]);
        uart_transmit("0123456789ABCDEF"[tempBuffer[i] & 0x0F]);
        uart_transmit(']');
    }
    uart_transmit('\n');
     */

    // === Actual command handling ===
    if (copyLen == 3 && tempBuffer[0] == 'R' && tempBuffer[1] == 'U' && tempBuffer[2] == 'N') {
        uart_transmit_string(PSTR("Jumping to RTOS...\n"));
        jump_to_rtos();
    } else if (copyLen > 5 && strncmp(tempBuffer, "TASK:", 5) == 0) {
        uint16_t dataSize = index - 5;
        uart_transmit_string(PSTR("Storing task...\n"));
        process_task((uint8_t *)&buffer[5], dataSize);
    } else {
        uart_transmit_string(PSTR("Unknown command\n"));
    }

    // Final cleanup
    while (UCSR0A & (1 << RXC0)) (void)UDR0;
}

// Dummy read_packet
void tempread_packet() {
    uart_transmit_string("<WAITING COMMAND>\n");
    while (1);
}

void clear_bss() {
    extern uint8_t __bss_start;
    extern uint8_t __bss_end;
    for (uint8_t* ptr = &__bss_start; ptr < &__bss_end; ++ptr) {
        *ptr = 0;
    }
}

__attribute__((naked, section(".init0"), used))
void _start(void) {
    cli();             // disable interrupts
    MCUSR = 0;         // clear any reset flags
    wdt_reset();       // clear watchdog
    wdt_disable();     // stop watchdog

    // Set up stack pointer manually
    asm volatile (
        "ldi r28, lo8(0x8FF)\n\t"
        "ldi r29, hi8(0x8FF)\n\t"
        "out __SP_L__, r28\n\t"
        "out __SP_H__, r29\n\t"
    );

    DDRD |= (1 << PD1);  // TXD as output
    PORTD |= (1 << PD1); // Set high to idle state
    
    uart_init();
    uart_transmit_string("_start reached\n");

    for (volatile uint32_t i = 0; i < 50000UL; ++i);

    asm volatile("jmp main");
}

int main(void) {

    uint16_t sp;
    asm volatile("in %A0, __SP_L__ \n\t"
                 "in %B0, __SP_H__"
                 : "=r" (sp));
    uart_transmit_string("SP: ");
    uart_transmit_hex(sp >> 8);
    uart_transmit_hex(sp & 0xFF);
    uart_transmit('\n');

    clear_bss(); // Test 4: Manually zero .bss

    // Test 1: Increment cold boot counter
    uint8_t boot_count = eeprom_read_byte((uint8_t*)EEPROM_BOOT_COUNTER_ADDR);
    eeprom_update_byte((uint8_t*)EEPROM_BOOT_COUNTER_ADDR, boot_count + 1);
    
    /*currentFlashAddress = read_flash_ptr_from_eeprom();
    if (currentFlashAddress < FLASH_TASK_START || currentFlashAddress >= FLASH_TASK_LIMIT) {
        currentFlashAddress = FLASH_TASK_START;
    }*/
    
    uart_transmit_string("Boot count: ");
    uart_transmit_hex(boot_count);
    uart_transmit('\n');
    
    DDRB |= (1 << PB1);// Arduino UNO LED pin
    for (int i = 0; i < 5; i++) {
        PORTB ^= (1 << PB1);
        _delay_ms(200);
    }
    
    uart_transmit_string(PSTR("Bootloader Ready\n"));
    
    while (1) {
        tempread_packet(); 
    }
}
