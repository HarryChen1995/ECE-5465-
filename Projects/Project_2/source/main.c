#include "board.h"
#include "fsl_tsi_v4.h"
#include "fsl_debug_console.h"
#include "fsl_lptmr.h"

#include "clock_config.h"
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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
tsi_calibration_data_t buffer;

// Stores raw readings from TSI
uint16_t readings[] = {0, 0};

/*******************************************************************************
 * Code
 ******************************************************************************/

// Called each time the TSI is sampled.
void TSI0_IRQHandler(void)
{
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

			uint16_t absPercentPos = ((100-percentPos1) + percentPos2)/2;
			PRINTF("Distance Percentage: %d\n", absPercentPos);
			TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_1);
    	}
    	else
    	{
    		PRINTF("No touch.\n");
    	}
    }

    /* Clear flags */
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
}

/*!
 * @brief Main function
 *
 * Algorithm is run calibration, setup TSI interrupt, then loop indefinitely.
 */
int main(void)
{
    volatile uint32_t i = 0;
    tsi_config_t tsiConfig_normal = {0};

    /* Initialize standard SDK demo application pins */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* TSI default hardware configuration for normal mode */
    TSI_GetNormalModeDefaultConfig(&tsiConfig_normal);

    /* Initialize the TSI */
    TSI_Init(TSI0, &tsiConfig_normal);

    NVIC_EnableIRQ(TSI0_IRQn);
    TSI_EnableModule(TSI0, true); /* Enable module */

    PRINTF("\r\nRunning calibration. Don't touch!\r\n");

    /*********  CALIBRATION PROCESS ************/
    memset((void *)&buffer, 0, sizeof(buffer));
    TSI_Calibrate(TSI0, &buffer);
    /* Print calibrated counter values */
    for (i = 0U; i < FSL_FEATURE_TSI_CHANNEL_COUNT; i++)
    {
        PRINTF("Calibrated counters for channel %d is: %d \r\n", i, buffer.calibratedData[i]);
    }

    /********** HARDWARE TRIGGER SCAN ********/
    TSI_EnableModule(TSI0, false);
    TSI_EnableHardwareTriggerScan(TSI0, true);
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    /* Select BOARD_TSI_ELECTRODE_1 as detecting electrode. */
    TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_1);
    TSI_EnableModule(TSI0, true);


    while (1)
    {
    }
}
