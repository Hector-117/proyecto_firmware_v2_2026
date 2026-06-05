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
#include "bsp_board.h"
#include "../components/timer_2026/include/timer_2026.h"
#include "driver/gptimer.h"
#include <inttypes.h> // Required for PRIu32


//gptimer_handle_t my_timer = NULL;


//#define HWREG32TIMER(x)        (*((volatile uint32_t *)(x)))
//#define TIMGn_T0CONFIG_REG 	   (HWREG32TIMER(0x3FF5F000))
//#define TIMGn_T0LO_REG 		   (HWREG32TIMER(0x3FF5F004))
//#define TIMGn_T0HI_REG 		   (HWREG32TIMER(0x3FF5F008))
//#define TIMGn_T0UPDATE_REG     (HWREG32TIMER(0x3FF5F00C))
//#define TIMGn_T0ALARMLO_REG    (HWREG32TIMER(0x3FF5F010))
//#define TIMGn_T0ALARMHI_REG    (HWREG32TIMER(0x3FF5F014))
//#define TIMGn_T0LOADLO_REG     (HWREG32TIMER(0x3FF5F018))
//#define TIMGn_T0LOAD_REG       (HWREG32TIMER(0x3FF5F020))


/*bool timer_callback(gptimer_handle_t timer,
                    const gptimer_alarm_event_data_t *edata,
                    void *user_data)
{
	printf("ikanaaaaaiiddeeeeee\n");
    bsp_RGB_led_toggle(BSP_RGB_BLUELED);
    return false;
}*/


