#ifndef RDT_H
#define RDT_H

char can_send;
int recv_timeout;
int send_timeout;
void (*recv_function)(char*, int, int);
void (*send_function)(char*, int);

void initialize (void (*__recv_function)(char*, int, int), void (*__send_function)(char*, int), int __recv_timeout, int __send_timeout);
char* recv_data (void);
void send_data (char* data, int len);

#endif
