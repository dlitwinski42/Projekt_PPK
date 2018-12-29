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

/** Funkcja porównuj¹ca dwie tablice stanów planszy.
@param Game Struktura gry, z której funkcja pobiera wielkoœæ planszy oraz obecny stan tablicy AI
@param board Tablica AI znajduj¹ca siê w danym wêŸle drzewa binarnego
@return wartoœæ -1,1,0 w zale¿noœci, czy tablica obecnego stanu gry mniejsza, wiêksza lub równa
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

/** Funkcja szukaj¹ca danego stanu planszy w drzewie binarnym, wykorzystuj¹ca funkcje compareBoards
@param root Korzeñ drzewa, z którego gra korzysta
@param Game Struktura gry, któr¹ funkcja przekazuje funkcji compareBoards
@return wskaŸnik na wêze³ drzewa zawieraj¹cy dan¹ tablice
*/
boardTree* find(boardTree * root, Gamestate Game);

/** Funkcja dodaj¹ca dany stanu planszy do drzewa binarnego, wykorzystuj¹ca funkcje compareBoards
@param root Korzeñ drzewa, z którego gra korzysta
@param Game Struktura gry, któr¹ funkcja przekazuje funkcji compareBoards
@return wskaŸnik na korzeñ drzewa
*/
boardTree* addToTree(boardTree* root, Gamestate Game);


/** Funkcja sczytuj¹ca drzewo binarne gry z odpowiedniego pliku
@param root wskaŸnik, do którego funkcja ma do³¹czyæ sczytane drzewo
@param Game Struktura gry, z której funkcja pobiera wielkoœæ planszy
@param f Strumien wejsciowy, za pomoca ktorego odczytujemy
@return wskaŸnik na pierwszy wêze³, który zosta³ do³¹czony
*/
boardTree* readFile(Gamestate &Game, ifstream &f);

/** Funkcja zapisuj¹ca drzewo binarne gry do odpowiedniego pliku
@param root Korzen zapisywanego drzewa
@param f Strumien wyjsciowy, za pomoca ktorego zapisujemy
*/
void writeFile(boardTree* root, int span, ofstream &f);

/** Funkcja sprawdzaj¹ca wartoœæ mo¿liwych ruchów w danym momencie 
@param Game Struktura gry, z której funkcja pobiera tablice AI
@return numer pola, który w danym momencie jest najlepszy lub 0, gdy wszystkie nastêpne mo¿liwoœci nieznane lub przegrywaj¹ce
*/
int scoreCheck(Gamestate Game);

/** Funkcja zmieniaj¹ca wartoœæ ruchów które wyst¹pi³y w danej rozgrywce zgodnie z wynikiem (Wygrana +3, Remis +1, Przegrana -1)
@param Game Struktura gry, z której funkcja pobiera tablice AI, tablice moves, oraz iloœæ ruchów, po którym rozgrywka siê zakoñczy³a
*/
void scoreUpdate(Gamestate &Game);

/** Funkcja inicjuj¹ca rozgrywkê, pyta gracza o parametry gry i tworzy potrzebne tablice
@param Game Niezainicjowana struktura gry
*/
void initializeGamestate(Gamestate &Game);

/** Funkcja podczas której odbywa siê ruch gracza
@param Game Struktura gry, do której zapisany bêdzie ruch gracza
@return Struktura gry zaktualizowana o ruch gracza
*/
Gamestate playerTurn(Gamestate &Game);

/** Funkcja podczas której odbywa siê ruch komputera
@param Game Struktura gry, do której zapisany bêdzie ruch komputera
@return Struktura gry zaktualizowana o ruch komputera
*/
Gamestate CPUTurn(Gamestate &Game);

/** Funkcja która sprawdza, czy gra powinna siê zakoñczyæ, jeœli tak, to ustawia zmienn¹ Winner w strukturze gry na odpowiedni¹ wartoœæ (3 dla remisu)
@param Game Struktura gry, do której bêdzie zapisane to, czy gra powinna siê skoñczyæ
*/
void checkForEnd(Gamestate &Game);

/** Funkcja która sprawdza, czy gra w danym momencie posiada zwyciêzce, jeœli tak, to ustawia zmienn¹ Winner w strukturze gry na odpowiedni¹ wartoœæ (1 dla Gracza, 2 dla Komputera)
@param Game Struktura gry, do której bêdzie zapisane to, czy gra powinna siê skoñczyæ
@return Zwraca true, jeœli gra ma zwyciêzce, false, jeœli nie
*/
bool checkForWin(Gamestate &Game);

/** Funkcja która rysuje pole gry
@param Game Struktura gry, z której funkcja bêdzie pobieraæ to jaki znak powinien siê pojawiæ na jakim polu
*/
void drawBoard(Gamestate &Game);

/** Funkcja, w której zawarte s¹ wszystkie funkcje potrzebne do rozegrania pojedynczej gry
@param Game Struktura gry, na której program operuje
@return Funkcja zwraca strukturê gry w takim stanie, który pozwala na ponowne rozegranie gry
*/
Gamestate singleGame(Gamestate &Game);

/** Funkcja, która wypisuje statystyki gracza i komputera, oraz czyœci wszystko, co by³o dynamicznie zaalokowane
@param Game Struktura gry, z której program uzyskuje dostêp do tablic i struktur do usuniêcia
*/
void sessionSummary(Gamestate &Game);

#endif
