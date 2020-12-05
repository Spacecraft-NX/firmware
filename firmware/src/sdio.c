#include <fpga.h>
#include <config.h>

void jump_bootloader_sdio_handler();
extern int firmware_version;

void sdio_handler()
{
	fpga_enter_cmd_mode();

	while (1)
	{
		fpga_pre_recv();

		uint8_t buffer[512];
		fpga_select_active_buffer(1);
		fpga_read_buffer(buffer, sizeof(buffer));
		fpga_post_recv();

		if (buffer[0] == 0xAA)
		{
			config cfg;
			if (config_load(&cfg) == 0x900D0007)
			{
				cfg.reflash = 1;
				config_save(&cfg);
			}
			jump_bootloader_sdio_handler();
		}
		else if (buffer[0] == 0x44)
		{
			buffer[0] = ~buffer[0];
			*(uint32_t *) &buffer[1] = firmware_version;
			fpga_select_active_buffer(1);
			fpga_write_buffer(buffer, sizeof(buffer));
			fpga_post_send();
		}
		else if (buffer[0] == 0x55)
			break;
	}
}