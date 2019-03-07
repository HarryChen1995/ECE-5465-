/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <erpc_touch_client.h>
#include "board.h"
#include "erpc_client_setup.h"
#include "erpc_error_handler.h"
#include "erpc_matrix_multiply.h"
#include "fsl_debug_console.h"
#include "fsl_tsi_v4.h"
#include "fsl_lptmr.h"
#include "fsl_tpm.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "MKL25Z4.h"
#include "fsl_port.h"
#include <stdlib.h>

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Available PAD names on board */
#define PAD_TSI_ELECTRODE_1_NAME "E1"

/* TSI leds of electrode 1/2 */
/* TSI indication leds for electrode 1/2 */
#define LED_INIT() LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE() LED_RED_TOGGLE()

/* Get source clock for LPTMR driver */
#define LPTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)
/* Define LPTMR microseconds counts value */
#define LPTMR_USEC_COUNT (300000U)

/* Define the delta value to indicate a touch event */
#define TOUCH_DELTA_VALUE 100U

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk/2)

/*******************************************************************************
 * Variables
 ******************************************************************************/

// erpc error flag
extern bool g_erpc_error_occurred;

// Stores raw readings from TSI
tsi_calibration_data_t buffer;

//electrode values
uint16_t readings[] = {0, 0};

//update duty cycle and other misc
volatile uint8_t updatedDutycycle = 0U;
volatile uint8_t getCharValue = 0U;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/



// Called each time the TSI is sampled.
void TSI0_IRQHandler(void)
{
	uint8_t absPercentPos= 0U;
    if (TSI_GetMeasuredChannelNumber(TSI0) == BOARD_TSI_ELECTRODE_1)
    {
    	readings[0] = TSI_GetCounter(TSI0) - buffer.calibratedData[BOARD_TSI_ELECTRODE_1];
    	TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_2);
    }
    else if (TSI_GetMeasuredChannelNumber(TSI0) == BOARD_TSI_ELECTRODE_2)
    {
    	//grab electrode reading
    	readings[1] = TSI_GetCounter(TSI0) - buffer.calibratedData[BOARD_TSI_ELECTRODE_2];

    	//PRINTF("Electrode 1: %d; Electrode 2: %d", readings[0], readings[1]);
    	if (readings[0] + readings[1] > 500)
    	{
			uint16_t percentPos1 = readings[0]*100/(readings[0]+readings[1]);
			uint16_t percentPos2 = readings[1]*100/(readings[0]+readings[1]);

		    absPercentPos = ((100-percentPos1) + percentPos2)/2;
		    updatedDutycycle=absPercentPos;

		    //send request to server
		    //erpcTouchUpdate(&absPercentPos);

			PRINTF("Distance Percentage: %d\n", absPercentPos);
			/*************disable channel output before updating duty cycle*********/
      		TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_1);
    	}
    }

    /* Clear flags */
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
}

/*!
 * @brief Main function
 */
int main()
{
	volatile uint32_t i = 0;
	tsi_config_t tsiConfig_normal = {0};

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    lptmr_config_t lptmrConfig;
	memset((void*)&lptmrConfig, 0, sizeof(lptmrConfig));

	/* Initialize standard SDK demo application pins */
	LPTMR_GetDefaultConfig(&lptmrConfig);
	/* TSI default hardware configuration for normal mode */
	TSI_GetNormalModeDefaultConfig(&tsiConfig_normal);

	/* Initialize the TSI */
	TSI_Init(TSI0, &tsiConfig_normal);
	LPTMR_Init(LPTMR0, &lptmrConfig);
	LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(5000U,LPTMR_SOURCE_CLOCK ));

	NVIC_EnableIRQ(TSI0_IRQn);
	TSI_EnableModule(TSI0, true); /* Enable module */

	PRINTF("\r\nRunning calibration. Don't touch!\r\n");

	/*********  CALIBRATION PROCESS ************/
	memset((void *)&buffer, 0, sizeof(buffer));
	TSI_Calibrate(TSI0, &buffer);
	/* Print calibrated counter values */
	for ( i = 0U; i < FSL_FEATURE_TSI_CHANNEL_COUNT; i++)
	{
		PRINTF("Calibrated counters for channel %d is: %d \r\n", i, buffer.calibratedData[i]);
	}

    /********** HARDWARE TRIGGER SCAN ********/
    TSI_EnableModule(TSI0, false);
    TSI_EnableHardwareTriggerScan(TSI0, true);
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_1);
    TSI_EnableModule(TSI0, true);
    LPTMR_StartTimer(LPTMR0);

    /* SPI transport layer initialization */
    erpc_transport_t transport;
#if defined(ERPC_BOARD_SPI_BASEADDR)
    transport = erpc_transport_spi_master_init((void *)ERPC_BOARD_SPI_BASEADDR, ERPC_BOARD_SPI_BAUDRATE,
                                               ERPC_BOARD_SPI_CLK_FREQ);
#elif defined(ERPC_BOARD_DSPI_BASEADDR)
    transport = erpc_transport_dspi_master_init((void *)ERPC_BOARD_DSPI_BASEADDR, ERPC_BOARD_DSPI_BAUDRATE,
                                                ERPC_BOARD_DSPI_CLK_FREQ);
#endif

    /* MessageBufferFactory initialization */
    erpc_mbf_t message_buffer_factory;
    message_buffer_factory = erpc_mbf_dynamic_init();

    /* eRPC client side initialization */
    erpc_client_init(transport, message_buffer_factory);
    erpc_client_set_error_handler(erpc_error_handler);

    while (1)
    {
    }
    return 0;
}
