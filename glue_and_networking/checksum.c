#include "checksum.h"

uint16_t ones_checksum (char* packet_bytes, int len)
{
	uint16_t checksum = 0;
	uint16_t current_word;
	int i = 0;

	while (i < len)
	{
		current_word = packet_bytes[i] << 8;
		i++;
		if (i < len)
			current_word |= packet_bytes[i];
		i++;

		checksum += current_word;
		if ((checksum >> 16) == 1)
		{
			checksum = checksum & 0xFFFF;
			checksum++;
		}
	}

	checksum = ~checksum;

	return checksum;
}

char check_checksum (char* packet_bytes, int len);
{
	uint16_t transmitted_checksum = packet_bytes[0] << 8;
	transmitted_checksum |= packet_bytes[1];

	packet_bytes[0] = 0;
	packet_bytes[1] = 0;

	return ones_checksum(packet_bytes) == transmitted_checksum;
}
