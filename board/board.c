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

#include "hal.h"

/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
const PALConfig pal_default_config =
{
    {VAL_GPIOAODR, VAL_GPIOACRL, VAL_GPIOACRH},
    {VAL_GPIOBODR, VAL_GPIOBCRL, VAL_GPIOBCRH},
    {VAL_GPIOCODR, VAL_GPIOCCRL, VAL_GPIOCCRH},
    {VAL_GPIODODR, VAL_GPIODCRL, VAL_GPIODCRH},
    {VAL_GPIOEODR, VAL_GPIOECRL, VAL_GPIOECRH},
    {VAL_GPIOFODR, VAL_GPIOFCRL, VAL_GPIOFCRH},
    {VAL_GPIOGODR, VAL_GPIOGCRL, VAL_GPIOGCRH},
};
#endif

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {

    stm32_clock_init();
}

#if HAL_USE_MMC_SPI
/* Board-related functions related to the MMC_SPI driver.*/
bool mmc_lld_is_card_inserted(MMCDriver *mmcp) {

    (void)mmcp;
    return false;
    //return palReadPad(GPIOC, GPIOC_MMCCP);
}

bool mmc_lld_is_write_protected(MMCDriver *mmcp) {
    (void)mmcp;
    return true;
    //return !palReadPad(GPIOC, GPIOC_MMCWP);
}
#endif

/*
 * Board-specific initialization code.
 */
void boardInit(void) {
    AFIO->MAPR |= AFIO_MAPR_USART1_REMAP; //Remap UART1 to 58 and 59 pins
    AFIO->MAPR |= AFIO_MAPR_I2C1_REMAP; //Remap I2C1 pins to PB8 and PB9 pins
}
