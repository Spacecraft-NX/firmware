#include <fpga.h>
#include <config.h>
#include <leds.h>
#include <sdio.h>
#include <statuscode.h>

void jump_bootloader_sdio_handler();
extern int firmware_version;

void sdio_handler()
{
	leds_set_pattern_delayed(&lp_toolbox, 3000);

	while (1)
	{
		fpga_pre_recv();

		uint8_t buffer[512];
		fpga_select_active_buffer(FPGA_BUFFER_CMD_DATA);
		fpga_read_buffer(buffer, sizeof(buffer));
		fpga_post_recv();

		sdio_req_t *req = (sdio_req_t*)buffer;
		switch (req->cmd)
		{
			case FW_ENTER_DFU:
			{
				config_t cfg;
				if (config_load(&cfg) == OK_CONFIG)
				{
					cfg.reflash = 1;
					config_save(&cfg);
				}
				jump_bootloader_sdio_handler();
				return;
			}

			case FW_GET_VER:
			{
				sdio_resp_t *resp = (sdio_resp_t *)buffer;
				resp->cmd = (uint8_t)~FW_GET_VER;
				resp->fw_info = firmware_version;

				fpga_select_active_buffer(FPGA_BUFFER_CMD_DATA);
				fpga_write_buffer(buffer, sizeof(buffer));
				fpga_post_send();
				break;
			}

			case FW_DEEP_SLEEP:
				return;

			case FW_GET_TRAIN_DATA:
			{
				sdio_resp_t *resp = (sdio_resp_t *)buffer;
				resp->cmd = (uint8_t)~FW_GET_TRAIN_DATA;
				config_t cfg;
				resp->train_data.load_result = config_load(&cfg);
				resp->train_data.cfg = cfg;

				fpga_select_active_buffer(FPGA_BUFFER_CMD_DATA);
				fpga_write_buffer(buffer, sizeof(buffer));
				fpga_post_send();
				break;
			}

			case FW_SET_TRAIN_DATA:
			{
				sdio_resp_t *resp = (sdio_resp_t *)buffer;
				resp->cmd = (uint8_t)~FW_SET_TRAIN_DATA;

				if (req->train_data.magic == TRAIN_DATA_SET_MAGIC)
				{
					config_t cfg = req->train_data.cfg;
					config_save(&cfg);
					resp->train_data_ack = 0xA11600D;
				}
				else
					resp->train_data_ack = 0xBAD00001;

				fpga_select_active_buffer(FPGA_BUFFER_CMD_DATA);
				fpga_write_buffer(buffer, sizeof(buffer));
				fpga_post_send();
				break;
			}

			case FW_RESET_TRAIN_DATA:
			{
				sdio_resp_t *resp = (sdio_resp_t *)buffer;
				resp->cmd = (uint8_t)~FW_RESET_TRAIN_DATA;

				if (req->train_data.magic == TRAIN_DATA_RESET_MAGIC)
				{
					config_reset();
					leds_set_pattern(&lp_config_reset);
					leds_set_pattern_delayed(&lp_toolbox, 2000);
					resp->train_data_ack = 0xA11600D;
				}
				else
					resp->train_data_ack = 0xBAD00001;

				fpga_select_active_buffer(FPGA_BUFFER_CMD_DATA);
				fpga_write_buffer(buffer, sizeof(buffer));
				fpga_post_send();
				break;
			}

			case FW_SESSION_INFO:
			{
				sdio_resp_t *resp = (sdio_resp_t *)buffer;
				resp->cmd = (uint8_t)~FW_SESSION_INFO;
				resp->session_info.format = SESSION_INFO_FORMAT_VER;
				resp->session_info.magic = SESSION_INFO_MAGIC;
				resp->session_info.data = g_session_info;

				fpga_select_active_buffer(FPGA_BUFFER_CMD_DATA);
				fpga_write_buffer(buffer, sizeof(buffer));
				fpga_post_send();
				break;
			}

			case 2:
			{
				// Might be a DFU command with length 2, verify
				if (buffer[2] == 0xA0)
				{
					buffer[0] = (uint8_t)~buffer[0];
					*(uint32_t *)&buffer[1] = 0x50000000; // ERROR_UNIMPLEMENTED
					fpga_select_active_buffer(FPGA_BUFFER_CMD_DATA);
					fpga_write_buffer(buffer, sizeof(buffer));
					fpga_post_send();
				}
				break;
			}
		}
	}
}