#include "glitch_heuristic.h"
#include <stdlib.h>

void heuristic_add_result(glitch_heuristic_t *heuristic, enum GLITCH_RESULT_TYPE result)
{
	switch (result)
	{
		case GLITCH_RESULT_FAIL_NO_EMMC_COMMS:
			heuristic->no_comms_count++;

		case GLITCH_RESULT_FAIL_TIMEOUT:
			heuristic->timeout_count++;
			break;

		case GLITCH_RESULT_FAILED_MMC:
			heuristic->block_read_count++;
			break;
	}
}

void heuristic_advice(glitch_heuristic_t *heuristic, bool *fatal_abort, bool *try_next_offset, int *width_adjust, int *offset_adjust)
{
	heuristic->total_count++;
	if ((heuristic->total_count % 8))
	{
		// Take decisions only after 8 and 16 inputs
		*fatal_abort = false;
		*try_next_offset = false;
		*width_adjust = 0;
		*offset_adjust = 0;
	}
	else
	{
		*fatal_abort = heuristic->no_comms_count >= 8;

		if (heuristic->block_read_count > 5)
			*width_adjust = 1; // most of the time, glitch pulse had no observable effect, so use longer width
		else if (heuristic->timeout_count > 5)
			*width_adjust = -1; // most of the time, glitch pulse caused CPU hang, so use shorter width
		else
			*width_adjust = 0;

		// Start looping through offsets when no longer unanimous about pulse width
		*try_next_offset = abs(heuristic->block_read_count - heuristic->timeout_count) < 6 || heuristic->total_count == 16;

		// First 8 attempts subcycle increments, so total offset is slightly longer than baseline
		// Next 8 attempts: use offset-1, i.e. slightly shorter than baseline
		*offset_adjust = !*try_next_offset && heuristic->total_count == 16 ? -1 : 0;

		// Reset category counters
		heuristic->no_comms_count = 0;
		heuristic->timeout_count = 0;
		heuristic->block_read_count = 0;
		// Reset entire heuristic after 16 cycles
		heuristic->total_count %= 16;
	}
}
