#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <fstream>
using namespace std;
enum znaki {
	horizontal = '-', 
	vertical = '|',
	intersection = '+', 
	cross = 'x',
	circle = 'o',
	none = 0
};
/** Wezel drzewa binarnego plansz */
struct boardTree {
	boardTree *left;
	int *AI; //Player jako -1, komputer jako 1
	boardTree *right;
	int score = 0;
};

/**Lista zawierajaca korzenie drzew dla danych wielkosci plansz*/
struct listOfRoots {
	int span;
	boardTree* root;
	listOfRoots* pNext;
};

/** Struktura, w ktorej przechowywane ss informacje dotyczace obecnego stanu gry*/
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
	listOfRoots* list_header;
	boardTree* current_root;
};

/** Funkcja porównująca dwie tablice stanów planszy.
@param Game Struktura gry, z której funkcja pobiera wielkość planszy oraz obecny stan tablicy AI
@param board Tablica AI znajdująca się w danym węźle drzewa binarnego
@return wartość -1,1,0 w zależności, czy tablica obecnego stanu gry mniejsza, większa lub równa
*/
int compareBoards(Gamestate Game, int *board);

/** Funkcja znajdujaca korzen drzewa dla danej wielkosci planszy 
@param header Pierwszy wezel listy korzeni drzewa
@param span Dlugosc planszy
@return Wskaznik na wezel listy zawierajacy wskaznik na odpowiedni korzen drzewa
*/
listOfRoots* findRoot(listOfRoots* header, int span);

/** Funkcja dodajaca wezel zawierajacy korzen drzewa dla odpowiedniej wielkosci planszy
@param header Pierwszy wezel listy korzeni drzewa
@param span Dlugosc planszy
@return Wskaznik na wezel listy zawierajacy wskaznik na odpowiedni korzen drzewa
*/
listOfRoots* addRoot(listOfRoots* header, int span);

/** Funkcja usuwajaca liste zawierajaca korzenie drzewa
@param header Pierwszy wezel listy korzeni drzewa
*/
void deleteList(listOfRoots* header);

/** Funkcja szukająca danego stanu planszy w drzewie binarnym, wykorzystująca funkcje compareBoards
@param root Korzeń drzewa, z którego gra korzysta
@param Game Struktura gry, którą funkcja przekazuje funkcji compareBoards
@return wskaźnik na węzeł drzewa zawierający daną tablice
*/
boardTree* find(boardTree * root, Gamestate Game);

/** Funkcja dodająca dany stanu planszy do drzewa binarnego, wykorzystująca funkcje compareBoards
@param root Korzeń drzewa, z którego gra korzysta
@param Game Struktura gry, którą funkcja przekazuje funkcji compareBoards
@return wskaźnik na korzeń drzewa
*/
boardTree* addToTree(boardTree* root, Gamestate Game);


/** Funkcja sczytująca drzewo binarne gry z odpowiedniego pliku
@param root wskaźnik, do którego funkcja ma dołączyć sczytane drzewo
@param Game Struktura gry, z której funkcja pobiera wielkość planszy
@param f Strumien wejsciowy, za pomoca ktorego odczytujemy
@return wskaźnik na pierwszy węzeł, który został dołączony
*/
boardTree* readFile(Gamestate &Game, ifstream &f);

/** Funkcja zapisująca drzewo binarne gry do odpowiedniego pliku
@param root Korzen zapisywanego drzewa
@param f Strumien wyjsciowy, za pomoca ktorego zapisujemy
*/
void writeFile(boardTree* root, int span, ofstream &f);

/** Funkcja sprawdzająca wartość możliwych ruchów w danym momencie 
@param Game Struktura gry, z której funkcja pobiera tablice AI
@return numer pola, który w danym momencie jest najlepszy lub 0, gdy wszystkie następne możliwości nieznane lub przegrywające
*/
int scoreCheck(Gamestate Game);

/**Funkcja tworzaca tablice, w ktorej ruchy gracza traktowane sa jak ruchy komputera i vice versa w celu wykorzystania ruchow gracza do nauki
@param Game Struktura gry, z której funkcja pobiera tablice AI, ktora jest baza do stworzenia nowej tablicy
@return wskaznik na nowa tablice
*/
int* invertAI(Gamestate Game);

/** Funkcja wykorzystujaca ruchy gracza do nauki sztucznej inteligencji
@param Game Struktura gry, z której funkcja pobiera tablice moves, oraz ilość ruchów, po którym rozgrywka się zakończyła
@param board Tablica stworzona przez funkcje invertAI
*/
void playerScoreUpdate(Gamestate Game, int* board);
/** Funkcja zmieniająca wartość ruchów które wystąpiły w danej rozgrywce zgodnie z wynikiem (Wygrana +3, Remis +1, Przegrana -1)
@param Game Struktura gry, z której funkcja pobiera tablice AI, tablice moves, oraz ilość ruchów, po którym rozgrywka się zakończyła
*/
void scoreUpdate(Gamestate &Game);
/** Funkcja sprawdzajaca, czy podany parametr jest prawidlowy i gdy jest prawidlowy zamieniajaca go na int
@param input parametr podany z linii komend
@return przetworzony parametr
*/
int convertParam(char* input);
/** Funkcja inicjująca rozgrywkę, pyta gracza o parametry gry i tworzy potrzebne tablice
@param Game Niezainicjowana struktura gry
*/
void initializeGamestate(Gamestate &Game);

/** Funkcja podczas której odbywa się ruch gracza
@param Game Struktura gry, do której zapisany będzie ruch gracza
@return Struktura gry zaktualizowana o ruch gracza
*/
Gamestate playerTurn(Gamestate &Game);

/** Funkcja podczas której odbywa się ruch komputera
@param Game Struktura gry, do której zapisany będzie ruch komputera
@return Struktura gry zaktualizowana o ruch komputera
*/
Gamestate CPUTurn(Gamestate &Game);

/** Funkcja która sprawdza, czy gra powinna się zakończyć, jeśli tak, to ustawia zmienną Winner w strukturze gry na odpowiednią wartość (3 dla remisu)
@param Game Struktura gry, do której będzie zapisane to, czy gra powinna się skończyć
*/
void checkForEnd(Gamestate &Game);

/** Funkcja która sprawdza, czy gra w danym momencie posiada zwycięzce, jeśli tak, to ustawia zmienną Winner w strukturze gry na odpowiednią wartość (1 dla Gracza, 2 dla Komputera)
@param Game Struktura gry, do której będzie zapisane to, czy gra powinna się skończyć
@return Zwraca true, jeśli gra ma zwycięzce, false, jeśli nie
*/
bool checkForWin(Gamestate &Game);

/** Funkcja która rysuje pole gry
@param Game Struktura gry, z której funkcja będzie pobierać to jaki znak powinien się pojawić na jakim polu
*/
void drawBoard(Gamestate &Game);

/** Funkcja, w której zawarte są wszystkie funkcje potrzebne do rozegrania pojedynczej gry
@param Game Struktura gry, na której program operuje
@return Funkcja zwraca strukturę gry w takim stanie, który pozwala na ponowne rozegranie gry
*/
Gamestate singleGame(Gamestate &Game);

/** Funkcja, która wypisuje statystyki gracza i komputera, oraz czyści wszystko, co było dynamicznie zaalokowane
@param Game Struktura gry, z której program uzyskuje dostęp do tablic i struktur do usunięcia
*/
void sessionSummary(Gamestate &Game);

#endif
