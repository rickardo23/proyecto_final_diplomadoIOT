
/*! @file : MCUX_FRDM_KL02Z_IoT_RTU_demo.c
 * @author  Ernesto Andres Rincon Cruz
 * @version 1.0.0
 * @date    8/01/2021
 * @brief   Funcion principal main
 * @details
 *			v0.1 dato recibido por puerto COM es contestado en forma de ECO
 *			v0.2 dato recibido por puerto COM realiza operaciones especiales
 *					A/a=invierte estado de LED conectado en PTB10
 *					v=apaga LED conectado en PTB7
 *					V=enciende LED conectado en PTB7
 *					r=apaga LED conectado en PTB6
 *			v0.3 nuevo comando por puerto serial para prueba de MMA8451Q
 *					M=detecta acelerometro MM8451Q en bus I2C0
 *
 *
 */

/*
 *  bme280_defs.h: este archivo de encabezado tiene las constantes, macros y declaraciones de tipos de datos.
 *  bme280.h: este archivo de encabezado contiene las declaraciones de las API del controlador del sensor.
 *  bme280.c: este archivo fuente contiene las definiciones de las API del controlador del sensor.
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
#include "sdk_pph_mma8451Q.h"
#include "sdk_pph_ec25au.h"
#include "sdk_pph_bme280.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define  BME280_I2C_config        0xF5   //registro para configurar el BME280
#define  BME280_I2C_ID		      0xD0   //registro para identificar al sensor, El valor de lectura es 0x60

#define BME280_temp_lsb         0xFB   //registros para leer los datos (8 bits) de la temperatura de MSB y LSB.
#define BME280_temp_msb         0xFA
//#define BME280_temp_xlsb        0xFC
#define BME280_hum_lsb          0xFE    //registros para leer los datos (8 bits) de la Humedad de MSB y LSB.
#define BME280_hum_msb          0xFD
//#define BME280_press_xlsb       0xF9
#define BME280_press_lsb        0xF8    //registros para leer los datos (8 bits) de  la presion de MSB y LSB.
#define BME280_press_msb        0xF7



/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Local vars
 ******************************************************************************/
uint8_t mensaje_de_texto[]="Hola desde EC25_dtk_&_jmp";

/*
 * variables que almacenan los datos de cada uno respectivamente
 * pressure, temperature, humidity
 */



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

     //por darle uso
	//status_t status;
	//uint8_t nuevo_byte_uart;
	//uint8_t	nuevo_dato_i2c;
	//uint16_t Registro_i2c0_MSB;    //variable para guardar la parte mas significativa
    //uint16_t Registro_i2c0_LSB;    //variable para guardar la parte menos significativa
    //int16_t Dato_Final ;           //variable para guardar la union de MSB y LSB

    //inicializa el hardware de la tarjeta
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

    /*
    //LLamado a funcion que indeitifica acelerometro MMA8451Q
    if (mma8451QWhoAmI() == kStatus_Success){
    	(void)mma8451QInit();	//inicializa acelerometro MMA8451Q
    }
    */

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
