/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define SOPT4_TPM2CH0SRC_TPM2_CH0 0x00u /*!<@brief TPM2 channel 0 input capture source select: TPM2_CH0 signal */
#define SOPT5_UART0RXSRC_UART_RX 0x00u  /*!<@brief UART0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u  /*!<@brief UART0 transmit data source select: UART0_TX pin */

/*! @name PORTA2 (number 28), J1[4]/D1/UART0_TX
  @{ */
#define BOARD_INITPINS_DEBUG_UART_TX_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN 2U     /*!<@brief PORTA pin index: 2 */
                                                /* @} */

/*! @name PORTA1 (number 27), J1[2]/D0/UART0_RX
  @{ */
#define BOARD_INITPINS_DEBUG_UART_RX_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN 1U     /*!<@brief PORTA pin index: 1 */
                                                /* @} */

/*! @name PORTB18 (number 53), D3[1]/LEDRGB_RED
  @{ */
#define BOARD_INITPINS_LED_RED_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITPINS_LED_RED_PIN 18U    /*!<@brief PORTB pin index: 18 */
                                          /* @} */

/*! @name PORTB17 (number 52), TSI0_CH10
  @{ */
#define BOARD_INITPINS_TSI_ELECTRODE_2_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITPINS_TSI_ELECTRODE_2_PIN 17U    /*!<@brief PORTB pin index: 17 */
                                                  /* @} */

/*! @name PORTB16 (number 51), TSI0_CH9
  @{ */
#define BOARD_INITPINS_TSI_ELECTRODE_1_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITPINS_TSI_ELECTRODE_1_PIN 16U    /*!<@brief PORTB pin index: 16 */
                                                  /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
