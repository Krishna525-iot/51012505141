#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include <stdint.h>

// Initialize the command manager
void CommandManager_Init(void);

// Functions to send specific commands
void send_command(const char *cmd);
void send_intensity_command(uint8_t intensity);
void send_color_command(int8_t color);
void send_cam_command(uint8_t cam);
void send_sensor_command(uint8_t state);
void send_lamp_command(uint8_t state);
void send_endo_command(uint8_t state);
void send_depth_command(uint8_t state);

// Function to send focus-related commands
void send_focus_command(uint8_t focus_option);

#endif // COMMAND_MANAGER_H
