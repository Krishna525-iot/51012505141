#include "command_manager.h"
#include "uart.h" // Include UART driver for communication
#include <stdio.h>
#include "globals.h"
#include <string.h>


void CommandManager_Init(void) {
	// Initialize UART or other communication interface if needed
	UART_Init();
}

void send_command(const char *cmd) {
	// Check if the command is valid
	if (cmd == NULL || strlen(cmd) == 0) {
		return; // Invalid command, return early
	}

	// Send the command using UART
	UART_Send(cmd, strlen(cmd));
}
void send_intensity_command(uint8_t intensity) {
	char cmd[8];

	if (intensity == 10) {
		// Special case for intensity 10
		snprintf(cmd, sizeof(cmd), "@I0:#");
	} else {
		// Normal case for other intensity values
		snprintf(cmd, sizeof(cmd), "@I%02d#", intensity);
	}

	send_command(cmd);
}

void send_color_command(int8_t color) {
	char cmd[8];
//	snprintf(cmd, sizeof(cmd), "@C%d#", color);
	if (color == -5) {
		snprintf(cmd, sizeof(cmd), "@C-5#");
	} else if (color == 0) {
		snprintf(cmd, sizeof(cmd), "@C05#");
	} else if (color == +5) {
		snprintf(cmd, sizeof(cmd), "@C+5#");
	}
	send_command(cmd);
}

void send_lamp_command(uint8_t state) {
	char cmd[8];
	snprintf(cmd, sizeof(cmd), "@L_%d#", state); // 0 = OFF, 1 = ON
	send_command(cmd);
}

void send_endo_command(uint8_t state) {
	char cmd[8];
	snprintf(cmd, sizeof(cmd), "@E_%d#", state); // 0 = OFF, 1 = ON
	send_command(cmd);
}
//------------------------------------------------------------------------------
void send_cam_command(uint8_t state) {
	char cmd[8];
	snprintf(cmd, sizeof(cmd), "@M0%d#", state); // 0 = OFF, 1 = ON
	send_command(cmd);
}
//------------------------------------------------------------------------------
void send_depth_command(uint8_t state) {
	char cmd[8];
	snprintf(cmd, sizeof(cmd), "@D_%d#", state); // 0 = OFF, 1 = ON
	send_command(cmd);

}

// Function to send a focus command based on the selected option
void send_focus_command(uint8_t focus_option) {
	if (focus_state == 1){
		char cmd[8];
		snprintf(cmd, sizeof(cmd), "@F_1#"); // 0 = OFF, 1 = ON
		send_command(cmd);
		if (focus_option < 3) {
				char cmd[8]; // Command buffer
				snprintf(cmd, sizeof(cmd), "@F0%d#", focus_option + 1); // Format: @F01#, @F02#, @F03#
				send_command(cmd); // Send the command
			}
	}else if (focus_state == 0){
		char cmd[8];
		snprintf(cmd, sizeof(cmd), "@F_0#"); // 0 = OFF, 1 = ON
		send_command(cmd);
	}

}

