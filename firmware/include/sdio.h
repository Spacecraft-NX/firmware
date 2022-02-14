#ifndef __SDIO_H__
#define __SDIO_H__

#include "session_info.h"
#include "config.h"

enum FW_COMMAND
{
	FW_GET_VER = 0x44,
	FW_DEEP_SLEEP = 0x55,
	FW_GET_TRAIN_DATA = 0x66,
	FW_SET_TRAIN_DATA = 0x77,
	FW_RESET_TRAIN_DATA = 0x88,
	FW_SESSION_INFO = 0x99,
	FW_ENTER_DFU = 0xAA
};

#define TRAIN_DATA_RESET_MAGIC 0x14CCB847
#define TRAIN_DATA_SET_MAGIC 0xC88350AE

typedef struct
{
	uint8_t cmd; // FW_COMMAND
	struct
	{
		uint32_t magic;
		config_t cfg;
	} train_data;
} __attribute__((packed)) sdio_req_t;

typedef struct
{
	uint8_t cmd; // ~FW_COMMAND
	union
	{
		uint32_t fw_info;
		uint32_t train_data_ack;
		struct
		{
			uint32_t magic : 24;
			uint32_t format : 8;
			session_info_t data;			
		} session_info;
		struct
		{
			uint32_t load_result;
			config_t cfg;
		} train_data;
	};
} __attribute__((packed)) sdio_resp_t;

void sdio_handler();

#endif