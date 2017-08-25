#include "ch.h"
#include "hal.h"

#include "gui_thread.h"

#include "gfx.h"
#include "gui.h"

#include "rtc6715.h"
#include "frequencies.h"
#include "race.h"

#include <stdio.h>
#include <stdlib.h>

systime_t countdown_end_time = 0;

void update_timer_label(uint16_t seconds);

void update_frequency_labels(uint8_t band, uint8_t channel, uint16_t frequency);

/*char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}*/

static THD_WORKING_AREA(waGuiThread, 2048);
static THD_FUNCTION(GuiThread, arg) {

    (void)arg;

    chRegSetThreadName("gui_manager");

    geventListenerInit(&glistener);
    gwinAttachListener(&glistener);

    guiCreate();

    uint8_t band_sel = 0;
    uint8_t channel_sel = 0;
    uint16_t frequency_sel = bandMap[band_sel][channel_sel];

    update_frequency_labels(band_sel, channel_sel, frequency_sel);

  while (true) {
        GEvent* pe;

        // Get an event
        pe = geventEventWait(&glistener, 0);
        switch (pe->type) {
        case GEVENT_GWIN_BUTTON:
            if (((GEventGWinButton*)pe)->gwin == ghSetFreqPage) {
                guiShowPage(1);
            } else if(((GEventGWinButton*)pe)->gwin == ghBackButton) {
                guiShowPage(0);
            } else if(((GEventGWinButton*)pe)->gwin == ghSetFreq) {
                setFrequency(frequency_sel);
            } else if(((GEventGWinButton*)pe)->gwin == ghUpBand) {
                if(band_sel++ == NUM_BANDS-1) {
                    band_sel = 0;
                }
                frequency_sel = bandMap[band_sel][channel_sel];
                update_frequency_labels(band_sel, channel_sel, frequency_sel);
            } else if(((GEventGWinButton*)pe)->gwin == ghDownBand) {
                if(band_sel-- == 0) {
                    band_sel = NUM_BANDS-1;
                }
                frequency_sel = bandMap[band_sel][channel_sel];
                update_frequency_labels(band_sel, channel_sel, frequency_sel);
            } else if(((GEventGWinButton*)pe)->gwin == ghUpChannel) {
                if(channel_sel++ == 7) {
                    channel_sel = 0;
                }
                frequency_sel = bandMap[band_sel][channel_sel];
                update_frequency_labels(band_sel, channel_sel, frequency_sel);
            } else if(((GEventGWinButton*)pe)->gwin == ghDownChannel) {
                if(channel_sel-- == 0) {
                    channel_sel = 7;
                }
                frequency_sel = bandMap[band_sel][channel_sel];
                update_frequency_labels(band_sel, channel_sel, frequency_sel);
            } else if(((GEventGWinButton*)pe)->gwin == ghStartRace) {
                start_race();
            }
        }
        systime_t now = chVTGetSystemTime();
        int32_t time_to_start = countdown_end_time - now;
        uint16_t sec = 0;
        static uint8_t slow_down = 200;
        if(time_to_start > 0 && slow_down-- == 0 && race_start_requested) {
            slow_down = 200;
            sec = ST2S(time_to_start);
            update_timer_label(sec);
        } else if (time_to_start <= 0 && slow_down-- == 0) {
            if(!race_started && race_start_requested) {
                start_race();
            }
            if(!race_started) {
                update_timer_label(0);
            } else {
                slow_down = 200;
                sec = ST2S(-time_to_start);
                update_timer_label(sec);
            }
        }

        chThdSleepMilliseconds(5);
    }
}

void update_timer_label(uint16_t seconds) {
    uint16_t m = seconds/60;
    uint16_t s = seconds - m*60;
    char timeLabel[7];
    snprintf(timeLabel, 7, "%02u:%02u", m, s);
    gwinSetText(ghTimerLabel, timeLabel, true);
}

void set_start_time_and_start_count(uint16_t sec) {
    countdown_end_time = chVTGetSystemTime() + S2ST(sec);
    update_timer_label(sec);
}

void update_frequency_labels(uint8_t band, uint8_t channel, uint16_t frequency) {
    char channel_letter[2];
    char frequency_text[5];
    itoa(channel+1, channel_letter, 10);
    itoa(frequency, frequency_text, 10);

    gwinSetText(ghBandLabel, bandLetters[band], true);
    gwinSetText(ghChannelLabel, channel_letter, true);
    gwinSetText(ghFrequency, frequency_text, true);
}

void gui_manager_init(void) {
    chThdCreateStatic(waGuiThread, sizeof(waGuiThread), NORMALPRIO, GuiThread, NULL);
}


