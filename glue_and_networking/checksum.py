def ones_checksum(packet_bytes):
	checksum = 0

	if len(packet_bytes) % 2 == 1
		packet_bytes = packet_bytes + bytearray(1)

	for i in range(0, len(packet_bytes)/2)
		current_word = packet_bytes[i*2] << 8
		current_word |= packet_bytes[i*2 + 1]

		checksum += current_word
		if (checksum >> 16) == 1
			checksum = checksum & 0xFFFF
			checksum += 1

	checksum = ~checksum

	return checksum

def check_checksum(packet_bytes):
	transmitted_checksum = packet_bytes[0] << 8
	transmitted_checksum |= packet_bytes[1]

	packet_bytes[0] = 0
	packet_bytes[1] = 0

	return ones_checksum(packet_bytes) == transmitted_checksum
