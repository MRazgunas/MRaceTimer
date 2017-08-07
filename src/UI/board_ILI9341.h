/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#ifndef _GDISP_LLD_BOARD_H
#define _GDISP_LLD_BOARD_H

#include "ch.h"
#include "hal.h"

/*
 * Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig hs_spicfg = {
  NULL,
  GPIOC,
  GPIOC_LCD_CS,
  0,
  0
};

static GFXINLINE void init_board(GDisplay *g) {
    spiStart(&SPID1, &hs_spicfg);

	(void) g;
}

static GFXINLINE void post_init_board(GDisplay *g) {
	(void) g;
}

static GFXINLINE void setpin_reset(GDisplay *g, bool_t state) {
	(void) g;
    if(state) {
        // reset lcd
        palClearPad(GPIOB, GPIOB_LCD_RESET);
    } else {
        palSetPad(GPIOB, GPIOB_LCD_RESET);
    }
}

static GFXINLINE void set_backlight(GDisplay *g, uint8_t percent) {
	(void) g;
	(void) percent;
}

static GFXINLINE void acquire_bus(GDisplay *g) {
    spiAcquireBus(&SPID1);
	(void) g;
}

static GFXINLINE void release_bus(GDisplay *g) {
    spiReleaseBus(&SPID1);
	(void) g;
}

static GFXINLINE void write_index(GDisplay *g, uint16_t index) {
	(void) g;
	uint8_t buff[1];
	buff[0] = index;
	while(SPID1.state != SPI_READY);
	palClearPad(GPIOB, GPIOB_LCD_DC);
	spiSelect(&SPID1);
    spiStartSend(&SPID1, 1, buff);
    spiUnselect(&SPID1);
}

static GFXINLINE void write_data(GDisplay *g, uint16_t data) {
	(void) g;
    uint8_t buff[1];
    buff[0] = data;
    while(SPID1.state != SPI_READY);
    palSetPad(GPIOB, GPIOB_LCD_DC);
    spiSelect(&SPID1);
    spiStartSend(&SPID1, 1, buff);
    spiUnselect(&SPID1);
}

static GFXINLINE void setreadmode(GDisplay *g) {
	(void) g;

}

static GFXINLINE void setwritemode(GDisplay *g) {
	(void) g;
}

static GFXINLINE uint16_t read_data(GDisplay *g) {
	(void) g;
	return 0;
}

#endif /* _GDISP_LLD_BOARD_H */
