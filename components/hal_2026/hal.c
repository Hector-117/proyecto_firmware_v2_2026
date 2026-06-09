/**
 * @file hal.c
 * @brief Hardware Abstraction Layer (HAL) source file for ESP32.
 *
 * @details
 * This file contains functions implementation, and arrays for iterate through vectors
 * for implementing abstract functionality of board elements.
 *
 * @author Hector Said Herrera Niño
 * @author José Francisco Padilla Torres
 * @date 2026-06-07
 */

#include <stdio.h>
#include "hal.h"

#include "esp_intr_alloc.h"
#include "esp_attr.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

/**
 * @brief Array for iterating through all board LEDs.
 *
 * @details
 * This array maps the HAL LED vector positions
 * with the BSP LED definitions.
 */
uint32_t hal_vector_leds [] = {
	BSP_LED0,
	BSP_LED1,
	BSP_LED2,
	BSP_LED3,
	BSP_LED4,
};

/**
 * @brief Array for iterating through all RGB LEDs.
 *
 * @details
 * This array maps the HAL RGB vector positions
 * with the BSP RGB LED definitions.
 */
uint32_t hal_vector_rgb [] = {
	BSP_RGB_REDLED,
	BSP_RGB_GREENLED,
	BSP_RGB_BLUELED
};

/**
 * @brief Initialize ESP32 board through HAL.
 *
 * @details
 * Calls the BSP initialization function to configure
 * LEDs, RGB LEDs and user buttons.
 */
void hal_esp_init (void){
	bsp_init();
}

/**
 * @brief Get bit value from a vector.
 *
 * @details
 * Returns true if the selected bit position is 1,
 * otherwise returns false.
 */
#define HAL_GET_BIT_VECTOR(x,bit) ((x & (1<<bit)) != 0)

/**
 * @brief Set board LED status vector.
 * @param vector_value Value used to control the board LEDs.
 *
 * @details
 * Uses the five least significant bits of vector_value.
 * If a bit is 1, the corresponding LED turns on.
 * If a bit is 0, the corresponding LED turns off.
 */
void hal_status_vector(uint8_t vector_value){
	for(int i = 0; i < 5; i++){
		if (HAL_GET_BIT_VECTOR(vector_value,i)){
			bsp_led_on(hal_vector_leds[i]);
		}
		else{
			bsp_led_off(hal_vector_leds[i]);
		}
	}
}

/**
 * @brief Set RGB LED color.
 * @param vector_value RGB color value.
 *
 * @details
 * Uses the three least significant bits of vector_value.
 * Each bit controls one RGB channel. This function can
 * also receive values from hal_color_t enum.
 */
void hal_set_RGB_color(uint8_t vector_value){
	for(int i = 0; i < 3; i++){
		if (HAL_GET_BIT_VECTOR(vector_value,i)){
			bsp_RGB_on(hal_vector_rgb[i]);
		}
		else{
			bsp_RGB_off(hal_vector_rgb[i]);
		}
	}
}

/**
 * @brief Toggle all board LEDs.
 *
 * @details
 * Iterates through all board LEDs and toggles
 * their current state using BSP functions.
 */
void hal_toggle_status_vector(){
	for(int i = 0; i < 5; i++){
		bsp_led_toggle(hal_vector_leds[i]);
	}
}

/**
 * @brief Get actual button state.
 * @param button GPIO assigned to the button.
 * @return Current button state stored in BSP.
 */
bool hal_btn_get_actual_state(int button){
	return bsp_btn_get_actual_state(button);
}

/**
 * @brief Detect falling edge on a button.
 * @param button GPIO assigned to the button.
 * @param callback Function to execute when falling edge is detected.
 *
 * @details
 * A falling edge occurs when the button changes from
 * HIGH to LOW. The function reads the button, compares
 * actual and last states, executes the callback if the
 * edge is detected, and then updates the last state.
 */
void hal_falling_edge(int button, hal_callback_t callback){
	bsp_pressed_button(button);
	
	if ((bsp_btn_get_actual_state(button) == false) && (bsp_btn_get_last_state(button) == true)){
			callback();
	}
	
	bsp_update_last_btn_state(button);
}

