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

#define TIMG0_T0CONFIG_REG   (*((volatile uint32_t *)(0x3FF5F000)))
#define TIMGn_T0ALARMLO_REG  (*((volatile uint32_t *)(0x3FF5F010)))
#define TIMGn_T0ALARMHI_REG  (*((volatile uint32_t *)(0x3FF5F014)))
#define TIMGn_T0LOADLO_REG   (*((volatile uint32_t *)(0x3FF5F018)))
#define TIMGn_T0LOADHI_REG   (*((volatile uint32_t *)(0x3FF5F01C)))
#define TIMGn_T0LOAD_REG     (*((volatile uint32_t *)(0x3FF5F020)))
#define TIMGn_T0UPDATE_REG   (*((volatile uint32_t *)(0x3FF5F00C)))
#define TIMGn_T0LO_REG       (*((volatile uint32_t *)(0x3FF5F004)))
#define TIMGn_Tx_INT_RAW_REG (*((volatile uint32_t *)(0x3FF5F09C)))

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
void app_main(void)
{
	//Ver configuracion inicial del registro
	printf("Registro config(default): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	
	//configurar no enable
	TIMG0_T0CONFIG_REG &= ~(1<<31);
	printf("Registro config(enable desactivado): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	
	//configurar modo ascendente
	TIMG0_T0CONFIG_REG |= (1<<30);
	printf("Registro config(modo ascendente): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	
	//configurar modo autoreload
	TIMG0_T0CONFIG_REG |= (1<<29);
	printf("Registro config(modo Autoreolad): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);

	//configurar modo divisor de frecuencia
	TIMG0_T0CONFIG_REG &= ~(0xFFFF<<13);
	TIMG0_T0CONFIG_REG |= (80<<13);
	printf("Registro config(divisor de freq): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);

	//configurar la alarma
	TIMG0_T0CONFIG_REG |= (1<<10);
	printf("Registro config(enable alarma): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	
	//configurar modo de interrupcion
	TIMG0_T0CONFIG_REG &= ~(1<<11); //Tumbar el level
	TIMG0_T0CONFIG_REG |= (1<<12); //subir el edge
	printf("Registro config(modo int): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
	
	TIMGn_T0ALARMLO_REG = 300000;
	printf("Registro TIMGn_T0ALARMLO_REG: %" PRIx32 "\n", TIMGn_T0ALARMLO_REG);

	TIMGn_T0ALARMHI_REG = 0;
	printf("Registro TIMGn_T0ALARMHI_REG: %" PRIx32 "\n", TIMGn_T0ALARMHI_REG);

	TIMGn_T0LOADLO_REG = 0;
	printf("Registro TIMGn_T0LOADLO_REG: %" PRIx32 "\n", TIMGn_T0LOADLO_REG);

	TIMGn_T0LOADHI_REG = 0;
	printf("Registro TIMGn_T0LOADHI_REG: %" PRIx32 "\n", TIMGn_T0LOADHI_REG);

	TIMGn_T0LOAD_REG = 1;
	printf("Registro TIMGn_T0LOAD_REG: %" PRIx32 "\n", TIMGn_T0LOAD_REG);

	//Finalmente habilitalo
	TIMG0_T0CONFIG_REG |= (1<<31);
	printf("Registro config(enable desactivado): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);

	//Configuracion final
	printf("Registro config(final): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);

	bsp_init();
	
	bool led_state_18 [] = {false, true}; // ESTADOS DEL LED PARA APAGADO Y ENCENDIDO
	bool state_pin18 = true;
	bool state_pin18_old = true;

	bool led_state_19 [] = {false, true}; // ESTADOS DEL LED PARA APAGADO Y ENCENDIDO
	bool state_pin19 = true;
	bool state_pin19_old = true;

	TIMGn_T0UPDATE_REG = 1;
	printf("CNT=%" PRIu32 " RAW=%" PRIu32, TIMGn_T0LO_REG, TIMGn_Tx_INT_RAW_REG);
	
	while(true){
		printf("Programa prueba RAW\n");

		//Checar que no haya cambios en la configuracion
		printf("Registro config(final): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);

		while(1){
			//LL_SET_32BIT_REG(LL_TIMG0_T0UPDATE_REG,1);
			if(TIMGn_Tx_INT_RAW_REG)
				{
					printf("===================================\n");
				    //Checar que no haya cambios en la configuracion
					printf("Registro config(final): %" PRIx32 "\n", TIMG0_T0CONFIG_REG);
				    TIMGn_T0UPDATE_REG = 1;
					printf("CNT=%" PRIu32 " RAW=%" PRIu32, TIMGn_T0LO_REG, TIMGn_Tx_INT_RAW_REG);
           			
				}    	
	    	vTaskDelay(pdMS_TO_TICKS(10));
		}
		
		/*state_pin18 = bsp_pressed_button(BSP_PUSH_BUTTON_0);
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
		
		printf("jalooooooooo\n");*/
	    
	}
}