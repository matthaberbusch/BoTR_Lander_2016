#include <stdint.h>

#ifndef CHECKSUM_H
#define CHECKSUM_H

uint16_t ones_checksum (char* packet_bytes, int len);
char check_checksum (char* packet_bytes, int len);

#endif
