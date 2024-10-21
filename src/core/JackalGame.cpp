#include "JackalGame.h"
#include "time.h"
#include <fstream>

#ifndef DEBUG_MODE
#define DEBUG_MODE
#endif


#ifdef DEBUG_MODE
std::ofstream full_debug("Full_Debug.txt");
std::ofstream local_debug("Local_Debug.txt");
#endif

vector<std::pair<vector<TypeCell>,int> > StartClosedCells;
void initStartClosedCells()
{
	/*
	2,8,16,64			...10
	1,4,32,128			...10
	165					...5
	90					...5
	...
	Horse				...5
	Ice					...10
	Cannibal			1
	Fort				2
	NativeWoman			1
	Pitfall				...5
	Rum					...5
	Crocodile			...15
	DifficultTerrain	10
	Treasure			15
	CannonUp			0
	CannonLeft			0
	CannonRight			0
	CannonDown			2
	Baloon				2
	Airplane			1
	Grass				13
	*/
	std::pair<vector<TypeCell>,int> tmp_pair;

	//Стрелки
	tmp_pair.first.push_back(2);
	tmp_pair.first.push_back(8);
	tmp_pair.first.push_back(16);
	tmp_pair.first.push_back(64);
	tmp_pair.second = 10;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();

	tmp_pair.first.push_back(1);
	tmp_pair.first.push_back(4);
	tmp_pair.first.push_back(32);
	tmp_pair.first.push_back(128);
	tmp_pair.second = 10;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();

	tmp_pair.first.push_back(165);
	tmp_pair.second = 5;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();

	tmp_pair.first.push_back(90);
	tmp_pair.second = 5;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();

	// Horse
	tmp_pair.first.push_back(Horse);
	tmp_pair.second = 5;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// Ice
	tmp_pair.first.push_back(Ice);
	tmp_pair.second = 10;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// Cannibal
	tmp_pair.first.push_back(Cannibal);
	tmp_pair.second = 1;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// Fort
	tmp_pair.first.push_back(Fort);
	tmp_pair.second = 2;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// NativeWoman
	tmp_pair.first.push_back(NativeWoman);
	tmp_pair.second = 1;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// Pitfall
	tmp_pair.first.push_back(Pitfall);
	tmp_pair.second = 5;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// Rum
	tmp_pair.first.push_back(Rum);
	tmp_pair.second = 5;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// Crocodile
	tmp_pair.first.push_back(Crocodile);
	tmp_pair.second = 15;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// Длинные убогие пути
	tmp_pair.first.push_back(DifficultTerrain);
	tmp_pair.second = 10;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// Сокровище
	tmp_pair.first.push_back(Treasure);
	tmp_pair.second = 15;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// Пушки
	tmp_pair.first.push_back(CannonUp);
	tmp_pair.first.push_back(CannonLeft);
	tmp_pair.first.push_back(CannonRight);
	tmp_pair.first.push_back(CannonDown);
	tmp_pair.second = 2;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// Baloon
	tmp_pair.first.push_back(Baloon);
	tmp_pair.second = 2;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// Airplane
	tmp_pair.first.push_back(Airplane);
	tmp_pair.second = 1;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
	// Grass
	tmp_pair.first.push_back(Grass);
	tmp_pair.second = 13;
	StartClosedCells.push_back(tmp_pair);
	tmp_pair.first.clear();
}

JackalGame::PirateCanComeThis JackalGame::CheckLegalityCell(const Pirate& pirate, const Point& to)
{
	Cell* cell_from = GetCell(state,pirate.place);
	Cell* cell_to = GetCell(state,to);
	if (cell_from->type == Water) //Пират в воде
	{
		if (pirate.place == state.ships[pirate.id_player])
		{

		}
		else
			if(cell_to->type == Water)
				return YES;
			else
				return NO;
	}

	if (cell_to->type == Water)
	{
		if (state.ships[pirate.id_player] == to)
			return YES;
		else
			return ONLY_FROM_ARROW;
	}

	if (cell_to->type == Closed)
	{
		if (pirate.money)
			return NO;
		else
			return YES;
	}
	if ((cell_to->type == Fort)||(cell_to->type == NativeWoman))
	{
		if ((cell_to->pirate.size()>0)&&(state.pirates[cell_to->pirate[0]].id_player!=pirate.id_player))
			return NO;
		else
			if (pirate.money)
				return NO;
			else
				return YES;
	}
	if (cell_to->type == Crocodile)
		return ONLY_FROM_ARROW;
	return YES;

}




