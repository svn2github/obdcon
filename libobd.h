/*************************************************************************
* libobd - The OBD-II access library
* Distributed under MPL 1.1
*
* Copyright (c) 2010 Stanley Huang <stanleyhuangyc@gmail.com>
* All rights reserved.
**************************************************************************/

#ifndef _LIBOBD_H
#define _LIBOBD_H

#include <string>
#include "ctb.h"

#define QUERY_INTERVAL 200
#define QUERY_INTERVAL_MIN 50
#define QUERY_INTERVAL_MAX 500
#define QUERY_INTERVAL_STEP 50
#define ADAPT_PERIOD  60000 /* ms */

typedef struct {
	DWORD time;
	int value;
} PID_DATA;

typedef struct {
	int pid;
	int dataBytes;
	int priority;
	const char* name;
	int active;
	PID_DATA data;
} PID_INFO;

#define PID_RPM 0x010C
#define PID_SPEED 0x010D
#define PID_THROTTLE 0x0111 
#define PID_ENGINE_LOAD 0x0104
#define PID_COOLANT_TEMP 0x0105
#define PID_FUEL_SHORT_TERM_1 0x0106
#define PID_FUEL_LONG_TERM_1 0x0107
#define PID_FUEL_SHORT_TERM_2 0x0108
#define PID_FUEL_LONG_TERM_2 0x0109
#define PID_INTAKE_TEMP 0x010F
#define PID_MAF_FLOW 0x0110
#define PID_ABS_LOAD 0x0143

#define INVALID_PID_DATA 0x80000000

#define SPECIAL_DELIMITER   '\t'

//process_response return values
#define HEX_DATA           0
#define BUS_BUSY           1
#define BUS_ERROR          2
#define BUS_INIT_ERROR     3
#define UNABLE_TO_CONNECT  4
#define CAN_ERROR          5
#define DATA_ERROR         6
#define DATA_ERROR2        7
#define ERR_NO_DATA        8
#define BUFFER_FULL        9
#define SERIAL_ERROR       10
#define UNKNOWN_CMD        11
#define BUS_STOPPED        12
#define RUBBISH            13

#define INTERFACE_ID       13
#define INTERFACE_ELM320   13
#define INTERFACE_ELM322   14
#define INTERFACE_ELM323   15
#define INTERFACE_ELM327   16
#define INTERFACE_OBDLINK  17
#define STN_MFR_STRING     18
#define ELM_MFR_STRING     19

#if defined(BUILD_DLL)
#define EXPORT __declspec(dllexport)
#elif defined(LIBOBD_STATIC)
#define EXPORT 
#else
#define EXPORT __declspec(dllimport)
#endif

class COBD;

class EXPORT COBD
{
public:
	COBD():device(0),running(true),lastTick(0),comport(4),baudrate(115200),queryInterval(QUERY_INTERVAL),fplog(0)
	{
		memset(protocol, 0, sizeof(protocol));
		strcpy(protocol, "8N1");
	}
	~COBD() { 
		Uninit();
	}
	void ClearFlags();
	bool StartLogging(const char* dir = 0);
	void StopLogging();
	void QuerySensor(int id);
	char* SendCommand(const char* cmd, const char* answer = 0);
	DWORD Update();
	bool Init();
	void Uninit();
	bool WaitReady(int seconds = 5);
	void Wait(int interval, int minimum = 10);
	static PID_INFO* GetPidInfo(int pid);
	static PID_INFO* GetPidInfo(const char* name);
	int queryInterval;
	int comport;
	int baudrate;
	char protocol[4];
	bool running;
private:
	int RetrieveValue(int pid_l, char* data);
	int ProcessResponse(char *msg_received);
	bool RetrieveSensor(int pid);
	DWORD lastTick;
	DWORD startTime;
	ctb::IOBase* device;
	char rcvbuf[256];
	FILE* fplog;
};

#ifdef WINCE
#define MSG(x) MessageBox(0, TEXT(x), 0, MB_OK);
#else
#define MSG(x) fprintf(stderr, "%s\n", x);
#endif

#endif
