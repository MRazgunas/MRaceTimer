#ifndef SRC_TELEMETRY_H_
#define SRC_TELEMETRY_H_

#define FIRMWARE_STRING "0.1_alfa"

#include "parameters.h"

enum streams {STREAM_SENSORS,
              STREAM_PARAMS,
              NUM_STREAMS};

enum Control_State {
    ENGINE_STOPED = 0, //Always has to be 0
    ENGINE_START_REQUESTED,
    ENGINE_STARTING_WITH_CHOKE,
    ENGINE_STARTING_WO_CHOKE,
    ENGINE_WARMUP,
    ENGINE_VOLTAGE_RAMP_UP,
    ENGINE_LOAD_RAMP_UP,
    ENGINE_LOAD_RAMP_DOWN,
    ENGINE_TRANSION_TO_RUNNING,
    ENGINE_RUNNING,
    ENGINE_COOLDOWN,
    ENGINE_EMERGENCY_SHUTDOWN,
};


void init_telemetry(void);
void send_parameter_value_all(const char *param_name, ap_var_type param_type,
        float param_value);

extern bool in_cli_mode;

#endif /* SRC_TELEMETRY_H_ */