void JackalGame::NewGame(int count_players)
{
	this->count_players = count_players;
	srand(time(NULL));

	//Заполнение стартового массива закрытых, в паре первый элемент - набор типов, в который равновероятно могут превратиться второй параметр ячеек
	initStartClosedCells();

	//Заполнение массива закрытых
	for (int  i =0; i<StartClosedCells.size(); i++)
	{
		for (int j =0; j<StartClosedCells[i].second; j++)
			closed_cell.push_back(StartClosedCells[i].first[rand()%StartClosedCells[i].first.size()]);
	}
	for (int i = 1; i<=5 ; i++)
	{
		for (int j = 0; j<i; j++)
		{
			closed_treasure.push_back(6-i);
		}
	}
	for (int i = 1; i<5 ; i++)
	{
		for (int j = 0; j<i; j++)
		{
			closed_difficuilt_terrain.push_back(6-i);
		}
	}
	ResetGameState(this->state);
}

GameState JackalGame::GetGameState()
{
	return this->state;
}

ChangeMap JackalGame::GetChangesMap(TimeState from)
{
	int index = Find(history,from);
	return GetFrom(history,index);
}

vector<Point> JackalGame::GetLegalSteps(int pirate)
{
	Pirate corsair = state.pirates[pirate];
	vector<Point> ans;
	ans.clear();
	Point dot;

	if ((!corsair.live)||(corsair.movie!=run))
		return ans;

	Cell* cell = GetCell(state,corsair.place);
	bool StandartStep = true;

	//Пират в море на своем корабле
	if ((cell->type == Water)&&(state.ships[corsair.id_player] == corsair.place))
	{
		StandartStep = false;
		if	(2 == corsair.place.x) {
			dot.x = corsair.place.x+1; dot.y = corsair.place.y;   dot.z = 1; if (YES == CheckLegalityCell(corsair,dot)) ans.push_back(dot);
			if (corsair.place.y<(3 + SIZE_ISLAND - 2)) {dot.x = corsair.place.x;   dot.y = corsair.place.y+1; dot.z = 1; ans.push_back(dot);}
			if (corsair.place.y>4) {dot.x = corsair.place.x;   dot.y = corsair.place.y-1; dot.z = 1; ans.push_back(dot);}
		}
		else if ((SIZE_ISLAND+3) == corsair.place.x) {
			dot.x = corsair.place.x-1; dot.y = corsair.place.y;   dot.z = 1; if (YES == CheckLegalityCell(corsair,dot)) ans.push_back(dot);
			if (corsair.place.y<(3 + SIZE_ISLAND - 2)) { dot.x = corsair.place.x;   dot.y = corsair.place.y+1; dot.z = 1; ans.push_back(dot);}
			if (corsair.place.y>4) {dot.x = corsair.place.x;   dot.y = corsair.place.y-1; dot.z = 1; ans.push_back(dot);}
		}
		else if (2 == corsair.place.y) {
			dot.x = corsair.place.x;   dot.y = corsair.place.y+1; dot.z = 1; if (YES == CheckLegalityCell(corsair,dot)) ans.push_back(dot);
			if (corsair.place.x<(3 + SIZE_ISLAND - 2)) {dot.x = corsair.place.x+1; dot.y = corsair.place.y;   dot.z = 1; ans.push_back(dot);}
			if (corsair.place.x>4) {dot.x = corsair.place.x-1; dot.y = corsair.place.y;   dot.z = 1; ans.push_back(dot);}
		}
		else if ((SIZE_ISLAND+3) == corsair.place.y) {
			dot.x = corsair.place.x;   dot.y = corsair.place.y-1; dot.z = 1; if (YES == CheckLegalityCell(corsair,dot)) ans.push_back(dot);
			if (corsair.place.x<(3 + SIZE_ISLAND - 2)) {dot.x = corsair.place.x+1; dot.y = corsair.place.y;   dot.z = 1; ans.push_back(dot);}
			if (corsair.place.x>4) {dot.x = corsair.place.x-1; dot.y = corsair.place.y;   dot.z = 1; ans.push_back(dot);}
		}
	}
	//Пират на суше
	else
	{
		//Если пират на стрелке
		if ((int)cell->type <= 255)
		{
			StandartStep = false;
			vector<Point> second_ans;
			PirateCanComeThis p;
			dot.z = 1;
			for (int i = -1; i<=1; i++)
				for (int j = -1; j<=1; j++)
				{
					if ((i==0)&&(j == 0)) continue;
					int shift = 3*(i+1) + j+1;
					if (shift>=5) shift--;
					int mask = 1<<shift;
					if ((((int)cell->type)&(mask))>0)
					{
						dot.x = corsair.place.x + i;
						dot.y = corsair.place.y + j;
						p = CheckLegalityCell(corsair,dot);
						if (YES == p) ans.push_back(dot);
						else
							if (ONLY_FROM_ARROW == p)
							{
								if (PIRATE_CAN_MOVE_TO_WATER_FROM_ARROW)
								{
									ans.push_back(dot);
								}
								else
								{
									second_ans.push_back(dot);
								}
							}
					}
				}
				if (ans.size()<1)
					ans = second_ans;
		}
		if (cell->type == Horse)
		{
			//обработать лошадь
			vector<Point> second_ans;
			PirateCanComeThis p;
			dot.z = 1;
			for (int i = -2; i<=2; i++)
				for (int j = -2; j<=2; j++)
				{
					if ((abs(i) + abs(j)) == 3)
					{
						dot.x = corsair.place.x + i;
						dot.y = corsair.place.y + j;
						p = CheckLegalityCell(corsair,dot);
						if (YES == p) ans.push_back(dot);
						else
							if (ONLY_FROM_ARROW == p)
							{
								if (PIRATE_CAN_MOVE_TO_WATER_FROM_ARROW)
								{
									ans.push_back(dot);
								}
								else
								{
									second_ans.push_back(dot);
								}
							}
					}
				}
				if (ans.size()<1)
					ans = second_ans;
			StandartStep = false;
		}
		if (cell->type == Ice)
		{
			//обработать лёд
			int pointer = history.turns.size()-1;
			while (history.turns[pointer].from == history.turns[pointer].to)
			{
				pointer--;
			}
			Point shift = history.turns[pointer].to - history.turns[pointer].from;
			int length = abs(shift.x) + abs(shift.y);
			if (length>3) {
				if ((abs(shift.x)) > (abs(shift.y)))
				{
					shift.x/=abs(shift.x);
					shift.x*=2;
					if (shift.y!=0)
						shift.y/=abs(shift.y);
				}
				else
				{
					shift.y/=abs(shift.y);
					shift.y*=2;
					if (shift.x!=0)
						shift.x/=abs(shift.x);
				}
			}
			if (NO != CheckLegalityCell(corsair,corsair.place+shift))
			{
				ans.push_back(corsair.place+shift);
			}
			else
				ans.push_back(corsair.place-shift);
			StandartStep = false;
		}
		if (cell->type == Airplane)
		{
			for (int i =0; i<MAX_SIDE; i++)
			{
				for (int j = 0; j<MAX_SIDE; j++)
				{
					Point tmp_point;
					tmp_point.x = i;
					tmp_point.y = j;
					tmp_point.z = 1;
					Cell* tmp_cell = GetCell(state,tmp_point);
					if ((tmp_cell->type!=Root)&&(tmp_cell->type!=Water))
						if (YES == CheckLegalityCell(corsair,tmp_point))
							ans.push_back(tmp_point);
				}
			}
			StandartStep = false;
		}
		if (cell->type == DifficultTerrain)
		{
			if (cell->next)
			{
				StandartStep = false;
				ans.push_back(corsair.place);
				ans[ans.size()-1].z++;
			}
		}
		if (cell->type == NativeWoman)
		{
			bool One_my_pirat_not_live = false;
			for (int i =0; i<state.pirates.size(); i++)
			{
				if ((state.pirates[i].id_player == corsair.id_player)&&(!state.pirates[i].live))
					ans.push_back(corsair.place);
			}
		}
	}

	if (StandartStep)
	{
		//обработать стандартные 8 клеток
		Point place = corsair.place;
		PirateCanComeThis p;
		dot = place;

		for (int i=-1; i<2; i++)
			for (int j=-1; j<2; j++)
				if ((i!=0) || (j!=0)) {
					dot.x = place.x+i;
					dot.y = place.y+j;
					dot.z = 1;
					p = CheckLegalityCell(corsair,dot);
					if (YES == p) ans.push_back(dot);
				}
	}

	return ans;
}

