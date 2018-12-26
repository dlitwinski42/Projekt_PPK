#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <ctime>
#include <fstream>
#include "Game.h"
using namespace std;

int compareBoards(Gamestate Game, int *board) {
	for (int i = 0; i < (Game.span * Game.span); i++) {
		if (Game.AI[i] < board[i]) {
			return -1;
		}
		else if (Game.AI[i] > board[i]) {
			return 1;
		}
	}
	return 0;
}

boardTree* find(boardTree * root, Gamestate Game) {
	boardTree* work_root = root;
	if (root == nullptr) {
		return 0;
	}
	if (root != nullptr) {
		int compared = compareBoards(Game, work_root->AI);
		if (compared < 0) {
			//	cout << "Szukaj w lewo" << endl;
			return find(work_root->left, Game);
		}
		else if (compared > 0) {
			//	cout << "Szukaj w prawo" << endl;
			return find(work_root->right, Game);
		}
		else {
			//	cout << "Znaleziono" << endl;
			return root;
		}
	}
}

boardTree* addToTree(boardTree* root, Gamestate Game) {
	boardTree* work_root;
	if (root == nullptr) {
		work_root = new boardTree;
		work_root->AI = new int[(Game.span * Game.span)];
		work_root->left = nullptr;
		work_root->right = nullptr;
		work_root->score = 0;
		for (int i = 0; i < (Game.span * Game.span); i++) {
			work_root->AI[i] = Game.AI[i];
		}
		root = work_root;
		return root;
	}
	else {
		if (compareBoards(Game, root->AI) < 0) {
			if (root->left == nullptr) {
				//	cout << "Dodaj w lewo" << endl;
				root->left = addToTree(root->left, Game);
			}
			else {
				//	cout << "Przejdz w lewo" << endl;
				addToTree(root->left, Game);
			}
		}
		else if (compareBoards(Game, root->AI) > 0) {
			if (root->right == nullptr) {
				//	cout << "Dodaj w prawo" << endl;
				root->right = addToTree(root->right, Game);
			}
			else {
				//	cout << "Przejdz w prawo" << endl;
				addToTree(root->right, Game);
			}
		}
		else {
			return root;
		}
	}
}
void maziaj_tablice(int *T) {
	for (int i = 0; i < 25; i++) {
		cout << T[i] << " ";
	}
}
void preOrder(boardTree* root) {
	if (root == nullptr) {
		cout << "Drzewo nie istnieje!" << endl;
	}
	else {
		maziaj_tablice(root->AI);
		cout << ":" << root->score << endl;
		/*if (root->left != nullptr) {
			cout << "1";
		}
		else {
			cout << "0";
		}
		if (root->right != nullptr) {
			cout << "1";
		}
		else {
			cout << "0";
		}*/
		if (root->left != nullptr)
			preOrder(root->left);
		if (root->right != nullptr)
			preOrder(root->right);
	}
}
void deleteTree(boardTree* root) {
	if (root != nullptr) {
		if (root->left != nullptr)
			deleteTree(root->left);
		if (root->right != nullptr)
			deleteTree(root->right);
		delete[] root->AI;
		delete root;
		root = nullptr;
	}
}
void Maziaj_AI(Gamestate Game) {
	for (int i = 0; i < (Game.span * Game.span); i++) {
		cout << Game.AI[i] << " ";
	}
	cout << endl;
}
void Maziaj_moves(Gamestate Game) {
	for (int i = 0; i < (Game.span * Game.span); i++) {
		cout << Game.moves[i] << " ";
	}
	cout << endl;
}
void write_file_3(boardTree* root, ofstream &f) {
	if (f.is_open()) {
		if (root == nullptr) {
			cout << "Drzewo nie istnieje!" << endl;
		}
		for (int i = 0; i < 9; i++) {
			int tab = root->AI[i];
			f << tab << " ";
		}
		f << root->score << " ";
		if (root->left != nullptr)
			write_file_3(root->left, f);
		if (root->right != nullptr)
			write_file_3(root->right, f);

	}
}
boardTree* read_file_3(boardTree* root, Gamestate Game, ifstream &f) {
	bool root_exist = 0;
	if (f.is_open()) {
		while (!f.eof()) {
			for (int i = 0; i < (Game.span*Game.span); i++) {
				f >> Game.AI[i];
			}
			if (!root_exist) {
				root = addToTree(root, Game);
				root_exist = 1;
			}
			else {
				addToTree(root, Game);
			}
			boardTree* work_node = find(root, Game);
			f >> work_node->score;

		}
	}
	for (int i = 0; i < (Game.span * Game.span); ++i) {
		Game.AI[i] = 0;
	}
	//preOrder(root);
	return root;
}
void write_file_5(boardTree* root, ofstream &f) {
	if (f.is_open()) {
		if (root == nullptr) {
			cout << "Drzewo nie istnieje!" << endl;
		}
		for (int i = 0; i < 25; i++) {
			int tab = root->AI[i];
			f << tab << " ";
		}
		f << root->score << " ";
		if (root->left != nullptr)
			write_file_5(root->left, f);
		if (root->right != nullptr)
			write_file_5(root->right, f);

	}
}
boardTree* read_file_5(boardTree* root, Gamestate Game, ifstream &f) {
	bool root_exist = 0;
	if (f.is_open()) {
		while (!f.eof()) {
			for (int i = 0; i < (Game.span*Game.span); i++) {
				f >> Game.AI[i];
			}
			if (!root_exist) {
				root = addToTree(root, Game);
				root_exist = 1;
			}
			else {
				addToTree(root, Game);
			}
			boardTree* work_node = find(root, Game);
			f >> work_node->score;

		}
	}
	for (int i = 0; i < (Game.span * Game.span); ++i) {
		Game.AI[i] = 0;
	}
	//preOrder(root);
	return root;
}
int scoreCheck(Gamestate Game) {
	int* available_moves = new int[(Game.span*Game.span) - Game.Turn_Counter];
	int j = 0;
	int best_score;
	bool best_init = false;
	int preferred_move = 0;
	boardTree* score_donor = nullptr;
	for (int i = 0; i < (Game.span * Game.span); i++) {

		if (Game.AI[i] == 0) {
			available_moves[j] = i + 1;
			j++;
		}
	}
	for (int i = 0; i < j; i++) {
		Game.AI[(available_moves[i] - 1)] = 1;
		if (Game.span == 3) {
			score_donor = find(Game.Tree_root_3, Game);
		}
		else {
			score_donor = find(Game.Tree_root_5, Game);
		}
		if (score_donor == nullptr) {
		}
		else {
			if (score_donor->score < 0) {

			}
			else if (!best_init) {
				best_score = score_donor->score;
				preferred_move = i;
				best_init = true;
			}
			else if (score_donor->score > best_score) {
				best_score = score_donor->score;
				preferred_move = i;
			}
			else if (score_donor->score == best_score) {
				int conf_resolve = rand() % 2;
				if (conf_resolve == 0) {

				}
				else {
					best_score = score_donor->score;
					preferred_move = i;
				}
			}
		}
		Game.AI[(available_moves[i] - 1)] = 0;
	}
	if (preferred_move != 0) {
		preferred_move = available_moves[preferred_move];
		cout << "Chosen:" << preferred_move << endl;
		return preferred_move;
	}
	else {
		cout << "Chosen: 0" << endl;
		return 0;
	}
}
void scoreUpdate(Gamestate &Game) {
	if (Game.Winner == 1) {   //Zły rezultat
		boardTree* Updated_Node = nullptr;
		if (Game.span == 3) {
			Updated_Node = find(Game.Tree_root_3, Game);
		}
		else {
			Updated_Node = find(Game.Tree_root_5, Game);
		}
		Updated_Node->score -= 1;
		for (int i = 0; i < Game.Turn_Counter; i++) {
			Game.AI[(Game.moves[((Game.Turn_Counter) - i) - 1]) - 1] = 0;
			cout << Game.moves[((Game.Turn_Counter) - i) - 1] << endl;
			Maziaj_AI(Game);
			boardTree* Updated_Node = nullptr;
			if (Game.span == 3) {
				Updated_Node = find(Game.Tree_root_3, Game);
			}
			else {
				Updated_Node = find(Game.Tree_root_5, Game);
			}
			Updated_Node->score -= 1;
		}

	}
	else if (Game.Winner == 3) { //Dobry rezultat (Komputer zremisował)
		boardTree* Updated_Node = nullptr;
		if (Game.span == 3) {
			Updated_Node = find(Game.Tree_root_3, Game);
		}
		else {
			Updated_Node = find(Game.Tree_root_5, Game);
		}
		Updated_Node->score += 1;
		for (int i = 0; i < Game.Turn_Counter; i++) {
			Game.AI[(Game.moves[((Game.Turn_Counter) - i) - 1]) - 1] = 0;
			cout << Game.moves[((Game.Turn_Counter) - i) - 1] << endl;
			Maziaj_AI(Game);
			boardTree* Updated_Node = nullptr;
			if (Game.span == 3) {
				Updated_Node = find(Game.Tree_root_3, Game);
			}
			else {
				Updated_Node = find(Game.Tree_root_5, Game);
			}
			Updated_Node->score += 1;
		}

	}
	else if (Game.Winner == 2) { //Najlepszy rezultat (Komputer wygrał)
		boardTree* Updated_Node = nullptr;
		if (Game.span == 3) {
			Updated_Node = find(Game.Tree_root_3, Game);
		}
		else {
			Updated_Node = find(Game.Tree_root_5, Game);
		}
		Updated_Node->score += 3;
		for (int i = 0; i < Game.Turn_Counter; i++) {
			Game.AI[(Game.moves[((Game.Turn_Counter) - i) - 1]) - 1] = 0;
			cout << Game.moves[((Game.Turn_Counter) - i) - 1] << endl;
			Maziaj_AI(Game);
			boardTree* Updated_Node = nullptr;
			if (Game.span == 3) {
				Updated_Node = find(Game.Tree_root_3, Game);
			}
			else {
				Updated_Node = find(Game.Tree_root_5, Game);
			}
			Updated_Node->score += 3;
		}

	}
}

