
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
 *
    En caso de la macro "BME280 FLOAT ENABLE" habilitada, las salidas son dobles y las unidades son
      -° C para temperatura
      -% humedad relativa
      -Pa Pascal para presión
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
#include "sdk_hal_i2c1.h"
#include "sdk_hal_adc.h"

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
//uint8_t mensaje_de_texto[]="Hola desde EC25 dtk & jmp";


//sprintf
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

    uint8_t ec25_mensaje_de_texto[]="Hola desde EC25";
	uint8_t estado_actual_ec25;
    uint8_t ec25_detectado=0;

	bme280_data_t bme280_datos;
	uint8_t bme280_detectado=0;
	uint8_t bme280_base_de_tiempo=0;

    
    //inicializa el hardware de la tarjeta
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif

    //inicializa puerto UART0 y solo avanza si es exitoso el proceso
    printf("Inicializa UART0:");
    if(uart0Inicializar(115200)!=kStatus_Success){	//115200bps
    	return 0 ;
    }
    printf("OK\r\n");

    //inicializa puerto I2C0 y solo avanza si es exitoso el proceso
    printf("Inicializa I2C0:");
    if(i2c0MasterInit(100000)!=kStatus_Success){	//100kbps
    	return 0 ;
    }
    printf("OK\r\n");

 
    //inicializa puerto I2C1 y solo avanza si es exitoso el proceso
    printf("Inicializa I0C1:");
    if(i2c1MasterInit(100000)!=kStatus_Success){	//100kbps
    	printf("Error");
    	return 0 ;
    }
    printf("OK\r\n");

#if HABILITAR_SENSOR_BME280
    printf("Detectando BME280:");
    //LLamado a funcion que identifica sensor BME280
    if (bme280WhoAmI() == kStatus_Success){
    	printf("OK\r\n");
    	(void)bme280Init();	//inicializa sensor bme280
    	bme280_detectado=1;	//activa bandera que indica (SI HAY BME280)
    }
#endif

#if HABILITAR_MODEM_EC25
    //Inicializa todas las funciones necesarias para trabajar con el modem EC25
    printf("Inicializa modem EC25\r\n");
    ec25Inicializacion();

    //Configura FSM de modem para enviar mensaje de texto
    printf("Enviando mensaje de texto por modem EC25\r\n");
    ec25EnviarMensajeDeTexto(&ec25_mensaje_de_texto[0], sizeof(ec25_mensaje_de_texto));
#endif

    /*
    //LLamado a funcion que indeitifica acelerometro MMA8451Q
    if (mma8451QWhoAmI() == kStatus_Success){
    	(void)mma8451QInit();	             //inicializa acelerometro MMA8451Q
    }
    */

    //LLamado a funcion que indeitifica al sensor BME280
   //     if (BME280WhoAmI() == kStatus_Success){ // falta identificar donde esta whoAmI para el sensor
     //   	(void)bme280_init();	            //inicializa el sensor BME280
       // }
 /*
    struct bme280_dev dev;
    int8_t rslt = BME280_OK;
    uint8_t dev_addr = BME280_I2C_ADDR_PRIM;

    dev.intf_ptr = &dev_addr;
    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_ms = user_delay_ms;

    rslt = bme280_init(&dev);
*/
#if HABILITAR_MODEM_EC25
    //Inicializa todas las funciones necesarias para trabajar con el modem EC25
    printf("Inicializa modem EC25\r\n");
    ec25Inicializacion();

    //Configura FSM de modem para enviar mensaje de texto
    printf("Enviando mensaje de texto por modem EC25\r\n");
    ec25EnviarMensajeDeTexto(&ec25_mensaje_de_texto[0], sizeof(ec25_mensaje_de_texto));
#endif

	//inicia el SUPERLOOP
    while(1) {

    	waytTime();		//base de tiempo fija aproximadamente 200ms

#if HABILITAR_SENSOR_BME280
    	if(bme280_detectado==1){
    		bme280_base_de_tiempo++;	//incrementa base de tiempo para tomar dato bme280
    		if(bme280_base_de_tiempo>10){	//	>10 equivale aproximadamente a 2s
    			bme280_base_de_tiempo=0; //reinicia contador de tiempo
    			if(bme280ReadData(&bme280_datos)==kStatus_Success){	//toma lectura humedad, presion, temperatura
        			printf("BME280 ->");
    				printf("temperatura:0x%X ",bme280_datos.temperatura);	//imprime temperatura sin procesar
        			printf("humedad:0x%X ",bme280_datos.humedad);	//imprime humedad sin procesar
        			printf("presion:0x%X ",bme280_datos.presion);	//imprime presion sin procesar
        			printf("\r\n");	//Imprime cambio de linea
    			}
    		}
    	}
#endif

#if HABILITAR_MODEM_EC25
		estado_actual_ec25 = ec25Polling();	//actualiza maquina de estados encargada de avanzar en el proceso interno del MODEM
											//retorna el estado actual de la FSM

    	switch(estado_actual_ec25){       //controla color de los LEDs dependiendo de estado modemEC25
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
