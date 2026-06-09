/**
 * @file hal_board.c
 * @brief Implementación HAL para placa ESP32.
 *
 * @details
 * Implementa HAL para control de placa.
 *
 * @author Hector Said Herrera Niño
 * @author José Francisco Padilla Torres
 * @date 2026-06-05
 */

#include "hal_board.h"
#include "bsp_board.h"
#include "timer_2026.h"

/* Matrices de mapeo desde tipos HAL a tipos BSP */
static const int hal_led_to_bsp_map[HAL_LED_COUNT] = {
    BSP_LED0,
    BSP_LED1,
    BSP_LED2,
    BSP_LED3,
    BSP_LED4
};

static const int hal_rgb_to_bsp_map[HAL_RGB_COUNT] = {
    BSP_RGB_REDLED,
    BSP_RGB_GREENLED,
    BSP_RGB_BLUELED
};

static const int hal_button_to_bsp_map[HAL_BUTTON_COUNT] = {
    BSP_PUSH_BUTTON_0,
    BSP_PUSH_BUTTON_1
};

/*Inicialización de la placa*/

void hal_board_init(void)
{
    bsp_init();
}

/*Control de LEDs*/

void hal_led_on(hal_led_t led)
{
    if (led < HAL_LED_COUNT) {
        bsp_led_on(hal_led_to_bsp_map[led]);
    }
}

void hal_led_off(hal_led_t led)
{
    if (led < HAL_LED_COUNT) {
        bsp_led_off(hal_led_to_bsp_map[led]);
    }
}

void hal_led_toggle(hal_led_t led)
{
    if (led < HAL_LED_COUNT) {
        bsp_led_toggle(hal_led_to_bsp_map[led]);
    }
}

bool hal_led_is_on(hal_led_t led)
{
    if (led < HAL_LED_COUNT) {
        return bsp_get_bit_vector_led(led);
    }
    return false;
}

/*Control de LED RGB*/

void hal_rgb_on(hal_rgb_led_t rgb_led)
{
    if (rgb_led < HAL_RGB_COUNT) {
        bsp_RGB_on(hal_rgb_to_bsp_map[rgb_led]);
    }
}

void hal_rgb_off(hal_rgb_led_t rgb_led)
{
    if (rgb_led < HAL_RGB_COUNT) {
        bsp_RGB_off(hal_rgb_to_bsp_map[rgb_led]);
    }
}

void hal_rgb_toggle(hal_rgb_led_t rgb_led)
{
    if (rgb_led < HAL_RGB_COUNT) {
        bsp_RGB_led_toggle(hal_rgb_to_bsp_map[rgb_led]);
    }
}

bool hal_rgb_is_on(hal_rgb_led_t rgb_led)
{
    if (rgb_led < HAL_RGB_COUNT) {
        return bsp_get_bit_vector_RGB(rgb_led);
    }
    return false;
}

/*Control de botones*/

bool hal_button_pressed(hal_button_t button)
{
    if (button < HAL_BUTTON_COUNT) {
        return bsp_pressed_button(hal_button_to_bsp_map[button]);
    }
    return false;
}

/*Control de temporizadores*/

int hal_timer_init(hal_timer_group_t group, hal_timer_t timer, uint32_t period_ms)
{
    // Inicialización del temporizador
    return 0;
}

void hal_timer_start(hal_timer_group_t group, hal_timer_t timer)
{
    // Inicio del temporizador

}

void hal_timer_stop(hal_timer_group_t group, hal_timer_t timer)
{
    // Detención del temporizador
}

int hal_timer_set_isr(hal_timer_group_t group, hal_timer_t timer, 
                      hal_timer_isr_t handler, void *arg)
{
    // Registro de ISR del temporizador

    (void)group;
    (void)timer;
    (void)handler;
    (void)arg;
    return 0;
}
