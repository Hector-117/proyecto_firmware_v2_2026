#include <stdio.h>
#include "hal.h"

/**
 * @brief array for iterate through all LEDS and configure in bsp_init
 */
uint32_t hal_vector_leds [] = {
	BSP_LED0,
	BSP_LED1,
	BSP_LED2,
	BSP_LED3,
	BSP_LED4,
};

uint32_t hal_vector_rgb [] = {
	BSP_RGB_REDLED,
	BSP_RGB_GREENLED,
	BSP_RGB_BLUELED
};

void hal_esp_init (void){
	bsp_init();
}

//(bsp_led_vector & (1<<bit)) != 0

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