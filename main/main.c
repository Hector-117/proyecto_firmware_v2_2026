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
#include "hal.h"
#include "esp_task_wdt.h"

void fase_presentacion();
void rgb_logic();

/**
 * @brief Global counters used by the application.
 */
int contador_pulsaciones;
int count_pulse_btn1;
int count_pulse_btn2;

int div_freq;

/**
 * @brief Auxiliary flags used by the state machine.
 */
bool aux1 = true;
bool aux2 = true;
bool aux3 = true;
bool aux4 = true;
bool aux5 = true;
bool aux6 = true;
bool aux7 = true;

volatile bool flag = false;

/**
 * @brief Callback executed when button 1 is pressed.
 *
 * @details
 * Increments the button 1 press counter.
 */
void function_boton1(void){
	count_pulse_btn1++;
}

/**
 * @brief Callback executed when button 2 is pressed.
 *
 * @details
 * Increments the button 2 press counter.
 */
void function_boton2(void){
	count_pulse_btn2++;
}

/**
 * @brief Timer callback function.
 *
 * @details
 * Sets a flag that is later processed
 * inside the main application loop.
 */
void function_callback(void){
	flag = true;
}


/**
 * @brief Main application entry point.
 *
 * @details
 * Initializes the HAL, configures the periodic timer
 * callback and executes the application state machine.
 *
 * States:
 * - Presentation phase.
 * - System initialization phase.
 * - RGB control phase.
 */
int btn1, btn2;
void app_main(void)
{
	esp_task_wdt_deinit();
	hal_esp_init();
	hal_set_RGB_color(HAL_BLACK);

	hal_periodic_fun(250000, function_callback);
    
    while(true){
		hal_falling_edge(HAL_USER_BTN0, function_boton1);
		hal_falling_edge(HAL_USER_BTN1, function_boton2);
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
		
		hal_delay(250000);
		//vTaskDelay(pdMS_TO_TICKS(250));
	}
}


/**
 * @brief Presentation phase of the application.
 *
 * @details
 * Displays project information and instructions
 * through the serial terminal. This function is
 * executed only once at startup.
 */
void fase_presentacion(){	
	if(aux1){
		aux1 = false;
		printf(	"================================================================================\n"
		   		"		Bienvenido a Proyecto Firmware Ene-jun 2026 V14\n"
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


/**
 * @brief RGB control state machine.
 *
 * @details
 * Evaluates the number of button presses and
 * selects the corresponding RGB color.
 *
 * Functions:
 * - Magenta color.
 * - Blue color.
 * - Yellow color.
 * - System restart.
 */
void rgb_logic(){
	if (aux5){
		aux5 = false;
		count_pulse_btn1 = (!hal_btn_get_actual_state(HAL_USER_BTN0)) ? 2 : 1;
		count_pulse_btn2 = (!hal_btn_get_actual_state(HAL_USER_BTN1)) ? 2 : 1;
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