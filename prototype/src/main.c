#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mysql/mysql.h>

#include "Entry.h"
#include "Sensor.h"
#include "types.h"
#include "timer.h"

#include "sensors/WaterSensor.h"

#define TOTAL_PINS 13

struct EMS_Entry readSensor(struct EMS_Sensor* s){
	return s->read(s->pins);
}

void printEntry(struct EMS_Sensor* s, struct EMS_Entry* e){
	char* str = s->format(e, s->settings);
	printf("%u %s\n", e->time, str);
	free(str);
}

void freeEntry(struct EMS_Entry* e){
	free(e->data);
}

void freeSensor(struct EMS_Sensor* s){
	free(s->pins);
	free(s->settings);
}

float pinGet(EMS_Pin p){
	if(p >= TOTAL_PINS){
		return -1.f;
	}

	if(p == 3){
		return rand() / (float)RAND_MAX * (0.7f - 0.5f) + 0.5f;
	}else if(p == 5){
		return rand() / (float)RAND_MAX * (0.8f - 0.75f) + 0.75f;
	}
	return 0.f;
}

int main(int argc, char* argv[]){
	struct EMS_Sensor arr[] = {
		makeWaterSensor(3, 5, _EMS_OPTION_WATER_SENSOR_BOTH),
		//makeWaterSensor(3, 5, _EMS_OPTION_WATER_SENSOR_PH),
		//makeWaterSensor(3, 5, _EMS_OPTION_WATER_SENSOR_O2),
	};

	MYSQL* c;

	c = mysql_init(NULL);

	if(!mysql_real_connect(c, argv[1], "connor", "connor", "test", 3306, NULL, 0)){
		puts("fuck");
		return 1;
	}

	mysql_query(c, "if table exists drop WaterData;");
	mysql_query(c, "create table WaterData (time INT, type CHAR(100), value FLOAT);");

	for(int a = 0; a < 10; ++a){
		sleep(1);
		EMS_GlobalTimer++;
		struct EMS_Entry e;
		for(int a = 0; a < sizeof(arr) / sizeof(arr[0]); ++a){
			e = readSensor(arr + a);
			char* str = arr[a].format(&e, arr[a].settings);
			puts(str);
			mysql_query(c, str);
			free(str);
			freeEntry(&e);
		}
	}

	mysql_close(c);
	return 0;
}
