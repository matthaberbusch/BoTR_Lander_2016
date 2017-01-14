import packet
import checksum

can_send = True
recv_timeout = 0
send_timeout = 0
recv_function = None
send_function = None
to_send = bytearray(MAX_PACK_SIZE)
to_recv = bytearray(MAX_PACK_SIZE)
recv_data_buf = bytearray(MAX_PACK_SIZE)
send_data_len = 0
recv_new_data = False
has_unack_data = False
send_seq = 0
recv_seq = 0
reset_counter = 0

RESET_LIMIT = 10

def initialize(__recv_function, __send_function, __recv_timeout, __send_timeout):
	global recv_function
	global send_function
	global recv_timeout
	global send_timeout

	recv_function = __recv_function
	send_function = __send_function
	recv_timeout = __recv_timeout
	send_timeout = __send_timeout

def recv_cycle():
	buf = bytearray(MAX_PACK_SIZE)

	buf = recv_function(MAX_PACK_SIZE, recv_timeout)
	

	if len(buf) == 0:
		return
	else if not check_checksum(buf):
		return
	else:
		parsed_packet = unpack_packet(buf)

		if parsed_packet[2] == PACK_UNK:
			return
		else if parsed_packet[2] == PACK_ACK:
			if parsed_packet[3] == send_seq:
				can_send = True
				has_unack_data = False

				send_seq++

				reset_counter = 0
			else if parsed_packet[3] == 0:
				reset_counter++
		else if parsed_packet[2] == PACK_DAT:
			if parsed_packet[3] == recv_seq:
				global to_recv

				to_recv[:] = buf
				recv_new_data = True

				recv_seq++

				reset_counter = 0
			else if parsed_packet[3] == 0
				reset_counter++

			send_function(create_packet(bytearray(0), PACK_ACK, recv_seq - 1))

	if reset_counter == RESET_LIMIT:
		send_seq = 0
		recv_seq = 0
		reset_counter = 0


def send_cycle():
	if not has_unack_data:
		return

	send_function(to_send)

def recv_data():
	if not recv_new_data
		return None

	recv_new_data = False

	recv_data_buf[:] = unpack_packet(to_recv)[4]

	return recv_data_buf

def send_data(data):
	if not can_send
		return

	if len(data) + header_size > MAX_PACK_SIZE
		return

	to_send[:] = create_packet(data, PACK_DAT, send_seq)

	has_unack_data = True
	can_send = False

def dispatch():
	send_cycle()
	recv_cycle()
