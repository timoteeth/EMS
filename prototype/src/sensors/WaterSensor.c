#include <stdlib.h>
#include <stdio.h>

#include "sensors/WaterSensor.h"
#include "timer.h"

float pinGet(EMS_Pin);

static struct EMS_Entry waterSensorRead(EMS_Pin* pins){
	float ph = pinGet(pins[_EMS_PIN_WATER_SENSOR_PH]);
	float o2 = pinGet(pins[_EMS_PIN_WATER_SENSOR_O2]);
	float* data = malloc(sizeof(float) * 2);
	data[_EMS_DATA_WATER_SENSOR_PH] = ph;
	data[_EMS_DATA_WATER_SENSOR_O2] = o2;
	return (struct EMS_Entry){
		.data = data,
		.time = EMS_GlobalTimer
	};
}

static char* waterFormat(struct EMS_Entry* e, EMS_Option* s){
	// needs accurate buf sizing
	char* buf = malloc(200);
	float ph = ((float*)e->data)[_EMS_DATA_WATER_SENSOR_PH];
	float o2 = ((float*)e->data)[_EMS_DATA_WATER_SENSOR_O2];
	EMS_Time t = e->time;
	const char* prefix = "insert into WaterData(time, type, value) values";
	if(s[_EMS_SETTING_WATER_SENSOR_RECORDING_MODE] == _EMS_OPTION_WATER_SENSOR_PH){
		snprintf(buf, 200, "%s (%u, \'%s\', %.3f);", prefix, t, "pH", ph);
	}
	if(s[_EMS_SETTING_WATER_SENSOR_RECORDING_MODE] == _EMS_OPTION_WATER_SENSOR_O2){
		snprintf(buf, 200, "%s (%u, \'%s\', %.3f);", prefix, t, "O2", o2);
	}
	if(s[_EMS_SETTING_WATER_SENSOR_RECORDING_MODE] == _EMS_OPTION_WATER_SENSOR_BOTH){
		snprintf(buf, 200, "%s (%u, \'%s\', %.3f), (%u, \'%s\', %.3f);", prefix, t, "pH", ph, t, "O2", o2);
	}
	return buf;
}

struct EMS_Sensor makeWaterSensor(EMS_Pin phPin, EMS_Pin o2Pin, EMS_Option mode){
	struct EMS_Sensor s;
	
	s.pins = malloc(sizeof(EMS_Pin) * 2);
	// malloc check
	s.pins[_EMS_PIN_WATER_SENSOR_PH] = phPin;
	s.pins[_EMS_PIN_WATER_SENSOR_O2] = o2Pin;

	s.settings = malloc(sizeof(EMS_Option) * 1);
	s.settings[_EMS_SETTING_WATER_SENSOR_RECORDING_MODE] = mode;

	s.read = &waterSensorRead;
	s.format = &waterFormat;
	return s;
}
