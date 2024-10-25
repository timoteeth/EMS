#ifndef __EMS_SENSOR_H__
#define __EMS_SENSOR_H__

#include "Entry.h"
#include "types.h"

struct EMS_Sensor{
	EMS_Option* settings;
	EMS_Pin* pins;
	struct EMS_Entry (*read)(EMS_Pin* pins);
	struct EMS_Entry* buffer;
	char* (*format)(struct EMS_Entry* e, EMS_Option* s);
};

#endif
