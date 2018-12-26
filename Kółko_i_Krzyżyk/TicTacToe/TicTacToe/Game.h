#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <fstream>
using namespace std;
enum znaki {
	lewy_gorny = 201,
	prawy_gorny = 187,
	prawy_dolny = 188,
	lewy_dolny = 200,
	pozioma = 205,
	pionowa = 186,
	t_dol = 203,
	t_gora = 202,
	t_lewo = 185,
	t_prawo = 204,
	skrzyz = 206,
	cross = 'x',
	circle = 'o',
	none = 0
};
struct boardTree {
	boardTree *left;
	int *AI; //Player jako -1, komputer jako 1
	boardTree *right;
	int score = 0;
};
struct Gamestate {
	char **T;
	int *AI; 
	int *moves;
	bool initialized = 0;
	int span;
	char CPU;
	char Player;
	int Winner;
	int Turn_Counter;
	int Who_First;
	int CPU_Wins;
	int Player_Wins;
	char Rematch;
	boardTree* Tree_root_3;
	boardTree* Tree_root_5;
};


int compareBoards(Gamestate Game, int *board);

boardTree* find(boardTree * root, Gamestate Game);

boardTree* addToTree(boardTree* root, Gamestate Game);

void maziaj_tablice(int *T);

void preOrder(boardTree* root);

void Maziaj_AI(Gamestate Game);

void Maziaj_moves(Gamestate Game);

void write_file_3(boardTree* root, ofstream &f);

boardTree* read_file_3(boardTree* root, Gamestate Game, ifstream &f);

void write_file_5(boardTree* root, ofstream &f);

boardTree* read_file_5(boardTree* root, Gamestate Game, ifstream &f);

int scoreCheck(Gamestate Game);

void scoreUpdate(Gamestate &Game);

Gamestate Initialize_Gamestate(Gamestate &Game);

Gamestate Player_Turn(Gamestate &Game);

Gamestate CPU_Turn(Gamestate &Game);

void Check_For_End(Gamestate &Game);

bool Check_for_Win(Gamestate &Game);

void Draw_Board(Gamestate &Game);

Gamestate Single_Game(Gamestate &Game);

void Session_Summary(Gamestate &Game);

#endif
