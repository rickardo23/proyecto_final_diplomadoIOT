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
#include "sdk_pph_bme280_defs.h"


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


//------------------------------------------------------------------------------
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
 //-------------------------------------------------------------------------------
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
 //-------------------------------------------------------------------------------------------------
status_t bme280ReadData(bme280_data_t *data) {
	status_t status;
	uint8_t i2c_data[8];

	status = bme280ReadByte(&i2c_data[0], 8, BME280_ADDRESS, BME280_REG_PRESS_MSB, 1);
	if (status == kStatus_Success) {
		data->presion = (uint32_t)(i2c_data[0] << 16);
		data->presion |= (uint32_t)(i2c_data[1] << 8);
		data->presion |= (uint32_t)(i2c_data[2]);

		data->temperatura = (uint32_t)(i2c_data[3] << 16);
		data->temperatura |= (uint32_t)(i2c_data[4] << 8);
		data->temperatura |= (uint32_t)(i2c_data[5]);

		data->humedad = (uint32_t)(i2c_data[6] << 8);
		data->humedad |= (uint32_t)(i2c_data[7]);

		return (kStatus_Success);
	}else{
		return (status);
	}
}
  //-----------------------------------------------------------------------------------------------------
/*
static int32_t compensate_temperature(bme280_data_t *dato, struct bme280_calib_data *calib)
{
    int32_t var1;
    int32_t var2;
    int32_t temperatura;
    int32_t temperatura_min = -4000;
    int32_t temperatura_max = 8500;

    var1 = (int32_t)((dato->temperatura / 8) - ((int32_t)calib->dig_t1 * 2));
    var1 = (var1 * ((int32_t)calib->dig_t2)) / 2048;
    var2 = (int32_t)((dato->temperatura / 16) - ((int32_t)calib->dig_t1));
    var2 = (((var2 * var2) / 4096) * ((int32_t)calib->dig_t3)) / 16384;
    calib->t_fine = var1 + var2;
    temperatura = (calib->t_fine * 5 + 128) / 256;

    if (temperatura < temperatura_min)
    {
        temperatura = temperatura_min;
    }
    else if (temperatura > temperatura_max)
    {
        temperatura = temperatura_max;
    }

    dato->temperatura = temperatura;
}
*/
//---------------------------------------------------------------------------------------
/*
static double compensate_temperature(bme280_data_t *dato, struct bme280_calib_data *calib)
{
    double var1;
    double var2;
    double temperatura;
    double temperatura_min = -40;
    double temperatura_max = 85;

    var1 = ((double)dato->temperatura) / 16384.0 - ((double)calib->dig_t1) / 1024.0;
    var1 = var1 * ((double)calib->dig_t2);
    var2 = (((double)dato->temperatura) / 131072.0 - ((double)calib->dig_t1) / 8192.0);
    var2 = (var2 * var2) * ((double)calib->dig_t3);
    calib->t_fine = (int32_t)(var1 + var2);
    temperatura = (var1 + var2) / 5120.0;

    if (temperatura < temperatura_min)
    {
        temperatura = temperatura_min;
    }
    else if (temperatura > temperatura_max)
    {
        temperatura = temperatura_max;
    }

    return temperatura;
}
*/
 //--------------------------------------------------------------------------------------

/*
// Returns temperature in DegC, double precision. Output value of “51.23” equals 51.23 DegC.
// t_fine carries fine temperature as global value
BME280_S32_t t_fine;
double BME280_compensate_T_double(BME280_S32_t adc_T){
double var1, var2, T;
var1 = (((double)adc_T)/16384.0 – ((double)dig_T1)/1024.0) * ((double)dig_T2);
var2 = ((((double)adc_T)/131072.0 – ((double)dig_T1)/8192.0) *
(((double)adc_T)/131072.0 – ((double) dig_T1)/8192.0)) * ((double)dig_T3);
t_fine = (BME280_S32_t)(var1 + var2);
T = (var1 + var2) / 5120.0;
return T;
}
*/
