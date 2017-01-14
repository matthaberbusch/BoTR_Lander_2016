from struct import *
import checksum

PACK_ACK = 2
PACK_DAT = 3
PACK_UNK = 0

MAX_PACK_SIZE = 1024

header_format = '<HHBB'
header_size = 6

def create_packet(to_packetize, packet_type, seq_num)
	tmp = pack(header_format + str(len(to_packetize)) + 's', 0, len(to_packetize) + header_size, packet_type, seq_num)
	new_checksum = ones_checksum(tmp)
	return pack(header_format + str(len(to_packetize)) + 's', new_checksum, len(to_packetize) + header_size, packet_type, seq_num)

def unpack_packet(to_unpack)
	length = to_unpack[2] << 8
	length |= to_unpack[3]

	return unpack(header_format + str(length - header_size) + 's', to_unpack)
