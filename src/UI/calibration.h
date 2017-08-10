#ifndef _CALIBRATION_H
#define _CALIBRATION_H

#include <string.h>
#include "gfx.h"

// Calibration data
float calibrationData[] = {
	-0.0674,		// ax
	-0.00127,		// bx
	262.56,		// cy
	-0.009,		// ay
	0.09471,		// by
	-24.1904 		// cy
};

// The loading routine
bool_t LoadMouseCalibration(unsigned instance, void *data, size_t sz)
{
	(void)instance;
	
	if (sz != sizeof(calibrationData)) {
		return FALSE;
	}
	
	memcpy(data, (void*)&calibrationData, sz);
	
	return TRUE;
}

#endif /* _CALIBRATION_H */

