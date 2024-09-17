#ifndef PIRATE_H_
#define PIRATE_H_
#include "Point.h"

enum MovieType{
	run,
	drink_rum,
	none
};

struct Pirate {
	bool live;
	bool money;
	MovieType movie;
	Point place;
	int id_player;
};



#endif
