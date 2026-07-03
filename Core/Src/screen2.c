//#include "screen2.h"
//#include "screen1.h"
//#include "lcd.h"
//#include "globals.h"
//#include "input_manager.h"
//
//// Cursor positions based on Focus state
//#define FOCUS_INDEX 0
//#define CAMERA_INDEX 1
//#define SMALL_INDEX 1
//#define MEDIUM_INDEX 2
//#define WIDE_INDEX 3
//#define TOTAL_FOCUS_POSITIONS 4
//#define TOTAL_NON_FOCUS_POSITIONS 2
//
//static uint8_t current_position = 0; // Cursor position
//static uint8_t camera_state = 0; // Camera ON/OFF state
//
//// Initialize the second screen
//void init_second_screen(void) {
//    LCD_Clear();
//
//    lcd_puts(0, 1, (int8_t*) "FOCUS  :");
//    lcd_puts(0, 10, focus_state ? (int8_t*) "ON " : (int8_t*) "OFF");
//
////    lcd_puts(1, 1, (int8_t*) "CAMERA :");
////    lcd_puts(1, 10, camera_state ? (int8_t*) "ON " : (int8_t*) "OFF");
//
//    if (focus_state) {
////        load_focus_options();
//    	load_initial_focus();
//    }
//
//    cursor_row = 0;
//    cursor_col = 0;
//    update_cursor();
//}
//
//// Handle input actions for the second screen
//void handle_second_screen_input(InputAction action) {
//	static uint8_t current_position = 0; // Index of the current cursor position
//	static const uint8_t cursor_positions[][2] = { { 0, 0 }, { 1, 0 }, { 2, 0 },
//			{ 3, 0 } }; // Cursor positions for FOCUS OFF and ON states
//	static const uint8_t num_positions = sizeof(cursor_positions)
//			/ sizeof(cursor_positions[0]);
//
//	switch (action) {
//	case INPUT_MOVE_DOWN:
//		if (focus_state == 0) {
//			// Transition back to the first screen
//			screen_state = 0;
//			LCD_Clear();
//			init_screen1(0);
//			update_screen1_with_current_values();
//			return;
//		} else {
//			// Move the cursor down within focus options
//			current_position = (current_position + 1) % num_positions;
//		}
//		break;
//
//	case INPUT_MOVE_UP:
//		if (focus_state == 0) {
//			// Transition back to the first screen
//			screen_state = 0;
//			LCD_Clear();
//			init_screen1(5);
//			update_screen1_with_current_values();
//			return;
//		} else {
//			// Move the cursor up within focus options
//			current_position =
//					(current_position == 0) ?
//							num_positions - 1 : current_position - 1;
//		}
//		break;
//
//	case INPUT_SELECT:
//		if (cursor_positions[current_position][0] == 0
//				&& cursor_positions[current_position][1] == 0) {
//			// Toggle FOCUS state
//			focus_state = 1;
//			lcd_puts(0, 10, focus_state ? (int8_t*) "ON " : (int8_t*) "OFF");
//
//			// Send command for FOCUS ON
//			if (focus_state) {
//				send_command("@F_1#");
//			}
//
//			load_focus_options(); // Load focus options
//			current_position = 0; // Reset cursor to the first option
//		} else if (focus_state) {
//			// Handle FOCUS option selection
//			toggle_focus_option(current_position - 1); // Adjust index for option positions
//		}
//		break;
//
//	case INPUT_BACK:
//		if (cursor_positions[current_position][0] == 0
//				&& cursor_positions[current_position][1] == 0) {
//			// Disable FOCUS mode and reset the screen
//			focus_state = 0;
//
//			// Send command for FOCUS OFF
//			send_command("@F_0#");
//
//			init_second_screen(); // Reset second screen
//			reset_focus_options();
//		} else if (focus_state) {
//			// Handle FOCUS option deselection
//			toggle_focus_option(current_position - 1); // Adjust index for option positions
//
//		}
//		break;
//
//	default:
//		return; // Ignore unhandled actions
//	}
//
//	// Update cursor position
//	cursor_row = cursor_positions[current_position][0];
//	cursor_col = cursor_positions[current_position][1];
//	update_cursor();
//}
//// Toggle focus state (ON/OFF)
//void toggle_focus_state(void) {
//    focus_state = !focus_state;
//    lcd_puts(0, 10, focus_state ? (int8_t*)"ON " : (int8_t*)"OFF");
//    send_command(focus_state ? "@F_1#" : "@F_0#");
//
//    if (focus_state) {
//        lcd_puts(1, 0, (int8_t*)"                ");
//        lcd_puts(2, 0, (int8_t*)"                ");
//        lcd_puts(3, 0, (int8_t*)"                ");
//
//        load_initial_focus();
//        current_position = SMALL_INDEX;
//        option_states[0] = 1;
//    } else {
//        current_position = FOCUS_INDEX;
//        init_second_screen();
//    }
//}
//
//
//// Toggle Camera ON/OFF
////void toggle_camera_state(void) {
////    camera_state = !camera_state;
////    lcd_puts(1, 10, camera_state ? (int8_t*) "ON " : (int8_t*) "OFF");
////    send_command(camera_state ? "@M01#" : "@M00#");
////}
//
//// Load focus options
////void load_focus_options(void) {
////    lcd_puts(1, 1, (int8_t*) "SMALL  :");
////    lcd_puts(2, 1, (int8_t*) "MEDIUM :");
////    lcd_puts(3, 1, (int8_t*) "WIDE   :");
////
////    lcd_puts(1, 10, option_states[0] ? (int8_t*) "ON " : (int8_t*) "OFF");
////    lcd_puts(2, 10, option_states[1] ? (int8_t*) "ON " : (int8_t*) "OFF");
////    lcd_puts(3, 10, option_states[2] ? (int8_t*) "ON " : (int8_t*) "OFF");
////}
//
//
//void load_focus_options(void) {
//    const int8_t* labels[3] = { "SMALL  :", "MEDIUM :", "WIDE   :" };
//
////	load_focus_options();
//    for (uint8_t i = 0; i < 3; i++) {
//        lcd_puts(i + 1, 1, (int8_t*) labels[i]);
//        lcd_puts(i + 1, 10, option_states[i] ? (int8_t*)"ON " : (int8_t*)"OFF");
//
//    }
//}
//
//void load_initial_focus(void){
//    lcd_puts(1, 1, (int8_t*) "SMALL  :");
//    lcd_puts(2, 1, (int8_t*) "MEDIUM :");
//    lcd_puts(3, 1, (int8_t*) "WIDE   :");
//
//    lcd_puts(1, 10, (int8_t*) "ON");
//    lcd_puts(2, 10, (int8_t*) "OFF");
//    lcd_puts(3, 10, (int8_t*) "OFF");
//}
//
//// Set focus options
//void set_small_focus(void) {
//    reset_focus_options();
//    option_states[0] = 1;
//    send_focus_command(0);
//    load_initial_focus();
//}
//
//void set_medium_focus(void) {
//    reset_focus_options();
//    option_states[1] = 1;
//    send_focus_command(1);
//    load_focus_options();
//}
//
//void set_wide_focus(void) {
//    reset_focus_options();
//    option_states[2] = 1;
//    send_focus_command(2);
//    load_focus_options();
//}
//
//// Reset focus options
//void reset_focus_options(void) {
//    for (uint8_t i = 0; i < 3; i++) {
//        option_states[i] = 0;
//    }
//}
//
//void toggle_focus_option(uint8_t option_index) {
//    if (option_index < 3) {
//        for (uint8_t i = 0; i < 3; i++) {
//            option_states[i] = (i == option_index); // Enable only selected option
//        }
//
//        send_focus_command(option_index);
//        load_focus_options();
//    }
//}




