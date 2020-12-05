/*
 * Copyright (c) 2020 Spacecraft-NX
 * Copyright (c) 2017 GigaDevice Semiconductor Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <usbd_int.h>
#include <cdc_acm_core.h>

#define USBD_VID						  0xBAAD
#define USBD_PID						  0xC0DE

static uint32_t cdc_cmd = 0xFF;
static __IO uint32_t usbd_cdc_altset = 0;

__ALIGN_BEGIN uint8_t usb_send_data_buffer[CDC_ACM_DATA_PACKET_SIZE] __ALIGN_END;
__ALIGN_BEGIN uint8_t usb_recv_data_buffer[CDC_ACM_DATA_PACKET_SIZE] __ALIGN_END;
__ALIGN_BEGIN uint8_t usb_cmd_buffer[CDC_ACM_CMD_PACKET_SIZE] __ALIGN_END;

usbd_int_cb_struct *usbd_int_fops = NULL;

__IO uint8_t packet_sent = 1;
__IO uint8_t packet_receive = 1;
__IO uint32_t receive_length = 0;

__ALIGN_BEGIN line_coding_struct linecoding __ALIGN_END =
{
	115200, /* baud rate	 */
	0x00,   /* stop bits - 1 */
	0x00,   /* parity - none */
	0x08	/* num of bits 8 */
};

/* note:it should use the C99 standard when compiling the below codes */
/* USB standard device descriptor */
__ALIGN_BEGIN const usb_descriptor_device_struct device_descriptor __ALIGN_END =
{
	.Header = 
	 {
		 .bLength = USB_DEVICE_DESC_SIZE, 
		 .bDescriptorType = USB_DESCTYPE_DEVICE
	 },
	.bcdUSB = 0x0200,
	.bDeviceClass = 0x02,
	.bDeviceSubClass = 0x00,
	.bDeviceProtocol = 0x00,
	.bMaxPacketSize0 = USB_MAX_EP0_SIZE,
	.idVendor = USBD_VID,
	.idProduct = USBD_PID,
	.bcdDevice = 0x0100,
	.iManufacturer = USBD_MFC_STR_IDX,
	.iProduct = USBD_PRODUCT_STR_IDX,
	.iSerialNumber = USBD_SERIAL_STR_IDX,
	.bNumberConfigurations = USBD_CFG_MAX_NUM
};

