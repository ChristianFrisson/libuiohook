/* libUIOHook: Cross-platfrom userland keyboard and mouse hooking.
 * Copyright (C) 2006-2015 Alexander Barker. 2015 Christian Frisson. All Rights Received.
 * https://github.com/ChristianFrisson/libuiohook/
 *
 * libUIOHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libUIOHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <uiohook.h>

FILE* properties_file;

bool logger_proc(unsigned int level, const char *format, ...) {
	return false;
}

int main() {
	// Disable the logger.
    hook_set_logger_proc(&logger_proc);

    // Open the log file
    // Get the local system time in UTC.
    struct timeval system_time;
    gettimeofday(&system_time, NULL);
    struct tm* local_time;
    char properties_file_name[40];
    local_time = localtime (&system_time.tv_sec);
    strftime (properties_file_name, sizeof (properties_file_name), "properties-%Y-%m-%d-%H-%M-%S.txt", local_time);

    // currently only defined for x11
    #ifdef linux
    // Retrieves an array of screen data for each available monitor.
    properties_file = fopen (properties_file_name, "w+");
	uint8_t count = 0;
	screen_data *screens = hook_create_screen_info(&count);
	if (screens != NULL) {
		fprintf(stdout,	"Found %d Monitors:\n", count);

		for (int i = 0; i < count; i++) {
            fprintf(stdout,	"\tNumber:\t\t%d\n", screens[i].number);
            fprintf(stdout,	"\tOffset X:\t%d\n", screens[i].x);
            fprintf(stdout,	"\tOffset Y:\t%d\n", screens[i].y);
            fprintf(stdout,	"\tWidth:\t\t%d\n", screens[i].width);
            fprintf(stdout,	"\tHeight:\t\t%d\n", screens[i].height);
            fprintf(stdout,	"\n");

            char buffer[256] = { 0 };
            size_t length = snprintf(buffer, sizeof(buffer),
                    "screen=%d,offset_x=%d,offset_y=%d,width=%d,height=%d",
                    screens[i].number,screens[i].x,screens[i].y,
                    screens[i].width,screens[i].height);
            fprintf(properties_file, "%s\n",	 buffer);
		}

		// You are responsible for freeing the memory returned by hook_create_screen_info.
		free(screens);
	}
	else {
		fprintf(stderr,	"Failed to aquire screen information!\n");
	}
	#endif

	// Retrieves the keyboard auto repeat rate.
	long int repeat_rate = hook_get_auto_repeat_rate();
	if (repeat_rate >= 0) {
		fprintf(stdout,	"Auto Repeat Rate:\t%ld\n", repeat_rate);
	}
	else {
		fprintf(stderr,	"Failed to aquire keyboard auto repeat rate!\n");
	}

	// Retrieves the keyboard auto repeat delay.
	long int repeat_delay = hook_get_auto_repeat_delay();
	if (repeat_delay >= 0) {
		fprintf(stdout,	"Auto Repeat Delay:\t%ld\n", repeat_delay);
	}
	else {
		fprintf(stderr,	"Failed to aquire keyboard auto repeat delay!\n");
	}

	// Retrieves the mouse acceleration multiplier.
	long int acceleration_multiplier = hook_get_pointer_acceleration_multiplier();
	if (acceleration_multiplier >= 0) {
		fprintf(stdout,	"Mouse Acceleration Multiplier:\t%ld\n", acceleration_multiplier);
	}
	else {
		fprintf(stderr,	"Failed to aquire mouse acceleration multiplier!\n");
	}

	// Retrieves the mouse acceleration threshold.
	long int acceleration_threshold = hook_get_pointer_acceleration_threshold();
	if (acceleration_threshold >= 0) {
		fprintf(stdout,	"Mouse Acceleration Threshold:\t%ld\n", acceleration_threshold);
	}
	else {
		fprintf(stderr,	"Failed to aquire mouse acceleration threshold!\n");
	}

	// Retrieves the mouse sensitivity.
	long int sensitivity = hook_get_pointer_sensitivity();
	if (sensitivity >= 0) {
		fprintf(stdout,	"Mouse Sensitivity:\t%ld\n", sensitivity);
	}
	else {
		fprintf(stderr,	"Failed to aquire keyboard auto repeat rate!\n");
	}

	// Retrieves the double/triple click interval.
	long int click_time = hook_get_multi_click_time();
	if (click_time >= 0) {
		fprintf(stdout,	"Multi-Click Time:\t%ld\n", click_time);
	}
	else {
		fprintf(stderr,	"Failed to aquire keyboard auto repeat rate!\n");
	}

    fflush(properties_file);
    fclose(properties_file);

	return EXIT_SUCCESS;
}
