#include "GameState.h"

TimeState getNull()
{
	TimeState ans;
	ans.day = 0;
	ans.step = 0;
	return ans;
}

void ResetGameState(GameState& state)
{
	//заполняем поле
	for (int i = 0; i<MAX_SIDE; i++)
		for (int j = 0; j<MAX_SIDE; j++)
		{
			state.map[i][j].money = 0;
			state.map[i][j].next = NULL;
			state.map[i][j].pirate.clear();
			state.map[i][j].type = Root;
		}

	//проливаем воду
	for (int i = 1; i<(MAX_SIDE-1); i++)
		for (int j = 1; j<(MAX_SIDE-1); j++)
		{
			state.map[i][j].type = Water;
		}

	//закидываем остров землей
	for (int i = 3; i<(3+SIZE_ISLAND); i++)
		for (int j = 3; j<(3+SIZE_ISLAND); j++)
		{
			state.map[i][j].type = Closed;
		}

	// уголки с водой
	state.map[3][3].type = Water;
	state.map[3 + SIZE_ISLAND - 1][3].type = Water;
	state.map[3 + SIZE_ISLAND - 1][3 + SIZE_ISLAND - 1].type = Water;
	state.map[3][3 + SIZE_ISLAND - 1].type = Water;
	state.cur_time = getNull();
	
	//выращиваем корабли
	state.ships[0].x = 2;
	state.ships[0].y = 3+(SIZE_ISLAND/2);
	state.ships[0].z = 1;
	state.ships[1].x = 3+(SIZE_ISLAND/2);
	state.ships[1].y = 3+SIZE_ISLAND;
	state.ships[1].z = 1;
	state.ships[2].x = 3+SIZE_ISLAND;
	state.ships[2].y = 3+(SIZE_ISLAND/2);
	state.ships[2].z = 1;
	state.ships[3].x = 3+(SIZE_ISLAND/2);
	state.ships[3].y = 2;
	state.ships[3].z = 1;

	//в муках создаем пиратов
	Pirate start_pirate;
	start_pirate.live  = true;
	start_pirate.money = false;
	start_pirate.movie = run;

	for (int i = 0; i<MAX_PIRATS_OF_PLAYER*MAX_PLAYERS; i++)
	{
		start_pirate.id_player = i/MAX_PIRATS_OF_PLAYER;
		start_pirate.place = state.ships[i/MAX_PIRATS_OF_PLAYER];
		Cell* ship_cell = GetCell(state,start_pirate.place);
		ship_cell->pirate.push_back(i);
		state.pirates.push_back(start_pirate);
	}

	//делаем активными множество пиратов первого игрока
	for (int i = 0; i<MAX_PIRATS_OF_PLAYER; i++)
	{
		state.movie_pirate.push_back(i);
	}
}

//вернуть указатель на ячейку по координате
Cell* GetCell(GameState& state, const Point& point)
{
	int z = point.z;
	Cell* ans = &(state.map[point.x][point.y]);
	while (z>1)
	{
		ans = ans->next;
		z--;
	}
	return ans;
}