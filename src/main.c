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
#include "race.h"

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
    init_race();

    gfxInit();
    gdispSetOrientation(GDISP_ROTATE_270);
    gui_manager_init();
    //guiEventLoop();
    chThdSetPriority(NORMALPRIO+5);

    while (true) {
        if(SD1.state == SD_READY && shelltp == NULL && in_cli_mode) {
            shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                                         "shell", NORMALPRIO + 1,
                                                         shellThread, (void *)&shell_cfg);
             //chThdWait(shelltp);               /* Waiting termination.             */
        }

        update_race();


        if(rssi_en == true) {
            chprintf((BaseSequentialStream*)&SD1, "%u\r\n", race_status.rssi);
        }
        chThdSleepMilliseconds(10); //100Hz
    }
}
