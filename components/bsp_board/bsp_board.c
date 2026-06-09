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

// GPIO 

/**
 * @brief Internal vector for board LED states.
 */
static uint8_t bsp_led_vector = 0;

/**
 * @brief Internal vector for RGB LED states.
 */
static uint8_t bsp_rgb_vector = 0;

/**
 * @brief Stores actual and last state of user buttons.
 */
bsp_button_state_t bsp_user_button_vector [2] = {
	[BSP_VECTOR_BTN0] = {true, true},
	[BSP_VECTOR_BTN1] = {true, true}
};

/**
 * @brief Array for mapping GPIO number with bit position in button vector.
 */
uint32_t bsp_button_Vector_Gpio_Map [] = {
	[LL_GPIO18]  = BSP_VECTOR_BTN0,
	[LL_GPIO19]  = BSP_VECTOR_BTN1
};

/**
 * @brief Array for mapping GPIO number with bit position in LED vector.
 */
uint32_t bsp_led_Vector_Gpio_Map [] = {
	[LL_GPIO2]  = BSP_VECTOR_LED0,
	[LL_GPIO4]  = BSP_VECTOR_LED1,
	[LL_GPIO16] = BSP_VECTOR_LED2,
	[LL_GPIO17] = BSP_VECTOR_LED3,
	[LL_GPIO5]  = BSP_VECTOR_LED4
};

/**
 * @brief Array for mapping GPIO number with bit position in RGB vector.
 */
uint32_t bsp_RGB_Vector_Gpio_Map [] = {
	[LL_GPIO14] = BSP_VECTOR_LEDRED,
	[LL_GPIO13] = BSP_VECTOR_LEDGREEN,
	[LL_GPIO12] = BSP_VECTOR_LEDBLUE
};

/**
 * @brief Array for iterating through all board LEDs.
 */
uint32_t bsp_board_element_leds [] = {
	BSP_LED0,
	BSP_LED1,
	BSP_LED2,
	BSP_LED3,
	BSP_LED4,
};

/**
 * @brief Array for iterating through all RGB LEDs.
 */
uint32_t bsp_board_element_rgb [] = {
	BSP_RGB_REDLED,
	BSP_RGB_GREENLED,
	BSP_RGB_BLUELED
};

/**
 * @brief Initialize board components.
 *
 * @details
 * Configures board LEDs and RGB LEDs as outputs.
 * Board LEDs start turned off, while RGB channels
 * start with logic HIGH. User buttons are configured
 * as inputs with pull-up resistors enabled.
 */
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
	
	// Configure buttons as inputs 
	ll_gpio_config_in(BSP_PUSH_BUTTON_0, LL_PULL_UP);
	ll_gpio_config_in(BSP_PUSH_BUTTON_1, LL_PULL_UP);
}

/**
 * @brief Get LED state from LED vector.
 * @param bit Bit position to read.
 * @return true if the selected LED bit is set, false otherwise.
 */
bool bsp_get_bit_vector_led(uint8_t bit){
	return ((bsp_led_vector & (1<<bit)) != 0);
}

/**
 * @brief Get RGB state from RGB vector.
 * @param bit Bit position to read.
 * @return true if the selected RGB bit is set, false otherwise.
 */
bool bsp_get_bit_vector_RGB(uint8_t bit){
	return ((bsp_rgb_vector & (1<<bit)) != 0);
}

/**
 * @brief Turn on a board LED.
 * @param led GPIO assigned to the selected LED.
 *
 * @details
 * Writes HIGH to the GPIO and updates the internal
 * LED state vector.
 */
void bsp_led_on(int led){
	ll_gpio_write(led, true);
	bsp_led_vector |= (1<<bsp_led_Vector_Gpio_Map[led]);
}

/**
 * @brief Turn off a board LED.
 * @param led GPIO assigned to the selected LED.
 *
 * @details
 * Writes LOW to the GPIO and updates the internal
 * LED state vector.
 */
void bsp_led_off(int led){
	ll_gpio_write(led, false);
	bsp_led_vector &= ~(1<<bsp_led_Vector_Gpio_Map[led]);
}

/**
 * @brief Toggle a board LED.
 * @param led GPIO assigned to the selected LED.
 *
 * @details
 * Changes the current LED state using the internal
 * LED state vector.
 */
void bsp_led_toggle(int led){
	ll_gpio_write(led, !bsp_get_bit_vector_led(bsp_led_Vector_Gpio_Map[led]));
    bsp_led_vector ^= (1<<bsp_led_Vector_Gpio_Map[led]);
}

