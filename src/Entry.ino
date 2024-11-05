// Team Ecosense
// Environmental Monitoring System
// Last updated: 11/5/2024
// 
// Entry Structure
//
// this code sets up the struct for a data entry and all associated variables
struct Entry{
	char* data;	// raw data read from sensor
	uint8_t size;	// length of data
	uint32_t time;	// time data was read
};
