#include "rtc6715.h"

#include "ch.h"
#include "hal.h"

uint32_t calculateRegBData(int freq);
void spiWrite0(void);
void spiWrite1(void);
void spiSelectMod(uint8_t rxNumber);
void spiUnselectMod(uint8_t rxNumber);


void initRTC6715() {
    palSetPad(GPIOB, GPIOB_SD_CS);
    palClearPad(GPIOB, GPIOB_SPI2_MOSI);
}

void setFrequency(int freq) {
    uint32_t regB;
    regB = calculateRegBData(freq);

    spiSelectMod(0);

    //Address (0x1)
    spiWrite1();
    spiWrite0();
    spiWrite0();
    spiWrite0();

    //R/W (Write)
    spiWrite1();

    for(int i = 16; i; i--, regB >>= 1) {
        if(regB & 0x1) spiWrite1();
        else spiWrite0();
    }

    spiWrite0();
    spiWrite0();
    spiWrite0();
    spiWrite0();

    spiUnselectMod(0);

/*    spiAcquireBus(&SPID2);
    spiSelect(&SPID2);
    spiSend(&SPID2, 3, txBuf);
    spiUnselect(&SPID2);
    spiReleaseBus(&SPID2);*/

}

uint32_t calculateRegBData(int freq) {
    uint32_t N;
    uint8_t A;
    freq = (freq - 479) / 2;
    N = freq / 32;
    A = freq % 32;
    return (N << 7) | A;
}


void spiWrite0() {
    palClearPad(GPIOB, GPIOB_SPI2_SCK);
    palClearPad(GPIOB, GPIOB_SPI2_MOSI);
    palSetPad(GPIOB, GPIOB_SPI2_SCK);
    palClearPad(GPIOB, GPIOB_SPI2_SCK);

}

void spiWrite1() {
    palClearPad(GPIOB, GPIOB_SPI2_SCK);
    palSetPad(GPIOB, GPIOB_SPI2_MOSI);
    palSetPad(GPIOB, GPIOB_SPI2_SCK);
    palClearPad(GPIOB, GPIOB_SPI2_SCK);
}

void spiSelectMod(uint8_t rxNumber) {
    switch(rxNumber) {
    case 0:
        palClearPad(GPIOB, GPIOB_SD_CS);
    }
}

void spiUnselectMod(uint8_t rxNumber) {
    switch(rxNumber) {
    case 0:
        palSetPad(GPIOB, GPIOB_SD_CS);
    }
}



