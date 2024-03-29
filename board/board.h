/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the Olimex STM32-P103 proto board.
 */

/*
 * Board identifier.
 */
#define BOARD_OLIMEX_STM32_P103
#define BOARD_NAME              "el_load Rev. A"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            32768
#define STM32_HSECLK            16000000

/*
 * MCU type, supported types are defined in ChibiOS/os/hal/platforms/hal_lld.h.
 */
#define STM32F103xE

/*
 * IO pins assignments.
 */
#define GPIOB_LCD_RESET         0
#define GPIOB_LCD_DC            1
#define GPIOB_TOUCH_CS         11
#define GPIOB_SD_CS            12
#define GPIOB_SPI2_SCK         13
#define GPIOB_SPI2_MOSI        15

#define GPIOC_LED0              0
#define GPIOC_LED1              1
#define GPIOC_LCD_CS            5
#define GPIOC_TOUCH_IRQ         6

#define EEPROM_ADDRESS  0x50
#define EEPROM_SIZE     4096 //bytes
#define EEPROM_PAGE_SIZE 32 //bytes
#define EEPROM_BUS      I2CD1
/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 *
 * The digits have the following meaning:
 *   0 - Analog input.
 *   1 - Push Pull output 10MHz.
 *   2 - Push Pull output 2MHz.
 *   3 - Push Pull output 50MHz.
 *   4 - Digital input.
 *   5 - Open Drain output 10MHz.
 *   6 - Open Drain output 2MHz.
 *   7 - Open Drain output 50MHz.
 *   8 - Digital input with PullUp or PullDown resistor depending on ODR.
 *   9 - Alternate Push Pull output 10MHz.
 *   A - Alternate Push Pull output 2MHz.
 *   B - Alternate Push Pull output 50MHz.
 *   C - Reserved.
 *   D - Alternate Open Drain output 10MHz.
 *   E - Alternate Open Drain output 2MHz.
 *   F - Alternate Open Drain output 50MHz.
 * Please refer to the STM32 Reference Manual for details.
 */

/*
 * Port A setup.
 * Everything input with pull-up except:
 * PA0  - Analog input                          (RSSI0).
 * PA5  - Alternate Push Pull output            (SPI1_SCK).
 * PA6  - Digital input with PullUp             (SPI1_MISO).
 * PA7  - Alternate Push Pull output            (SPI1_MOSI).
 */
#define VAL_GPIOACRL            0xB8B88880      /*  PA7...PA0 */
#define VAL_GPIOACRH            0x88888888      /* PA15...PA8 */
#define VAL_GPIOAODR            0xFFFFFFFF

/*
 * Port B setup.
 * Everything input with pull-up except:
 * PB0  - Push Pull output 50MHz.               (LCD_RESET).
 * PB1  - Push Pull output 50MHz.               (LCD_DC).
 * PB6  - Alternate Open Drain output           (UART1_TX).
 * PB7  - Digital input with pull up            (UART1_RX).
 * PB8  - Alternate Open Drain output           (SCL_1).
 * PB9  - Alternate Open Drain output           (SDA_1).
 * PB11 - Push Pull output 50MHz.               (TOUCH_CS).
 * PB12 - Push Pull output                      (SD_CS).
 * PB13 - Push Pull output                      (SPI2_SCK).
 * PB14 - Digital input with PullUp             (SPI2_MISO).
 * PB15 - Push Pull output                      (SPI2_MOSI).
 */
#define VAL_GPIOBCRL            0x4B888833      /*  PB7...PB0 */
#define VAL_GPIOBCRH            0xB8B338FF      /* PB15...PB8 */
//#define VAL_GPIOBCRH            0x38338888      /* PB15...PB8 */
#define VAL_GPIOBODR            0xFFFFFFFF

/*
 * Port C setup.
 * Everything input with pull-up except:
 * PC0  - Push Pull output                      (LED_0).
 * PC1  - Push Pull output                      (LED_1).
 * PC5  - Push Pull output 50MHz.               (LCD_CS).
 * PC6  - Digital input                         (TOUCH_IRQ).
 * PC14 - Digital input                         (OSC32).
 * PC15 - Digital input                         (OSC32).
 */
#define VAL_GPIOCCRL            0x88588822      /*  PC7...PC0 */
#define VAL_GPIOCCRH            0x88888888      /* PC15...PC8 */
#define VAL_GPIOCODR            0xFFFFFFFF

/*
 * Port D setup.
 * Everything input with pull-up except:
 * PD0  - Normal input (XTAL).
 * PD1  - Normal input (XTAL).
 */
#define VAL_GPIODCRL            0x88888844      /*  PD7...PD0 */
#define VAL_GPIODCRH            0x88888888      /* PD15...PD8 */
#define VAL_GPIODODR            0xFFFFFFFF

/*
 * Port E setup.
 * Everything input with pull-up except:
 */
#define VAL_GPIOECRL            0x88888888      /*  PE7...PE0 */
#define VAL_GPIOECRH            0x88888888      /* PE15...PE8 */
#define VAL_GPIOEODR            0xFFFFFFFF

/*
 * Port F setup.
 * Everything input with pull-up except:
 */
#define VAL_GPIOFCRL            0x88888888      /*  PF7...PF0 */
#define VAL_GPIOFCRH            0x88888888      /* PF15...PF8 */
#define VAL_GPIOFODR            0xFFFFFFFF

/*
 * Port G setup.
 * Everything input with pull-up except:
 */
#define VAL_GPIOGCRL            0x88888888      /*  PG7...PG0 */
#define VAL_GPIOGCRH            0x88888888      /* PG15...PG8 */
#define VAL_GPIOGODR            0xFFFFFFFF

/*
 * USB bus activation macro, required by the USB driver.
 */
#define usb_lld_connect_bus(usbp) palClearPad(GPIOC, GPIOC_USB_DISC)

/*
 * USB bus de-activation macro, required by the USB driver.
 */
#define usb_lld_disconnect_bus(usbp) palSetPad(GPIOC, GPIOC_USB_DISC)

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
