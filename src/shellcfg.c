/*
    Copyright (C) 2016 Jonathan Struebel
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
        http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    common/shellcfg.c
 * @brief   CLI shell config.
 *
 * @addtogroup SHELL
 * @{
 */
#include <stdlib.h>

#include "hal.h"
#include "shell.h"
#include "chprintf.h"

#include "rtc6715.h"
#include "race.h"

char * endptr;
bool rssi_en = false;

/*
 * Shell history buffer
 */
char history_buffer[SHELL_MAX_HIST_BUFF];

/*
 * Shell completion buffer
 */
char *completion_buffer[SHELL_MAX_COMPLETIONS];

/*
 * Shell commands
 */
static void cmd_setfreq(BaseSequentialStream *chp, int argc, char *argv[]) {
    if(argc == 0 || argc > 2) {
        chprintf(chp, "Usage: setfreq rx_number rx_channel\r\n");
        return;
    }
    uint32_t rx_num = strtol(argv[0], &endptr, 0);
    uint32_t rx_freq = strtol(argv[1], &endptr, 0);

    if(rx_num > 4) {
        chprintf(chp, "Invalid RX number\r\n");
        return;
    }
    if(rx_freq < 5600 || rx_freq > 6000) {
        chprintf(chp, "Invalid RX frequency\r\n");
        return;
    }
    setFrequency(rx_freq);
    chprintf(chp, "Frequency on RX %u set to %u\r\n", rx_num, rx_freq);

}

static void cmd_enrssi(BaseSequentialStream *chp, int argc, char *argv[]) {
    if(argc > 1) {
        chprintf(chp, "Usage: enrssi is_enabled\r\n");
        return;
    }
    uint32_t en = strtol(argv[0], NULL, 0);
    if(en==1) { rssi_en = true; }
    else if(en == 0) { rssi_en = false; }

}

static void cmd_getlaps(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void) argv;
    if(argc > 0) {
        chprintf(chp, "Usage: getlaps\r\n");
    }
    if(current_lap == 0) {
        chprintf(chp, "Currently calibrating\r\n");
        return;
    } else if(current_lap == 1) {
        chprintf(chp, "No laps recorded\r\n");
        return;
    }
    for(int i = 1; i < current_lap; i++) {
        chprintf(chp, "Lap: %u\r\n" , i);
        uint32_t min = lap_time[i].lap_time / 60000;
        uint32_t sec = (lap_time[i].lap_time / 1000) % 60;
        uint32_t ms = lap_time[i].lap_time % 60000;
        chprintf(chp, "Lap time: %um:%us:%ums\r\n", min, sec, ms);
        chprintf(chp, "======================================\r\n");
    }

}

static const ShellCommand commands[] = {
  {"setfreq", cmd_setfreq},
  {"enrssi", cmd_enrssi},
  {"getlaps", cmd_getlaps},
  {NULL, NULL}
};

/*
 * Shell configuration
 */
const ShellConfig shell_cfg = {
  (BaseSequentialStream *)&SD1,
  commands,
  history_buffer,
  sizeof(history_buffer),
  completion_buffer
};

/** @} */
