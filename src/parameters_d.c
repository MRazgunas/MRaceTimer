#include "ch.h"
#include "hal.h"

#include "parameters_d.h"
#include "parameters.h"
#include "telemetry.h"

#define GSCALAR(t, v, name, def) { t, name, k_param_ ## v, &v, def , 0}
#define GSCALARA(t, v, arr, name, def) { t, name, k_param_ ## v, &arr, def , 0} //for array type

int16_t format_version;
int16_t stream_rates[NUM_STREAMS];

const struct Info var_info[] = {
        // @Param: FORMAT_VERSION
        // @DisplayName: Eeprom format version number
        // @Description: This value is incremented when changes are made to the eeprom format
        // @User: Advanced
        GSCALAR(AP_PARAM_INT16, format_version, "FORMAT_VERSION", 0),

        // @Param: SR_PARAM
         // @DisplayName: Parameter stream frequency
         // @Description: This is frequency of param stream
         // @User: Advanced
         GSCALARA(AP_PARAM_INT16, stream_param, stream_rates[STREAM_PARAMS], "SR_PARAM", 10),

         // @Param: SR_SENSOR
         // @DisplayName: Sensor stream frequency
         // @Description: This is frequency of sensor stream
         // @User: Advanced
         GSCALARA(AP_PARAM_INT16, stream_sensors, stream_rates[STREAM_SENSORS], "SR_SENSOR", 10),

        AP_VAREND,
};


void load_parameters(void) {
    init_param_lib(var_info);
    if(!check_var_info()) {
        chSysHalt("Bad var_info table");
    }

    if(!load_value_using_pointer(&format_version) ||
            format_version != k_format_version) {
        //erase all parameters
        //TODO: debug message for erasing eeprom
        erase_all();

        set_and_save_using_pointer(&format_version, (float)k_format_version, false);
        //save the current format version
    }
    load_all_parameters();

}