ResponseType JackalGame::UseMoney(int pirate)
{
	Cell *cell = GetCell(state,state.pirates[pirate].place);
	if (state.pirates[pirate].money) //пират несет монету
	{
		if (cell->type == Water)
			return IllegalTurn;
		else
			if ((int)cell->type>257)   // если не ячейки мгновенного движения
			{
				cell->money++;
				state.pirates[pirate].money = false;
				return WaitTurn;
			}
			else
				if (CAN_MONEY_DOWN_ON_ARROW)  // если пират может класть монеты на стрелки
				{
					cell->money++;
					state.pirates[pirate].money = false;
					return WaitTurn;
				}
				else
					return IllegalTurn;
	}
	else
	{
		if ((cell->type!=Water)&&(cell->money>0))  //если пират не в воде и под ним есть монеты
		{
			cell->money--;
			state.pirates[pirate].money = true;
			return WaitTurn;
		}
		else
			return IllegalTurn;
	}
}

void JackalGame::OpenCell(const Point &to)
{
	Cell* cell = GetCell(state,to);
	int index = rand()%closed_cell.size();
	std::vector<TypeCell>::iterator beg(closed_cell.begin());
	beg+=index;
	std::vector<TypeCell>::iterator fin(beg);
	fin++;
	TypeCell cur = closed_cell[index];
	closed_cell.erase(beg,fin);
	cell->type = cur;
	if (cur == Treasure)
	{
		int index_money = rand()%closed_treasure.size();
		std::vector<int>::iterator begin(closed_treasure.begin());
		begin+=index_money;
		std::vector<int>::iterator finish(begin);
		finish++;
		cell->money = closed_treasure[index_money];
		closed_treasure.erase(begin,finish);
	}
	if (cur == DifficultTerrain)
	{
		int index_dif = rand()%this->closed_difficuilt_terrain.size();
		std::vector<int>::iterator begin(closed_difficuilt_terrain.begin());
		begin+=index_dif;
		std::vector<int>::iterator finish(begin);
		finish++;
		int dif = closed_difficuilt_terrain[index_dif];
		closed_difficuilt_terrain.erase(begin,finish);
		Cell* cur = cell;
		while (dif>1)
		{
			Cell* next = new Cell();
			next->money = 0;
			next->pirate.clear();
			next->next = NULL;
			next->type = DifficultTerrain;
			cur->next = next;
			dif--;
			cur = next;
		}
	}

	//Допилить
}