#include "screen2.h"
#include "screen1.h"
#include "lcd.h"
#include "globals.h"
#include "input_manager.h"

// Cursor positions based on Focus state
#define FOCUS_INDEX 0
#define SMALL_INDEX 1
#define MEDIUM_INDEX 2
#define WIDE_INDEX 3
#define TOTAL_FOCUS_POSITIONS 4
#define TOTAL_NON_FOCUS_POSITIONS 2

static uint8_t current_position = 0; // Cursor position
static uint8_t camera_state = 0; // Camera ON/OFF state

// Initialize the second screen
void init_second_screen(void) {
    LCD_Clear();

    lcd_puts(0, 1, (int8_t*) "FOCUS  :");
    lcd_puts(0, 10, focus_state ? (int8_t*) "ON " : (int8_t*) "OFF");

    if (focus_state) {
        load_focus_options(); // Properly show current states
    }

    cursor_row = 0;
    cursor_col = 0;
    update_cursor();
}

// Handle input actions for the second screen
void handle_second_screen_input(InputAction action) {
    static const uint8_t cursor_positions[][2] = {
        { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }
    };
    static const uint8_t num_positions = sizeof(cursor_positions) / sizeof(cursor_positions[0]);

    switch (action) {
        case INPUT_MOVE_DOWN:
            if (!focus_state) {
                screen_state = 0;
                LCD_Clear();
                init_screen1(0);
                update_screen1_with_current_values();
                return;
            } else {
                current_position = (current_position + 1) % num_positions;
            }
            break;

        case INPUT_MOVE_UP:
            if (!focus_state) {
                screen_state = 0;
                LCD_Clear();
                init_screen1(5);
                update_screen1_with_current_values();
                return;
            } else {
                current_position = (current_position == 0) ? (num_positions - 1) : (current_position - 1);
            }
            break;

        case INPUT_SELECT:
            if (cursor_positions[current_position][0] == 0 &&
                cursor_positions[current_position][1] == 0) {
                // Toggle FOCUS ON
                focus_state = 1;
                lcd_puts(0, 10, (int8_t*)"ON ");
                send_command("@F_1#");

                reset_focus_options();
                option_states[0] = 1; // SMALL ON
                load_focus_options();

                current_position = SMALL_INDEX;
            } else if (focus_state) {
                toggle_focus_option(current_position - 1);
            }
            break;

        case INPUT_BACK:
            if (cursor_positions[current_position][0] == 0 &&
                cursor_positions[current_position][1] == 0) {
                // Turn off FOCUS
                focus_state = 0;
                send_command("@F_0#");
                reset_focus_options();
                init_second_screen();
            } else if (focus_state) {
                toggle_focus_option(current_position - 1);
            }
            break;

        default:
            return;
    }

    cursor_row = cursor_positions[current_position][0];
    cursor_col = cursor_positions[current_position][1];
    update_cursor();
}

