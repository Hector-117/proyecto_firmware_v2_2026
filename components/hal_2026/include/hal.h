#ifndef HAL_2026
#define HAL_2026

# include "../../bsp_board/include/bsp_board.h"
/************ HAL init ************/

typedef enum{
	HAL_WHITE	= 0,
	HAL_CYAN 	= 1,
	HAL_MAGENTA = 2,
	HAL_BLUE 	= 3,
	HAL_YELLOW  = 4,
	HAL_GREEN 	= 5,
	HAL_RED 	= 6,
	HAL_BLACK	= 7
} hal_color_t;

typedef void (*hal_callback_t)(void);

void hal_falling_edge(int button, hal_callback_t callback);
void hal_rising_edge(int button, hal_callback_t callback);

void hal_esp_init (void);
void hal_status_vector(uint8_t vector_value);
void hal_set_RGB_color(uint8_t vector_value);
#endif

