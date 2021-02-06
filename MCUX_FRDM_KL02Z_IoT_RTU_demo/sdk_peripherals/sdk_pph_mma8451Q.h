/*! @file : sdk_pph_mma8460.h
 * @author  Ernesto Andres Rincon Cruz
 * @version 1.0.0
 * @date    17/01/2021
 * @brief   Driver para MMA8451Q
 * @details
 *
 */
#ifndef SDK_PPH_MMA8451Q_H_
#define SDK_PPH_MMA8451Q_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sdk_hal_i2c0.h"

/*!
 * @addtogroup PERIPHERAL
 * @{
 */
/*!
 * @addtogroup MMA8451Q
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/

#define MMA8451_ADDRESS  (0x1D) //!< Default MMA8451 I2C address, if SA0 is GND, its 0x1C

#define REG_STATUS  0x00 //(R) Real time status
#define REG_OUT_X_MSB  0x01 //(R) [7:0] are 8 MSBs of 14-bit sample
#define REG_OUT_X_LSB  0x02 //(R) [7:2] are 2 LSBs of 14-bit sample
#define REG_OUT_Y_MSB  0x03 //(R) [7:0] are 8 MSBs of 14-bit sample
#define REG_OUT_Y_LSB  0x04 //(R) [7:2] are 2 LSBs of 10-bit sample
#define REG_OUT_Z_MSB  0x05 //(R) [7:0] are 8 MSBs of 10-bit sample
#define REG_OUT_Z_LSB  0x06 //(R) [7:2] are 2 LSBs of 10-bit sample
#define REG_F_SETUP  0x09 //FIFO Setup
#define REG_TRIG_CFG  0x0a;
#define REG_SYSMOD  0x0b //(R) Current system mode
#define REG_INT_SOURCE  0x0c //(R) Interrupt status
#define REG_WHO_AM_I  0x0d //(R) Device ID (0x1A)
#define REG_XYZ_DATA_CFG  0xe //(R/W) Dynamic range settings
#define REG_HP_FILTER_CUTOFF  0x0f //(R/W) cut-off frequency is set to 16Hz @ 800Hz
#define REG_PL_STATUS  0x10 //(R) Landscape/Portrait orientation status
#define REG_PL_CFG  0x11 //(R/W) Landscape/Portrait configuration
#define REG_PL_COUNT  0x12 //(R) Landscape/Portrait debounce counter
#define REG_PL_BF_ZCOMP  0x13 //(R) Back-Front, Z-Lock trip threshold
#define REG_P_L_THS_REG  0x14 //(R/W) Portrait to Landscape trip angle is 29 degree
#define REG_FF_MT_CFG  0x15 //(R/W) Freefall/motion functional block configuration
#define REG_FF_MT_SRC  0x16 //(R) Freefall/motion event source register
#define REG_FF_MT_THS  0x17 //(R/W) Freefall/motion threshold register
#define REG_FF_MT_COUNT  0x18 //(R/W) Freefall/motion debounce counter
#define REG_TRANSIENT_CFG  0x1d //(R/W) Transient functional block configuration
#define REG_TRANSIENT_SRC  0x1e //(R) Transient event status register
#define REG_TRANSIENT_THS  0x1f //(R/W) Transient event threshold
#define REG_TRANSIENT_COUNT  0x20 //(R/W) Transient debounce counter
#define REG_PULSE_CFG  0x21 //(R/W) ELE, Double_XYZ or Single_XYZ
#define REG_PULSE_SRC  0x22 //(R) EA, Double_XYZ or Single_XYZ
#define REG_PULSE_THSX  0x23 //(R/W) X pulse threshold
#define REG_PULSE_THSY  0x24 //(R/W) Y pulse threshold
#define REG_PULSE_THSZ  0x25 //(R/W) Z pulse threshold
#define REG_PULSE_TMLT  0x26 //(R/W) Time limit for pulse
#define REG_PULSE_LTCY  0x27 //(R/W) Latency time for 2nd pulse
#define REG_PULSE_WIND  0x28 //(R/W) Window time for 2nd pulse
#define REG_ASLP_COUNT  0x29 //(R/W) Counter setting for auto-sleep
#define REG_CTRL_REG1  0x2a //(R/W) ODR  800 Hz, STANDBY mode
#define REG_CTRL_REG2  0x2b //(R/W) Sleep enable, OS Modes, RST, ST
#define REG_CTRL_REG3  0x2c //(R/W) Wake from sleep, IPOL, PP_OD
#define REG_CTRL_REG4  0x2d //(R/W) Interrupt enable register
#define REG_CTRL_REG5  0x2e //(R/W) Interrupt pin (INT1/INT2) map
#define REG_OFF_X  0x2f //(R/W) X-axis offset adjust
#define REG_OFF_Y  0x30 //(R/W) Y-axis offset adjust
#define REG_OFF_Z  0x31 //(R/W) Z-axis offset adjust


/*!
 * @brief The MMA8451 three-axis accelerometer data structure
 *
 */
typedef struct _mma8451_data{
	uint8_t  status;
	uint16_t x_value;
	uint16_t y_value;
	uint16_t z_value;
}mma8451_data_t;
/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
/*!
 * @brief read Who Am I register for MMA8451Q detection
 *
 * @return	execution error code
 * @code
 * 		kStatus_Success
 * 		kStatus_Fail
 * @endcode
 */
status_t mma8451QWhoAmI(void);

/*!
 * @brief read status and 3-axis accel registers from MMA8451Q
 * @param data	data structure pointer
 * @return	execution error code
 * @code
 * 		kStatus_Success
 * 		kStatus_Fail
 * @endcode
 */
status_t	mma8451QReadAccel(mma8451_data_t *data );

/*!
 * @brief perform initialization of MMA8451Q
 * @return	execution error code
 * @code
 * 		kStatus_Success
 * 		kStatus_Fail
 * @endcode
 */
status_t mma8451QInit(void);

/** @} */ // end of MMA8451Q group
/** @} */ // end of PERIPHERAL group

#endif /* SDK_PPH_MMA8451Q_H_ */
