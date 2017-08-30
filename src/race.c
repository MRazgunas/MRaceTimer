#include "race.h"

#include "ch.h"
#include "hal.h"

#include "chprintf.h"

#include "rtc6715.h"
#include "gui_thread.h"
#include "telemetry.h"

vrx_status race_status = { 0 };
vrx_settings race_settings = { 0 };
racer_lap lap_time[100] = { 0 };
uint8_t current_lap = 0;

bool race_started = false;
bool race_start_requested = false;


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

void init_race(void) {
    race_status = vrx_status_defaults;
    //race_settings = vrx_set_defaults;
}

void update_race(void) {
    race_status.rawRssi = measure_rssi();
    race_status.filtRssi = (race_settings.filterRatio * (float)race_status.rawRssi) +
            ((1.0f - race_settings.filterRatio) * race_status.filtRssi);
    race_status.rssi = race_status.filtRssi;

    if(race_status.rssiTrigger > 0) {
        if(!race_status.crossing && race_status.rssi > race_status.rssiTrigger) {
            race_status.crossing = true;
            //chprintf((BaseSequentialStream*)&SD1, "Racer %u is crossing\r\n", 0);
        }

        if(race_status.crossing) {
            uint16_t triggerThreshold = race_settings.triggerThreshold;

            if(race_status.rawRssi > race_status.peakRawRssi) {
                race_status.peakRawRssi = race_status.rawRssi;
                race_status.peakRssiTime = chVTGetSystemTime();
            }

            if(race_status.calibrating) {
                if(race_status.rssiTrigger < race_status.rssi - race_settings.calibrationOffset) {
                    race_status.rssiTrigger = race_status.rssi - race_settings.calibrationOffset;
                }
                triggerThreshold = race_settings.calibrationThreshold;
            }

            if(race_status.rssiTrigger > triggerThreshold && race_status.rssi < (race_status.rssiTrigger - triggerThreshold)) {


                lap_time[current_lap].lap = current_lap;
                lap_time[current_lap].pass_time = race_status.peakRssiTime;
                lap_time[current_lap].rssi = race_status.peakRawRssi;
                if(current_lap == 0) {
                    lap_time[current_lap].lap_time = 0;
                    send_racer_pass_status(current_lap);
                } else {
                    lap_time[current_lap].lap_time =
                            ST2MS(lap_time[current_lap].pass_time - lap_time[current_lap - 1].pass_time);
                    send_racer_lap_time(current_lap);
                }
                send_racer_pass_status(current_lap);

                current_lap++;

                race_status.crossing = false;
                race_status.calibrating = false;
                race_status.peakRawRssi = 0;
            }
        }
    }
}

void start_race(void) {
    race_status.rssiTrigger = race_status.rssi - race_settings.calibrationOffset;
    race_status.calibrating = true;
    race_status.crossing = false;
    race_status.peakRawRssi = 0;
    race_status.peakRssiTime = 0;
    current_lap = 0;
    race_started = true;
    race_start_requested = false;
}

void stop_race(void) {
    race_started = false;
    race_start_requested = false;
    race_status.rssiTrigger = 0;
}

