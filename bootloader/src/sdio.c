#include <dfu.h>
#include <fpga.h>
#include <gd32f3x0.h>

uint8_t *g_receive_buffer;

int receive_data()
{
	fpga_select_active_buffer(FPGA_BUFFER_CMD_DATA);
	fpga_read_buffer(g_receive_buffer, 512);
	fpga_post_recv();

	return g_receive_buffer[0];
}

uint8_t *g_send_buffer;

void send_data(int len)
{
	g_send_buffer[0] = ~g_receive_buffer[0];
	fpga_select_active_buffer(FPGA_BUFFER_CMD_DATA);
	fpga_write_buffer(g_send_buffer, 512);
	fpga_post_send();
}

void wait_till_ready()
{
	fpga_pre_recv();
}

void SDIO_Handler()
{
	uint8_t receive_buffer[512];
	uint8_t send_buffer[512];

	struct bootloader_usb usb;
	usb.receive_buffer = &receive_buffer[1];
	g_receive_buffer = receive_buffer;
	usb.send_buffer = &send_buffer[1];
	g_send_buffer = send_buffer;
	usb.receive_data = receive_data;
	usb.send_data = send_data;
	usb.wait_till_ready = wait_till_ready;

	SCB->VTOR = 0x8000000;
	dfu(&usb);
}