#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rdt.h"
#include "packet.h"
#include "checksum.h"

#define RESET_LIMIT 10

char to_send[MAX_PACK_SIZE];
int send_data_len;
char has_unack_data;

char to_recv[MAX_PACK_SIZE];
char recv_data_buf[MAX_PACK_SIZE];
int recv_data_len;
char recv_new_data;

uint8_t send_seq;
uint8_t recv_seq;

uint8_t reset_counter;

void initialize (int (*__recv_function)(char*, int, int), void (*__send_function)(char*, int), int __recv_timeout, int __send_timeout)
{
	struct packet_hdr reset_packet;

	can_send = 1;
	recv_new_data = 0;
	has_unack_data = 0;

	send_seq = 0;
	recv_seq = 0;

	reset_counter = 0;

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
	int len;

	len = recv_function(buf, MAX_PACK_SIZE, recv_timeout);

	if (!len)
		return;
	else if (!check_checksum(buf, len))
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

			reset_counter = 0;
		}
		else if (packet_header->seq_num == 0)
			reset_counter++;
	}
	else if (packet_header->type == PACK_DAT)
	{
		if (packet_header->seq_num == recv_seq)
		{
			recv_data_len = packet_header->len;
			memcpy (to_recv, buf, recv_data_len);
			recv_new_data = 1;

			recv_seq++;

			reset_counter = 0;
		}
		else if (packet_header->seq_num == 0)
			reset_counter++;

		ack_packet.checksum = 0;
		ack_packet.len = sizeof(struct packet_hdr);
		ack_packet.type = PACK_ACK;
		ack_packet.seq_num = recv_seq-1;
		ack_packet.checksum = ones_checksum ((char*)&ack_packet, sizeof(struct packet_hdr));
		send_function ((char*)&ack_packet, sizeof(struct packet_hdr));
	}

	if (reset_counter == RESET_LIMIT) //We're getting a lot of packets from remote host with a 0 sequence number, probably means it crashed and we need to reset our sequence numbers to catch up
	{
		send_seq = 0;
		recv_seq = 0;
		reset_counter = 0;
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
	struct packet_hdr* recv_header = (struct packet_hdr*)to_recv;

	if (!recv_new_data)
		return NULL;

	recv_new_data = 0;

	memcpy (recv_data_buf, to_recv + sizeof(struct packet_hdr), recv_header->len - sizeof(struct packet_hdr));

	return recv_data_buf;
}

void send_data (char* data, int len)
{
	struct packet_hdr* send_header = (struct packet_hdr*)to_send;

	if (!can_send)
		return;

	if (len + sizeof(struct packet_hdr) > MAX_PACK_SIZE)
		return;

	send_header->checksum = 0;
	send_header->len = len + sizeof(struct packet_hdr);
	send_header->type = PACK_DAT;
	send_header->seq_num = send_seq;

	memcpy (to_send + sizeof(struct packet_hdr), data, len);
	send_data_len = send_header->len;

	send_header->checksum = ones_checksum (to_send, send_header->len);

	has_unack_data = 1;
	can_send = 0;
}

void dispatch (void)
{
	send_cycle ();
	recv_cycle ();
}
