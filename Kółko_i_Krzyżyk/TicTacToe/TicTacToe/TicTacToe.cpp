#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <ctime>
#include <fstream>
#include "Game.h"
using namespace std;

int main()
{
	srand(time(NULL));
	int stop;
	Gamestate Game;
	Game.Player_Wins = 0;
	Game.CPU_Wins = 0;
	Game.Tree_root_3 = nullptr;
	Game.Tree_root_5 = nullptr;
	do {
		Single_Game(Game);
		cout << "Czy chcesz zagrac jeszcze raz?[Y/N]";
		do {
			cin >> Game.Rematch;
		} while (Game.Rematch != 'Y' && Game.Rematch != 'N');
	} while (Game.Rematch == 'Y');
	Session_Summary(Game);
	cin >> stop;
}