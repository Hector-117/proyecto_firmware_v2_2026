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