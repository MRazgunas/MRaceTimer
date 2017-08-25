#include "rtc6715.h"

#include "ch.h"
#include "hal.h"

uint32_t calculateRegBData(int freq);
void spiWrite0(void);
void spiWrite1(void);
void spiSelectMod(uint8_t rxNumber);
void spiUnselectMod(uint8_t rxNumber);

/*
 * ADC conversion group.
 * Mode:        Linear buffer, 8 samples of 1 channel, SW triggered.
 * Channels:    IN0.
 */
static const ADCConversionGroup adcVoltage = {
FALSE,
1,
NULL,
NULL,//adcerrorcallback,
0, 0, /* CR1, CR2 */
0, /* SMPR1 */
ADC_SMPR2_SMP_AN0(ADC_SAMPLE_41P5), /* SMPR2 */
ADC_SQR1_NUM_CH(1), /* SQR1 */
0, /* SQR2 */
ADC_SQR3_SQ1_N(ADC_CHANNEL_IN0)  /* SQR3 */
};

adcsample_t measure_rssi(void) {
    adcsample_t sample;

    adcConvert(&ADCD1, &adcVoltage, &sample, 1);

    return sample;
}



void initRTC6715() {
    palSetPad(GPIOB, GPIOB_SD_CS);
    //palClearPad(GPIOB, GPIOB_SPI2_MOSI);
}

void setFrequency(int freq) {
    uint32_t regB;
    regB = calculateRegBData(freq);

    spiAcquireBus(&SPID2);

    palSetPadMode(GPIOB, GPIOB_SPI2_SCK, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, GPIOB_SPI2_MOSI, PAL_MODE_OUTPUT_PUSHPULL);

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

    palSetPadMode(GPIOB, GPIOB_SPI2_SCK, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
    palSetPadMode(GPIOB, GPIOB_SPI2_MOSI, PAL_MODE_STM32_ALTERNATE_PUSHPULL);

    spiReleaseBus(&SPID2);

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



