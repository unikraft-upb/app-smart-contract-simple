#ifndef TIMER_H_
#define TIMER_H_	1

typedef struct timespec TIMETYPE;

void get_time(TIMETYPE *time_ptr);
unsigned long us_time(TIMETYPE *time_ptr);
unsigned long ms_time(TIMETYPE *time_ptr);
void print_duration_us(TIMETYPE *start, TIMETYPE *stop);
void print_duration_ms(TIMETYPE *start, TIMETYPE *stop);

#endif
