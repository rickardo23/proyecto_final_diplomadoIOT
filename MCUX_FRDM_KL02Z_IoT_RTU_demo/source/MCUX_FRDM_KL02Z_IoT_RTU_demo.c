
/*! @file : MCUX_FRDM_KL02Z_IoT_RTU_demo.c
 * @author  Ernesto Andres Rincon Cruz
 * @version 1.0.0
 * @date    8/01/2021
 * @brief   Funcion principal main
 * @details
 *
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"

#include "sdk_hal_uart0.h"
#include "sdk_hal_gpio.h"
#include "sdk_hal_i2c0.h"

#include "sdk_mdlw_leds.h"
#include "sdk_pph_ec25au.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Local vars
 ******************************************************************************/

uint8_t mensaje_de_texto[]="Hola desde EC25_dtk";

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
void waytTime(void) {
	uint32_t tiempo = 0xFFFFF;
	do {
		tiempo--;
	} while (tiempo != 0x0000);
}


/*
 * @brief   Application entry point.
 */
int main(void) {
	uint8_t estado_actual_ec25;

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif
    //inicializa puerto UART0 y solo avanza si es exitoso el proceso
    if(uart0Inicializar(115200)!=kStatus_Success){	//115200bps
    	return 0 ;
    }

    //inicializa puerto I2C0 y solo avanza si es exitoso el proceso
    if(i2c0MasterInit(100000)!=kStatus_Success){	//100kbps
    	return 0 ;
    }


    //inicializa todas las funciones necesarias para trabajar con el modem EC25
    ec25Inicializacion();
    ec25EnviarMensajeDeTexto(&mensaje_de_texto[0], sizeof(mensaje_de_texto));

	//Ciclo infinito encendiendo y apagando led verde
	//inicia el SUPERLOOP
    while(1) {
    	waytTime();		//base de tiempo fija aproximadamente 200ms

		estado_actual_ec25 = ec25Polling();	//actualiza maquina de estados encargada de avanzar en el proceso interno del MODEM
											//retorna el estado actual de la FSM

    	switch(estado_actual_ec25){
    	case kFSM_RESULTADO_ERROR:
    		toggleLedRojo();
    		apagarLedVerde();
    		apagarLedAzul();
    		break;

    	case kFSM_RESULTADO_EXITOSO:
    		apagarLedRojo();
    		toggleLedVerde();
    		apagarLedAzul();
    		break;

    	case kFSM_RESULTADO_ERROR_RSSI:
    		toggleLedRojo();
    		apagarLedVerde();
    		toggleLedAzul();
    		break;

    	default:
    		apagarLedRojo();
    		apagarLedVerde();
    		toggleLedAzul();
    		break;
    	}
    }

    return 0 ;
}
