/*
 * File:   bootmain.c
 * Author: Sirapob-ASUSTUF
 *
 * Created on April 6, 2025, 8:39 PM
 */

#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

#define F_CPU 8000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

#define START_ADDR 0x2800  // Start writing tasks after RTOS (e.g., 10 KB)
#define PAGE_SIZE 128      // Page size in bytes (typical for ATmega328P)

void uart_init() {
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

uint8_t uart_receive() {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void jump_to_rtos() {
    // Reset watchdog and jump to 0x0000
    wdt_disable();
    void (*rtos_start)(void) = 0x0000;
    rtos_start();
}

void bootloader_main() {
    uart_init();
    uart_transmit('B');  // Signal bootloader start

    uint32_t flash_addr = START_ADDR;
    uint8_t page_buffer[PAGE_SIZE];
    uint8_t bytes_received = 0;

    while (1) {
        uint8_t data = uart_receive();
        if (data == 0xFF) break;  // End signal from sender

        page_buffer[bytes_received++] = data;

        if (bytes_received >= PAGE_SIZE) {
            // Wait for EEPROM to finish
            boot_spm_busy_wait();

            // Erase and write page
            uint16_t i;
            for (i = 0; i < PAGE_SIZE; i += 2) {
                uint16_t w = page_buffer[i] | (page_buffer[i + 1] << 8);
                boot_page_fill(flash_addr + i, w);
            }

            boot_page_erase(flash_addr);
            boot_spm_busy_wait();

            boot_page_write(flash_addr);
            boot_spm_busy_wait();

            boot_rww_enable();

            flash_addr += PAGE_SIZE;
            bytes_received = 0;
        }
    }

    jump_to_rtos();
}

int main(void) {
    bootloader_main();
    while (1); // just in case
}

