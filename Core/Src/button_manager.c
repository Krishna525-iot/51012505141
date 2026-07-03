#include "button_manager.h"
#include "main.h"
#include "globals.h"
// Button pin definitions
static const uint16_t button_pins[] = {S_PRV_Pin, S_NEXT_Pin,  CHANGE_P_Pin,
		CHANGE_N_Pin, DEPTH_Pin };

// Unique button IDs (can be customized as needed)
static const uint8_t button_ids[] = { 0, 1, 2, 3, 4 };

// Button state tracking
ButtonState button_states[sizeof(button_pins) / sizeof(button_pins[0])];

// Debounce and long press thresholds
//#define DEBOUNCE_THRESHOLD 50    // 50 ms debounce time
//#define LONG_PRESS_THRESHOLD 1500 // 2 seconds for long press

// Callback for button events
static ButtonEventCallback event_callback = NULL;

// Initialization function
void ButtonManager_Init(ButtonEventCallback callback) {
	event_callback = callback;
}


uint32_t lastInterruptTime = 0;
uint32_t debounceDelay = 1000;
#define DEBOUNCE_THRESHOLD 	50
// Handle GPIO interrupt
//-------------------------------------

// Handle GPIO interrupt


//--------------------------------------------
void ButtonManager_HandleInterrupt(uint16_t GPIO_Pin) {

	uint32_t current_time = HAL_GetTick();
	// Find the button index for the given GPIO pin
	int button_index = -1;
	for (int i = 0; i < sizeof(button_pins) / sizeof(button_pins[0]); i++) {
		if (button_pins[i] == GPIO_Pin) {
			button_index = i;
			break;
		}
	}

	// If no matching button index found or callback is NULL, return
	if (button_index == -1 || event_callback == NULL) {
		return;
	}

	ButtonState *state = &button_states[button_index];
//	if (button_index == 3){
//		state->is_pressed = 0; // Mark as released
//	}
	// Debounce logic: Ignore events within DEBOUNCE_THRESHOLD
	if (current_time - state->last_interrupt_time < DEBOUNCE_THRESHOLD) {
		return;
	}
	state->last_interrupt_time = current_time;

	// Handle button press or release
	if (!state->is_pressed) {
		// Button pressed
		state->press_start_time = current_time; // Record press start time
		state->is_pressed = 1;                 // Mark as pressed
	} else {
		// Button released
		uint32_t press_duration = current_time - state->press_start_time;
		state->is_pressed = 0; // Mark as released

		// Generate a button event
		ButtonEvent event = { .button_id = button_ids[button_index], .action =
				(press_duration > LONG_PRESS_THRESHOLD) ?
						BUTTON_ACTION_LONG_PRESS : BUTTON_ACTION_SHORT_PRESS };

		// Call the callback function
		event_callback(&event);
	}
}