Gamestate Initialize_Gamestate(Gamestate &Game) {
	string input_span;
	Game.span = -1;
	Game.Winner = 0;
	Game.Turn_Counter = 0;
	cout << "Wybierz wielkosc planszy: 3x3[3] lub 5x5[5]" << endl;
	do {
		cin >> input_span;
		if (input_span.size() == 1 && isdigit(input_span[0])) {
			Game.span = stoi(input_span);
		}
	} while (Game.span != 3 && Game.span != 5);
	char znak;
	cout << "Wybierz swoj znak [x lub o]" << endl;
	do {
		cin >> znak;
	} while (znak != cross && znak != circle);
	Game.Player = znak;
	if (Game.Player == cross) {
		Game.CPU = circle;
	}
	else {
		Game.CPU = cross;
	}
	Game.T = new char*[Game.span];
	for (int i = 0; i < Game.span; ++i)
		Game.T[i] = new char[Game.span];
	int field_nr = '0';

	for (int i = 0; i < Game.span; ++i) {
		for (int j = 0; j < Game.span; ++j) {
			field_nr++;
			Game.T[i][j] = none;
		}
	}
	Game.AI = new int[(Game.span * Game.span)];
	for (int i = 0; i < (Game.span * Game.span); ++i) {
		Game.AI[i] = 0;
	}
	Game.moves = new int[(Game.span * Game.span)];
	for (int i = 0; i < (Game.span * Game.span); ++i) {
		Game.moves[i] = 0;
	}
	Game.initialized = true;
	return Game;
}
Gamestate Player_Turn(Gamestate &Game) {
	int chosen_field = -1;
	int i, j;
	string field_input;
	if (Game.span == 3) {
		if (!(find(Game.Tree_root_3, Game))) {
			Game.Tree_root_3 = addToTree(Game.Tree_root_3, Game);
		}
	}
	else {
		if (!(find(Game.Tree_root_5, Game))) {
			Game.Tree_root_5 = addToTree(Game.Tree_root_5, Game);
		}
	}
	do {
		do {
			cin >> field_input;
			if (field_input.size() == 2) {
				if (isdigit(field_input[0]) && isdigit(field_input[1])) {
					chosen_field = stoi(field_input);
				}
				else cout << "Wprowadz liczbe" << endl;
			}
			else if (field_input.size() == 1) {
				if (isdigit(field_input[0])) {
					chosen_field = stoi(field_input);
				}
				else cout << "Wprowadz liczbe" << endl;
			}
			else cout << "Wprowadz liczbe mniejszą od 99" << endl;
		} while (!(((Game.span*Game.span) >= chosen_field) && (chosen_field > 0)));
		i = (chosen_field - 1) / Game.span;
		j = (chosen_field + (Game.span - 1)) % Game.span;
		if (Game.T[i][j] == none) {
			Game.T[i][j] = Game.Player;
			Game.AI[(chosen_field - 1)] = -1;
			Maziaj_AI(Game);
			if (Game.span == 3) {
				if (find(Game.Tree_root_3, Game) == nullptr) {
					addToTree(Game.Tree_root_3, Game);
				}
			}
			else {
				if (find(Game.Tree_root_5, Game) == nullptr) {
					addToTree(Game.Tree_root_5, Game);
				}
			}
			Game.moves[Game.Turn_Counter] = chosen_field;
			Maziaj_moves(Game);
			Game.Turn_Counter++;
			break;
		}
		cout << "Wybierz pole ktore nie jest juz zajete" << endl;
	} while (true);
	return Game;
}