// Toggle FOCUS ON/OFF (e.g., from external trigger)
void toggle_focus_state(void) {
    focus_state = !focus_state;
    lcd_puts(0, 10, focus_state ? (int8_t*)"ON " : (int8_t*)"OFF");
    send_command(focus_state ? "@F_1#" : "@F_0#");

    if (focus_state) {
        lcd_puts(1, 0, (int8_t*)"                ");
        lcd_puts(2, 0, (int8_t*)"                ");
        lcd_puts(3, 0, (int8_t*)"                ");

        reset_focus_options();
        option_states[0] = 1;
        load_focus_options();
        current_position = SMALL_INDEX;
    } else {
        current_position = FOCUS_INDEX;
        init_second_screen();
    }
}

// Load focus options from current option_states
void load_focus_options(void) {
    const int8_t* labels[3] = { "SMALL  :", "MEDIUM :", "WIDE   :" };
    for (uint8_t i = 0; i < 3; i++) {
        lcd_puts(i + 1, 1, (int8_t*) labels[i]);
        lcd_puts(i + 1, 10, option_states[i] ? (int8_t*)"ON " : (int8_t*)"OFF");
    }
}

// Reset all focus options to OFF
void reset_focus_options(void) {
    for (uint8_t i = 0; i < 3; i++) {
        option_states[i] = 0;
    }
}

// Toggle focus selection (only one can be ON)
void toggle_focus_option(uint8_t option_index) {
    if (option_index < 3) {
        for (uint8_t i = 0; i < 3; i++) {
            option_states[i] = (i == option_index);
        }
        send_focus_command(option_index);
        load_focus_options();
    }
}

// (Optional legacy fallback)
//void set_small_focus(void) { ... }
//void set_medium_focus(void) { ... }
//void set_wide_focus(void) { ... }