/* USB device configuration descriptor */
__ALIGN_BEGIN const usb_descriptor_configuration_set_struct configuration_descriptor __ALIGN_END = 
{
	.config = 
	{
		.Header = 
		 {
			.bLength = sizeof(usb_descriptor_configuration_struct), 
			.bDescriptorType = USB_DESCTYPE_CONFIGURATION
		 },
		.wTotalLength = USB_CDC_ACM_CONFIG_DESC_SIZE,
		.bNumInterfaces = 0x02,
		.bConfigurationValue = 0x01,
		.iConfiguration = 0x00,
		.bmAttributes = 0x80,
		.bMaxPower = 0x32
	},

	.cdc_loopback_interface = 
	{
		.Header = 
		 {
			 .bLength = sizeof(usb_descriptor_interface_struct), 
			 .bDescriptorType = USB_DESCTYPE_INTERFACE 
		 },
		.bInterfaceNumber = 0x00,
		.bAlternateSetting = 0x00,
		.bNumEndpoints = 0x01,
		.bInterfaceClass = 0x02,
		.bInterfaceSubClass = 0x02,
		.bInterfaceProtocol = 0x01,
		.iInterface = 0x00
	},

	.cdc_loopback_header = 
	{
		.Header =
		 {
			.bLength = sizeof(usb_descriptor_header_function_struct), 
			.bDescriptorType = USB_DESCTYPE_CS_INTERFACE
		 },
		.bDescriptorSubtype = 0x00,
		.bcdCDC = 0x0110
	},

	.cdc_loopback_call_managment = 
	{
		.Header = 
		 {
			.bLength = sizeof(usb_descriptor_call_managment_function_struct), 
			.bDescriptorType = USB_DESCTYPE_CS_INTERFACE
		 },
		.bDescriptorSubtype = 0x01,
		.bmCapabilities = 0x00,
		.bDataInterface = 0x01
	},

	.cdc_loopback_acm = 
	{
		.Header = 
		 {
			.bLength = sizeof(usb_descriptor_acm_function_struct), 
			.bDescriptorType = USB_DESCTYPE_CS_INTERFACE
		 },
		.bDescriptorSubtype = 0x02,
		.bmCapabilities = 0x02,
	},

	.cdc_loopback_union = 
	{
		.Header = 
		 {
			.bLength = sizeof(usb_descriptor_union_function_struct), 
			.bDescriptorType = USB_DESCTYPE_CS_INTERFACE
		 },
		.bDescriptorSubtype = 0x06,
		.bMasterInterface = 0x00,
		.bSlaveInterface0 = 0x01,
	},

	.cdc_loopback_cmd_endpoint = 
	{
		.Header = 
		 {
			.bLength = sizeof(usb_descriptor_endpoint_struct), 
			.bDescriptorType = USB_DESCTYPE_ENDPOINT
		 },
		.bEndpointAddress = CDC_ACM_CMD_EP,
		.bmAttributes = 0x03,
		.wMaxPacketSize = CDC_ACM_CMD_PACKET_SIZE,
		.bInterval = 0x0A
	},

	.cdc_loopback_data_interface = 
	{
		.Header = 
		 {
			.bLength = sizeof(usb_descriptor_interface_struct), 
			.bDescriptorType = USB_DESCTYPE_INTERFACE
		 },
		.bInterfaceNumber = 0x01,
		.bAlternateSetting = 0x00,
		.bNumEndpoints = 0x02,
		.bInterfaceClass = 0x0A,
		.bInterfaceSubClass = 0x00,
		.bInterfaceProtocol = 0x00,
		.iInterface = 0x00
	},

	.cdc_loopback_out_endpoint = 
	{
		.Header = 
		 {
			 .bLength = sizeof(usb_descriptor_endpoint_struct), 
			 .bDescriptorType = USB_DESCTYPE_ENDPOINT 
		 },
		.bEndpointAddress = CDC_ACM_DATA_OUT_EP,
		.bmAttributes = 0x02,
		.wMaxPacketSize = CDC_ACM_DATA_PACKET_SIZE,
		.bInterval = 0x00
	},

	.cdc_loopback_in_endpoint = 
	{
		.Header = 
		 {
			 .bLength = sizeof(usb_descriptor_endpoint_struct), 
			 .bDescriptorType = USB_DESCTYPE_ENDPOINT 
		 },
		.bEndpointAddress = CDC_ACM_DATA_IN_EP,
		.bmAttributes = 0x02,
		.wMaxPacketSize = CDC_ACM_DATA_PACKET_SIZE,
		.bInterval = 0x00
	}
};

/* USB language ID Descriptor */
__ALIGN_BEGIN const usb_descriptor_language_id_struct usbd_language_id_desc __ALIGN_END = 
{
	.Header = 
	 {
		 .bLength = sizeof(usb_descriptor_language_id_struct), 
		 .bDescriptorType = USB_DESCTYPE_STRING
	 },
	.wLANGID = ENG_LANGID
};

__ALIGN_BEGIN uint8_t* usbd_strings[] __ALIGN_END = 
{
	[USBD_LANGID_STR_IDX] = (uint8_t *)&usbd_language_id_desc,
	[USBD_MFC_STR_IDX] = USBD_STRING_DESC("MCU"),
	[USBD_PRODUCT_STR_IDX] = USBD_STRING_DESC("DFU"),
	[USBD_SERIAL_STR_IDX] = USBD_STRING_DESC("SERIAL")
};

