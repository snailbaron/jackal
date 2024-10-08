#include "JackalGame.h"
#include <iostream>
#include <string>
using namespace std;

void tostring(int i, string& str)
{
	str.clear();
	while(i>0)
	{
		string tmp("0");
		tmp[0]+=(i%10);
		str =tmp + str;
		i/=10;
	}
}


void main() {
	JackalGame myGame;
	myGame.NewGame();
	for(;;)
	{
		ResponseType res = WaitTurn;
		while(res != Ok){
		GameState state = myGame.GetGameState();
		cout<<" ";
		for (int i = 2; i<=14; i++)
		{
			cout<<"    ";
			if (i<10) cout<<0;
			cout<<i;
		}
		cout<<endl;
		for (int i = 2; i<=14; i++)
		{
			cout<<std::endl;
			if (i<10) cout<<0;
			cout<<i<<" ";
			for (int j =2; j<=14; j++)
			{
				string str("");
				if ((int)state.map[i][j].type<=255)
				{
					tostring(state.map[i][j].type,str);
					while (str.size()<4)
						str=str+" ";
				}
				else
				switch((TypeCellEnum)state.map[i][j].type)
				{
				case (Horse): str = "Hors"; break;
				case (Ice):  str = "Ice "; break;
				case(Cannibal): str = "Cann"; break;
				case(Fort): str = "Fort"; break;
					case (NativeWoman ): str = "Puru"; break;
					case  (Pitfall):    str = "Pitf"; break;
					case (Rum):   str = 	"Rum "; break;
					case (Crocodile	):  str = "Croc"; break;
					case (DifficultTerrain): str = "Diff"; break;
					case (Treasure): str = "Trea"; break;
					case (CannonUp): str = "CaUP"; break;
 					case (CannonLeft): str = "CaLF"; break;
					case (CannonRight): str = "CaRG"; break;
					case (CannonDown): str = "CaDN"; break;
					case (Baloon): str = "Ball"; break;
					case (Airplane): str = "Plan"; break;
					case (Grass): str = "Grss"; break;
					case (Closed): str = "CLOS"; break;
					case (Water): str = "WATE"; break;
					case (Root): str = "RRRR"; break;
				}
				str = str+" ";
				if (state.map[i][j].money>0)
					str[4] = '0' + (char)state.map[i][j].money;
				if (state.map[i][j].pirate.size()>0)
				{
					for (int k =0; k<state.map[i][j].pirate.size(); k++)
						if (state.pirates[state.map[i][j].pirate[k]].id_player == 0)
							str[3-k] = '+';
						else
							if (state.pirates[state.map[i][j].pirate[k]].id_player == 1)
								str[3-k] = '*';
							else
								if (state.pirates[state.map[i][j].pirate[k]].id_player == 2)
									str[3-k] = '-';
								else
									str[3-k] = '|';
				}
				if ((state.ships[0].y == j)&&(state.ships[0].x == i))
					str[0] = 'S';
				if ((state.ships[1].y == j)&&(state.ships[1].x == i))
					str[0] = 's';
				if ((state.ships[2].y == j)&&(state.ships[2].x == i))
					str[0] = 's';
				if ((state.ships[3].y == j)&&(state.ships[3].x == i))
					str[0] = 's';
				cout<<str<<" ";
			}
		}
		cout<<endl;
		cout<<"my money:  "<<state.map[state.ships[0].x][state.ships[0].y].money<<endl;
		vector<Point> tur;

		for (int i =0; i<state.movie_pirate.size(); i++)
		{
			tur = myGame.GetLegalSteps(state.movie_pirate[i]);
			cout<<state.movie_pirate[i]<<":\t";
			for (int j =0; j<tur.size(); j++)
			{
				cout<<j<<"::  "<<tur[j].x<<" "<<tur[j].y<<" "<<tur[j].z<<"  ";
			}
			cout<<endl;
		}
		int pir, turn;
		cin>>pir>>turn;
		if (turn<0)
			res = myGame.UseMoney(pir);
		else
		{
		tur = myGame.GetLegalSteps(pir);
		res = myGame.Turn(pir,tur[turn]);
		}
		cout<<endl;cout<<endl;cout<<endl;cout<<endl;cout<<endl;cout<<endl;cout<<endl;cout<<endl;
		}

		for (int i = 0; i<3; i++)
		{
			ResponseType res = WaitTurn;
			while (res !=Ok)
			{
				GameState state = myGame.GetGameState();
				if (state.movie_pirate.size() == 0)
					int y = 0;
				int mov = rand()%state.movie_pirate.size();
				Cell* cell = GetCell(state,state.pirates[state.movie_pirate[mov]].place);
				if (cell->type!=Water)
					if ((cell->money>0)&&(!state.pirates[state.movie_pirate[mov]].money))
					{
						myGame.UseMoney(state.movie_pirate[mov]);
					}
				vector<Point> tur = myGame.GetLegalSteps(state.movie_pirate[mov]);
				if (tur.size()<=0) continue;
				res = myGame.Turn(state.movie_pirate[mov],tur[rand()%tur.size()]);
			}
		}
	}
}
/*
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
*/