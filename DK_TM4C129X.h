/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       DK_TM4C129X.h
 *
 *  @brief      DK_TM4C129X Board Specific APIs
 *
 *  The DK_TM4C129X header file should be included in an application as follows:
 *  @code
 *  #include <DK_TM4C129X.h>
 *  @endcode
 *
 *  ============================================================================
 */

#ifndef __DK_TM4C129X_H
#define __DK_TM4C129X_H

#ifdef __cplusplus
extern "C" {
#endif

/* LEDs on DK_TM4C129X are active high. */
#define DK_TM4C129X_LED_OFF (0)
#define DK_TM4C129X_LED_ON  (1)

/*!
 *  @def    DK_TM4C129X_EMACName
 *  @brief  Enum of EMAC names on the DK_TM4C129X dev board
 */
typedef enum DK_TM4C129X_EMACName {
    DK_TM4C129X_EMAC0 = 0,

    DK_TM4C129X_EMACCOUNT
} DK_TM4C129X_EMACName;

/*!
 *  @def    DK_TM4C129X_GPIOName
 *  @brief  Enum of LED names on the DK_TM4C129X dev board
 */
typedef enum DK_TM4C129X_GPIOName {
    DK_TM4C129X_BUTTON_SELECT = 0,
    DK_TM4C129X_BUTTON_UP,
    DK_TM4C129X_BUTTON_DOWN,
    DK_TM4C129X_LED_G,
    DK_TM4C129X_LED_B,
    DK_TM4C129X_LED_R,

    DK_TM4C129X_GPIOCOUNT
} DK_TM4C129X_GPIOName;

/*!
 *  @def    DK_TM4C129X_I2CName
 *  @brief  Enum of I2C names on the DK_TM4C129X dev board
 */
typedef enum DK_TM4C129X_I2CName {
    DK_TM4C129X_I2C2 = 0,
    DK_TM4C129X_I2C3,
    DK_TM4C129X_I2C6,
    DK_TM4C129X_I2C7,

    DK_TM4C129X_I2CCOUNT
} DK_TM4C129X_I2CName;

/*!
 *  @def    DK_TM4C129X_PWMName
 *  @brief  Enum of PWM names on the DK_TM4C129X dev board
 */
typedef enum DK_TM4C129X_PWMName {
    DK_TM4C129X_PWM1 = 0,
    DK_TM4C129X_PWM6,

    DK_TM4C129X_PWMCOUNT
} DK_TM4C129X_PWMName;

/*!
 *  @def    DK_TM4C129X_SDSPIName
 *  @brief  Enum of SDSPI names on the DK_TM4C129X dev board
 */
typedef enum DK_TM4C129X_SDSPIName {
    DK_TM4C129X_SDSPI0 = 0,

    DK_TM4C129X_SDSPICOUNT
} DK_TM4C129X_SDSPIName;

/*!
 *  @def    DK_TM4C129X_SPIName
 *  @brief  Enum of SPI names on the DK_TM4C129X dev board
 */
typedef enum DK_TM4C129X_SPIName {
    DK_TM4C129X_SPI0 = 0,
    DK_TM4C129X_SPI2,
    DK_TM4C129X_SPI3,

    DK_TM4C129X_SPICOUNT
} DK_TM4C129X_SPIName;

/*!
 *  @def    DK_TM4C129X_UARTName
 *  @brief  Enum of UARTs on the DK_TM4C129X dev board
 */
typedef enum DK_TM4C129X_UARTName {
    DK_TM4C129X_UART0 = 0,

    DK_TM4C129X_UARTCOUNT
} DK_TM4C129X_UARTName;

/*!
 *  @def    DK_TM4C129X_USBMode
 *  @brief  Enum of USB setup function on the DK_TM4C129X dev board
 */
typedef enum DK_TM4C129X_USBMode {
    DK_TM4C129X_USBDEVICE,
    DK_TM4C129X_USBHOST
} DK_TM4C129X_USBMode;

/*!
 *  @def    DK_TM4C129X_USBMSCHFatFsName
 *  @brief  Enum of USBMSCHFatFs names on the DK_TM4C129X dev board
 */
typedef enum DK_TM4C129X_USBMSCHFatFsName {
    DK_TM4C129X_USBMSCHFatFs0 = 0,

    DK_TM4C129X_USBMSCHFatFsCOUNT
} DK_TM4C129X_USBMSCHFatFsName;

/*
 *  @def    DK_TM4C129X_WatchdogName
 *  @brief  Enum of Watchdogs on the DK_TM4C129X dev board
 */
typedef enum DK_TM4C129X_WatchdogName {
    DK_TM4C129X_WATCHDOG0 = 0,

    DK_TM4C129X_WATCHDOGCOUNT
} DK_TM4C129X_WatchdogName;

/*!
 *  @def    DK_TM4C129X_WiFiName
 *  @brief  Enum of WiFi names on the DK_TM4C129X dev board
 */
typedef enum DK_TM4C129X_WiFiName {
    DK_TM4C129X_WIFI = 0,

    DK_TM4C129X_WIFICOUNT
} DK_TM4C129X_WiFiName;

/*!
 *  @brief  Initialize the general board specific settings
 *
 *  This function initializes the general board specific settings.
 *  This includes:
 *     - Enable clock sources for peripherals
 */
extern void DK_TM4C129X_initGeneral(void);

/*!
 *  @brief Initialize board specific EMAC settings
 *
 *  This function initializes the board specific EMAC settings and
 *  then calls the EMAC_init API to initialize the EMAC module.
 *
 *  The EMAC address is programmed as part of this call.
 *
 */
extern void DK_TM4C129X_initEMAC(void);

/*!
 *  @brief  Initialize board specific GPIO settings
 *
 *  This function initializes the board specific GPIO settings and
 *  then calls the GPIO_init API to initialize the GPIO module.
 *
 *  The GPIOs controlled by the GPIO module are determined by the GPIO_PinConfig
 *  variable.
 */
extern void DK_TM4C129X_initGPIO(void);

/*!
 *  @brief  Initialize board specific I2C settings
 *
 *  This function initializes the board specific I2C settings and then calls
 *  the I2C_init API to initialize the I2C module.
 *
 *  The I2C peripherals controlled by the I2C module are determined by the
 *  I2C_config variable.
 */
extern void DK_TM4C129X_initI2C(void);

/*!
 *  @brief  Initialize board specific PWM settings
 *
 *  This function initializes the board specific PWM settings and then calls
 *  the PWM_init API to initialize the PWM module.
 *
 *  The PWM peripherals controlled by the PWM module are determined by the
 *  PWM_config variable.
 */
extern void DK_TM4C129X_initPWM(void);

/*!
 *  @brief  Initialize board specific SDSPI settings
 *
 *  This function initializes the board specific SDSPI settings and then calls
 *  the SDSPI_init API to initialize the SDSPI module.
 *
 *  The SDSPI peripherals controlled by the SDSPI module are determined by the
 *  SDSPI_config variable.
 */
extern void DK_TM4C129X_initSDSPI(void);

/*!
 *  @brief  Initialize board specific SPI settings
 *
 *  This function initializes the board specific SPI settings and then calls
 *  the SPI_init API to initialize the SPI module.
 *
 *  The SPI peripherals controlled by the SPI module are determined by the
 *  SPI_config variable.
 */
extern void DK_TM4C129X_initSPI(void);

/*!
 *  @brief  Initialize board specific UART settings
 *
 *  This function initializes the board specific UART settings and then calls
 *  the UART_init API to initialize the UART module.
 *
 *  The UART peripherals controlled by the UART module are determined by the
 *  UART_config variable.
 */
extern void DK_TM4C129X_initUART(void);

/*!
 *  @brief  Initialize board specific USB settings
 *
 *  This function initializes the board specific USB settings and pins based on
 *  the USB mode of operation.
 *
 *  @param      usbMode    USB mode of operation
 */
extern void DK_TM4C129X_initUSB(DK_TM4C129X_USBMode usbMode);

/*!
 *  @brief  Initialize board specific USBMSCHFatFs settings
 *
 *  This function initializes the board specific USBMSCHFatFs settings and then
 *  calls the USBMSCHFatFs_init API to initialize the USBMSCHFatFs module.
 *
 *  The USBMSCHFatFs peripherals controlled by the USBMSCHFatFs module are
 *  determined by the USBMSCHFatFs_config variable.
 */
extern void DK_TM4C129X_initUSBMSCHFatFs(void);

/*!
 *  @brief  Initialize board specific Watchdog settings
 *
 *  This function initializes the board specific Watchdog settings and then
 *  calls the Watchdog_init API to initialize the Watchdog module.
 *
 *  The Watchdog peripherals controlled by the Watchdog module are determined
 *  by the Watchdog_config variable.
 */
extern void DK_TM4C129X_initWatchdog(void);

/*!
 *  @brief  Initialize board specific WiFi settings
 *
 *  This function initializes the board specific WiFi settings and then calls
 *  the WiFi_init API to initialize the WiFi module.
 *
 *  The hardware resources controlled by the WiFi module are determined by the
 *  WiFi_config variable.
 *
 *  A SimpleLink CC3100 device or module is required and must be connected to
 *  use the WiFi driver.
 */
extern void DK_TM4C129X_initWiFi(void);

#ifdef __cplusplus
}
#endif

#endif /* __DK_TM4C129X_H */
