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
#include "../components/timer_2026/include/timer_2026.h"
#include <inttypes.h> // Required for PRIu32
//#include "esp_err.h"
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
    /*timer_group_clr_intr_status_in_isr(
        TIMER_GROUP_0,
        TIMER_0
    );*/

    webo = true;

	TIMGn_T0UPDATE_REG = 1;
	TIMG0_T0CONFIG_REG |= (1<<10);
	TIMGn_Tx_INT_CLR_REG = 1;

    /*timer_group_enable_alarm_in_isr(
        TIMER_GROUP_0,
        TIMER_0
    );*/
}

void app_main(void)
{
	//Ver configuracion inicial del registro
	printf("Registro config(default): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	//configurar no enable
	TIMG0_T0CONFIG_REG &= ~(1<<31);
	printf("Registro config(enable desactivado): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	//configurar modo divisor de frecuencia
	TIMG0_T0CONFIG_REG &= ~(0xFFFF<<13);
	TIMG0_T0CONFIG_REG |= (80<<13);
	printf("Registro config(divisor de freq): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	//configurar modo ascendente
	TIMG0_T0CONFIG_REG |= (1<<30);
	printf("Registro config(modo ascendente): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	//configurar la alarma
	TIMG0_T0CONFIG_REG |= (1<<10);
	printf("Registro config(enable alarma): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	//configurar modo autoreload
	TIMG0_T0CONFIG_REG |= (1<<29);
	printf("Registro config(modo Autoreolad): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	//configurar modo de interrupcion
	TIMG0_T0CONFIG_REG &= ~(1<<12); //Tumbar el level
	TIMG0_T0CONFIG_REG |= (1<<11); //subir el edge
	printf("Registro config(modo int): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	//Finalmente habilitalo
	TIMG0_T0CONFIG_REG |= (1<<31);
	printf("Registro config(enable desactivado): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	
	/*timer_config_t config = {
        .divider = 80,          // 80 MHz / 80 = 1 MHz
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = true,
    };*/
    //timer_init(TIMER_GROUP_0, TIMER_0, &config);

    // 1 MHz => 1 000 000 ticks = 1 segundo
    /*timer_set_alarm_value(
        TIMER_GROUP_0,
        TIMER_0,
        1000000
    );
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);*/

	TIMGn_T0ALARMLO_REG = 1000000;
	printf("Registro TIMGn_T0ALARMLO_REG: %" PRIx32 "\n", TIMGn_T0ALARMLO_REG);

	TIMGn_T0ALARMHI_REG = 0;
	printf("Registro TIMGn_T0ALARMHI_REG: %" PRIx32 "\n", TIMGn_T0ALARMHI_REG);

	TIMGn_T0LOADLO_REG = 0;
	printf("Registro TIMGn_T0LOADLO_REG: %" PRIx32 "\n", TIMGn_T0LOADLO_REG);

	TIMGn_T0LOADHI_REG = 0;
	printf("Registro TIMGn_T0LOADHI_REG: %" PRIx32 "\n", TIMGn_T0LOADHI_REG);

	TIMGn_T0LOAD_REG = 1;
	printf("Registro TIMGn_T0LOAD_REG: %" PRIx32 "\n", TIMGn_T0LOAD_REG);


    esp_intr_alloc(
        ETS_TG0_T0_LEVEL_INTR_SOURCE,
        ESP_INTR_FLAG_IRAM,
        timer_isr,
        NULL,
        &timer_handle
    );

    //timer_start(TIMER_GROUP_0, TIMER_0);
    
    while(true){
		if (webo){
			webo = false;
			printf("vamos viendo tengo aura\n");
		}
		vTaskDelay(pdMS_TO_TICKS(250));
	}
}