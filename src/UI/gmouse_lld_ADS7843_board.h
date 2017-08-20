/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#ifndef _GINPUT_LLD_MOUSE_BOARD_H
#define _GINPUT_LLD_MOUSE_BOARD_H

// Resolution and Accuracy Settings
#define GMOUSE_ADS7843_PEN_CALIBRATE_ERROR		8
#define GMOUSE_ADS7843_PEN_CLICK_ERROR			6
#define GMOUSE_ADS7843_PEN_MOVE_ERROR			4
#define GMOUSE_ADS7843_FINGER_CALIBRATE_ERROR	14
#define GMOUSE_ADS7843_FINGER_CLICK_ERROR		18
#define GMOUSE_ADS7843_FINGER_MOVE_ERROR		14

// How much extra data to allocate at the end of the GMouse structure for the board's use
#define GMOUSE_ADS7843_BOARD_DATA_SIZE			0

/*
 * Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig hs_spicfg = {
  NULL,
  GPIOB,
  GPIOB_TOUCH_CS,
  SPI_CR1_BR_1 | SPI_CR1_BR_0,
  0
};

static bool_t init_board(GMouse* m, unsigned driverinstance) {
    (void) m;
    (void) driverinstance;
    spiStart(&SPID2, &hs_spicfg);

    return true;
}

static GFXINLINE bool_t getpin_pressed(GMouse* m) {
    (void) m;
    if(palReadPad(GPIOC, GPIOC_TOUCH_IRQ)) {
        return false;
    } else {
        return true;
    }

}

static GFXINLINE void aquire_bus(GMouse* m) {
    (void) m;
    spiAcquireBus(&SPID2);
    spiStart(&SPID2, &hs_spicfg);
}

static GFXINLINE void release_bus(GMouse* m) {
    (void) m;
    spiReleaseBus(&SPID2);
}

static GFXINLINE uint16_t read_value(GMouse* m, uint16_t port) {
    (void) m;
    spiSelect(&SPID2);
    static uint8_t txbuf[3] = {0};
    static uint8_t rxbuf[3] = {0};
    (void)      m;

    txbuf[0] = port;
    spiExchange(&SPID2, 3, txbuf, rxbuf);

    spiUnselect(&SPID2);

    return ((uint16_t)rxbuf[1] << 5) | (rxbuf[2] >> 3);

}

#endif /* _GINPUT_LLD_MOUSE_BOARD_H */
