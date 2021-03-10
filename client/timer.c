#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "timer.h"

#define SEC_TO_USEC(s)		((s) * 1000 * 1000)
#define SEC_TO_MSEC(s)		((s) * 1000)
#define NSEC_TO_USEC(s)		((s) / 1000)
#define NSEC_TO_MSEC(s)		((s) / 1000 / 1000)

void get_time(TIMETYPE *time_ptr)
{
	if (clock_gettime(CLOCK_REALTIME, time_ptr) < 0) {
		perror("clock_gettime");
		exit(EXIT_FAILURE);
	}
}

unsigned long us_time(TIMETYPE *time_ptr)
{
	return SEC_TO_USEC(time_ptr->tv_sec) + NSEC_TO_USEC(time_ptr->tv_nsec);
}

unsigned long ms_time(TIMETYPE *time_ptr)
{
	return SEC_TO_MSEC(time_ptr->tv_sec) + NSEC_TO_MSEC(time_ptr->tv_nsec);
}

void print_duration_us(TIMETYPE *start, TIMETYPE *stop)
{
	printf("Duration in microseconds: %0lu\n",
			us_time(stop) - us_time(start));
}

void print_duration_ms(TIMETYPE *start, TIMETYPE *stop)
{
	printf("Duration in milliseconds: %0lu\n",
			ms_time(stop) - ms_time(start));
}
