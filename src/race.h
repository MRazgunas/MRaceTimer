#ifndef SRC_RACE_H_
#define SRC_RACE_H_

#include "hal.h"

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

typedef struct {
    systime_t pass_time; //system_time
    uint32_t lap_time; //ms
    uint8_t lap;
    uint16_t rssi;
} racer_lap;

extern racer_lap lap_time[100];
extern uint8_t current_lap;

extern vrx_status race_status;
extern vrx_settings race_settings;
extern bool race_started;
extern bool race_start_requested;

void update_race(void);
void init_race(void);
void start_race(void); //Start race and calibration
void stop_race(void);



#endif /* SRC_RACE_H_ */
