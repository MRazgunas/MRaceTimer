#ifndef SRC_RTC6715_H_
#define SRC_RTC6715_H_

#include "ch.h"
#include "hal.h"

void initRTC6715(void);
void setFrequency(int freq);

adcsample_t measure_rssi(void);



#endif /* SRC_RTC6715_H_ */
