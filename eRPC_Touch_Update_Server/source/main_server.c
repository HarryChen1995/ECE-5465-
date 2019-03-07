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

#include "board.h"
#include "erpc_server_setup.h"
#include "erpc_matrix_multiply_server.h"
#include "erpc_matrix_multiply.h"
#include "erpc_error_handler.h"
#include "fsl_tsi_v4.h"
#include "fsl_lptmr.h"
#include "fsl_tpm.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "MKL25Z4.h"
#include "fsl_port.h"

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUTTON_INIT() GPIO_PinInit(BOARD_SW1_GPIO, BOARD_SW1_GPIO_PIN, &button_config)
#define IS_BUTTON_PRESSED() !GPIO_ReadPinInput(BOARD_SW1_GPIO, BOARD_SW1_GPIO_PIN)

#define BOARD_TPM_BASEADDR TPM2
#define BOARD_FIRST_TPM_CHANNEL 0U
#define BOARD_TPM_CHANNEL 0U

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define TPM_CHANNEL_FLAG kTPM_Chnl0Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM2_IRQn
#define TPM_LED_HANDLER TPM2_IRQHandler

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk/2)
/*******************************************************************************
 * Variables
 ******************************************************************************/
tpm_pwm_level_select_t pwmLevel = kTPM_LowTrue;
volatile uint8_t updatedDutycycle = 0U;
volatile uint8_t getCharValue = 0U;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void erpcTouchUpdate(const uint8_t* percent){
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_FIRST_TPM_CHANNEL, kTPM_EdgeAlignedPwm,
			            		    		 *percent );
	/***update duty cycle ********/
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_CenterAlignedPwm,
	                                   *percent);
	 /* Start channel output with updated dutycycle */
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, pwmLevel);
}

/*!
 * @brief Main function
 */
int main()
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*****initializing PWM   (control brightness of LED1 PORTB_19 )******/
	tpm_config_t tpmInfo;
	tpm_chnl_pwm_signal_param_t tpmParam;

	/* Configure tpm params with frequency 24kHZ */
	tpmParam.chnlNumber = (tpm_chnl_t)BOARD_TPM_CHANNEL;
	tpmParam.level = pwmLevel;
	tpmParam.dutyCyclePercent = updatedDutycycle;


	/* Select the clock source for the TPM counter as kCLOCK_PllFllSelClk */
	CLOCK_SetTpmClock(1U);

	// TPM module init and config
	TPM_GetDefaultConfig(&tpmInfo);
	TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);
	TPM_SetupPwm(BOARD_TPM_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
	TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);

    /* SPI transport layer initialization */
    erpc_transport_t transport;

#if defined(ERPC_BOARD_SPI_BASEADDR)
    transport = erpc_transport_spi_slave_init((void *)ERPC_BOARD_SPI_BASEADDR, ERPC_BOARD_SPI_BAUDRATE,
                                              ERPC_BOARD_SPI_CLK_FREQ);
#elif defined(ERPC_BOARD_DSPI_BASEADDR)
    transport = erpc_transport_dspi_slave_init((void *)ERPC_BOARD_DSPI_BASEADDR, ERPC_BOARD_DSPI_BAUDRATE,
                                               ERPC_BOARD_DSPI_CLK_FREQ);
#endif

    /* MessageBufferFactory initialization */
    erpc_mbf_t message_buffer_factory;
    message_buffer_factory = erpc_mbf_dynamic_init();

    /* eRPC client side initialization */
    erpc_server_init(transport, message_buffer_factory);
    erpc_add_service_to_server(create_TouchUpdateService_service());

    while (1)
    {
        /* process message */
        erpc_status_t status = erpc_server_poll();

        /* handle error status */
        if (status != kErpcStatus_Success)
        {
            erpc_error_handler(status);
            erpc_server_stop();
            break;
        }

        /* do other tasks */
        int i;
        for (i = 0; i < 10000; i++)
        {
        }
    }
    return 0;
}