//Изменить текущее время и множество пиратов, которые могут ходить
void JackalGame::changeTime(int pirate)
{
	state.movie_pirate.clear();
	int next_player = state.pirates[pirate].id_player +1;
	if (next_player == count_players)
	{
		next_player = 0;
		state.cur_time.step = 0;
		state.cur_time.day++;
	}
	else
		state.cur_time.step++;
	for (int i = 0; i<state.pirates.size(); i++)
	{
		if (state.pirates[i].id_player == next_player)
		{
			if (state.pirates[i].movie == run)
			{
				vector<Point> tmp = GetLegalSteps(i);
				if (tmp.size()>0)
					state.movie_pirate.push_back(i);
			}
			if (state.pirates[i].movie == drink_rum)
			{
				state.pirates[i].movie = run;
			}
		}
	}
	if (state.movie_pirate.size() == 0)
	{
		for (int i = 0; i<state.pirates.size(); i++)
			if (state.pirates[i].id_player == next_player)
			{
				changeTime(i);
				return;
			}
	}

}

void JackalGame::saveToHistory(TypeCell cell_to,Point from, Point to, MoneyEventType money_event, int pirate, PirateEventType pirate_event)
{
	state.cur_time.step++;
	TurnDescript to_history;
	to_history.cell_to = cell_to;
	to_history.from = from;
	to_history.to = to;
	to_history.money_event = money_event;
	to_history.pirate = pirate;
	to_history.pirate_event = pirate_event;
	history.turns.push_back(to_history);
	history.time.push_back(state.cur_time);

	#ifdef DEBUG_MODE
	local_debug<<(TypeCellEnum)cell_to<<"\t"<<from<<"\t"<<to<<"\t"<<money_event<<"\t"<<pirate<<"\t"<<pirate_event<<" "<<state.cur_time.day<<" "<<state.cur_time.step<<std::endl;
	local_debug.flush();
	#endif
}

