#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shellcfg.h"

#include "rtc6715.h"

/*
 * Blinker thread.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;

  chRegSetThreadName("blinker");
  while (true) {
    palSetPad(GPIOC, GPIOC_LED0);
    chThdSleepMilliseconds(500);
    palClearPad(GPIOC, GPIOC_LED0);
    chThdSleepMilliseconds(500);
  }
}

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

static adcsample_t samples1[8];

float measure_rssi(void) {
    float voltage;

    adcConvert(&ADCD1, &adcVoltage, samples1, 8);
    uint16_t sum = 0;
    for(int i = 0; i < 8; i++) {
        sum += samples1[i];
    }
    float avg = sum / 8.0f;

    voltage = avg*0.0008056640625;

    return voltage;
}

/*
 * Application entry point.
 */
int main(void) {
    thread_t *shelltp = NULL;

    halInit();
    chSysInit();

    //sdStart(&SD2, NULL);
    initRTC6715();
    shellInit();

    sdStart(&SD1, NULL);
    adcStart(&ADCD1, NULL);

    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL);

    setFrequency(5945);

    while (true) {
        if(SD1.state == SD_READY && shelltp == NULL ) {
            shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                                         "shell", NORMALPRIO + 1,
                                                         shellThread, (void *)&shell_cfg);
             //chThdWait(shelltp);               /* Waiting termination.             */
        }
        float rssi = measure_rssi();
        uint32_t voltage = rssi * 1000;
        if(rssi_en == true) {
            chprintf((BaseSequentialStream*)&SD1, "%u\n", voltage);
        }
        chThdSleepMilliseconds(50);
    }
}
