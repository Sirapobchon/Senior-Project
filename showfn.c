/*
 * File:   showfn.c
 * Author: Sirapob-ASUSTUF
 *
 * Created on December 11, 2024, 10:48 PM
 */

#include <xc.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>

// Task Handles
TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL;

// Queue Handle
QueueHandle_t QueueHandle;

// Semaphore Handles
SemaphoreHandle_t BinarySemaphore;
SemaphoreHandle_t MutexSemaphore;

// Event Group Handle
EventGroupHandle_t EventGroup;

// Timer Handle
TimerHandle_t TimerHandle;

// Event Group Bits
#define EVENT_TASK1_BIT (1 << 0)
#define EVENT_TASK2_BIT (1 << 1)

// Timer Callback
void TimerCallback(TimerHandle_t xTimer) {
    printf("Timer Triggered!\n");

    // Set Event Group Bit
    xEventGroupSetBits(EventGroup, EVENT_TASK2_BIT);
}

// Task 1: Periodic Task
void Task1(void *pvParameters) {
    while (1) {
        // Wait for Mutex to access shared resource
        if (xSemaphoreTake(MutexSemaphore, portMAX_DELAY) == pdTRUE) {
            // Perform critical section
            printf("Task 1: Accessing shared resource...\n");

            // Release Mutex
            xSemaphoreGive(MutexSemaphore);
        }

        // Delay for 1000 ms
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Task 2: Event-Driven Task
void Task2(void *pvParameters) {
    while (1) {
        // Wait for Event Group Bit
        EventBits_t events = xEventGroupWaitBits(EventGroup, EVENT_TASK2_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
        if (events & EVENT_TASK2_BIT) {
            // Perform action for Timer Event
            printf("Task 2: Timer Event Triggered!\n");
        }
    }
}

// Interrupt Service Routine (ISR)
ISR(INT0_vect) {
    // Give Binary Semaphore
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken);

    // Context switch if necessary
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Task 3: Binary Semaphore Usage
void Task3(void *pvParameters) {
    while (1) {
        // Wait for Binary Semaphore signal
        if (xSemaphoreTake(BinarySemaphore, portMAX_DELAY) == pdTRUE) {
            printf("Task 3: Interrupt Signal Received!\n");
        }
    }
}

// Main Function
int maintemp(void) {
    // Initialize Peripherals (if any)
    printf("System Initialized...\n");

    // Create Queue
    QueueHandle = xQueueCreate(10, sizeof(int));
    if (QueueHandle == NULL) {
        printf("Failed to create Queue.\n");
        while (1);
    }

    // Create Binary Semaphore
    BinarySemaphore = xSemaphoreCreateBinary();
    if (BinarySemaphore == NULL) {
        printf("Failed to create Binary Semaphore.\n");
        while (1);
    }

    // Create Mutex Semaphore
    MutexSemaphore = xSemaphoreCreateMutex();
    if (MutexSemaphore == NULL) {
        printf("Failed to create Mutex Semaphore.\n");
        while (1);
    }

    // Create Event Group
    EventGroup = xEventGroupCreate();
    if (EventGroup == NULL) {
        printf("Failed to create Event Group.\n");
        while (1);
    }

    // Create Timer
    TimerHandle = xTimerCreate("Timer", pdMS_TO_TICKS(5000), pdTRUE, (void *)0, TimerCallback);
    if (TimerHandle == NULL) {
        printf("Failed to create Timer.\n");
        while (1);
    }

    // Start Timer
    xTimerStart(TimerHandle, 0);

    // Create Tasks
    xTaskCreate(Task1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, &Task1Handle);
    xTaskCreate(Task2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 2, &Task2Handle);
    xTaskCreate(Task3, "Task 3", configMINIMAL_STACK_SIZE, NULL, 3, NULL);

    // Enable Interrupts (Example for INT0)
    EIMSK |= (1 << INT0);  // Enable INT0
    EICRA |= (1 << ISC01); // Falling edge triggers INT0

    // Start Scheduler
    vTaskStartScheduler();

    // Should never reach here
    while (1);
}


