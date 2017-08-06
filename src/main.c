#include "ch.h"
#include "hal.h"

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

  halInit();
  chSysInit();

  //sdStart(&SD2, NULL);

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL);

  while (true) {
    chThdSleepMilliseconds(500);
  }
}
