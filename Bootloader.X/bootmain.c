__attribute__((naked, section(".init0"), used))
void _start(void) {
    asm volatile ("jmp main");
}

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

#define BAUD 9600
#define UBRR_VALUE 51

#define START_MARKER '<'
#define END_MARKER   '>'

#define FLASH_TASK_START  0x2800
#define FLASH_TASK_LIMIT  0x7000
#define SPM_PAGESIZE      128
 #define PACKET_TIMEOUT_MS 1000

uint32_t currentFlashAddress;

void uart_init() {
    UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
    UBRR0L = (unsigned char)(UBRR_VALUE);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable RX and TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
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

uint8_t uart_receive() {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

uint8_t uart_receive_timeout(uint16_t timeout_ms) {
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
        uint8_t buffer[SPM_PAGESIZE];
        uint16_t chunk = (size > SPM_PAGESIZE) ? SPM_PAGESIZE : size;

        memset(buffer, 0xFF, SPM_PAGESIZE);
        memcpy(buffer, data, chunk);

        write_flash_page(addr, buffer);

        addr += SPM_PAGESIZE;
        data += chunk;
        size -= chunk;
    }

    send_flash_address(currentFlashAddress);
    currentFlashAddress = addr;
}

void blink_light(void){
    PORTB ^= (1 << PB1);
    _delay_ms(200);
}

void read_packet() {
    static char buffer[128];
    char tempBuffer[16] = {0};
    uint16_t index = 0;
    uint8_t b;

    uart_transmit_string(PSTR("<TASK> or <RUN>\n"));
    while (UCSR0A & (1 << RXC0)) (void)UDR0;

    // ? First: wait for '<'
    uart_transmit_string(PSTR("Waiting for '<'...\n"));
    while (1) {
        b = uart_receive_timeout(10);
        if (b == 0xFF) continue;
        if (b == START_MARKER) break;
        blink_light();
    }

    // ? THEN this extra read loop was accidentally included
    // Read AFTER '<' but before we start processing again ? this caused issues!
    index = 0;
    while (index < sizeof(buffer) - 1) {
        b = uart_receive(); // blocking
        if (b == END_MARKER) break;

        buffer[index++] = b;
        uart_transmit('[');
        uart_transmit(b);
        uart_transmit(']');
    }

    // ? BUT ? second read loop was still there too!
    index = 0;
    while (index < sizeof(buffer) - 1) {
        b = uart_receive_timeout(200);
        if (b == 0xFF || b == END_MARKER) break;

        buffer[index++] = b;
        uart_transmit('[');
        uart_transmit(b);
        uart_transmit(']');
    }

    PORTB &= ~(1 << PB1); // LED off
    buffer[index] = '\0';

    while (index > 0 && (buffer[index - 1] == '\r' || buffer[index - 1] == '\n'))
        buffer[--index] = '\0';

    uint8_t copyLen = (index < sizeof(tempBuffer) - 1) ? index : sizeof(tempBuffer) - 1;
    for (uint8_t i = 0; i < copyLen; ++i)
        tempBuffer[i] = buffer[i];
    tempBuffer[copyLen] = '\0';

    uart_transmit_string(PSTR("CMD: "));
    if (tempBuffer[0]) uart_transmit_string_R(tempBuffer);
    else uart_transmit_string(PSTR("(empty)"));
    uart_transmit('\n');

    uart_transmit_string(PSTR("HEX: "));
    for (uint8_t i = 0; i < copyLen; ++i) {
        uart_transmit('[');
        uart_transmit("0123456789ABCDEF"[(tempBuffer[i] >> 4) & 0xF]);
        uart_transmit("0123456789ABCDEF"[tempBuffer[i] & 0xF]);
        uart_transmit(']');
    }
    uart_transmit('\n');

    if (strcmp(tempBuffer, "RUN") == 0) {
        uart_transmit_string(PSTR("Jumping to RTOS...\n"));
        jump_to_rtos();
    } else if (strncmp(tempBuffer, "TASK:", 5) == 0) {
        uint16_t dataSize = index - 5;
        if (dataSize > 0) {
            uart_transmit_string(PSTR("Storing task...\n"));
            process_task(&buffer[5], dataSize);
        } else {
            uart_transmit_string(PSTR("Empty TASK payload\n"));
        }
    } else {
        uart_transmit_string(PSTR("Unknown command\n"));
    }

    while (UCSR0A & (1 << RXC0)) (void)UDR0;
}

int main(void) {
    DDRB |= (1 << PB1);// Arduino UNO LED pin
    PORTB |= (1 << PB1);// Turn LED on

    uart_init();
    
    uart_transmit_string(PSTR("Bootloader Ready\n"));

    while (1) {
        read_packet(); // <-- This is now referenced!
    }
}
