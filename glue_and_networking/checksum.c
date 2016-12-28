#include "checksum.h"
#include "packet.h"

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
	struct packet_hdr* header = (struct packet_hdr*)packet_bytes;
	uint16_t transmitted_checksum = header->checksum;

	header->checksum = 0;

	return ones_checksum(packet_bytes) == transmitted_checksum;
}
