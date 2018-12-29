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

/** Funkcja por�wnuj�ca dwie tablice stan�w planszy.
@param Game Struktura gry, z kt�rej funkcja pobiera wielko�� planszy oraz obecny stan tablicy AI
@param board Tablica AI znajduj�ca si� w danym w�le drzewa binarnego
@return warto�� -1,1,0 w zale�no�ci, czy tablica obecnego stanu gry mniejsza, wi�ksza lub r�wna
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

/** Funkcja szukaj�ca danego stanu planszy w drzewie binarnym, wykorzystuj�ca funkcje compareBoards
@param root Korze� drzewa, z kt�rego gra korzysta
@param Game Struktura gry, kt�r� funkcja przekazuje funkcji compareBoards
@return wska�nik na w�ze� drzewa zawieraj�cy dan� tablice
*/
boardTree* find(boardTree * root, Gamestate Game);

/** Funkcja dodaj�ca dany stanu planszy do drzewa binarnego, wykorzystuj�ca funkcje compareBoards
@param root Korze� drzewa, z kt�rego gra korzysta
@param Game Struktura gry, kt�r� funkcja przekazuje funkcji compareBoards
@return wska�nik na korze� drzewa
*/
boardTree* addToTree(boardTree* root, Gamestate Game);


/** Funkcja sczytuj�ca drzewo binarne gry z odpowiedniego pliku
@param root wska�nik, do kt�rego funkcja ma do��czy� sczytane drzewo
@param Game Struktura gry, z kt�rej funkcja pobiera wielko�� planszy
@param f Strumien wejsciowy, za pomoca ktorego odczytujemy
@return wska�nik na pierwszy w�ze�, kt�ry zosta� do��czony
*/
boardTree* readFile(Gamestate &Game, ifstream &f);

/** Funkcja zapisuj�ca drzewo binarne gry do odpowiedniego pliku
@param root Korzen zapisywanego drzewa
@param f Strumien wyjsciowy, za pomoca ktorego zapisujemy
*/
void writeFile(boardTree* root, int span, ofstream &f);

/** Funkcja sprawdzaj�ca warto�� mo�liwych ruch�w w danym momencie 
@param Game Struktura gry, z kt�rej funkcja pobiera tablice AI
@return numer pola, kt�ry w danym momencie jest najlepszy lub 0, gdy wszystkie nast�pne mo�liwo�ci nieznane lub przegrywaj�ce
*/
int scoreCheck(Gamestate Game);

/** Funkcja zmieniaj�ca warto�� ruch�w kt�re wyst�pi�y w danej rozgrywce zgodnie z wynikiem (Wygrana +3, Remis +1, Przegrana -1)
@param Game Struktura gry, z kt�rej funkcja pobiera tablice AI, tablice moves, oraz ilo�� ruch�w, po kt�rym rozgrywka si� zako�czy�a
*/
void scoreUpdate(Gamestate &Game);

/** Funkcja inicjuj�ca rozgrywk�, pyta gracza o parametry gry i tworzy potrzebne tablice
@param Game Niezainicjowana struktura gry
*/
void initializeGamestate(Gamestate &Game);

/** Funkcja podczas kt�rej odbywa si� ruch gracza
@param Game Struktura gry, do kt�rej zapisany b�dzie ruch gracza
@return Struktura gry zaktualizowana o ruch gracza
*/
Gamestate playerTurn(Gamestate &Game);

/** Funkcja podczas kt�rej odbywa si� ruch komputera
@param Game Struktura gry, do kt�rej zapisany b�dzie ruch komputera
@return Struktura gry zaktualizowana o ruch komputera
*/
Gamestate CPUTurn(Gamestate &Game);

/** Funkcja kt�ra sprawdza, czy gra powinna si� zako�czy�, je�li tak, to ustawia zmienn� Winner w strukturze gry na odpowiedni� warto�� (3 dla remisu)
@param Game Struktura gry, do kt�rej b�dzie zapisane to, czy gra powinna si� sko�czy�
*/
void checkForEnd(Gamestate &Game);

/** Funkcja kt�ra sprawdza, czy gra w danym momencie posiada zwyci�zce, je�li tak, to ustawia zmienn� Winner w strukturze gry na odpowiedni� warto�� (1 dla Gracza, 2 dla Komputera)
@param Game Struktura gry, do kt�rej b�dzie zapisane to, czy gra powinna si� sko�czy�
@return Zwraca true, je�li gra ma zwyci�zce, false, je�li nie
*/
bool checkForWin(Gamestate &Game);

/** Funkcja kt�ra rysuje pole gry
@param Game Struktura gry, z kt�rej funkcja b�dzie pobiera� to jaki znak powinien si� pojawi� na jakim polu
*/
void drawBoard(Gamestate &Game);

/** Funkcja, w kt�rej zawarte s� wszystkie funkcje potrzebne do rozegrania pojedynczej gry
@param Game Struktura gry, na kt�rej program operuje
@return Funkcja zwraca struktur� gry w takim stanie, kt�ry pozwala na ponowne rozegranie gry
*/
Gamestate singleGame(Gamestate &Game);

/** Funkcja, kt�ra wypisuje statystyki gracza i komputera, oraz czy�ci wszystko, co by�o dynamicznie zaalokowane
@param Game Struktura gry, z kt�rej program uzyskuje dost�p do tablic i struktur do usuni�cia
*/
void sessionSummary(Gamestate &Game);

#endif
