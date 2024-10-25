#ifndef __EMS_WATER_SENSOR_H__
#define __EMS_WATER_SENSOR_H__

#include "Entry.h"
#include "Sensor.h"
#include "types.h"

enum{
	_EMS_PIN_WATER_SENSOR_PH,
	_EMS_PIN_WATER_SENSOR_O2,
};

enum{
	_EMS_SETTING_WATER_SENSOR_RECORDING_MODE
};

enum{
	_EMS_OPTION_WATER_SENSOR_PH,
	_EMS_OPTION_WATER_SENSOR_O2,
	_EMS_OPTION_WATER_SENSOR_BOTH,
};

enum{
	_EMS_DATA_WATER_SENSOR_PH,
	_EMS_DATA_WATER_SENSOR_O2,
};

struct EMS_Sensor makeWaterSensor(EMS_Pin phPin, EMS_Pin o2Pin, EMS_Option mode);

#endif
