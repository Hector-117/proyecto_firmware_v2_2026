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



volatile bool webo = false;

static intr_handle_t timer_handle;

static void IRAM_ATTR timer_isr(void *arg)
{
    webo = true;

	ll_timer_alarm_enable(LL_TIMG0_T0, true);
	ll_clear_int(LL_TIMG0_T0, LL_CLR_T0_INT);
}

void app_main(void)
{
	//Ver configuracion inicial del registro
	printf("Registro config(default): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	
	bsp_timer_config_t timer_struct_config = {
		.bsp_timer = LL_TIMG0_T0,
		.bsp_freq_divider = 80,
		.bsp_count_mode = LL_UP,
		.bsp_alarm_enable = true,
		.bsp_autoreload_enable = true,
		.bsp_int_mode = LL_LEVEL,
		.bsp_alarm_value = 1000000,
		.bsp_load_value = 0
	};
	
	bsp_timer_init(&timer_struct_config);
	
	//configurar no enable
	//ll_timer_enable(LL_TIMG0_T0, false);
	
	//configurar modo divisor de frecuencia
	//ll_set_freq_divider (LL_TIMG0_T0, 80);
	
	//configurar modo ascendente
	//ll_timer_count_mode(LL_TIMG0_T0, LL_UP);
	
	//configurar la alarma
	//ll_timer_alarm_enable(LL_TIMG0_T0, true);
	
	//configurar modo autoreload
	//ll_timer_autoreload(LL_TIMG0_T0, true);
	
	//configurar modo de interrupcion
	//ll_timer_int_mode(LL_TIMG0_T0,LL_LEVEL);
	
	//Finalmente habilitalo
	//ll_timer_enable(LL_TIMG0_T0, true);
	
	printf("Registro config(enable activado): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	
	
	//Set alarm
	//ll_set_alarm_value(LL_TIMG0_T0, 1000000);

	//cargar el load
	//ll_set_load_value(LL_TIMG0_T0, 0);
	//ll_charge_load_value(LL_TIMG0_T0);
	
	printf("Registro TIMGn_T0LOAD_REG: %" PRIx32 "\n", TIMGn_T0LOAD_REG);


    esp_intr_alloc(
        ETS_TG0_T0_LEVEL_INTR_SOURCE,
        ESP_INTR_FLAG_IRAM,
        timer_isr,
        NULL,
        &timer_handle
    );

    
    while(true){
		if (webo){
			webo = false;
			printf("Hasta con BSP mijito, como la ve\n");
		}
		vTaskDelay(pdMS_TO_TICKS(250));
	}
}