/*
 * laps.h
 *
 *  Created on: 2017 rugp. 20
 *      Author: matas
 */

#ifndef SRC_LAPS_H_
#define SRC_LAPS_H_

#include "ch.h"
#include "hal.h"

typedef struct {
    systime_t pass_time; //system_time
    uint32_t lap_time; //ms
    uint8_t lap;
} racer_lap;

extern racer_lap lap_time[100];
extern uint8_t current_lap;



#endif /* SRC_LAPS_H_ */