/*!
	\brief	  initialize the CDC ACM device
	\param[in]  pudev: pointer to USB device instance
	\param[in]  config_index: configuration index
	\param[out] none
	\retval	 USB device operation status
*/
uint8_t cdc_acm_init (void *pudev, uint8_t config_index)
{
	/* initialize the data Tx/Rx endpoint */
	usbd_ep_init(pudev, &(configuration_descriptor.cdc_loopback_in_endpoint));
	usbd_ep_init(pudev, &(configuration_descriptor.cdc_loopback_out_endpoint));

	/* initialize the command Tx endpoint */
	usbd_ep_init(pudev, &(configuration_descriptor.cdc_loopback_cmd_endpoint));

	return USBD_OK;
}

/*!
	\brief	  de-initialize the CDC ACM device
	\param[in]  pudev: pointer to USB device instance
	\param[in]  config_index: configuration index
	\param[out] none
	\retval	 USB device operation status
*/
uint8_t cdc_acm_deinit (void *pudev, uint8_t config_index)
{
	/* deinitialize the data Tx/Rx endpoint */
	usbd_ep_deinit(pudev, CDC_ACM_DATA_IN_EP);
	usbd_ep_deinit(pudev, CDC_ACM_DATA_OUT_EP);

	/* deinitialize the command Tx endpoint */
	usbd_ep_deinit(pudev, CDC_ACM_CMD_EP);

	return USBD_OK;
}

/*!
	\brief	  handle CDC ACM data
	\param[in]  pudev: pointer to USB device instance
	\param[in]  rx_tx: data transfer direction:
	  \arg		USBD_TX
	  \arg		USBD_RX
	\param[in]  ep_id: endpoint identifier
	\param[out] none
	\retval	 USB device operation status
*/
uint8_t cdc_acm_data_handler (void *pudev, usb_dir_enum rx_tx, uint8_t ep_num)
{
	if ((USB_TX == rx_tx) && ((CDC_ACM_DATA_IN_EP & 0x7F) == ep_num)) {
		packet_sent = 1;
		return USBD_OK;
	} else if ((USB_RX == rx_tx) && ((EP0_OUT & 0x7F) == ep_num)) {
		cdc_acm_EP0_RxReady (pudev);
	} else if ((USB_RX == rx_tx) && ((CDC_ACM_DATA_OUT_EP & 0x7F) == ep_num)) {
		packet_receive = 1;
		receive_length = usbd_rxcount_get(pudev, CDC_ACM_DATA_OUT_EP);
		return USBD_OK;
	}
	return USBD_FAIL;
}

