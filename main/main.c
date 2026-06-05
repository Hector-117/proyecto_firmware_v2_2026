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

volatile bool webo = false;

static intr_handle_t timer_handle;

static void IRAM_ATTR timer_isr(void *arg)
{
    timer_group_clr_intr_status_in_isr(
        TIMER_GROUP_0,
        TIMER_0
    );

    webo = true;

    timer_group_enable_alarm_in_isr(
        TIMER_GROUP_0,
        TIMER_0
    );
}

void app_main(void)
{
	timer_config_t config = {
        .divider = 80,          // 80 MHz / 80 = 1 MHz
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = true,
    };

    timer_init(TIMER_GROUP_0, TIMER_0, &config);

    // 1 MHz => 1 000 000 ticks = 1 segundo
    timer_set_alarm_value(
        TIMER_GROUP_0,
        TIMER_0,
        1000000
    );

    timer_enable_intr(TIMER_GROUP_0, TIMER_0);

    esp_intr_alloc(
        ETS_TG0_T0_LEVEL_INTR_SOURCE,
        ESP_INTR_FLAG_IRAM,
        timer_isr,
        NULL,
        &timer_handle
    );

    timer_start(TIMER_GROUP_0, TIMER_0);
    
    while(true){
		if (webo){
			webo = false;
			printf("Hola activado por la ISR\n");
		}
		vTaskDelay(pdMS_TO_TICKS(250));
	}
}