/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
void app_main(void)
{
	ll_timer_enable(LL_TIMG0_T0, false);
	printf("Value enable: %" PRIx32 "\n", *LL_TIMG0_T0CONFIG_REG);
	
	ll_timer_count_mode(LL_TIMG0_T0, LL_UP);
	printf("Value up/down: %" PRIx32 "\n", *LL_TIMG0_T0CONFIG_REG);
	
	ll_timer_autoreload(LL_TIMG0_T0, true);
	printf("Value reload: %" PRIx32 "\n", *LL_TIMG0_T0CONFIG_REG);
	
	ll_set_freq_divider(LL_TIMG0_T0, 80);
	printf("Value divisor: %" PRIx32 "\n", *LL_TIMG0_T0CONFIG_REG);
	
	ll_timer_alarm(LL_TIMG0_T0, true);
	printf("Valu alarme: %" PRIx32 "\n", *LL_TIMG0_T0CONFIG_REG);
	
	ll_timer_int_mode(LL_TIMG0_T0, LL_EDGE);
	printf("Value edge: %" PRIx32 "\n", *LL_TIMG0_T0CONFIG_REG);
	//ojo, debe ser en este orden, si no agrega un 1 de más y aun no se porque
	
	LL_SET_32BIT_REG(LL_TIMG0_T0ALARMLO_REG, 300000);
	LL_SET_32BIT_REG(LL_TIMG0_T0ALARMHI_REG, 0);
	LL_SET_32BIT_REG(LL_TIMG0_T0LOADLO_REG, 0);
	LL_SET_32BIT_REG(LL_TIMG0_T0LOADHI_REG, 0);
	LL_SET_32BIT_REG(LL_TIMG0_T0LOAD_REG, 1);
	
	ll_timer_enable(LL_TIMG0_T0, true);
	
	LL_SET_32BIT_REG(LL_TIMG0_T0UPDATE_REG,1);
	printf("Value: %" PRIu32 "\n", LL_TIMG0_T0LO_REG);
		
	printf("ALARM_LO=%" PRIu32 "\n", *LL_TIMG0_T0ALARMLO_REG);
	printf("ALARM_HI=%" PRIu32 "\n", *LL_TIMG0_T0ALARMHI_REG);
	
	LL_SET_32BIT_REG(LL_TIMG0_T0UPDATE_REG,1);
	printf("CNT=%" PRIu32 " RAW=%d\n",
           		LL_TIMG0_T0LO_REG,
           		LL_READ_STATE_T0_INT_RAW(LL_TIMG0_INT_RAW_REG));
	
	//
	bsp_init();

	
	bool led_state_18 [] = {false, true}; // ESTADOS DEL LED PARA APAGADO Y ENCENDIDO
	bool state_pin18 = true;
	bool state_pin18_old = true;

	bool led_state_19 [] = {false, true}; // ESTADOS DEL LED PARA APAGADO Y ENCENDIDO
	bool state_pin19 = true;
	bool state_pin19_old = true;
	
	LL_SET_32BIT_REG(LL_TIMG0_T0UPDATE_REG,1);
	printf("CNT=%" PRIu32 " RAW=%d\n",
           		LL_TIMG0_T0LO_REG,
           		LL_READ_STATE_T0_INT_RAW(LL_TIMG0_INT_RAW_REG));
	
	while(true){
		printf("ching cheng hanji\n");
		
		printf("Value: %" PRIx32 "\n", *LL_TIMG0_T0CONFIG_REG);//LL_TIMG0_T0CONFIG_REG
		printf("Autoreolad LOW: %" PRIx32 "\n", *LL_TIMG0_T0LOADLO_REG);//LL_TIMG0_T0CONFIG_REG
		printf("Autoreolad HIGH: %" PRIx32 "\n", *LL_TIMG0_T0LOADHI_REG);//LL_TIMG0_T0CONFIG_REG
		printf("ALARM_LO=%" PRIu32 "\n", *LL_TIMG0_T0ALARMLO_REG);
		printf("ALARM_HI=%" PRIu32 "\n", *LL_TIMG0_T0ALARMHI_REG);
		printf("olaywan to sey is da dey donrili ker abaos\n");
		
		LL_SET_32BIT_REG(LL_TIMG0_T0UPDATE_REG,1);
	    printf("fuera: CNT=%" PRIu32 " RAW=%d\n",
           		LL_TIMG0_T0LO_REG,
           		LL_READ_STATE_T0_INT_RAW(LL_TIMG0_INT_RAW_REG));
		
		while(1){
			//LL_SET_32BIT_REG(LL_TIMG0_T0UPDATE_REG,1);
			if(LL_READ_STATE_T0_INT_RAW(LL_TIMG0_INT_RAW_REG))
				{
					printf("===================================\n");
				    //printf("Value: %" PRIx32 "\n", *LL_TIMG0_T0CONFIG_REG);
				    LL_SET_32BIT_REG(LL_TIMG0_T0UPDATE_REG,1);
				    printf("dentro: CNT=%" PRIu32 " RAW=%d\n",
           					LL_TIMG0_T0LO_REG,
           					LL_READ_STATE_T0_INT_RAW(LL_TIMG0_INT_RAW_REG));
           			
           			printf("ALARM_LO=%" PRIu32 "\n", *LL_TIMG0_T0ALARMLO_REG);
					printf("ALARM_HI=%" PRIu32 "\n", *LL_TIMG0_T0ALARMHI_REG);
					printf("Value: %" PRIx32 "\n", *LL_TIMG0_T0CONFIG_REG);
					ll_timer_enable(LL_TIMG0_T0, false);
					ll_timer_alarm(LL_TIMG0_T0, true);
					printf("cbrn no se muevaValue: %" PRIx32 "\n", *LL_TIMG0_T0CONFIG_REG);
					ll_timer_enable(LL_TIMG0_T0, true);
				
				    LL_SET_BIT_T0_INT_CLR(LL_TIMG0_INT_CLR_REG);
				}
			
			/*if(LL_READ_STATE_T0_INT_RAW(LL_TIMG0_INT_RAW_REG)){
		        printf("INT!================================================================\n");
		        LL_SET_BIT_T0_INT_CLR(LL_TIMG0_INT_CLR_REG);
		    }
		    
		    LL_SET_32BIT_REG(LL_TIMG0_T0UPDATE_REG,1);
	    	printf("CNT=%" PRIu32 " RAW=%d\n",
           		LL_TIMG0_T0LO_REG,
           		LL_READ_STATE_T0_INT_RAW(LL_TIMG0_INT_RAW_REG));*/
			
			/*printf("CNT=%" PRIu32 " RAW=%d\n",
           		LL_TIMG0_T0LO_REG,
           		LL_READ_STATE_T0_INT_RAW(LL_TIMG0_INT_RAW_REG));*/
			/*if(LL_READ_STATE_T0_INT_RAW(LL_TIMG0_INT_RAW_REG)){
				LL_SET_BIT_T0_INT_CLR(LL_TIMG0_INT_CLR_REG);
			}*/
			//LL_SET_32BIT_REG(LL_TIMG0_T0UPDATE_REG,1);
	    	//printf("Value: %" PRIu32 "\n", LL_TIMG0_T0LO_REG);
	    	
	    	/*printf("CNT=%" PRIu32 " RAW=%d\n",
           		LL_TIMG0_T0LO_REG,
           		LL_READ_STATE_T0_INT_RAW(LL_TIMG0_INT_RAW_REG));*/
           		
           	//printf("INT_RAW = 0x%08" PRIX32 "\n", *LL_TIMG0_INT_RAW_REG);
	    	
	    	vTaskDelay(pdMS_TO_TICKS(10));
		}
		
		state_pin18 = bsp_pressed_button(BSP_PUSH_BUTTON_0);
		state_pin19 = bsp_pressed_button(BSP_PUSH_BUTTON_1);

		if ((state_pin18 == false) && (state_pin18_old == true)){ //DETECCIÓN DE FALNCO DESCENDENTE EN PIN18
			printf("Boton 1 presionado wo shi zhong guo ren\n");
			led_state_18[0] = !led_state_18[0]; //CAMBIO DE ESTADO DEL LED1
			led_state_18[1] = !led_state_18[1]; //CAMBOI DE ESTADO DEL LED2
		}
		state_pin18_old = state_pin18;

		if ((state_pin19 == false) && (state_pin19_old == true)){ //DETECCIÓN DE FALNCO DESCENDENTE EN PIN18
			printf("Boton 2 presionado sarlacc\n");
			led_state_19[0] = !led_state_19[0]; //CAMBIO DE ESTADO DEL LED1
			led_state_19[1] = !led_state_19[1]; //CAMBOI DE ESTADO DEL LED2
		}
		state_pin19_old = state_pin19;
		
	    /*bsp_led_toggle(BSP_LED0);
	    bsp_led_toggle(BSP_LED1);
	    bsp_led_toggle(BSP_LED2);
	    bsp_led_toggle(BSP_LED3);
	    bsp_led_toggle(BSP_LED4);
	    
	    bsp_RGB_led_toggle(BSP_RGB_REDLED);
	    bsp_RGB_led_toggle(BSP_RGB_GREENLED);
	    bsp_RGB_led_toggle(BSP_RGB_BLUELED);*/
	    	    
	    printf("RAW antes clear = %08" PRIX32 "\n", *LL_TIMG0_INT_RAW_REG);

		LL_SET_BIT_T0_INT_CLR(LL_TIMG0_INT_CLR_REG);
		
		printf("RAW despues clear = %08" PRIX32 "\n", *LL_TIMG0_INT_RAW_REG);
	    
	    //LL_SET_BIT_T0_INT_CLR(LL_TIMG0_INT_CLR_REG);	
	    LL_SET_32BIT_REG(LL_TIMG0_T0UPDATE_REG,1);
	    
	    LL_SET_32BIT_REG(LL_TIMG0_T0UPDATE_REG,1);

		printf("CNT actual = %" PRIu32 "\n",LL_TIMG0_T0LO_REG);
	        
	    while(1){
			printf("no int_detect: CNT=%" PRIu32 " ALARM=%" PRIu32 "\n",
       		LL_TIMG0_T0LO_REG,
       		*LL_TIMG0_T0ALARMLO_REG);
			
			if(*LL_TIMG0_INT_RAW_REG){
		        printf("RAW = %08" PRIX32 "\n", *LL_TIMG0_INT_RAW_REG);
		    }
		    
		    if(LL_READ_STATE_T0_INT_RAW(LL_TIMG0_INT_RAW_REG)){
				printf("ya la detecto: CNT=%" PRIu32 " ALARM=%" PRIu32 "\n",
	       		LL_TIMG0_T0LO_REG,
	       		*LL_TIMG0_T0ALARMLO_REG);
					
				
		        printf("INT!\n");
		        LL_SET_BIT_T0_INT_CLR(LL_TIMG0_INT_CLR_REG);
		        break;
		    }
			
			/*if(LL_READ_STATE_T0_INT_RAW(LL_TIMG0_INT_RAW_REG)){
				LL_SET_BIT_T0_INT_CLR(LL_TIMG0_INT_CLR_REG);
				break;
			}*/
		}
		
		printf("jalooooooooo\n");
	    
	    //vTaskDelay(1000 / portTICK_PERIOD_MS); //RETRAZO DE 5 SEGUNDOS
	}
}