void JackalGame::makeMovie(Point from, Point to, int pirate)
{
	Cell* cell_from = GetCell(state,from);
	Cell* cell_to = GetCell(state,to);
	vector<int>::iterator fin(cell_from->pirate.begin());
	while ((*fin)!=pirate) fin++;
	vector<int>::iterator beg(fin++);
	cell_from->pirate.erase(beg,fin);
	cell_to->pirate.push_back(pirate);
	state.pirates[pirate].place = to;
}

ResponseType JackalGame::Turn(int pirate, Point to, bool forced)
{
	#ifdef DEBUG_MODE

	#endif

	//Проверить, можно ли ходить данным пиратом
	if (!forced)
	{
	bool flag = true;
	for (int i =0; i< state.movie_pirate.size(); i++)
	{
		if (pirate == state.movie_pirate[i])
			flag = false;
	}
	if (flag) return IllegalTurn;
	//Проверка, является ли ход допустимым
	vector<Point> PirateLegalSteps = GetLegalSteps(pirate);
	flag = true;
	for (int i =0; i<PirateLegalSteps.size(); i++)
	{
		if (to == PirateLegalSteps[i]) flag = false;
	}
	if (flag) return IllegalTurn;
	}



	Pirate& corsair = state.pirates[pirate];
	Cell* cell_from = GetCell(state,corsair.place);
	if ((cell_from->type == Airplane)||((!TRIPS_REPEAT)&&((cell_from->type == Pitfall)||(cell_from->type == Rum))))
	{
		cell_from->type = Grass;
		//записать в хистори
		saveToHistory(Grass,corsair.place,corsair.place,not_money_event,pirate,not_pirate_event);
	}
	Cell* cell_to = GetCell(state,to);

	//Попал в цикл
	if ((state.cur_time.step>100)&&(history.turns[history.turns.size()-100].pirate == history.turns[history.turns.size()-1].pirate))
	{
		MoneyEventType event_money = not_money_event;
		if (corsair.money)
		{
			corsair.money = false;
			event_money = down;
		}
		saveToHistory(cell_to->type,corsair.place,to,event_money,pirate,die);
		makeMovie(corsair.place,to,pirate);
		cell_to->pirate.clear();
		corsair.live = false;
		changeTime(pirate);
		return Ok;
	}


	if ((cell_from->type == NativeWoman)&&(corsair.place == to))
	{
		for (int i =0; i<state.pirates.size(); i++)
		{
			if ((state.pirates[i].id_player == corsair.id_player)&&(!state.pirates[i].live))
			{
				state.pirates[i].place = corsair.place;
				state.pirates[i].money = false;
				state.pirates[i].movie = run;
				state.pirates[i].live = true;
				cell_from->pirate.push_back(i);
				//Записать в хистори
				saveToHistory(NativeWoman,corsair.place,corsair.place,not_money_event,i,revive);
			}
			break;
		}
		//записать в хистори
		saveToHistory(NativeWoman,corsair.place,corsair.place,not_money_event,pirate,not_pirate_event);
		changeTime(pirate);
		return Ok;
	}
	//пират на корабле
	if (state.ships[corsair.id_player] == corsair.place)
	{
		//если пират шевелит кораблем
		if ((cell_to->type == Water)&&(!forced))
		{
			cell_to->money = cell_from->money;
			if ((cell_to->pirate.size()>0)&&(state.pirates[cell_to->pirate[0]].id_player != corsair.id_player))
			{
				for (int i = 0; i<cell_to->pirate.size(); i++)
				{
					saveToHistory(Water,state.pirates[cell_to->pirate[i]].place,state.pirates[cell_to->pirate[i]].place,not_money_event,cell_to->pirate[i],die);
					state.pirates[cell_to->pirate[i]].live = false;
					saveToHistory(Water,to,to,not_money_event,pirate,die);
				}
				cell_to->pirate.clear();
			}
			Point from = corsair.place;
			cell_to->pirate.push_back(cell_from->pirate[0]);
			state.pirates[cell_from->pirate[0]].place = to;
			saveToHistory(Water,from,to,not_money_event,cell_from->pirate[0],with_ship);
			for (int i =1; i<cell_from->pirate.size(); i++)
			{
				cell_to->pirate.push_back(cell_from->pirate[i]);
				state.pirates[cell_from->pirate[i]].place = to;
				saveToHistory(Water,from,to,not_money_event,cell_from->pirate[i],not_pirate_event);
			}
			cell_from->money = 0;
			state.ships[corsair.id_player] = to;
			cell_from->pirate.clear();
			//записать в хистори
			changeTime(pirate);
			return Ok;
		}
	}
	//если пират ступает на свой корабль
	if (state.ships[corsair.id_player] == to)
	{
		saveToHistory(Water,corsair.place,to,not_money_event,pirate,not_pirate_event);
		makeMovie(corsair.place,to,pirate);
		if (corsair.money)
		{
			corsair.money = false;
			cell_to->money++;
			saveToHistory(Water,to,to,down,pirate,not_pirate_event);
		}
		changeTime(pirate);
		return Ok;
	}
	//Если пират ступает на вражеский корабЫль
	for (int i =0; i<count_players; i++)
	{
		if (state.ships[i] == to)
		{
			MoneyEventType event_money = not_money_event;
			if (corsair.money)
			{
				corsair.money = false;
				event_money = down;
			}
			saveToHistory(cell_to->type,corsair.place,to,event_money,pirate,die);
			makeMovie(corsair.place,to,pirate);
			vector<int>::iterator beg = cell_to->pirate.end();
			beg--;
			cell_to->pirate.erase(beg,cell_to->pirate.end());
			corsair.live = false;
			changeTime(pirate);
			return Ok;
		}
	}


	if (cell_to->type == Closed)
	{
		OpenCell(to);
		cell_to = GetCell(state,to);
		//записать в хистори
		saveToHistory(cell_to->type,to,to,not_money_event,-1,open_cell);
	}

	//Выгоняем всех врагов на корабль
	if ((cell_to->pirate.size()>0)&&(state.pirates[cell_to->pirate[0]].id_player != corsair.id_player))
	{
		//Обработать случай, когда некоторые пираты не могут ходить
		for (int i = 0; i<cell_to->pirate.size(); i++)
			{
				if (state.pirates[cell_to->pirate[i]].movie != run)
				{
					saveToHistory(cell_to->type,state.pirates[cell_to->pirate[i]].place,state.pirates[cell_to->pirate[i]].place,not_money_event,cell_to->pirate[i],can_movie);
					state.pirates[cell_to->pirate[i]].movie = run;
				}
			}

		//отправить домой
		if (cell_to->type!=Water)
		{
			Point ship_place = state.ships[state.pirates[cell_to->pirate[0]].id_player];
			Cell* ship_cell = GetCell(state,ship_place);
			for (int i = 0; i<cell_to->pirate.size(); i++)
			{
				if (state.pirates[cell_to->pirate[i]].money)
				{
					cell_to->money++;
					state.pirates[cell_to->pirate[i]].money = false;
					saveToHistory(Water,state.pirates[cell_to->pirate[i]].place,ship_place,down,cell_to->pirate[i],not_pirate_event);
				}
				else
					saveToHistory(Water,state.pirates[cell_to->pirate[i]].place,ship_place,not_money_event,cell_to->pirate[i],not_pirate_event);
				state.pirates[cell_to->pirate[i]].place = ship_place;
				ship_cell->pirate.push_back(cell_to->pirate[i]);
			}
			cell_to->pirate.clear();
		}
		else
		{
			for (int i = 0; i<cell_to->pirate.size(); i++)
			{
				if (state.pirates[cell_to->pirate[i]].money)
				{
					state.pirates[cell_to->pirate[i]].money = false;
					saveToHistory(Water,state.pirates[cell_to->pirate[i]].place,state.pirates[cell_to->pirate[i]].place,down,cell_to->pirate[i],die);
				}
				else
					saveToHistory(Water,state.pirates[cell_to->pirate[i]].place,state.pirates[cell_to->pirate[i]].place,not_money_event,cell_to->pirate[i],die);
				state.pirates[cell_to->pirate[i]].live = false;
			}
			cell_to->pirate.clear();
		}
	}


	//обрабатываем мгновенные клетки
	//дирижабль
	if (cell_to->type == Baloon)
	{
		return Turn(pirate,state.ships[corsair.id_player],true);
	}
	//Крокодил
	if (cell_to->type == Crocodile )
	{
		saveToHistory(cell_to->type,corsair.place,to,not_money_event,pirate,not_pirate_event);
		Point tmp_from = corsair.place;
		makeMovie(corsair.place,to,pirate);
		return Turn (pirate,tmp_from,true);
	}
	//Волчья яма
	if (cell_to->type == Pitfall)
	{
		PirateEventType movie = not_pirate_event;
		if (cell_to->pirate.size() == 0)
		{
			corsair.movie = none;
			movie = fall_into_a_trap;
		}
		else
			for (int i = 0; i<cell_to->pirate.size(); i++)
			{
				if ((cell_to->pirate[i]!=pirate)&&(state.pirates[cell_to->pirate[i]].movie == none))
				{
					state.pirates[cell_to->pirate[i]].movie = run;
					saveToHistory(cell_to->type,state.pirates[cell_to->pirate[i]].place,state.pirates[cell_to->pirate[i]].place,not_money_event,cell_to->pirate[i],can_movie);
				}
			}
		saveToHistory(cell_to->type,corsair.place,to,not_money_event,pirate,movie);
		makeMovie(corsair.place,to,pirate);
		changeTime(pirate);
		return Ok;
	}
	if (cell_to->type<=Ice)
	{
		saveToHistory(cell_to->type,corsair.place,to,not_money_event,pirate,not_pirate_event);
		makeMovie(corsair.place,to,pirate);
		state.movie_pirate.clear();
		state.movie_pirate.push_back(pirate);
		if (!CAN_MONEY_DOWN_ON_ARROW)
		{
			vector<Point> next_turn = GetLegalSteps(pirate);
			if (next_turn.size() == 1)
				return Turn(pirate,next_turn[0]);

			if (next_turn.size() == 0)
			{
				MoneyEventType event_money = not_money_event;
				if (corsair.money)
				{
					corsair.money = false;
					event_money = down;
				}
				saveToHistory(cell_to->type,corsair.place,to,event_money,pirate,die);
				cell_to->pirate.clear();
				corsair.live = false;
				changeTime(pirate);
				return Ok;
			}
		}
		return WaitTurn;
	}
	if (cell_to->type == Cannibal)
	{
		MoneyEventType event_money = not_money_event;
		if (corsair.money)
		{
			corsair.money = false;
			event_money = down;
		}
		saveToHistory(cell_to->type,corsair.place,to,event_money,pirate,die);
		makeMovie(corsair.place,to,pirate);
		cell_to->pirate.clear();
		corsair.live = false;
		changeTime(pirate);
		return Ok;
	}
	if (cell_to->type == Rum)
	{
		saveToHistory(cell_to->type,corsair.place,to,not_money_event,pirate,drink);
		makeMovie(corsair.place,to,pirate);
		corsair.movie = drink_rum;
		changeTime(pirate);
		return Ok;
	}
	//Пушки
	if ((cell_to->type>=CannonUp)&&(cell_to->type<=CannonDown))
	{
		Point new_place = to;
		Point shift;
		shift.x = shift.y = shift.z = 0;
		if (cell_to->type==CannonUp)
		{
			shift.x = 1;
		}
		if (cell_to->type==CannonDown)
		{
			shift.x= -1;
		}
		if (cell_to->type==CannonLeft)
		{
			shift.y = -1;
		}
		if (cell_to->type==CannonRight)
		{
			shift.y = 1;
		}
		Cell* tmp_cell = GetCell(state,new_place);
		while(tmp_cell->type!=Water)
		{
			new_place += shift;
			tmp_cell = GetCell(state,new_place);
		}
		saveToHistory(cell_to->type,corsair.place,to,not_money_event,pirate,not_pirate_event);
		return Turn(pirate,new_place,true);
	}
	if (cell_to->type == Water)
	{
		MoneyEventType event_money = not_money_event;
		if (corsair.money)
		{
			corsair.money = false;
			event_money = down;
		}
		saveToHistory(cell_to->type,corsair.place,to,event_money,pirate,not_pirate_event);
		makeMovie(corsair.place,to,pirate);
		changeTime(pirate);
		return Ok;
	}
	saveToHistory(cell_to->type,corsair.place,to,not_money_event,pirate,not_pirate_event);
	makeMovie(corsair.place,to,pirate);
	changeTime(pirate);
	return Ok;
}





