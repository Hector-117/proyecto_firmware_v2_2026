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
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
 
#include "bsp_board.h"
#include "hal.h"
//#include "../components/timer_2026/include/timer_2026.h"
#include <inttypes.h> // Required for PRIu32
#include "driver/timer.h"
#include "esp_intr_alloc.h"


//Para los registros miados
#define TIMG0_T0CONFIG_REG   (*((volatile uint32_t *)(0x3FF5F000)))
#define TIMGn_T0ALARMLO_REG  (*((volatile uint32_t *)(0x3FF5F010)))
#define TIMGn_T0ALARMHI_REG  (*((volatile uint32_t *)(0x3FF5F014)))
#define TIMGn_T0LOADLO_REG   (*((volatile uint32_t *)(0x3FF5F018)))
#define TIMGn_T0LOADHI_REG   (*((volatile uint32_t *)(0x3FF5F01C)))
#define TIMGn_T0LOAD_REG     (*((volatile uint32_t *)(0x3FF5F020)))
#define TIMGn_T0UPDATE_REG   (*((volatile uint32_t *)(0x3FF5F00C)))
#define TIMGn_T0LO_REG       (*((volatile uint32_t *)(0x3FF5F004)))
#define TIMGn_Tx_INT_RAW_REG (*((volatile uint32_t *)(0x3FF5F09C)))
#define TIMGn_Tx_INT_CLR_REG (*((volatile uint32_t *)(0x3FF5F0A4)))


void function_boton1(void){
	printf("Boton 1 presionado callback\n");
}

void function_boton2(void){
	printf("Boton 2 presionado callback\n");
}

volatile bool webo = false;

static intr_handle_t timer_handle;

static void IRAM_ATTR timer_isr(void *arg)
{
    webo = true;

	bsp_reset_timer_loop(BSP_TIMER0);
}

void app_main(void)
{
	hal_esp_init();
	//Ver configuracion inicial del registro
	printf("Registro config(default): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	
	bsp_timer_config_t timer_struct_config = {
		.bsp_timer = BSP_TIMER0,
		.bsp_freq_divider = 80,
		.bsp_count_mode = BSP_UP,
		.bsp_alarm_enable = true,
		.bsp_autoreload_enable = true,
		.bsp_int_mode = BSP_LEVEL,
		.bsp_alarm_value = 1000000,
		.bsp_load_value = 0
	};
	
	bsp_timer_init(&timer_struct_config);
	
	
	
	printf("Registro config(enable activado): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	
	
	
	printf("Registro TIMGn_T0LOAD_REG: %" PRIx32 "\n", TIMGn_T0LOAD_REG);


    esp_intr_alloc(
        ETS_TG0_T0_LEVEL_INTR_SOURCE,
        ESP_INTR_FLAG_IRAM,
        timer_isr,
        NULL,
        &timer_handle
    );

    
    
    while(true){
		/*bsp_pressed_button(BSP_PUSH_BUTTON_0);
		bsp_pressed_button(BSP_PUSH_BUTTON_1);
		
		if ((bsp_btn_get_actual_state(BSP_PUSH_BUTTON_0) == false) && (bsp_btn_get_last_state(BSP_PUSH_BUTTON_0) == true)){ //DETECCIÓN DE FALNCO DESCENDENTE EN PIN18
			printf("Boton 1 presionado ikanaaaaaaiiiideeee\n");
		}
		bsp_update_last_btn_state(BSP_PUSH_BUTTON_0);

		if ((bsp_btn_get_actual_state(BSP_PUSH_BUTTON_1) == false) && (bsp_btn_get_last_state(BSP_PUSH_BUTTON_1) == true)){ //DETECCIÓN DE FALNCO DESCENDENTE EN PIN18
			printf("Boton 2 presionado or wo shi huan xu xie\n");
		}
		bsp_update_last_btn_state(BSP_PUSH_BUTTON_1);*/
		
		
		hal_falling_edge(BSP_PUSH_BUTTON_0, function_boton1);
		
		hal_falling_edge(BSP_PUSH_BUTTON_1, function_boton2);
		
		if (webo){
			webo = false;
			//printf("a ver si no la kgue\n");
		}
		vTaskDelay(pdMS_TO_TICKS(250));
		/*hal_status_vector(0x55);
		hal_set_RGB_color(HAL_RED);
		vTaskDelay(pdMS_TO_TICKS(500));
		
		hal_status_vector(0xAA);
		vTaskDelay(pdMS_TO_TICKS(500));
		hal_set_RGB_color(HAL_GREEN);
		
		hal_status_vector(0x55);
		vTaskDelay(pdMS_TO_TICKS(500));
		hal_set_RGB_color(HAL_BLUE);
		
		hal_status_vector(0xAA);*/
	}
}