/**
 * @brief Turn on an RGB channel.
 * @param led GPIO assigned to the RGB channel.
 *
 * @details
 * Writes HIGH to the GPIO and updates the RGB
 * status vector.
 */
void bsp_RGB_on(int led){
	ll_gpio_write(led, true);
	bsp_rgb_vector |= (1<<led);
}

/**
 * @brief Turn off an RGB channel.
 * @param led GPIO assigned to the RGB channel.
 *
 * @details
 * Writes LOW to the GPIO and updates the RGB
 * status vector.
 */
void bsp_RGB_off(int led){
	ll_gpio_write(led, false);
	bsp_rgb_vector &= ~(1<<led);
}

/**
 * @brief Toggle an RGB channel.
 * @param led GPIO assigned to the RGB channel.
 *
 * @details
 * Changes the current RGB channel state using
 * the internal RGB state vector.
 */
void bsp_RGB_led_toggle(int led){
	ll_gpio_write(led,  !bsp_get_bit_vector_RGB(bsp_RGB_Vector_Gpio_Map[led]));
	bsp_rgb_vector ^= (1<<bsp_RGB_Vector_Gpio_Map[led]);
}

/**
 * @brief Read button state.
 * @param button GPIO assigned to the button.
 * @return Current button state.
 *
 * @details
 * Reads the GPIO value and updates the actual
 * state stored in the button structure.
 */
bool bsp_pressed_button(int button){
	bool button_state = (ll_gpio_read(button) == 1);
	bsp_user_button_vector[bsp_button_Vector_Gpio_Map[button]].bsp_actual_state = button_state;
	return (button_state);
}

/**
 * @brief Update last button state.
 * @param button GPIO assigned to the button.
 *
 * @details
 * Copies the actual button state into the last
 * button state field.
 */
void bsp_update_last_btn_state(int button){
	bsp_user_button_vector[bsp_button_Vector_Gpio_Map[button]].bsp_last_state = bsp_user_button_vector[bsp_button_Vector_Gpio_Map[button]].bsp_actual_state;
}

/**
 * @brief Get current button state.
 * @param button GPIO assigned to the button.
 * @return Current button state.
 */
bool bsp_btn_get_actual_state(int button){
	return bsp_user_button_vector[bsp_button_Vector_Gpio_Map[button]].bsp_actual_state;
}

/**
 * @brief Get previous button state.
 * @param button GPIO assigned to the button.
 * @return Previous button state.
 */
bool bsp_btn_get_last_state(int button){
	return bsp_user_button_vector[bsp_button_Vector_Gpio_Map[button]].bsp_last_state;
}

// TIMER 

/**
 * @brief Configure and initialize a hardware timer.
 * @param timer_cfg Pointer to timer configuration structure.
 *
 * @details
 * Configures the selected timer using the values stored
 * in the timer configuration structure. It sets the
 * divider, count mode, alarm, autoreload, interrupt mode,
 * alarm value and load value.
 */
void bsp_timer_init(bsp_timer_config_t *timer_cfg)
{
    // quitar enable para configurarar 
	ll_timer_enable(timer_cfg->bsp_timer, false);
	
	// configurar modo divisor de frecuencia
	ll_set_freq_divider (timer_cfg->bsp_timer, timer_cfg->bsp_freq_divider);
	
	// configurar modo ascendente
	ll_timer_count_mode(timer_cfg->bsp_timer, timer_cfg->bsp_count_mode);
	
	// configurar la alarma
	ll_timer_alarm_enable(timer_cfg->bsp_timer, timer_cfg->bsp_alarm_enable);
	
	// configurar modo autoreload
	ll_timer_autoreload(timer_cfg->bsp_timer, timer_cfg->bsp_autoreload_enable);
	
	// configurar modo de interrupcion
	ll_timer_int_mode(timer_cfg->bsp_timer,timer_cfg->bsp_int_mode);
	
	// habilitar el timer
	ll_timer_enable(timer_cfg->bsp_timer, true);
	
	// Set alarm
	ll_set_alarm_value(timer_cfg->bsp_timer, timer_cfg->bsp_alarm_value);
	
	// cargar el load
	ll_set_load_value(timer_cfg->bsp_timer, timer_cfg->bsp_load_value);
	ll_charge_load_value(timer_cfg->bsp_timer);
}

/**
 * @brief Reset timer alarm cycle.
 * @param timer Timer to reset.
 *
 * @details
 * Re-enables the timer alarm and clears the interrupt
 * flag after an interrupt event occurs.
 */
void bsp_reset_timer_loop(ll_timer_t timer){
	ll_timer_alarm_enable(timer, true);
	ll_clear_int(timer);
}