Gamestate CPU_Turn(Gamestate &Game) {
	int chosen_field, i, j;
	if (Game.span == 3) {
		if (!(find(Game.Tree_root_3, Game))) {
			Game.Tree_root_3 = addToTree(Game.Tree_root_3, Game);
		}
	}
	else {
		if (!(find(Game.Tree_root_5, Game))) {
			Game.Tree_root_5 = addToTree(Game.Tree_root_3, Game);
		}
	}
	chosen_field = scoreCheck(Game);
	cout << "Odwolanie do scoreCheck" << endl;
	if (chosen_field == 0) {
		do {

			chosen_field = rand() % (Game.span*Game.span) + 1;
			cout << "Wylosowano pole nr:" << chosen_field << endl;
			i = (chosen_field - 1) / Game.span;
			j = (chosen_field + (Game.span - 1)) % Game.span;
			if (Game.T[i][j] == none) {
				cout << "Wylosowane pole puste" << endl;
				break;
			}
		} while (true);
	}
	else {
		i = (chosen_field - 1) / Game.span;
		j = (chosen_field + (Game.span - 1)) % Game.span;
	}
	Game.T[i][j] = Game.CPU;
	Game.AI[(chosen_field - 1)] = 1;
	Maziaj_AI(Game);
	if (Game.span == 3) {
		if (find(Game.Tree_root_3, Game) == nullptr) {
			addToTree(Game.Tree_root_3, Game);
		}
	}
	else {
		if (find(Game.Tree_root_5, Game) == nullptr) {
			addToTree(Game.Tree_root_5, Game);
		}
	}
	Game.moves[Game.Turn_Counter] = chosen_field;
	Game.Turn_Counter++;
	return Game;
}

