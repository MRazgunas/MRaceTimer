/*
 * gui_thread.h
 *
 *  Created on: 2017 rugp. 10
 *      Author: matas
 */

#ifndef SRC_UI_GUI_THREAD_H_
#define SRC_UI_GUI_THREAD_H_

typedef struct {
    uint16_t rawRssi;
    float filtRssi;
    uint16_t rssi;
    bool calibrating;
    bool crossing;
    uint16_t rssiTrigger;
    uint16_t peakRawRssi;
    systime_t peakRssiTime;
} vrx_status;

typedef struct {
    uint16_t freq;
    uint16_t calibrationOffset;
    uint16_t calibrationThreshold;
    uint16_t triggerThreshold;
    float filterRatio;
} vrx_settings;

void gui_manager_init(vrx_status *stat, vrx_settings *sett);



#endif /* SRC_UI_GUI_THREAD_H_ */
