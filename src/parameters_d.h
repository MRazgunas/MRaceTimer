#ifndef SRC_PARAMETERS_D_H_
#define SRC_PARAMETERS_D_H_
#include "ch.h"
#include "hal.h"

#include "parameters.h"
#include "parameters_d.h"
#include "telemetry.h"

//////////////////////////////////////////////////////////////////
// STOP!!! DO NOT CHANGE THIS VALUE UNTIL YOU FULLY UNDERSTAND THE
// COMMENTS ABOVE. IF UNSURE, ASK ANOTHER DEVELOPER!!!
static const uint16_t k_format_version = 1;
//////////////////////////////////////////////////////////////////


// EEPROM layout
enum {
    k_param_format_version = 0,
    k_param_stream_param,
    k_param_stream_sensors,
    k_param_vtx0_calibration_threshold,
    k_param_vtx0_calibartion_offser,
    k_param_vtx0_trigger_threshold,
    k_param_vtx0_filter_ration,
};

extern int16_t stream_rates[NUM_STREAMS];


void load_parameters(void);


#endif /* SRC_PARAMETERS_D_H_ */
