#include "ChangeMap.h"

//возвращается индекс первого из элементов, который больше time
int Find(const ChangeMap& c, const TimeState& time)
{
	int begin = 0;
	int end = (int)c.time.size() - 1;
	while (end> (begin+1))
	{
		int mid = (begin+end)/2;
		if (c.time[mid]<=time)
			begin = mid;
		else
			end = mid;
	}
	int ans;
	if (c.time[end] > time)
		ans = end;
	else
		ans = end+1;
	return ans;
}


//получить всё из source, начиная с index
ChangeMap GetFrom(const ChangeMap& source, int index)
{
	ChangeMap ans;
	ans.turns.reserve(source.turns.size() - index);
	ans.time.reserve(source.time.size() - index);
	for (unsigned int i = index; i<source.turns.size(); i++)
	{
		ans.turns.push_back(source.turns[i]);
		ans.time.push_back(source.time[i]);
	}
	return ans;
}

TurnDescript::TurnDescript():to(),from(),cell_to(Root),money_event(not_money_event), pirate_event(not_pirate_event)
{
}
