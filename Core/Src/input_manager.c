/*
 * This file handles user inputs and delegates the processing to the active screen handler.
 */

#include "input_manager.h"
#include <stddef.h> // For NULL
#include <stdio.h>
#include "screen2.h"
#include "lcd.h"
#include "screen1.h"
#include "main.h"

#define MAX_SCREENS 2

uint8_t current_screen = 0;

// Initialize the input queue
static InputQueue inputQueue = { .head = 0, .tail = 0 };

void update_cursor(void) {
	lcd_puts(prev_cursor_row, prev_cursor_col, (int8_t*) " "); // Clear old position
	lcd_puts(cursor_row, cursor_col, (int8_t*) ">");        // Draw new position
	prev_cursor_row = cursor_row;
	prev_cursor_col = cursor_col;
}

uint8_t que_status =0;
// Add an event to the queue
void enqueue_event(InputEvent event) {
	uint8_t next = (inputQueue.head + 1) % INPUT_QUEUE_SIZE;
	if (next != inputQueue.tail) {
		inputQueue.events[inputQueue.head] = event;
		inputQueue.head = next;
		que_status++;

	} else {
		// Log dropped event
//        printf("Queue full, event dropped: Source=%d Action=%d\n", event.source, event.action);

	}
}


// Remove an event from the queue
int dequeue_event(InputEvent *event) {
	if (inputQueue.head == inputQueue.tail) {
		return 0; // Queue is empty
		que_status = 0;
	}
	*event = inputQueue.events[inputQueue.tail];
	inputQueue.tail = (inputQueue.tail + 1) % INPUT_QUEUE_SIZE;
	que_status--;
	return 1;
}

// Process all pending events in the input queue
void process_input_events(void) {
    InputEvent event;
    while (dequeue_event(&event)) {
		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, SET);
		HAL_Delay(20);
		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, RESET);
        handle_input(event.action);
    }
}
//#define MAX_EVENTS_PER_LOOP 5
//void process_input_events(void) {
//	InputEvent event;
//	uint8_t events_processed = 0;
//	while (dequeue_event(&event) && events_processed < MAX_EVENTS_PER_LOOP) {
//		handle_input(event.action);
//		events_processed++;
//	}
//}

// Update the cursor on the screen

void handle_input(InputAction action) {
	if (screen_state == 0) {
		handle_first_screen_input(action); // Delegate to first screen handler
	} else if (screen_state == 1) {
		handle_second_screen_input(action); // Delegate to second screen handler
	}
}

char debug_msg[50];
void debugQueueState() {

	snprintf(debug_msg, sizeof(debug_msg), "Queue State: Head=%d, Tail=%d\n",
			inputQueue.head, inputQueue.tail);
//    debugPrintln(&huart2, debug_msg);
}

void display_uptime_and_return(uint32_t elapsed_time_ms) {
	// Calculate total time in hours, minutes, and seconds
	uint32_t total_seconds = elapsed_time_ms / 1000;
	uint32_t hours = total_seconds / 3600;
	uint32_t minutes = (total_seconds % 3600) / 60;
	uint32_t seconds = total_seconds % 60;

	// Format the uptime string
	char time_buffer[16];
//    snprintf(time_buffer, sizeof(time_buffer), "%02u:%02u:%02u", hours, minutes, seconds);
	snprintf(time_buffer, sizeof(time_buffer), "%02u:%02u:%02u",
			(unsigned int) hours, (unsigned int) minutes,
			(unsigned int) seconds);

	// Clear the screen and display the uptime in the center
	LCD_Clear();
	lcd_puts(1, 5, (int8_t*) "System Uptime:");
	lcd_puts(2, 7, (int8_t*) time_buffer);

	// Hold the display for 2 seconds
	HAL_Delay(2000);

	// Restore the previous screen
	if (screen_state == 0) {
		init_screen1(0);
		update_screen1_with_current_values();
	} else if (screen_state == 1) {
		init_second_screen();
		load_focus_options();
	}
}
