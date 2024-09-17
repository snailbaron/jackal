#ifndef JACKAL_CORE_H_
#define JACKAL_CORE_H_

#include "GameState.h"
#include "ChangeMap.h"
#include "Point.h"
#include "ResponseType.h"

//настройки
const bool PIRATE_CAN_SWIM_ROUND_SHIP				= true;
const bool TRIPS_REPEAT							= true;
//bool AUTOMATICALLY_MONEY_DOWN				= false;
const bool SHOW_MONEY_ANOTHER_PLAYERS				= true;
const bool CAN_MONEY_DOWN_ON_ARROW				= false;
const bool PIRATE_CAN_MOVE_TO_WATER_FROM_ARROW	= false;

class JackalGame{
private:
	GameState state;
	ChangeMap history;
	int count_players;
	vector<TypeCell> closed_cell;
	vector<int> closed_treasure;
	vector<int> closed_difficuilt_terrain;
	enum PirateCanComeThis {
		YES,
		ONLY_FROM_ARROW,
		NO
		};
PirateCanComeThis CheckLegalityCell(const Pirate& pirate, const Point& to);
void OpenCell(const Point& to);
void saveToHistory(TypeCell cell_to,Point from, Point to, MoneyEventType money_event, int pirate, PirateEventType pirate_event)	;
void makeMovie(Point from, Point to, int pirate);
void changeTime(int pirate);
public:
	void NewGame(int count_players = 4);
	GameState GetGameState();
	ChangeMap GetChangesMap(TimeState from);
	vector<Point> GetLegalSteps(int pirate);
//	vector<Path> GetLegalPaths(int pirate);
//	vector<Path> GetPathsTo(int pirate, Point to);
	ResponseType Turn(int pirate, Point to,bool forced = false);
	ResponseType UseMoney(int pirate);
};


#endif
