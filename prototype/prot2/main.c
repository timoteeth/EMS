#include <stdint.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>

typedef int32_t EMS_Int;
typedef float EMS_Float;

typedef uint32_t EMS_Time;

typedef uint8_t EMS_Option;
typedef uint8_t EMS_OptionCount;

typedef uint8_t EMS_Pin;
typedef uint8_t EMS_PinCount;

static EMS_Time clock;

enum EMS_DATA_TYPE{
	EMS_DATA_TYPE_INT,
	EMS_DATA_TYPE_FLOAT
};

size_t typeSizes[] = {
	[EMS_DATA_TYPE_INT] = sizeof(EMS_Int),
	[EMS_DATA_TYPE_FLOAT] = sizeof(EMS_Float),
};

struct EMS_DataPoint{
	EMS_Time recordedTime;
	union{
		EMS_Int dataInt;
		EMS_Float dataFloat;
	};
};

struct EMS_DataSeries{
	const char* name;
	enum EMS_DATA_TYPE type;
	struct EMS_DataPoint* array;
	struct EMS_DataSeries* next;
	size_t arrayLen;
};

enum EMS_SENSOR_TYPE{
	EMS_SENSOR_TYPE_PH,
	EMS_SENSOR_TYPE_WATER,
	EMS_SENSOR_TYPE_AIR,
};

struct EMS_Sensor{
	enum EMS_SENSOR_TYPE type;
	EMS_Option* settings;
	EMS_Pin* pins;
};

enum EMS_SETTING{
	EMS_SETTING_NULL = 0,

	EMS_SETTING_PH_MODE = 1,
};

enum{
	EMS_OPTION_PH_MODE_INT = 0,
	EMS_OPTION_PH_MODE_FLOAT = 1
};

enum EMS_PIN{
	EMS_PIN_NULL = 0,

	EMS_PIN_PH_MAIN = 1,

	EMS_PIN_WATER_O2 = 1,
	EMS_PIN_WATER_N2,

	EMS_PIN_AIR_MAIN = 1,
};

enum EMS_READ_MODE{
	EMS_READ_WATER_O2 = 0,
	EMS_READ_WATER_N2,
};

EMS_OptionCount sensorOptionCount[] = {
	[EMS_SENSOR_TYPE_PH] = 1,
	[EMS_SENSOR_TYPE_WATER] = 0,
	[EMS_SENSOR_TYPE_AIR] = 0,
};

EMS_PinCount sensorPinCount[] = {
	[EMS_SENSOR_TYPE_PH] = 1,
	[EMS_SENSOR_TYPE_WATER] = 2,
	[EMS_SENSOR_TYPE_AIR] = 1,
};

struct EMS_Sensor createSensor(enum EMS_SENSOR_TYPE type){
	struct EMS_Sensor s = {.type = type};
	s.settings = malloc(sizeof(EMS_Option) * sensorOptionCount[type]);
	memset(s.settings, 0, sizeof(EMS_Option) * sensorOptionCount[type]);
	s.pins = malloc(sizeof(EMS_Pin) * sensorPinCount[type]);
	memset(s.pins, 0, sizeof(EMS_Pin) * sensorPinCount[type]);
	return s;
}

void freeSensor(struct EMS_Sensor* sensor){
	if(sensor->settings){
		free(sensor->settings);
		sensor->settings = NULL;
	}
	if(sensor->pins){
		free(sensor->pins);
		sensor->pins = NULL;
	}
}

EMS_Option setSensorSetting(struct EMS_Sensor* sensor, enum EMS_SETTING setting, EMS_Option option){
	EMS_OptionCount c = sensorOptionCount[sensor->type];
	if(setting > c){
		return EMS_SETTING_NULL;
	}
	EMS_Option old = sensor->settings[setting - 1];
	sensor->settings[setting - 1] = option;
	return old;
}

EMS_Option getSensorSetting(const struct EMS_Sensor* sensor, enum EMS_SETTING setting){
	EMS_OptionCount c = sensorOptionCount[sensor->type];
	if(setting > c){
		return EMS_SETTING_NULL;
	}
	return sensor->settings[setting - 1];
}

EMS_Pin setSensorPin(struct EMS_Sensor* sensor, enum EMS_PIN pin, EMS_Pin v){
	EMS_PinCount c = sensorPinCount[sensor->type];
	if(pin > c){
		return EMS_PIN_NULL;
	}
	EMS_Pin old = sensor->pins[pin - 1];
	sensor->pins[pin - 1] = v;
	return old;
}

static struct EMS_DataSeries* seriesHead = NULL;

bool registerDataSeries(const char* name, enum EMS_DATA_TYPE type){
	struct EMS_DataSeries** ptr = &seriesHead;
	while(*ptr != NULL){
		if(strcmp((*ptr)->name, name) == 0){
			return false;
		}
		ptr = &(*ptr)->next;
	}
	struct EMS_DataSeries* newSeries = malloc(sizeof(struct EMS_DataSeries));
	newSeries->name = name;
	newSeries->type = type;
	newSeries->array = malloc(0);
	newSeries->next = NULL;
	newSeries->arrayLen = 0;
	*ptr = newSeries;
	return true;
}

bool addDataToSeries(const char* series, void* data, EMS_Time clock){
	struct EMS_DataSeries* s = seriesHead;
	while(s){
		if(strcmp(s->name, series) == 0){
			break;
		}
		s = s->next;
	}
	if(!s){
		return false;
	}
	void* p = realloc(s->array, sizeof(struct EMS_DataPoint) * (++s->arrayLen));
	if(!p){
		puts("uhhhh");
		return false;
	}
	s->array = p;
	if(s->type == EMS_DATA_TYPE_INT){
		s->array[s->arrayLen - 1].dataInt = *((EMS_Int*)data);
	}else if(s->type == EMS_DATA_TYPE_FLOAT){
		s->array[s->arrayLen - 1].dataFloat = *((EMS_Float*)data);
	}
	s->array[s->arrayLen - 1].recordedTime = clock;
	return true;
}

