int main(void);

__attribute__((section(".vectors"), used, naked))
void _start(void) {
    __asm__ __volatile__("jmp main");
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

uint32_t currentFlashAddress = FLASH_TASK_START;

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
    while (*str) {
        uart_transmit(*str++);
    }
}

uint8_t uart_receive() {
    while (!(UCSR0A & (1 << RXC0)));
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

void read_packet() {
    uint8_t buffer[512];
    uint16_t index = 0;
    uint8_t b;

    while (1) {
        // Wait for '<'
        while ((b = uart_receive()) != START_MARKER);

        // Read command
        index = 0;
        while ((b = uart_receive()) != END_MARKER && index < sizeof(buffer) - 1) {
            buffer[index++] = b;
        }
        buffer[index] = '\0';

        // Check for RUN
        if (strncmp((char *)buffer, "RUN", 3) == 0) {
            jump_to_rtos();
        }

        // Check for TASK
        if (strncmp((char *)buffer, "TASK:", 5) == 0) {
            uint16_t dataSize = index - 5;
            if (dataSize > 0) {
                process_task(&buffer[5], dataSize);
            } else {
                uart_transmit_string("Empty TASK payload\n");
            }
        } else {
            uart_transmit_string("Unknown command\n");
        }
    }
}

int main(void) {
    DDRB |= (1 << PB1);// Arduino UNO LED pin
    PORTB |= (1 << PB1);// Turn LED on

    uart_init();
    
    _delay_ms(1000);
    
    uart_transmit_string("Bootloader starts\n");

    while (1) {
        uart_transmit_string("Hello World\n");
        _delay_ms(1000);
        read_packet(); // <-- This is now referenced!
    }
}
