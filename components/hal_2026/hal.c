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

/**
 * @brief array for iterate through all LEDS vector
 */
uint32_t hal_vector_leds [] = {
	BSP_LED0,
	BSP_LED1,
	BSP_LED2,
	BSP_LED3,
	BSP_LED4,
};

/**
 * @brief array for iterate through all RGB LEDS vector
 */
uint32_t hal_vector_rgb [] = {
	BSP_RGB_REDLED,
	BSP_RGB_GREENLED,
	BSP_RGB_BLUELED
};

void hal_esp_init (void){
	bsp_init();
}

/**
 * @brief generic macro functions that returns bool value of desired bit position
 */
#define HAL_GET_BIT_VECTOR(x,bit) ((x & (1<<bit)) != 0)

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

void hal_toggle_status_vector(){
	for(int i = 0; i < 5; i++){
		bsp_led_toggle(hal_vector_leds[i]);
	}
}

bool hal_btn_get_actual_state(int button){
	return bsp_btn_get_actual_state(button);
}

void hal_falling_edge(int button, hal_callback_t callback){
	bsp_pressed_button(button);
	
	if ((bsp_btn_get_actual_state(button) == false) && (bsp_btn_get_last_state(button) == true)){
			callback();
	}
	
	bsp_update_last_btn_state(button);
}

void hal_rising_edge(int button, hal_callback_t callback){
	bsp_pressed_button(button);
	
	if ((bsp_btn_get_actual_state(button) == true) && (bsp_btn_get_last_state(button) == false)){
			callback();
	}
	
	bsp_update_last_btn_state(button);
}


/**
 * @brief callback that the timer alarm will call
 * @param arg void pointer to the main.c callback
 * @details 
 * The timer alarm will call hal_timer_isr, and this isr will call
 * the callback defined by user and send as argument in hal_periodic_fun()
 */
static void IRAM_ATTR hal_timer_isr(void *arg)
{
    hal_callback_t periodic_func = arg;
	periodic_func();
	bsp_reset_timer_loop(BSP_TIMER0);
}

/**
 * @brief handler for interruption. esp_intr_alloc() need it.
 */
static intr_handle_t hal_timer_handle;

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