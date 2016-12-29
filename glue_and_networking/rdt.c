#include <stdlib.h>

#include "irq.h"
#include "rdt.h"
#include "packet.h"
#include "checksum.h"

char to_send[1024];
int send_data_len;
char has_unack_data;

char recv_data[1024];
char recv_data_buf[1024];
int recv_data_len;
char recv_new_data;

uint8_t send_seq;
uint8_t recv_seq;

void initialize (void (*__recv_function)(char*, int, int), void (*__send_function)(char*, int), int __recv_timeout, int __send_timeout)
{
	has_unack_data = 0;
	recv_new_data = 0;
	can_send = 1;

	send_seq = 0;
	recv_seq = 0;

	recv_function = __recv_function;
	send_function = __send_function;
	recv_timeout = __recv_timeout;
	send_timeout = __send_timeout;
}

void recv_cycle (void)
{
	char buf[MAX_PACK_SIZE];
	struct packet_hdr* packet_header = (struct packet_hdr*)buf;
	struct packet_hdr ack_packet;

	recv_function(buf, MAX_PACK_SIZE);

	if (!check_checksum(buf, packet_header->len))
		return;
	else if (packet_header->type == PACK_UNK)
		return;
	else if (packet_header->type == PACK_ACK)
	{
		if (packet_header->seq_num == send_seq)
		{
			can_send = 1;
			has_unack_data = 0;

			send_seq++;
		}
	}
	else if (packet_header->type == PACK_DAT)
	{
		if (packet_header->seq_num == recv_seq)
		{
			recv_data_len = packet_header->len;
			memcpy (recv_data, buf, recv_data_len);
			recv_new_data = 1;

			recv_seq++;
		}

		ack_packet.checksum = 0;
		ack_packet.len = sizeof(struct packet_hdr);
		ack_packet.type = PACK_ACK;
		ack_packet.seq_num = recv_seq-1;
		ack_packet.checksum = ones_checksum (&ack_packet, sizeof(struct packet_hdr));
		send_function (&ack_packet, sizeof(struct packet_hdr));
	}
}

void send_cycle (void)
{
	if (!has_unack_data)
		return;

	send_function (to_send, send_data_len);
}

char* recv_data (void)
{
	struct packet_hdr* recv_header = (struct packet_hdr*)recv_data;

	disable_timer ();

	if (!recv_new_data)
		return NULL;

	recv_new_data = 0;

	memcpy (recv_data_buf, recv_data + sizeof(struct packet_hdr), recv_data->len - sizeof(struct packet_hdr));

	enable_timer ();

	return recv_data_buf;
}

void send_data (char* data, int len)
{
	struct packet_hdr* send_header = (struct packet_hdr*)to_send;

	disable_timer ();

	if (!can_send)
		return;

	if (len + sizeof(packet_hdr) > 1024)
		return;

	send_header->checksum = 0;
	send_header->len = len + sizeof(packet_hdr);
	send_header->type = PACK_DAT;
	send_header->seq_num = send_seq;

	memcpy (to_send + sizeof(packet_hdr), data, len);

	send_header->checksum = ones_checksum (to_send, send_header->len);

	has_unack_data = 1;
	can_send = 0;

	enable_timer ();
}

void dispatch (void)
{
	disable_timer ();

	send_cycle ();
	recv_cycle ();

	reset_timer ();
	enable_timer ();
}
