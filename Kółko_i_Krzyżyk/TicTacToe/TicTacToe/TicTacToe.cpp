 #include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <ctime>
#include <fstream>
#include "Game.h"
using namespace std;

int main(int argc, char* argv[])
{
	
	
	if (argc < 2) {
		cerr << "Skladnia:" << argv[0] << " dlugosc planszy (Tylko pierwszy/drugi znak brany pod uwage)" << endl;
	}
	int span;
	span = convertParam(argv[1]);
	if (span % 2 == 0) {
		cout << "Nieprawidlowy parametr, prosze podac nieparzysta liczbe naturalna";
		return 0;
	}
	cout << "Podany parametr: " << span << endl;
	ofstream fo;
	ifstream fi;
	srand(time(NULL));
	int stop;
	Gamestate Game;
	Game.Player_Wins = 0;
	Game.CPU_Wins = 0;
	Game.span = span;
	Game.list_header = nullptr;
	do {
		singleGame(Game);
		cout << "Czy chcesz zagrac jeszcze raz?[T/N]";
		do {
			cin >> Game.Rematch;
		} while (Game.Rematch != 'T' && Game.Rematch != 'N');
	} while (Game.Rematch == 'T');
	sessionSummary(Game);
	cout << "Prosze wprowadzic jakakolwiek liczbe aby zakonczyc program";
	cin >> stop;
	return 0;

}