/*!
	\brief	  handle the CDC ACM class-specific requests
	\param[in]  pudev: pointer to USB device instance
	\param[in]  req: device class-specific request
	\param[out] none
	\retval	 USB device operation status
*/
uint8_t cdc_acm_req_handler (void *pudev, usb_device_req_struct *req)
{
	uint16_t len = CDC_ACM_DESC_SIZE;
	uint8_t  *pbuf= (uint8_t*)(&configuration_descriptor) + 9;

	switch (req->bmRequestType & USB_REQ_MASK) {
	case USB_CLASS_REQ:
		switch (req->bRequest) {
		case SEND_ENCAPSULATED_COMMAND:
			break;
		case GET_ENCAPSULATED_RESPONSE:
			break;
		case SET_COMM_FEATURE:
			break;
		case GET_COMM_FEATURE:
			break;
		case CLEAR_COMM_FEATURE:
			break;
		case SET_LINE_CODING:
			/* set the value of the current command to be processed */
			cdc_cmd = req->bRequest;
			/* enable EP0 prepare to receive command data packet */
			len = USB_MIN(CDC_ACM_CMD_PACKET_SIZE, req->wLength);
			usbd_ctlrx (pudev, usb_cmd_buffer, len);
			break;
		case GET_LINE_CODING:
			usb_cmd_buffer[0] = (uint8_t)(linecoding.dwDTERate);
			usb_cmd_buffer[1] = (uint8_t)(linecoding.dwDTERate >> 8);
			usb_cmd_buffer[2] = (uint8_t)(linecoding.dwDTERate >> 16);
			usb_cmd_buffer[3] = (uint8_t)(linecoding.dwDTERate >> 24);
			usb_cmd_buffer[4] = linecoding.bCharFormat;
			usb_cmd_buffer[5] = linecoding.bParityType;
			usb_cmd_buffer[6] = linecoding.bDataBits;
			/* send the request data to the host */
			len = USB_MIN(CDC_ACM_CMD_PACKET_SIZE, req->wLength);
			usbd_ctltx (pudev, usb_cmd_buffer, len);
			break;
		case SET_CONTROL_LINE_STATE:
			break;
		case SEND_BREAK:
			break;
		default:
			break;
		}
		break;
	case USB_STANDARD_REQ:
		/* standard device request */
		switch(req->bRequest) {
		case USBREQ_GET_INTERFACE:
			usbd_ctltx(pudev, (uint8_t *)&usbd_cdc_altset, 1);
			break;
		case USBREQ_SET_INTERFACE:
			if ((uint8_t)(req->wValue) < USBD_ITF_MAX_NUM) {
				usbd_cdc_altset = (uint8_t)(req->wValue);
			} else {
				/* call the error management function (command will be nacked) */
				usbd_enum_error (pudev, req);
			}
			break;
		case USBREQ_GET_DESCRIPTOR:
			if(CDC_ACM_DESC_TYPE == (req->wValue >> 8)){
				len = USB_MIN(CDC_ACM_DESC_SIZE, req->wLength);
				pbuf = (uint8_t*)(&configuration_descriptor) + 9 + (9 * USBD_ITF_MAX_NUM);
			}

			usbd_ctltx(pudev, pbuf, len);
			break;
		default:
			break;
		}
	default:
		break;
	}

	return USBD_OK;
}

/*!
	\brief	  receive CDC ACM data
	\param[in]  pudev: pointer to USB device instance
	\param[out] none
	\retval	 USB device operation status
*/
void cdc_acm_data_receive(void *pudev)
{
	packet_receive = 0;

	usbd_ep_rx(pudev, CDC_ACM_DATA_OUT_EP, (uint8_t*)(usb_recv_data_buffer), CDC_ACM_DATA_PACKET_SIZE);
}

/*!
	\brief	  send CDC ACM data
	\param[in]  pudev: pointer to USB device instance
	\param[out] none
	\retval	 USB device operation status
*/
void cdc_acm_data_send (void *pudev, uint32_t data_len)
{
	/* limit the transfer data length */
	if (data_len <= CDC_ACM_DATA_PACKET_SIZE) {
		packet_sent = 0;
		usbd_ep_tx(pudev, CDC_ACM_DATA_IN_EP, (uint8_t*)(usb_send_data_buffer), data_len);
	}
}

/*!
	\brief	  command data received on control endpoint
	\param[in]  pudev: pointer to USB device instance
	\param[out] none
	\retval	 USB device operation status
*/
usbd_status_enum cdc_acm_EP0_RxReady (void *pudev)
{
	if (NO_CMD != cdc_cmd) {
		/* process the command data */
		linecoding.dwDTERate = (uint32_t)(usb_cmd_buffer[0] | 
										 (usb_cmd_buffer[1] << 8) |
										 (usb_cmd_buffer[2] << 16) |
										 (usb_cmd_buffer[3] << 24));

		linecoding.bCharFormat = usb_cmd_buffer[4];
		linecoding.bParityType = usb_cmd_buffer[5];
		linecoding.bDataBits = usb_cmd_buffer[6];

		cdc_cmd = NO_CMD;
	}

	return USBD_OK;
}
