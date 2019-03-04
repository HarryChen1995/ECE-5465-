#include "board.h"
#include "fsl_tsi_v4.h"
#include "fsl_debug_console.h"
#include "fsl_lptmr.h"
#include "fsl_tpm.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "MKL25Z4.h"
#include "fsl_port.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Available PAD names on board */
#define PAD_TSI_ELECTRODE_1_NAME "E1"
#define BOARD_FIRST_TPM_CHANNEL 0U

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

#define BOARD_TPM_BASEADDR TPM2
#define BOARD_TPM_CHANNEL 1U

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl1InterruptEnable
#define TPM_CHANNEL_FLAG kTPM_Chnl1Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM2_IRQn
#define TPM_LED_HANDLER TPM2_IRQHandler

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk/2)


tsi_calibration_data_t buffer;

// Stores raw readings from TSI
uint16_t readings[] = {0, 0};

/*******************************************************************************
 * Code
 ******************************************************************************/
gpio_pin_config_t config =
{
		kGPIO_DigitalOutput,
		0,
};




tpm_pwm_level_select_t pwmLevel = kTPM_LowTrue;
volatile uint8_t updatedDutycycle = 0U;
volatile uint8_t getCharValue = 0U;


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
    	readings[1] = TSI_GetCounter(TSI0) - buffer.calibratedData[BOARD_TSI_ELECTRODE_2];

    	//PRINTF("Electrode 1: %d; Electrode 2: %d", readings[0], readings[1]);
    	if (readings[0] + readings[1] > 400)
    	{
			uint16_t percentPos1 = readings[0]*100/(readings[0]+readings[1]);
			uint16_t percentPos2 = readings[1]*100/(readings[0]+readings[1]);

		    absPercentPos = ((100-percentPos1) + percentPos2)/2;
		    updatedDutycycle=absPercentPos;
		    TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_FIRST_TPM_CHANNEL, kTPM_EdgeAlignedPwm,
		            		    		 updatedDutycycle );
			PRINTF("Distance Percentage: %d\n", absPercentPos);
			/*************disable channel output before updating duty cycle*********/
      		TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_1);
      		/***update duty cycle ********/
      		TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_CenterAlignedPwm,
                                   updatedDutycycle);
         	 /* Start channel output with updated dutycycle */
            TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, pwmLevel);


    	}




    }





    /* Clear flags */
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
}




int main(void)
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



	/*****initializing PWM   (control brightness of LED1 PORTB_19 )******/
	tpm_config_t tpmInfo;
	tpm_chnl_pwm_signal_param_t tpmParam;

	/* Configure tpm params with frequency 24kHZ */
	tpmParam.chnlNumber = (tpm_chnl_t)BOARD_TPM_CHANNEL;
	tpmParam.level = pwmLevel;
	tpmParam.dutyCyclePercent = updatedDutycycle;


	/* Select the clock source for the TPM counter as kCLOCK_PllFllSelClk */
	CLOCK_SetTpmClock(1U);


	TPM_GetDefaultConfig(&tpmInfo);
	/* Initialize TPM module */
	TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);

	TPM_SetupPwm(BOARD_TPM_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);

	TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);

        while (1)
        {




        }
}