/**
 * @brief Detect rising edge on a button.
 * @param button GPIO assigned to the button.
 * @param callback Function to execute when rising edge is detected.
 *
 * @details
 * A rising edge occurs when the button changes from
 * LOW to HIGH. The function reads the button, compares
 * actual and last states, executes the callback if the
 * edge is detected, and then updates the last state.
 */
void hal_rising_edge(int button, hal_callback_t callback){
	bsp_pressed_button(button);
	
	if ((bsp_btn_get_actual_state(button) == true) && (bsp_btn_get_last_state(button) == false)){
			callback();
	}
	
	bsp_update_last_btn_state(button);
}

/**
 * @brief Timer interrupt service routine.
 * @param arg Pointer to user callback function.
 *
 * @details
 * This ISR is called when the timer alarm occurs.
 * It casts the received argument to hal_callback_t,
 * executes the user callback and resets the timer
 * alarm cycle through the BSP.
 */
static void IRAM_ATTR hal_timer_isr(void *arg)
{
    hal_callback_t periodic_func = arg;
	periodic_func();
	bsp_reset_timer_loop(BSP_TIMER0);
}

/**
 * @brief Timer interrupt handler.
 *
 * @details
 * This handler is required by esp_intr_alloc()
 * to store the interrupt allocation reference.
 */
static intr_handle_t hal_timer_handle;

/**
 * @brief Configure a periodic callback function.
 * @param time_us Period time in microseconds.
 * @param callback Function to execute periodically.
 *
 * @details
 * Configures BSP_TIMER0 using a frequency divider of 80,
 * which converts the 80 MHz APB clock into a 1 MHz timer
 * clock. This means each timer tick is approximately 1 us.
 * The timer alarm value is set using time_us, so the callback
 * is executed periodically according to that value.
 */
void hal_periodic_fun(int time_us, hal_callback_t callback){
	bsp_timer_config_t timer_struct_config = {
		.bsp_timer = BSP_TIMER0,
		.bsp_freq_divider = 80,
		.bsp_count_mode = BSP_UP,
		.bsp_alarm_enable = true,
		.bsp_autoreload_enable = true,
		.bsp_int_mode = BSP_LEVEL,
		.bsp_alarm_value = time_us,
		.bsp_load_value = 0
	};
	
	bsp_timer_init(&timer_struct_config);
	
	esp_intr_alloc(
        ETS_TG0_T0_LEVEL_INTR_SOURCE,
        ESP_INTR_FLAG_IRAM,
        hal_timer_isr,
        callback,
        &hal_timer_handle
    );
}

/**
 * @brief callback that the timer alarm will call (for delay)
 * @param arg void pointer but NULL is received
 * @details 
 * This callback will rise a flag, and the delay logic is constantly polling the flag
 * and continue next instruction when flag is true
 */
bool alarm_delay = false;
static void IRAM_ATTR hal_delay_isr(void *arg)
{
    alarm_delay = true;
	bsp_reset_timer_loop(BSP_TIMER1);
}

/**
 * @brief handler for interruption delay. esp_intr_alloc() need it.
 */
static intr_handle_t hal_delay_handle;

/**
 * @brief Configure a delay.
 * @param time_us delay time in microseconds.
 *
 * @details
 * This function configure timer and isr. always will configure for:
 * module 0 timer 1.
 */
bool loop_onetime = true;
void hal_delay(int time_us){
	if (loop_onetime){
		loop_onetime = false;
		
		bsp_timer_config_t timer_struct_config = {
			.bsp_timer = BSP_TIMER1,
			.bsp_freq_divider = 80,
			.bsp_count_mode = BSP_UP,
			.bsp_alarm_enable = true,
			.bsp_autoreload_enable = true,
			.bsp_int_mode = BSP_LEVEL,
			.bsp_alarm_value = time_us,
			.bsp_load_value = 0
		};
		
		bsp_timer_init(&timer_struct_config);
	
		esp_intr_alloc(
	        ETS_TG0_T1_LEVEL_INTR_SOURCE,
	        ESP_INTR_FLAG_IRAM,
	        hal_delay_isr,
	        NULL,
	        &hal_delay_handle
    	);
	}
	
	while(!alarm_delay){
	};
	
	alarm_delay = false;
}