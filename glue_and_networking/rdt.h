#ifndef RDT_H
#define RDT_H

char can_send;
int recv_timeout;
int send_timeout;
int (*recv_function)(char*, int, int);
void (*send_function)(char*, int);

void dispatch (void);
void initialize (int (*__recv_function)(char*, int, int), void (*__send_function)(char*, int), int __recv_timeout, int __send_timeout);
char* recv_data (void);
void send_data (char* data, int len);

#endif
