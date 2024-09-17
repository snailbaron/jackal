#ifndef CHANGE_MAP_H_
#define CHANGE_MAP_H_
#include "Cell.h"
#include "TimeState.h"

enum MoneyEventType{
	up,						// взять монету
	down,					// накласть монету на поле
	not_money_event
};

enum PirateEventType{
	revive,
	die,
	drink,
	can_movie,
	fall_into_a_trap,
	with_ship,
	open_cell,
	not_pirate_event
};

struct TurnDescript{
	int pirate;
	Point to,from;
	TypeCell cell_to;
	MoneyEventType money_event;
	PirateEventType pirate_event;
	TurnDescript();
};

struct ChangeMap{
	std::vector<TurnDescript> turns;
	std::vector<TimeState> time;
};

int Find(const ChangeMap& arr,const TimeState& time);
ChangeMap GetFrom(const ChangeMap& source, int index);

#endif