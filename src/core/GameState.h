#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include "Pirate.h"
#include "Cell.h"
#include "Point.h"
#include "TimeState.h"

using std::vector;

const int MAX_SIDE				= 20;
const int MAX_PIRATS_OF_PLAYER	= 3;
const int MAX_PLAYERS			= 4;
const int SIZE_ISLAND			= 11;

struct GameState {
	vector<Pirate> pirates;
	Cell map[MAX_SIDE][MAX_SIDE];
	vector<int> movie_pirate;
	Point ships[MAX_PLAYERS];
	TimeState cur_time;
};

void ResetGameState(GameState& state);
Cell* GetCell(GameState& state, const Point& point);

#endif