bool Check_for_Win(Gamestate &Game) {
	bool Win = false;
	for (int i = 0; i < Game.span; i++) {
		for (int j = 1; j < Game.span; j++) {
			if (Game.T[i][j] == none) {
				break;
			}
			else if (Game.T[i][j] != Game.T[i][j - 1]) {
				break;
			}
			else if ((j == (Game.span - 1) && (Game.T[i][j] == Game.T[i][j - 1]))) {
				Win = true;
				if (Game.T[i][j] == Game.Player) {
					Game.Player_Wins++;
					Game.Winner = 1;
					cout << "Gracz wygrywa!" << endl;
				}
				if (Game.T[i][j] == Game.CPU) {
					Game.CPU_Wins++;
					Game.Winner = 2;
					cout << "Komputer wygrywa!" << endl;
				}
			}
		}
	}
	for (int i = 0; i < Game.span; i++) {
		for (int j = 1; j < Game.span; j++) {
			if (Game.T[j][i] == none) {
				break;
			}
			else if (Game.T[j][i] != Game.T[j - 1][i]) {
				break;
			}
			else if ((j == (Game.span - 1) && (Game.T[j][i] == Game.T[j - 1][i]))) {
				Win = true;
				if (Game.T[j][i] == Game.Player) {
					Game.Player_Wins++;
					Game.Winner = 1;
					cout << "Gracz wygrywa!" << endl;
				}
				if (Game.T[j][i] == Game.CPU) {
					Game.CPU_Wins++;
					Game.Winner = 2;
					cout << "Komputer wygrywa!" << endl;
				}
			}
		}
	}
	for (int i = 1; i < Game.span; i++) {
		if (Game.T[i][i] == none) {
			break;
		}
		else if (Game.T[i][i] != Game.T[i - 1][i - 1]) {
			break;
		}
		else if ((i == (Game.span - 1) && (Game.T[i][i] == Game.T[i - 1][i - 1]))) {
			Win = true;
			if (Game.T[i][i] == Game.Player) {
				Game.Player_Wins++;
				Game.Winner = 1;
				cout << "Gracz wygrywa!" << endl;
			}
			if (Game.T[i][i] == Game.CPU) {
				Game.CPU_Wins++;
				Game.Winner = 2;
				cout << "Komputer wygrywa!" << endl;
			}
		}
	}
	for (int i = 1; i < Game.span; i++) {
		if (Game.T[i][(Game.span - 1) - i] == none) {
			break;
		}
		else if (Game.T[i][(Game.span - 1) - i] != Game.T[i - 1][(Game.span) - i]) {
			break;
		}
		else if ((i == (Game.span - 1) && (Game.T[i][(Game.span - 1) - i] == Game.T[i - 1][Game.span - i]))) {
			Win = true;
			if (Game.T[i][(Game.span - 1) - i] == Game.Player) {
				Game.Player_Wins++;
				Game.Winner = 1;
				cout << "Gracz wygrywa!" << endl;
			}
			if (Game.T[i][(Game.span - 1) - i] == Game.CPU) {
				Game.CPU_Wins++;
				Game.Winner = 2;
				cout << "Komputer wygrywa!" << endl;
			}
		}
	}
	return Win;
}
void Check_For_End(Gamestate &Game) {
	if (Game.Turn_Counter < Game.span) {
		return;
	}
	else if (Check_for_Win(Game)) {
		return;
	}
	else if (Game.Turn_Counter == Game.span * Game.span) {
		Game.Winner = 3;
		cout << "Remis!" << endl;
		return;
	}

}
void Draw_Board(Gamestate &Game) {
	int field_nr = 0;
	if (Game.initialized == false) {
		Initialize_Gamestate(Game);
	}
	cout << (char)(lewy_gorny); //╔
	for (int i = 0; i < (Game.span - 1); ++i) {
		cout << (char)(pozioma) << (char)(pozioma) << (char)(pozioma) << (char)(pozioma) << (char)(t_dol); //═══╦ * span
	}
	cout << (char)(pozioma) << (char)(pozioma) << (char)(pozioma) << (char)(pozioma) << (char)(prawy_gorny); // ═══╗
	cout << endl;
	for (int i = 0; i < Game.span; ++i) {
		cout << (char)(pionowa);
		for (int j = 0; j < Game.span; j++) {
			cout << "    " << (char)(pionowa);
		}
		cout << endl;
		cout << (char)(pionowa);
		for (int j = 0; j < Game.span; j++) {
			field_nr++;
			if (Game.T[i][j] == 0) {
				cout << " " << setw(2) << field_nr << " " << (char)(pionowa);
			}
			else {
				cout << " " << setw(2) << Game.T[i][j] << " " << (char)(pionowa);
			}
		}
		cout << endl;
		cout << (char)(pionowa);
		for (int j = 0; j < Game.span; j++) {
			cout << "    " << (char)(pionowa);
		}
		cout << endl;
		if (i != (Game.span - 1)) {
			cout << (char)(t_prawo);
			for (int j = 0; j < (Game.span - 1); j++) {
				cout << (char)(pozioma) << (char)(pozioma) << (char)(pozioma) << (char)(pozioma) << (char)(skrzyz);
			}
			cout << (char)(pozioma) << (char)(pozioma) << (char)(pozioma) << (char)(pozioma) << (char)(t_lewo);
			cout << endl;
		}
	}
	cout << (char)(lewy_dolny);
	for (int i = 0; i < (Game.span - 1); ++i) {
		cout << (char)(pozioma) << (char)(pozioma) << (char)(pozioma) << (char)(pozioma) << (char)(t_gora);
	}
	cout << (char)(pozioma) << (char)(pozioma) << (char)(pozioma) << (char)(pozioma) << (char)(prawy_dolny) << endl;
}
Gamestate Single_Game(Gamestate &Game) {
	Draw_Board(Game);
	if (Game.span == 3) {
		ifstream fi;
		fi.open("3x3");
		Game.Tree_root_3 = read_file_3(Game.Tree_root_3, Game, fi);
	}
	else {
		ifstream fi;
		fi.open("5x5");
		Game.Tree_root_5 = read_file_5(Game.Tree_root_5, Game, fi);
	}
	Game.Who_First = rand() % 2;
	if (Game.Who_First == 0) {
		cout << "Gre rozpoczyna Gracz";
		do {
			Player_Turn(Game);
			Draw_Board(Game);
			Check_For_End(Game);
			//preOrder(Game.Tree_root_5);
			if (Game.Winner != 0) {
				scoreUpdate(Game);
				if (Game.span == 3) {
					ofstream fo;
					fo.open("3x3");
					write_file_3(Game.Tree_root_3, fo);
				}
				else {
					ofstream fo;
					fo.open("5x5");
					write_file_5(Game.Tree_root_5, fo);
				}
				Game.initialized = false;
				break;
			}
			CPU_Turn(Game);
			Draw_Board(Game);
			Check_For_End(Game);
			//preOrder(Game.Tree_root_5);
			if (Game.Winner != 0) {
				scoreUpdate(Game);
				if (Game.span == 3) {
					ofstream fo;
					fo.open("3x3");
					write_file_3(Game.Tree_root_3, fo);
				}
				else {
					ofstream fo;
					fo.open("5x5");
					write_file_5(Game.Tree_root_5, fo);
				}
				Game.initialized = false;
				break;
			}
		} while (true);
		return Game;
	}
	else {
		cout << "Gre rozpoczyna Komputer";
		do {
			CPU_Turn(Game);
			//preOrder(Game.Tree_root_5);
			Draw_Board(Game);
			Check_For_End(Game);
			//preOrder(Game.Tree_root_5);
			if (Game.Winner != 0) {
				if (Game.span == 3) {
					ofstream fo;
					fo.open("3x3");
					write_file_3(Game.Tree_root_3, fo);
				}
				else {
					ofstream fo;
					fo.open("5x5");
					write_file_5(Game.Tree_root_5, fo);
				}
				scoreUpdate(Game);
				Game.initialized = false;
				break;
			}
			Player_Turn(Game);
			//preOrder(Game.Tree_root_5);
			Draw_Board(Game);
			Check_For_End(Game);
			if (Game.Winner != 0) {
				scoreUpdate(Game);
				if (Game.span == 3) {
					ofstream fo;
					fo.open("3x3");
					write_file_3(Game.Tree_root_3, fo);
				}
				else {
					ofstream fo;
					fo.open("5x5");
					write_file_5(Game.Tree_root_5, fo);
				}
				Game.initialized = false;
				break;
			}
		} while (true);
		return Game;
	}
}
void Session_Summary(Gamestate &Game) {
	cout << "Podsumowanie:" << endl;
	cout << "Gracz wygral " << Game.Player_Wins << " razy" << endl;
	cout << "Komputer wygral " << Game.CPU_Wins << " razy" << endl;
	for (int i = 0; i < Game.span; i++) {
		cout << "Usuwam tablice zawierajaca planszy" << endl;
		delete[] Game.T[i];
	}
	cout << "Usuwam tablice wskaznikow na tablice pol planszy" << endl;
	delete[] Game.T;
	cout << "Usuwam tablice AI" << endl;
	delete[] Game.AI;
	cout << "Usuwam tablice zapisujaca wykonane ruchy" << endl;
	delete[] Game.moves;
	cout << "Usuwam drzewo 3x3" << endl;
	deleteTree(Game.Tree_root_3);
	cout << "Usuwam drzewo 5x5" << endl;
	deleteTree(Game.Tree_root_5);
}
