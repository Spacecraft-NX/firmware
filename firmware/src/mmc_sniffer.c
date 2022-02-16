#include "mmc_sniffer.h"
#include "mmc_defs.h"

void mmc_sniff_parser_init(mmc_sniff_parser_ctx *ctx, uint8_t *data, int datalen)
{
	ctx->data = data;
	ctx->datalen = datalen;
	ctx->cmd = 0;
}

enum MMC_SNIFFER_PACKET_TYPE mmc_sniff_parser_parse(mmc_sniff_parser_ctx *ctx)
{
	if (ctx->datalen < 6)
		return MMC_SNIFF_PKT_TYPE_INVALID; // insufficient data left to sniff full command

	uint8_t *data = ctx->data;
	char flags = data[0];
	ctx->cmd = flags & 0x3F;

	enum MMC_SNIFFER_PACKET_TYPE packet_type;
	if (flags & 0x40) // host --> device command
	{
		packet_type = MMC_SNIFF_PKT_TYPE_COMMAND;
		// Host commands are always 48-bit in length
		ctx->data += 6;
		ctx->datalen -= 6;
	}
	else if (ctx->cmd == MMC_ALL_SEND_CID || ctx->cmd == MMC_SEND_CSD || ctx->cmd == MMC_SEND_CID)
	{
		// These 3 commands prompt an 'R2' response type of 136 bit length
		packet_type = MMC_SNIFF_PKT_TYPE_RESPONSE136;
		ctx->data += 17;
		ctx->datalen -= 17;
	}
	else
	{
		// All other commands receive a 48-bit response
		packet_type = MMC_SNIFF_PKT_TYPE_RESPONSE48;
		ctx->data += 6;
		ctx->datalen -= 6;
	}

	return packet_type;
}
