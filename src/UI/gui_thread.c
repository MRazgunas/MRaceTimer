#include "ch.h"
#include "hal.h"

#include "gfx.h"
#include "gui.h"

#include "frequencies.h"

void update_frequency_labels(uint8_t band, uint8_t channel, uint16_t frequency);

char* itoa(int i, char b[]){
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
}

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
            }
        }
        chThdSleepMilliseconds(5);
    }
}

void update_frequency_labels(uint8_t band, uint8_t channel, uint16_t frequency) {
    char channel_letter[2];
    char frequency_text[5];
    itoa(channel+1, channel_letter);
    itoa(frequency, frequency_text);

    gwinSetText(ghBandLabel, bandLetters[band], true);
    gwinSetText(ghChannelLabel, channel_letter, true);
    gwinSetText(ghFrequency, frequency_text, true);
}

void gui_manager_init(void) {
    chThdCreateStatic(waGuiThread, sizeof(waGuiThread), NORMALPRIO, GuiThread, NULL);
}


