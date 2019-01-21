#include <iostream>
#include <string>
#include "Funkcje.h"

#include <stdlib.h>	//sprawdzanie wyciekow pamieci
#include <crtdbg.h>
#define _DEBUG 
#define _CRTDBG_MAP_ALLOC

using namespace std;


int main(const int argc, char **argv)
{

	string * plikiMaratonowNazwy = new string[argc - 2];	//tablica dynamiczna przechowujaca nazwy plikow maratonow
	//Tablica musi byc dyamiczna, poniewaz nie znamy poczatkowej liczby plikow wejsciowych
	//Rozmiar argc - 2 dlatego, ze pierwszy i drugi argument tablicy argv to Maraton.exe i przelacznik "-i:"

	//srpawdzamy, czy podane w konsoli argumenty sa poprawne
	if (!sprawdzanie_argumentow(argc, argv, plikiMaratonowNazwy))
	{
		cout << "Nie udalo sie poprawnie odczytac argumentow." << endl << endl;
		return 0;

	}


	maratonczyk *pHead = nullptr;	//deklaracja glowy listy maratonczykow


	//Iterujemy po wszyskich plikach maratonow
	for (int i = 0; i < argc - 2; i++)
	{
		czytajDane(plikiMaratonowNazwy[i], pHead);
	}


	//funkcja koncowa - wypisujemy dane z listy list do plikow
	maratonczyk *tmp = pHead;
	while (tmp)
	{
		wypiszDoPliku(tmp);
		tmp = tmp->pNext;
	}

	//usuwanie grafu i tablicy dynamicznej
	usun(pHead);
	delete[] plikiMaratonowNazwy;

	cout << endl << "Program wykonany pomyslnie - sprawdz wyniki!" << endl;
	_CrtDumpMemoryLeaks();	//detekcja wyciekow pamieci
	return 0;
}