/**
 * @file bsp_board.c
 * @brief Board Support Package source file for ESP32.
 *
 * @details
 * This file contains function implementation for 
 * handling board elements (leds, RGB and buttons)
 *
 * @author Hector Said Herrera Niño
 * @author José Francisco Padilla Torres
 * @date 2026-06-01
 */
#include <stdio.h>
#include "bsp_board.h"
// ======================================================= GPIO ======================================================= 
static uint8_t bsp_led_vector = 0;
static uint8_t bsp_rgb_vector = 0;

bsp_button_state_t bsp_user_button_vector [2] = {
	[BSP_VECTOR_BTN0] = {true, true},
	[BSP_VECTOR_BTN1] = {true, true}
};

/**
 * @brief array for mapping GPIO number with bit position in Vector button
 */
uint32_t bsp_button_Vector_Gpio_Map [] = {
	[LL_GPIO18]  = BSP_VECTOR_BTN0,
	[LL_GPIO19]  = BSP_VECTOR_BTN1
};

/**
 * @brief array for mapping GPIO number with bit position in Vector led
 */
uint32_t bsp_led_Vector_Gpio_Map [] = {
	[LL_GPIO2]  = BSP_VECTOR_LED0,
	[LL_GPIO4]  = BSP_VECTOR_LED1,
	[LL_GPIO16] = BSP_VECTOR_LED2,
	[LL_GPIO17] = BSP_VECTOR_LED3,
	[LL_GPIO5]  = BSP_VECTOR_LED4
};

/**
 * @brief array for mapping GPIO number with bit position in Vector RGB
 */
uint32_t bsp_RGB_Vector_Gpio_Map [] = {
	[LL_GPIO14] = BSP_VECTOR_LEDRED,
	[LL_GPIO13] = BSP_VECTOR_LEDGREEN,
	[LL_GPIO12] = BSP_VECTOR_LEDBLUE
};

/**
 * @brief array for iterate through all LEDS and configure in bsp_init
 */
uint32_t bsp_board_element_leds [] = {
	BSP_LED0,
	BSP_LED1,
	BSP_LED2,
	BSP_LED3,
	BSP_LED4,
};

/**
 * @brief array for iterate through all RGB LEDS and configure in bsp_init
 */
uint32_t bsp_board_element_rgb [] = {
	BSP_RGB_REDLED,
	BSP_RGB_GREENLED,
	BSP_RGB_BLUELED
};

void bsp_init(void){
	int board_element_leds_lenght = sizeof(bsp_board_element_leds) / sizeof(bsp_board_element_leds[0]);
	int board_element_rgb_lenght = sizeof(bsp_board_element_rgb) / sizeof(bsp_board_element_rgb[0]);

	for (int i = 0; i < board_element_leds_lenght; i++){
		ll_gpio_config_out(bsp_board_element_leds[i]);
		ll_gpio_write(bsp_board_element_leds[i], false);
	}
	
	for (int i = 0; i < board_element_rgb_lenght; i++){
		ll_gpio_config_out(bsp_board_element_rgb[i]);
		ll_gpio_write(bsp_board_element_rgb[i], true);
	}
	
	// ========== Configure buttons as inputs ==========
	ll_gpio_config_in(BSP_PUSH_BUTTON_0, LL_PULL_UP);
	ll_gpio_config_in(BSP_PUSH_BUTTON_1, LL_PULL_UP);
	
}

bool bsp_get_bit_vector_led(uint8_t bit){
	return ((bsp_led_vector & (1<<bit)) != 0);
}

bool bsp_get_bit_vector_RGB(uint8_t bit){
	return ((bsp_rgb_vector & (1<<bit)) != 0);
}

void bsp_led_on(int led){
	ll_gpio_write(led, true);
	bsp_led_vector |= (1<<led);
}

void bsp_led_off(int led){
	ll_gpio_write(led, false);
	bsp_led_vector &= ~(1<<led);
}

void bsp_led_toggle(int led){
	ll_gpio_write(led, !bsp_get_bit_vector_led(bsp_led_Vector_Gpio_Map[led]));
    bsp_led_vector ^= (1<<bsp_led_Vector_Gpio_Map[led]);
}

void bsp_RGB_on(int led){
	ll_gpio_write(led, true);
	bsp_rgb_vector |= (1<<led);
}

void bsp_RGB_off(int led){
	ll_gpio_write(led, false);
	bsp_rgb_vector &= ~(1<<led);
}

void bsp_RGB_led_toggle(int led){
	ll_gpio_write(led,  !bsp_get_bit_vector_RGB(bsp_RGB_Vector_Gpio_Map[led]));
	bsp_rgb_vector ^= (1<<bsp_RGB_Vector_Gpio_Map[led]);
}

bool bsp_pressed_button(int button){
	bool button_state = (ll_gpio_read(button) == 1);
	bsp_user_button_vector[bsp_button_Vector_Gpio_Map[button]].bsp_actual_state = button_state;
	return (button_state);
}

void bsp_update_last_btn_state(int button){
	bsp_user_button_vector[bsp_button_Vector_Gpio_Map[button]].bsp_last_state = bsp_user_button_vector[bsp_button_Vector_Gpio_Map[button]].bsp_actual_state;
}

// ======================================================= TIMER ======================================================= 
void bsp_timer_init(bsp_timer_config_t *timer_cfg)
{
    //configurar no enable
	ll_timer_enable(timer_cfg->bsp_timer, false);
	
	//configurar modo divisor de frecuencia
	ll_set_freq_divider (timer_cfg->bsp_timer, timer_cfg->bsp_freq_divider);
	
	//configurar modo ascendente
	ll_timer_count_mode(timer_cfg->bsp_timer, timer_cfg->bsp_count_mode);
	
	//configurar la alarma
	ll_timer_alarm_enable(timer_cfg->bsp_timer, timer_cfg->bsp_alarm_enable);
	
	//configurar modo autoreload
	ll_timer_autoreload(timer_cfg->bsp_timer, timer_cfg->bsp_autoreload_enable);
	
	//configurar modo de interrupcion
	ll_timer_int_mode(timer_cfg->bsp_timer,timer_cfg->bsp_int_mode);
	
	//Finalmente habilitalo mijo
	ll_timer_enable(timer_cfg->bsp_timer, true);
	
	//Set alarm
	ll_set_alarm_value(timer_cfg->bsp_timer, timer_cfg->bsp_alarm_value);
	
	//cargar el load
	ll_set_load_value(timer_cfg->bsp_timer, timer_cfg->bsp_load_value);
	ll_charge_load_value(timer_cfg->bsp_timer);
}

void bsp_reset_timer_loop(ll_timer_t timer){
	ll_timer_alarm_enable(timer, true);
	ll_clear_int(timer);
}