#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shellcfg.h"

#include "eeprom.h"
#include "telemetry.h"
#include "parameters_d.h"
#include "gfx.h"
#include "rtc6715.h"
#include "gui.h"
#include "gui_thread.h"
#include "laps.h"

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

adcsample_t measure_rssi(void) {
    adcsample_t sample;

    adcConvert(&ADCD1, &adcVoltage, &sample, 1);

    return sample;
}

const vrx_settings vrx_set_defaults = {
        5800, //freq
        200, //calibrationOffset
        300, //calibrationThreshold
        200, //triggerThreshold
        0.01f //filterRatio
};

const vrx_status vrx_status_defaults = {
        0,          //rawRssi
        0.0f,       //filtRssi
        0,          //rssi
        true,//calibrating
        false,      //crossing
        0,          //rssiTrigger
        0,          //peakRawRssi
        0,          //peakRssiTime
};

/*
 * Application entry point.
 */
int main(void) {

    thread_t *shelltp = NULL;

    halInit();
    chSysInit();

    sdStart(&SD1, NULL);
    adcStart(&ADCD1, NULL);
    init_eeprom();

    initRTC6715();
    shellInit();

    load_parameters();

    init_telemetry();



    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL);

    //setFrequency(5945);
    vrx_status status = vrx_status_defaults;
    vrx_settings settings = vrx_set_defaults;

    gfxInit();
    gdispSetOrientation(GDISP_ROTATE_270);
    gui_manager_init(&status, &settings);
    //guiEventLoop();
    chThdSetPriority(NORMALPRIO+5);

    while (true) {
        if(SD1.state == SD_READY && shelltp == NULL && in_cli_mode) {
            shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                                         "shell", NORMALPRIO + 1,
                                                         shellThread, (void *)&shell_cfg);
             //chThdWait(shelltp);               /* Waiting termination.             */
        }
        status.rawRssi = measure_rssi();
        status.filtRssi = (settings.filterRatio * (float)status.rawRssi) +
                ((1.0f - settings.filterRatio) * status.filtRssi);
        status.rssi = status.filtRssi;

        if(status.rssiTrigger > 0) {
            if(!status.crossing && status.rssi > status.rssiTrigger) {
                status.crossing = true;
                chprintf((BaseSequentialStream*)&SD1, "Racer %u is crossing\r\n", 0);
            }

            if(status.rawRssi > status.peakRawRssi) {
                status.peakRawRssi = status.rawRssi;
                status.peakRssiTime = chVTGetSystemTime();
            }

            if(status.crossing) {
                uint16_t triggerThreshold = settings.triggerThreshold;

                if(status.calibrating) {
                    if(status.rssiTrigger < status.rssi - settings.calibrationOffset) {
                        status.rssiTrigger = status.rssi - settings.calibrationOffset;
                    }
                    triggerThreshold = settings.calibrationThreshold;
                }

                if(status.rssiTrigger > triggerThreshold && status.rssi < (status.rssiTrigger - triggerThreshold)) {
                    chprintf((BaseSequentialStream*)&SD1, "Racer %u finished crossing\r\n", 0);

                    lap_time[current_lap].lap = current_lap;
                    lap_time[current_lap].pass_time = status.peakRssiTime;
                    if(current_lap == 0) {
                        lap_time[current_lap].lap_time = 0;
                    } else {
                        lap_time[current_lap].lap_time =
                                ST2MS(lap_time[current_lap].pass_time - lap_time[current_lap - 1].pass_time);
                    }
                    current_lap++;

                    status.crossing = false;
                    status.calibrating = false;
                    status.peakRawRssi = 0;
                }
            }
        }


        if(rssi_en == true) {
            chprintf((BaseSequentialStream*)&SD1, "%u\r\n", status.rssi);
        }
        chThdSleepMilliseconds(10); //100Hz
    }
}
