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
#include <inttypes.h> // Required for PRIu32
#include "esp_intr_alloc.h"

void fase_presentacion();
void rgb_logic();

int contador_pulsaciones;
int count_pulse_btn1;
int count_pulse_btn2;

int div_freq;

bool aux1 = true;
bool aux2 = true;
bool aux3 = true;
bool aux4 = true;
bool aux5 = true;
bool aux6 = true;
bool aux7 = true;

volatile bool flag = false;

void function_boton1(void){
	count_pulse_btn1++;
}

void function_boton2(void){
	count_pulse_btn2++;
}

void function_callback(void){
	flag = true;
}

int btn1, btn2;

void app_main(void)
{
	hal_esp_init();
	hal_set_RGB_color(HAL_BLACK);

	hal_periodic_fun(250000, function_callback);
    
    while(true){
		hal_falling_edge(HAL_USER_BTN0, function_boton1);
		hal_falling_edge(HAL_USER_BTN1, function_boton2);
		printf("btn1 fuera = %d\n", hal_btn_get_actual_state(HAL_USER_BTN0));
		printf("btn2 fuera = %d\n", hal_btn_get_actual_state(HAL_USER_BTN1));
		contador_pulsaciones = count_pulse_btn1 + count_pulse_btn2;
		
		switch (contador_pulsaciones){
			case 0:
				fase_presentacion();
				break;
			case 1:
				if (aux6){
					aux6 = false;
					printf("\nSistema iniciado... \n");
				}
				
				if (flag){
					flag = false;
					div_freq++;
					
					if (div_freq % 2){
						if (aux7){
							aux7 = false;
							hal_status_vector(0x55);
						} else{
							hal_toggle_status_vector();	
						}
					}
				}
				break;
			default:
				hal_toggle_status_vector();
				rgb_logic();
		}
		
		vTaskDelay(pdMS_TO_TICKS(250));
	}
}


void fase_presentacion(){	
	if(aux1){
		aux1 = false;
		printf(	"================================================================================\n"
		   		"		Bienvenido a Proyecto Firmware Ene-jun 2026\n"
		   		"================================================================================\n"
		   		"   Integrantes:\n"
		  		"	* Hector Said Herrera Nino: 22061074\n"
		  		"   * Jose Francisco Padilla Torres: 21061044\n"
		   		"	Materia: Software Embebido\n\n"
		   		"________________________________________________________________________________\n"
		   		"		Instrucciones de operacion\n"
		   		"________________________________________________________________________________\n"
		   		"Para iniciar el sistema debe pulsar cualquier boton\n"
		   		"	a) Si se presiona dos veces el boton 1, el led de color RGB se pone violeta e imprime el estado del led RGB por terminal.\n"
		   		"	b) Si se presiona una vez el boton 2, el led de color se pone azul e imprime el estado del led RGB por terminal.\n"
		  		"	c) Si se presiona dos veces el boton 2, el led de color se pone amarillo e imprime el estado del led RGB por terminal.\n"
		   		"	Si se presiona cualquiera de los dos botones por tercera vez se imprime -Reinciando sistema-, y el algoritmo volvera a empezar.\n"
		   		"________________________________________________________________________________\n"
		   		"presione cualquier boton para iniciar...\n"
				);
	}
}

void rgb_logic(){
	if (aux5){
		aux5 = false;
		printf("btn1 = %d\n", hal_btn_get_actual_state(HAL_USER_BTN0));
		printf("btn2 = %d\n", hal_btn_get_actual_state(HAL_USER_BTN1));
		count_pulse_btn1 = (!hal_btn_get_actual_state(HAL_USER_BTN0)) ? 2 : 1;
		count_pulse_btn2 = (!hal_btn_get_actual_state(HAL_USER_BTN1)) ? 2 : 1;
		//printf("btn1 = %d\n", count_pulse_btn1);
		//printf("btn2 = %d\n", count_pulse_btn2);
	}
	
	if ((count_pulse_btn1 == 4) || (count_pulse_btn2 == 4)){
		printf("\nReiniciando sistema\n\n");
		hal_status_vector(0x00);
		hal_set_RGB_color(HAL_BLACK);
		contador_pulsaciones = 0;
		count_pulse_btn1 = 0;
		count_pulse_btn2 = 0;
		aux1 = true;
		aux2 = true;
		aux3 = true;
		aux4 = true;
		aux5 = true;
		aux6 = true;
		aux7 = true;	
	}
	
	else if ((count_pulse_btn1) == 3 && aux2){ //Color violeta
		aux2 = false;
		printf("Color violeta\n");
		hal_set_RGB_color(HAL_MAGENTA);
	}
	else if ((count_pulse_btn2) == 2 && aux3){ //Color azul
		aux3 = false;
		printf("Color azul\n");
		hal_set_RGB_color(HAL_BLUE);
	}
	else if((count_pulse_btn2) == 3 && aux4){ //Color amarillo
		aux4 = false;
		printf("Color amarillo\n");
		hal_set_RGB_color(HAL_YELLOW);
	}
	
}