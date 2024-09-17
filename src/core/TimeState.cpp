#include "TimeState.h"

bool operator<(const TimeState a, const TimeState b)
{
	return (a.day<b.day)||((a.day == b.day)&&(a.step<b.step));
}

bool operator>(const TimeState a, const TimeState b)
{
	return (a.day>b.day)||((a.day == b.day)&&(a.step>b.step));
}

bool operator==(const TimeState a, const TimeState b)
{
	return (a.day == b.day)&&(a.step == b.step);
}

bool operator<=(const TimeState a, const TimeState b)
{
	return !(a>b);
}

bool operator>=(const TimeState a, const TimeState b)
{
	return !(a<b);
}