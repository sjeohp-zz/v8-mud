#ifndef Mud_Timestamp_h
#define Mud_Timestamp_h

#include <sys/time.h>

typedef unsigned long long timestamp;

static timestamp get_timestamp()
{
	struct timeval now;
	gettimeofday (&now, NULL);
	return  now.tv_usec + (timestamp)now.tv_sec * 1000000;
}

double seconds_elapsed(timestamp start, timestamp end)
{
	return (end - start) / 1000000.0L;
}

#endif