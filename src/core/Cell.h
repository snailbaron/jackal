#ifndef CELL_H_
#define CELL_H_
#include <vector>
#include "Pirate.h"

enum TypeCellEnum {
	// до 255 -- стрелки движения
	Arrow				= 255,
	Horse				= 256,
	Ice					= 257,
	Cannibal			= 258,
	Fort				= 259,
	NativeWoman			= 260,
	Pitfall				= 261,
	Rum					= 262,
	Crocodile			= 263,
	DifficultTerrain	= 264,
	Treasure			= 265,
	CannonUp			= 266,
	CannonLeft			= 267,
	CannonRight			= 268,
	CannonDown			= 269,
	Baloon				= 270,
	Airplane			= 271,
	Grass				= 272,
	Closed				= 300,
	Water				= 400,
	Root				= 500
};

struct TypeCell{
	int type;
	TypeCell (TypeCellEnum arg);
	TypeCell (int arg);
	operator int ();
	operator TypeCellEnum ();
};

struct Cell {
	TypeCell type;
	int money;
	std::vector<int> pirate;
	Cell* next;
	Cell();
};





#endif
