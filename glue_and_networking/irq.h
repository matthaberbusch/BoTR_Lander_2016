#ifndef IRQ_H
#define IRQ_H

void disable_timer (void);
void enable_timer (void);
void reset_timer (void);

void attach_handler (void (*hander)(void), int msec);

#endif
