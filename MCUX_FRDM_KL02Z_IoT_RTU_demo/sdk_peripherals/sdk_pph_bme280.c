/*! @file : sdk_pph_bme280.c
 * @author  Ernesto Andres Rincon Cruz
 * @version 1.0.0
 * @date    9/02/2021
 * @brief   Driver para sensor de temperatura BME280
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sdk_pph_bme280.h"
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


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
//------------------------------------
 status_t bme280WhoAmI(void) {
	status_t status;
	uint8_t i2c_data;

	status = bme280ReadByte(&i2c_data, 1, BME280_ADDRESS, BME280_REG_ID, 1);

	if (status == kStatus_Success) {
		if (i2c_data == 0x60)
			return (kStatus_Success);
		else
			return (kStatus_Fail);
	} else {
		return (status);
	}
}
 //------------------------------------
 status_t bme280Init(void){
 	status_t status;
 	uint8_t i2c_data;

    /*  write 00000 101 = 0x05 to CTRL_HUM */
 	/*  [2-0] = 101	humidity oversampling x 16*/
 	i2c_data = 0x05;
 	status = bme280WriteByte(&i2c_data, 1, BME280_ADDRESS, BME280_REG_CTRL_HUM, 1);

    /*  write 101 101 11 = 0xB7 to CTRL_MEAS */
 	/*  [7-5] = 101 temperature oversampling x 16*/
 	/*  [4-2] = 101	pressure oversampling x 16*/
 	/*  [1-0] = 11 	Normal Mode*/
 	i2c_data = 0xB7;
 	status = bme280WriteByte(&i2c_data, 1, BME280_ADDRESS, BME280_REG_CTRL_MEAS, 1);

 	if(status!=kStatus_Success)
 		return(status);

 	return(kStatus_Success);
 }
 //------------------------------------
status_t bme280ReadData(bme280_data_t *data) {
	status_t status;
	uint8_t i2c_data[10];

	status = bme280ReadByte(&i2c_data[0], 8, BME280_ADDRESS, BME280_REG_PRESS_MSB, 1);
	status = bme280ReadByte(&i2c_data[1], 8, BME280_ADDRESS, BME280_REG_PRESS_lSB, 1);
	status = bme280ReadByte(&i2c_data[2], 8, BME280_ADDRESS, BME280_REG_PRESS_XlSB, 1);
	status = bme280ReadByte(&i2c_data[3], 8, BME280_ADDRESS, BME280_REG_TEMP_MSB, 1);
	status = bme280ReadByte(&i2c_data[4], 8, BME280_ADDRESS, BME280_REG_TEMP_lSB, 1);
	status = bme280ReadByte(&i2c_data[5], 8, BME280_ADDRESS, BME280_REG_TEMP_XlSB, 1);
	status = bme280ReadByte(&i2c_data[6], 8, BME280_ADDRESS, BME280_REG_HUM_MSB, 1);
	status = bme280ReadByte(&i2c_data[7], 8, BME280_ADDRESS, BME280_REG_HUM_lSB, 1);
	status = bme280ReadByte(&i2c_data[8], 8, BME280_ADDRESS, BME280_REG_STATUS, 1);
	status = bme280ReadByte(&i2c_data[9], 8, BME280_ADDRESS, BME280_REG_CONFIG, 1);

	if (status == kStatus_Success)
	{
		data->presion = (((uint32_t)(i2c_data[0])<<12)|((uint32_t)(i2c_data[1])<<4)|(uint32_t)(i2c_data[2])>>4);

		data->temperatura = (((uint32_t)(i2c_data[3])<<12)|((uint32_t)(i2c_data[4])<<4)|(uint32_t)(i2c_data[5])>>4);

		data->TempMSB = (uint32_t)i2c_data[3];

		data->TempLSB = (uint32_t)i2c_data[4];

		data->TempXLSB = (uint32_t)i2c_data[5];

		data->humedad = (((uint32_t)(i2c_data[6])<<8)|(uint32_t)(i2c_data[7]));

		data->status = (uint32_t)i2c_data[8];

		data->config = (uint32_t)i2c_data[9];

		return (kStatus_Success);
	}else{
		return (status);
	}
}