bool addDataPointToSeries(const char* series, struct EMS_DataPoint* data){
	struct EMS_DataSeries* s = seriesHead;
	while(s){
		if(strcmp(s->name, series) == 0){
			break;
		}
		s = s->next;
	}
	if(!s){
		return false;
	}
	void* p = realloc(s->array, sizeof(struct EMS_DataPoint) * (++s->arrayLen));
	if(!p){
		puts("uhhhh");
		return false;
	}
	s->array = p;
	if(s->type == EMS_DATA_TYPE_INT){
		data->dataInt = data->dataFloat;
	}
	s->array[s->arrayLen - 1] = *data;
	return true;
}

void printAllData(void){
	struct EMS_DataSeries* s = seriesHead;
	while(s){
		puts(s->name);
		if(s->type == EMS_DATA_TYPE_INT){
			puts("ints");
			for(size_t idx = 0; idx < s->arrayLen; ++idx){
				printf("%" PRId32 " @ %" PRIu32 "\n", ((struct EMS_DataPoint*)s->array)[idx].dataInt, ((struct EMS_DataPoint*)s->array)[idx].recordedTime);
			}
		}else if(s->type == EMS_DATA_TYPE_FLOAT){
			puts("floats");
			for(size_t idx = 0; idx < s->arrayLen; ++idx){
				printf("%f @ %" PRIu32 "\n", ((struct EMS_DataPoint*)s->array)[idx].dataFloat, ((struct EMS_DataPoint*)s->array)[idx].recordedTime);
			}
		}
		s = s->next;
	}
}

static float pinmap[] = {
	0,
	1,
	2,
	3,
	4.5,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13
};

static float readPin(const struct EMS_Sensor* s, enum EMS_PIN pin){
	return pinmap[s->pins[pin - 1]];
}

static void phsensorread(const struct EMS_Sensor* s, struct EMS_DataPoint* d, enum EMS_READ_MODE){
	EMS_Option mode = getSensorSetting(s, EMS_SETTING_PH_MODE);
	float value = readPin(s, EMS_PIN_PH_MAIN);
	if(mode == EMS_OPTION_PH_MODE_INT){
		d->dataFloat = (int)value;
	}else if(mode == EMS_OPTION_PH_MODE_FLOAT){
		d->dataFloat = value;
	}
}

static void watersensorread(const struct EMS_Sensor* s, struct EMS_DataPoint* d, enum EMS_READ_MODE m){
	if(m == EMS_READ_WATER_O2){
		float value = readPin(s, EMS_PIN_WATER_O2);
		d->dataFloat = value;
	}else if(m == EMS_READ_WATER_N2){
		float value = readPin(s, EMS_PIN_WATER_N2);
		d->dataFloat = value;
	}
}

void (*readArray[])(const struct EMS_Sensor*, struct EMS_DataPoint*, enum EMS_READ_MODE) = {
	[EMS_SENSOR_TYPE_PH] = &phsensorread,
	[EMS_SENSOR_TYPE_WATER] = &watersensorread,
};

static EMS_Time clock = 0;

struct EMS_DataPoint readSensor(const struct EMS_Sensor* s){
	struct EMS_DataPoint d = {.recordedTime = ++clock};
	readArray[s->type](s, &d, 0);
	return d;
}

struct EMS_DataPoint readSensorMode(const struct EMS_Sensor* s, enum EMS_READ_MODE m){
	struct EMS_DataPoint d = {.recordedTime = ++clock};
	readArray[s->type](s, &d, m);
	return d;
}

int main(void){
	//assert(sizeof(float) == sizeof(int32_t));
	struct EMS_Sensor pHSensor = createSensor(EMS_SENSOR_TYPE_PH);
	setSensorPin(&pHSensor, EMS_PIN_PH_MAIN, 4);
	setSensorSetting(&pHSensor, EMS_SETTING_PH_MODE, EMS_OPTION_PH_MODE_INT);

	struct EMS_Sensor waterSensor = createSensor(EMS_SENSOR_TYPE_WATER);
	setSensorPin(&waterSensor, EMS_PIN_WATER_O2, 7);
	setSensorPin(&waterSensor, EMS_PIN_WATER_N2, 8);

	registerDataSeries("ph_int", EMS_DATA_TYPE_INT);
	registerDataSeries("ph_float", EMS_DATA_TYPE_FLOAT);
	registerDataSeries("o2", EMS_DATA_TYPE_FLOAT);
	registerDataSeries("n2", EMS_DATA_TYPE_FLOAT);

	struct EMS_DataPoint data;
	data = readSensor(&pHSensor);
	addDataPointToSeries("ph_int", &data);
	
	setSensorSetting(&pHSensor, EMS_SETTING_PH_MODE, EMS_OPTION_PH_MODE_FLOAT);
	data = readSensor(&pHSensor);
	addDataPointToSeries("ph_float", &data);

	data = readSensorMode(&waterSensor, EMS_READ_WATER_O2);
	addDataPointToSeries("o2", &data);
	data = readSensorMode(&waterSensor, EMS_READ_WATER_N2);
	addDataPointToSeries("n2", &data);
	
	printAllData();
	freeSensor(&pHSensor);
	freeSensor(&waterSensor);
	return 0;
}
