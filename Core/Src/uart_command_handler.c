#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"
#include "command_manager.h"
#include "screen2.h"
#include "uart_command_handler.h"

// Function to handle UART commands and enqueue events
   char temp_buff[10];
int x=0;
   char intensity_str[4];
//   int intensity;
// Function to process incoming UART commands
void process_uart_command(const char *command) {
//	if (command == NULL || strlen(command) == 0) {
////        printf("Error: Empty or NULL command received.\n");
//		return;
//	}

    // Copy the command into the temporary buffer using strncpy.
    // We leave room for the null terminator.
    strncpy(temp_buff, command, sizeof(temp_buff) - 1);
//    printf("Processing command: %s\n", command);

// Parse and execute the command
    if (strncmp(command, "@I", 2) == 0) {  // Intensity command
       uint8_t intensity = atoi(&command[2]);    // Extract intensity value
      if (intensity == 0) {
        intensity_value = 10;  // Update global intensity value
        snprintf(intensity_str, sizeof(intensity_str), "%02d", 10);
        lcd_puts(1, 7, (int8_t*)intensity_str);  // Update LCD
        send_intensity_command(10);  // Send processed command
      } else if (intensity >= 0 && intensity <= 9) {

        intensity_value = intensity;  // Update global intensity value

        snprintf(intensity_str, sizeof(intensity_str), "%02d", intensity);
        lcd_puts(1, 7, (int8_t*)intensity_str);  // Update LCD
        send_intensity_command(intensity);       // Send processed command
      } else {
        printf("Error: Invalid intensity value.\n");
      }
	} else if (strncmp(command, "@C", 2) == 0) { // Color command
//		int color = atoi(&command[2]); // Extract color value
		int color = atoi(&command[2]); // Extract color value
		const char *color_modes[] = { "WW", "NW", "CW" };
		if (command[2] == '-' || command[2] == '0' || command[2] == '+'){
//		if (color == -5 || color == 05 || color == +5) {
//			color_state = color;
			if(command[2] == '-')
			{
				color_state = -5; // Update global color state
				lcd_puts(2, 7, (int8_t*) color_modes[0]); // Update LCD
				color =-5;
			}
			else if(command[2] == '0')
			{
				color_state = 0; // Update global color state
				lcd_puts(2, 7, (int8_t*) color_modes[1]); // Update LCD
				color =0;
			}
			else if(command[2] == '+')
			{
				color_state = +5;
				lcd_puts(2, 7, (int8_t*) color_modes[2]); // Update LCD
				color =+5;
			}
//			int index = (color + 5) / 5; // Map -5, 0, 5 to 0, 1, 2
//			lcd_puts(2, 7, (int8_t*) color_modes[index]); // Update LCD
			send_color_command(color); // Send processed command
		} else {
			printf("Error: Invalid color value.\n");
		}
	}
    //-------------------------------------------------------------

	else if (strncmp(command, "@M_", 3) == 0) { // Color command
			int cam = atoi(&command[2]); // Extract color value

			if (command[3] == '0' || command[3] == '1'  || command[3] == '2' || command[3] == '3'  ){

				if(command[3] == '0')
				{
					cam_state = 0;
					cam=0;
					init_second_screen();
					lcd_puts(1, 10, cam_state ? (int8_t*) "ON " : (int8_t*) "OFF"); // Update LCD
				}
				else if(command[3] == '1')
				{
					cam_state = 1;
					cam=1;
					init_second_screen();
					lcd_puts(1, 10, cam_state ? (int8_t*) "ON " : (int8_t*) "OFF"); // Update LCD
				}
				else if(command[3] == '2')
				{
					cam_state = 1;
					cam=2;
					init_second_screen();
					lcd_puts(1, 10, cam_state ? (int8_t*) "ON " : (int8_t*) "OFF"); // Update LCD
				}else if(command[3] == '3')
				{
					cam_state = 1;
					cam=3;
					init_second_screen();
					lcd_puts(1, 10, cam_state ? (int8_t*) "ON " : (int8_t*) "OFF"); // Update LCD
				}
				send_cam_command(cam); // Send processed command
			} else {
				printf("Error: Invalid cam value.\n");
			}
		}
	//------------------------------------------------------------
    else if (strncmp(command, "@L_", 3) == 0) { // Lamp command
		uint8_t state = (command[3] == '1') ? 1 : 0;
		lamp_state = state;
		lcd_puts(1, 17, lamp_state ? (int8_t*) "ON " : (int8_t*) "OFF"); // Update LCD
		send_lamp_command(state); // Send processed command
	} else if (strncmp(command, "@E_", 3) == 0) { // Endo command
		uint8_t state = (command[3] == '1') ? 1 : 0;
		endo_state = state;
		lcd_puts(2, 17, endo_state ? (int8_t*) "ON " : (int8_t*) "OFF"); // Update LCD
		send_endo_command(state); // Send processed command
	} else if (strncmp(command, "@D_", 3) == 0) { // Depth command
		uint8_t state = (command[3] == '1') ? 1 : 0;
		depth_state = state;
		lcd_puts(3, 17, depth_state ? (int8_t*) "ON " : (int8_t*) "OFF"); // Update LCD
		send_depth_command(state); // Send processed command
	} else if (strncmp(command, "@F_", 3) == 0) { // Focus ON/OFF command
		uint8_t state = (command[3] == '1') ? 1 : 0;
		focus_state = state;
		if (focus_state){
			init_second_screen();
			lcd_puts(0, 10, focus_state ? (int8_t*) "ON " : (int8_t*) "OFF"); // Update LCD
		}else {
			init_screen1();
//			screen_state=1;
		}
		send_focus_command(0); // Send processed command
	}
	else if (strncmp(command, "@F01", 4) == 0) { // Small Focus
			reset_focus_options();
			option_states[0] = command; // Enable SMALL focus
			send_focus_command(0); // Send processed command
			load_focus_options();
		}
		else if (strncmp(command,"@F02", 4) == 0) { // Medium Focus
			reset_focus_options();
			option_states[1] = command[3]; // Enable MEDIUM focus
			send_focus_command(1); // Send processed command
			load_focus_options();
		} else if (strncmp(command, "@F03", 4) == 0) { // Wide Focus
			reset_focus_options();
			option_states[2] = command[3]; // Enable WIDE focus
			send_focus_command(2); // Send processed command
			load_focus_options();
		}
		else {
			printf("Error: Unrecognized command: %s\n", command);
		}
	}
