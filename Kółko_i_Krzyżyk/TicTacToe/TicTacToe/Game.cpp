#include <iostream>
#include <iomanip>
#include <string>
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

listOfRoots* findRoot(listOfRoots* header, int span) {
	if (header == nullptr) {
		cout << "Nie znaleziono korzenia drzewa" << endl;
		return nullptr;
	}
	else {
		if (header->span == span) {
			return header;
		}
		else {
			findRoot(header->pNext, span);
		}
	}
}

listOfRoots* addRoot(listOfRoots* header, int span) {
		listOfRoots* new_node = new listOfRoots;
		cout << "stworzono nowy element listy" << endl;
		new_node->pNext = header;
		new_node->span = span;
		new_node->root = nullptr;
		header = new_node;
		return new_node;
}
void deleteList(listOfRoots* header) {
	if (header->pNext != nullptr) {
		deleteList(header->pNext);
	}
	else {
		delete header;
	}
}
boardTree* find(boardTree * root, Gamestate Game) {
	boardTree* work_root = root;
	if (root == nullptr) {
		return 0; //Nie znaleziono wezla
	}
	if (root != nullptr) {
		int compared = compareBoards(Game, work_root->AI);
		if (compared < 0) {
			return find(work_root->left, Game); //Szukaj w lewo
		}
		else if (compared > 0) {
			return find(work_root->right, Game); //Szukaj w prawo
		}
		else {
			return root; //Znaleziono
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
				root->left = addToTree(root->left, Game); //Dodaj w lewo
			}
			else {
				addToTree(root->left, Game); //Przejdz w lewo
			}
		}
		else if (compareBoards(Game, root->AI) > 0) {
			if (root->right == nullptr) {
				root->right = addToTree(root->right, Game); //Dodaj w prawo
			}
			else {
				addToTree(root->right, Game); //Przejdz w prawo
			}
		}
		else {
			return root;
		}
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

void writeFile(boardTree* root, int span, ofstream &fo) {
	if (fo.is_open()) {
		if (root == nullptr) {
			cout << "Drzewo nie istnieje!" << endl;
		}
		for (int i = 0; i < 9; i++) {
			int tab = root->AI[i];
			fo << tab << " ";
		}
		fo << root->score << " ";
		if (root->left != nullptr)
			writeFile(root->left, span, fo);
		if (root->right != nullptr)
			writeFile(root->right, span, fo);

	}
}

boardTree* readFile(Gamestate &Game, ifstream &f) {
	if (findRoot(Game.list_header, Game.span) == nullptr) {  
		Game.list_header = addRoot(Game.list_header, Game.span);
		Game.current_root = (findRoot(Game.list_header, Game.span))->root;
		if (f.is_open()) {
			while (!f.eof()) {
				for (int i = 0; i < (Game.span*Game.span); i++) {
					f >> Game.AI[i];
					if (Game.AI[i] > 1 || Game.AI[i] < -1) {
						cout << "Nieprawidlowe dane w pliku" << endl;
						return nullptr;
					}
				}
				if (Game.current_root == nullptr) {
				Game.current_root =	addToTree(Game.current_root, Game);
				}
				else {
				addToTree(Game.current_root, Game);
				}
				boardTree* work_node = find(Game.current_root, Game);
				f >> work_node->score;

			}
		}
		else {
			cout << "Nie znaleziono pliku z danymi drzewa" << endl;
		}
		for (int i = 0; i < (Game.span * Game.span); ++i) {
			Game.AI[i] = 0;
		}
		return Game.current_root;
	}
	else {
		if (f.is_open()) {
			while (!f.eof()) {
				for (int i = 0; i < (Game.span*Game.span); i++) {
					f >> Game.AI[i];
					if (Game.AI[i] > 1 || Game.AI[i] < -1) {
						cout << "Nieprawidlowe dane w pliku" << endl;
						return nullptr;
					}
				}
				addToTree(Game.current_root, Game);
				boardTree* work_node = find(Game.current_root, Game);
				f >> work_node->score;

			}
		}
		else {
			cout << "Nie znaleziono pliku z danymi drzewa" << endl;
		}
		for (int i = 0; i < (Game.span * Game.span); ++i) {
			Game.AI[i] = 0;
		}
		return Game.current_root;
	}

}
void checkIfLoaded(Gamestate &Game) {
	if (findRoot(Game.list_header, Game.span) == nullptr) {  
		Game.list_header = addRoot(Game.list_header, Game.span);
		Game.current_root = (findRoot(Game.list_header, Game.span))->root;
		if (Game.current_root == nullptr) {
			Game.current_root = addToTree(Game.current_root, Game);
		}
	}
	else {
		if (Game.current_root == nullptr) {
			Game.current_root = addToTree(Game.current_root, Game);
		}
	}
}
int scoreCheck(Gamestate Game) {
	int* available_moves = new int[(Game.span*Game.span) - Game.Turn_Counter];
	int j = 0;
	int best_score;
	int conf_resolve;
	bool best_init = false;
	int preferred_move = 0;
	int donor_count = 0;
	boardTree* score_donor = nullptr;
	for (int i = 0; i < (Game.span * Game.span); i++) {

		if (Game.AI[i] == 0) {
			//Wypełniamy tablice możliwymi ruchami w danej chwili (Pola, dla których tablica AI zawiera 0)
			available_moves[j] = i + 1;
			j++;
		}
	}
	for (int i = 0; i < j; i++) {
		//Symulujemy ruch - zmieniamy odpowiednie pole w tablicy AI na 1
		Game.AI[(available_moves[i] - 1)] = 1;
		//Sprawdzamy, czy taki stan planszy już istnieje w naszym drzewie
		score_donor = find(Game.current_root, Game);
		if (score_donor == nullptr) {
			if (!best_init) {
				/*Jeśli zmienna best_score nie ma jeszcze przypisanej jakiejkolwiek wartosci (best_init = false),
				znaczy, że jest to pierwszy ruch jaki rozważamy i przyjmujemy go bez sprawdzania jego wartosci.
				(Wartosc dla pól nieznanych jest przyjmowana jako 0)
				*/
				best_score = 0;
				preferred_move = i;
				best_init = true;
			}
			else if (best_score < 0) {
				/*Jeżeli pole uznane za najlepsze do tej pory ma wartość mniejszą od zero (powoduje przegraną),
				ustawiamy pole nieznane jako to, które komputer ma wybrać i zmieniamy najlepszą znalezioną wartość na 0
				*/
				best_score = 0;
				preferred_move = i;
			}
			else if (best_score == 0) {
				//W przypadku, kiedy dwa pola mają taką samą wartość, losowane jest jedno spośród nich
				conf_resolve = rand() % 2;
				if (conf_resolve == 0) {

				}
				else {
					best_score = 0;
					preferred_move = i;
				}
			}
		}
		else {
			if (!best_init) {
				/*Jeśli zmienna best_score nie ma jeszcze przypisanej jakiejkolwiek wartosci (best_init = false),
				znaczy, że jest to pierwszy ruch jaki rozważamy i przyjmujemy go bez sprawdzania jego wartosci.
				*/
				best_score = score_donor->score;
				preferred_move = i;
				best_init = true;
			}
			else if (score_donor->score > best_score) {
				/*Jeżeli pole uznane za najlepsze do tej pory ma wartość mniejszą od pola obecnie rozważanego, ustawiamy
				go jako pole do wybrania i jego wartość staje się największą wartością
				*/
				best_score = score_donor->score;
				preferred_move = i;
			}
			else if (score_donor->score == best_score) {
				//W przypadku, kiedy dwa pola mają taką samą wartość, losowane jest jedno spośród nich
				conf_resolve = rand() % 2;
				if (conf_resolve == 0) {

				}
				else {
					best_score = score_donor->score;
					preferred_move = i;
				}
			}
		}
		//Koniec symulacji - zmieniamy odpowiednie pole w tablicy AI spowrotem na 0, wracając do obecnego stanu
		Game.AI[(available_moves[i] - 1)] = 0;
	}
		preferred_move = available_moves[preferred_move];
		delete[] available_moves;
		return preferred_move;
}
void scoreUpdate(Gamestate &Game) {
	if (Game.Winner == 1) {   //Zły rezultat
		boardTree* Updated_Node = nullptr;
		Updated_Node = find(Game.current_root, Game);
		Updated_Node->score -= 1;
		for (int i = 0; i < Game.Turn_Counter; i++) {
			Game.AI[(Game.moves[((Game.Turn_Counter) - i) - 1]) - 1] = 0;
			if (i == 0) {
				/*
				Aby zapobiec "zrażaniu" się komputera do danych pól, w przypadku przegranej jedyne stany planszy, którym
				zmieniamy score to plansza, która jest wygrywającą dla gracza (ostatnia) oraz ta, która pozwoliła graczowi
				wygrać w następnym ruchu (przedostatnia)
				*/
				Updated_Node = find(Game.current_root, Game);
				Updated_Node->score -= 1;
			}
		}

	}
	else if (Game.Winner == 3) { //Dobry rezultat (Komputer zremisował)
		boardTree* Updated_Node = nullptr;
		Updated_Node = find(Game.current_root, Game);
		Updated_Node->score += 1;
		for (int i = 0; i < Game.Turn_Counter; i++) {
			/*
			W przypadku dobrego rezultatu, każdy stan planszy jaki się pojawił podczas gry ma score podwyższany o 1,
			aby komputer wiedział na przyszłość, że do takiego stanu ma dążyć. Wyciągamy od końca odpowiednie numery
			pól z tablicy moves, zerujemy odpowiadające im pola w tablicy AI i następnie zmieniamy score
			*/
			Game.AI[(Game.moves[((Game.Turn_Counter) - i) - 1]) - 1] = 0;
			Updated_Node = find(Game.current_root, Game);
			Updated_Node->score += 1;
		}

	}
	else if (Game.Winner == 2) { //Najlepszy rezultat (Komputer wygrał)
		boardTree* Updated_Node = nullptr;
		Updated_Node = find(Game.current_root, Game);
		Updated_Node->score += 3;
		for (int i = 0; i < Game.Turn_Counter; i++) {
			Game.AI[(Game.moves[((Game.Turn_Counter) - i) - 1]) - 1] = 0;
			Updated_Node = find(Game.current_root, Game);
			Updated_Node->score += 3;
		}

	}
}

void initializeGamestate(Gamestate &Game) {
	string input_span;
	Game.Winner = 0;
	Game.Turn_Counter = 0;
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
	/*
	Utworzenie tablicy odpowiadającej bezpośrednio za wyświetlanie planszy, przechowuje ona zera w przypadku pustych pól,
	w przypadku pól zajętych znaki odpowiadające danemu graczowi.
	*/
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
	/*
	Tworzenie tablicy AI, wykorzystywanej w wyborze ruchu dla komputera, każdy stan planszy jest zapisywany w tej tablicy.
	Dla planszy 3x3 tablica będzie miała 9 elementów. Wartość -1 oznacza pole zajęte przez gracza, wartość 1 pole zajęte
	przez komputer, natomiast pole 0 oznacza pole wolne.
	*/
	Game.AI = new int[(Game.span * Game.span)];
	for (int i = 0; i < (Game.span * Game.span); ++i) {
		Game.AI[i] = 0;
	}
	/*
	Tworzenie tablicy moves, w której zapisywane są ruchy wybrane zarówno przez gracza jak i przeciwnika.
	Tablica ta wykorzystywana jest potem, aby wyzerowywać w odpowiedni sposób tablice AI i zmieniać jej wartość(score)
	*/
	Game.moves = new int[(Game.span * Game.span)];
	for (int i = 0; i < (Game.span * Game.span); ++i) {
		Game.moves[i] = 0;
	}
	Game.initialized = true;
}
Gamestate playerTurn(Gamestate &Game) {
	int chosen_field = -1;
	int a, b, i, j;
	char* input;
	do {
		do {
			input = new char[3];
			cin.getline(input, 3);
			a = input[0] - 48;
			b = input[1] - 48;
			if ((b == -48) && (a > 0 && a < 9)) {
				chosen_field = a;
			}
			else if ((b > 0 && b < 9) && (a > 0 && a < 9)) {
				chosen_field = (a * 10) + b;
			}
			delete[] input;
		} while (!(((Game.span*Game.span) >= chosen_field) && (chosen_field > 0)));
		i = (chosen_field - 1) / Game.span;
		j = (chosen_field + (Game.span - 1)) % Game.span;
		if (Game.T[i][j] == none) {
			// Ustawiamy odpowiedni znak w tablicy odpowiadającej za plansze
			Game.T[i][j] = Game.Player;
			// Ustawiamy -1 w miejscu tablicy AI odpowiadającym polu, które wybrał gracz
			Game.AI[(chosen_field - 1)] = -1;
			if (find(Game.current_root, Game) == nullptr) {
				//Jeżeli dana plansza pojawiła się po raz pierwszy i nie ma jej w drzewie, dodajemy ją do drzewa
				addToTree(Game.current_root, Game);
			}
			// Zapisujemy, które pole zostało wybrane w danym ruchu
			Game.moves[Game.Turn_Counter] = chosen_field;
			Game.Turn_Counter++;
			break;
		}
		cout << "Wybierz pole ktore nie jest juz zajete" << endl;
	} while (true);
	return Game;
}

Gamestate CPUTurn(Gamestate &Game) {
	int chosen_field, i, j;
	chosen_field = scoreCheck(Game);
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
	// Ustawiamy odpowiedni znak w tablicy odpowiadającej za plansze
	Game.T[i][j] = Game.CPU;
	// Ustawiamy 1 w miejscu tablicy AI odpowiadającym polu, które wybrał komputer
	Game.AI[(chosen_field - 1)] = 1;
	if (find(Game.current_root, Game) == nullptr) {
		//Jeżeli dana plansza pojawiła się po raz pierwszy i nie ma jej w drzewie, dodajemy ją do drzewa
		addToTree(Game.current_root, Game);
	}
	Game.moves[Game.Turn_Counter] = chosen_field;
	Game.Turn_Counter++;
	return Game;
}

bool checkForWin(Gamestate &Game) {
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
				//Sprawdzenie, czy gra zakończyła się, ponieważ któryś z graczy ułożył x znaków w linii poziomej
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
				//Sprawdzenie, czy gra zakończyła się, ponieważ któryś z graczy ułożył x znaków w linii pionowej
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
			/*
			Sprawdzenie, czy gra zakończyła się, ponieważ któryś z graczy ułożył x znaków po przekątnej od lewego
			górnego rogu do prawego dolnego
			*/
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
		/*
		Sprawdzenie, czy gra zakończyła się, ponieważ któryś z graczy ułożył x znaków po przekątnej od prawego
		górnego rogu do lewego dolnego
		*/
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
void checkForEnd(Gamestate &Game) {
	if (Game.Turn_Counter < Game.span) {
		return;
	}
	else if (checkForWin(Game)) {
		return;
	}
	else if (Game.Turn_Counter == Game.span * Game.span) {
		/*Jeżeli liczba wykonanych ruchów równa się długości planszy do potęgi drugiej i nie ma zwycięzcy, znaczy, że gra
		zakończyła się remisem
		*/
		Game.Winner = 3;
		cout << "Remis!" << endl;
		return;
	}

}
void drawBoard(Gamestate &Game) {
	int field_nr = 0;
	if (Game.initialized == false) {
		initializeGamestate(Game);
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
Gamestate singleGame(Gamestate &Game) {
	drawBoard(Game);
	ifstream fi;
	fi.open(to_string(Game.span));
	Game.current_root = readFile(Game, fi);
	checkIfLoaded(Game);
	Game.Who_First = rand() % 2;
	if (Game.Who_First == 0) {
		cout << "Gre rozpoczyna Gracz" << endl;
		do {
			playerTurn(Game);
			drawBoard(Game);
			checkForEnd(Game);
			if (Game.Winner != 0) {
				scoreUpdate(Game);
				ofstream fo;
				fo.open(to_string(Game.span));
				writeFile(Game.current_root, Game.span, fo);
				scoreUpdate(Game);
				Game.initialized = false;
				break;
			}
			CPUTurn(Game);
			drawBoard(Game);
			checkForEnd(Game);
			if (Game.Winner != 0) {
				scoreUpdate(Game);
				ofstream fo;
				fo.open(to_string(Game.span));
				writeFile(Game.current_root, Game.span, fo);
				scoreUpdate(Game);
				Game.initialized = false;
				break;
			}
		} while (true);
		return Game;
	}
	else {
		cout << "Gre rozpoczyna Komputer" << endl;
		do {
			CPUTurn(Game);
			drawBoard(Game);
			checkForEnd(Game);
			if (Game.Winner != 0) {
				ofstream fo;
				fo.open(to_string(Game.span));
				writeFile(Game.current_root, Game.span, fo);
				scoreUpdate(Game);
				Game.initialized = false;
				break;
			}
			playerTurn(Game);
			drawBoard(Game);
			checkForEnd(Game);
			if (Game.Winner != 0) {
				scoreUpdate(Game);
				ofstream fo;
				fo.open(to_string(Game.span));
				writeFile(Game.current_root, Game.span, fo);
				Game.initialized = false;
				break;
			}
		} while (true);
		return Game;
	}
}
void sessionSummary(Gamestate &Game) {
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
	cout << "Usuwam obecnie uzywane drzewo" << endl;
	deleteTree(Game.current_root);
	cout << "Usuwam liste przechowujaca korzenie drzew" << endl;
	deleteList(Game.list_header);
}
