/**
 * @file main.c
 * @brief App program.
 *
 * @details
 * This file contains the App program which its a RGB 
 * control via buttons 16 and 17
 *
 * @author Hector Said Herrera Niño
 * @author José Francisco Padilla Torres
 * @date 2026-06-01
 */
#include "bsp_board.h"
#include "../components/timer_2026/include/timer_2026.h"
#include "driver/gptimer.h"
#include <inttypes.h> // Required for PRIu32
#include "esp_err.h"

bool timer_flag = false;

static bool example_timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
	{
	    //printf("Hola desde la interrupcion jeje\n");
	    timer_flag = true;
	    return false;
	}

void app_main(void)
{
	gptimer_handle_t gptimer = NULL;

	gptimer_config_t timer_config = {
	    .clk_src = GPTIMER_CLK_SRC_DEFAULT, // Select the default clock source
	    .direction = GPTIMER_COUNT_UP,      // Counting direction is up
	    .resolution_hz = 1 * 1000 * 1000,   // Resolution is 1 MHz, i.e., 1 tick equals 1 microsecond
	};
	// Create a timer instance
	ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));
	
	gptimer_alarm_config_t alarm_config = {
	    .reload_count = 0,      // When the alarm event occurs, the timer will automatically reload to 0
	    .alarm_count = 1000000, // Set the actual alarm period, since the resolution is 1us, 1000000 represents 1s
	    .flags.auto_reload_on_alarm = true, // Enable auto-reload function
	};
	// Set the timer's alarm action
	ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
	
	gptimer_event_callbacks_t cbs = {
	    .on_alarm = example_timer_on_alarm_cb, // Call the user callback function when the alarm event occurs
	};
	// Register timer event callback functions, allowing user context to be carried
	ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
	// Enable the timer
	ESP_ERROR_CHECK(gptimer_enable(gptimer));
	// Start the timer
	ESP_ERROR_CHECK(gptimer_start(gptimer));
	
	while(true){
		printf("Programa prueb GPTIMER chido\n");
		
		if (timer_flag)
    	{
        	timer_flag = false;
        	printf("Hola desde el timer\n");
    	}

	    vTaskDelay(pdMS_TO_TICKS(1000));
	}
}