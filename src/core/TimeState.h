#ifndef TIME_STATE_H_
#define TIME_STATE_H_

struct TimeState{
	int day;
	int step;
};


bool operator<(const TimeState a, const TimeState b);
bool operator>(const TimeState a, const TimeState b);
bool operator==(const TimeState a, const TimeState b);
bool operator<=(const TimeState a, const TimeState b);
bool operator>=(const TimeState a, const TimeState b);

#endif