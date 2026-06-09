/**
 * @file hal_board.h
 * @brief HAL para placa ESP32.
 *
 * @details
 * HAL que unifica el BSP y drivers
 *
 * @author Hector Said Herrera Niño
 * @author José Francisco Padilla Torres
 * @date 2026-06-05
 */

#ifndef HAL_BOARD_H
#define HAL_BOARD_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeración de LEDs de la placa
 */
typedef enum {
    HAL_LED_0 = 0,    /**< LED 0 en GPIO2  */
    HAL_LED_1,        /**< LED 1 en GPIO4  */
    HAL_LED_2,        /**< LED 2 en GPIO16 */
    HAL_LED_3,        /**< LED 3 en GPIO17 */
    HAL_LED_4,        /**< LED 4 en GPIO5  */
    HAL_LED_COUNT
} hal_led_t;

/**
 * @brief Enumeración de LED RGB
 */
typedef enum {
    HAL_RGB_RED = 0,      /**< Canal rojo en GPIO14   */
    HAL_RGB_GREEN,        /**< Canal verde en GPIO13 */
    HAL_RGB_BLUE,         /**< Canal azul en GPIO12  */
    HAL_RGB_COUNT
} hal_rgb_led_t;

/**
 * @brief Enumeración de botones de la placa
 */
typedef enum {
    HAL_BUTTON_0 = 0,   /**< Botón 0 en GPIO18 */
    HAL_BUTTON_1,       /**< Botón 1 en GPIO19 */
    HAL_BUTTON_COUNT
} hal_button_t;

/**
 * @brief Enumeración de temporizadores para los temporizadores de hardware
 */
typedef enum {
    HAL_TIMER_0 = 0,
    HAL_TIMER_1,
    HAL_TIMER_COUNT
} hal_timer_t;

/**
 * @brief Enumeración de grupo de temporizadores
 */
typedef enum {
    HAL_TIMER_GROUP_0 = 0,
    HAL_TIMER_GROUP_1,
    HAL_TIMER_GROUP_COUNT
} hal_timer_group_t;

/**
 * @brief Tipo de callback de interrupción del temporizador
 */
typedef void (*hal_timer_isr_t)(void *arg);

/*Inicialización de la placa*/

/**
 * @brief Inicializar el HAL completo de la placa
 */
void hal_board_init(void);

/*Control de LEDs*/

/**
 * @brief Encender un LED de la placa
 * @param led Identificador del LED
 */
void hal_led_on(hal_led_t led);

/**
 * @brief Apagar un LED de la placa
 * @param led Identificador del LED
 */
void hal_led_off(hal_led_t led);

/**
 * @brief Alternar un LED de la placa
 * @param led Identificador del LED
 */
void hal_led_toggle(hal_led_t led);

/**
 * @brief Obtener estado del LED
 * @param led Identificador del LED
 * @return Verdadero si el LED está encendido, falso si está apagado
 */
bool hal_led_is_on(hal_led_t led);

/*Control de LED RGB*/

/**
 * @brief Encender un canal RGB
 * @param rgb_led Identificador del canal RGB
 */
void hal_rgb_on(hal_rgb_led_t rgb_led);

/**
 * @brief Apagar un canal RGB
 * @param rgb_led Identificador del canal RGB
 */
void hal_rgb_off(hal_rgb_led_t rgb_led);

/**
 * @brief Alternar un canal RGB
 * @param rgb_led Identificador del canal RGB
 */
void hal_rgb_toggle(hal_rgb_led_t rgb_led);

/**
 * @brief Obtener estado del LED RGB
 * @param rgb_led Identificador del canal RGB
 * @return Verdadero si el LED RGB está encendido, falso si está apagado
 */
bool hal_rgb_is_on(hal_rgb_led_t rgb_led);

/*Control de botones*/

/**
 * @brief Verificar si un botón está presionado (nivel bajo debido a configuración pull-up)
 * @param button Identificador del botón
 * @return Verdadero si el botón está presionado, falso si no está presionado
 */
bool hal_button_pressed(hal_button_t button);

/*Control de temporizadores*/

/**
 * @brief Inicializar un temporizador de hardware
 * @param group Grupo de temporizador
 * @param timer Temporizador en el grupo
 * @param period_ms Período en milisegundos
 * @return 0 si es exitoso, -1 en caso de error
 */
int hal_timer_init(hal_timer_group_t group, hal_timer_t timer, uint32_t period_ms);

/**
 * @brief Iniciar un temporizador de hardware
 * @param group Grupo de temporizador
 * @param timer Temporizador en el grupo
 */
void hal_timer_start(hal_timer_group_t group, hal_timer_t timer);

/**
 * @brief Detener un temporizador de hardware
 * @param group Grupo de temporizador
 * @param timer Temporizador en el grupo
 */
void hal_timer_stop(hal_timer_group_t group, hal_timer_t timer);

/**
 * @brief Registrar manejador de interrupción del temporizador
 * @param group Grupo de temporizador
 * @param timer Temporizador en el grupo
 * @param handler Función callback ISR
 * @param arg Argumento para pasar al manejador
 * @return 0 si es exitoso, -1 en caso de error
 */
int hal_timer_set_isr(hal_timer_group_t group, hal_timer_t timer, 
                      hal_timer_isr_t handler, void *arg);

#ifdef __cplusplus
}
#endif

#endif /* HAL_BOARD_H */
