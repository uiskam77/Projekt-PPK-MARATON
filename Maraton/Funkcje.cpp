#include <iostream>
#include <string>
#include <fstream>
#include "Funkcje.h"

using namespace std;


void budujGraf(maratonczyk *& pMaratonczyk, const string &nazwisko, const string &data, const string &nazwaMaratonu, const string &czas);
maratonczyk * znajdzLubDodajMaratonczyka(maratonczyk *&pMaratonczyk, const string &nazwisko);
void dodajMaraton(maraton *&pMaraton, const string &nazwaMaratonu, const string &data, const string &czas);


/*Funckja sprawdzajaca poprawnosc podanych argumentow w konsoli oraz
odczytujaca nazwy plikow z maratonami*/
bool sprawdzanie_argumentow(int argc, char **argv, string plikiMaratonowNazwy[])
{
	if (argc > 1)
	{
		//funkcja strcmp() zwraca wartoœæ zero, gdy str1 jest takie same jak str2, mniejsz¹ od zero gdy str1 < str2 oraz wiêksz¹ od zero gdy str1 > str2.
		if (!strcmp(argv[1], "-i"))	//sprawdzamy, czy podano prawidlowy przelacznik
		{
			for (int i = 0; i < argc - 2; i++)	//pierwszy i drugi argument tablicy argv to Maraton.exe i przelacznik "-i:"
			{
				plikiMaratonowNazwy[i] = argv[i + 2];	//dodajemy do tablicy dynamicznej odczytane nazwy plikow maratonow
			}
		}
		else
		{
			cout << "Podano zly przelacznik" << endl;
			return false;
		}
	}
	else
	{
		cout << "Podano zbyt malo argumentow" << endl;
		return false;
	}

	return true;
}

/*Funckja odczytuje dane z pliku i wysyla je do funckji budujGraf,
ktora tworzy z nich liste list*/
bool czytajDane(string nazwaPliku, maratonczyk *& pMaratonczyk)
{
	ifstream plikMaratonu;
	plikMaratonu.open(nazwaPliku);
	if (plikMaratonu.is_open() && plikMaratonu.good());
	else
	{
		cout << "Nie udalo sie otworzyc pliku protokolu: " << nazwaPliku << endl;
		return false;
	}

	string nazwisko;
	string data;
	string nazwaMaratonu;
	string czas;
	string miejsce;
	string nrMaratonczyka;

	getline(plikMaratonu, nazwaMaratonu);	//Pobranie pierwszej linii pliku - nazwy maratonu
	getline(plikMaratonu, data);			//Pobranie drugiej linii pliku - daty odbycia maratonu


	/*
	Po poprzednich getline'ach "wskaznik lini" w pliku jest ustawiony na 3 linii
	dlatego od teraz mozna pobierac dane w prostszy sposob - strumieniowo

	Z wczytanych danych budujemy graf - liste list
	*/
	if ((nazwaMaratonu.length() != 0) && (data.length() != 0))
	{
		while (plikMaratonu >> miejsce >> nazwisko >> nrMaratonczyka >> czas)
		{
			budujGraf(pMaratonczyk, nazwisko.substr(0,nazwisko.size()-1), data, nazwaMaratonu, czas);
			//nazwisko.substr(0,nazwisko.size()-1) - w pliku wejsciowym nazwisko maratonczyka podane jest z ',' - funkcja substr() usuwa go (usuwa ostatni znak)
		}
	}



	plikMaratonu.close();
	return true;
}



/*Funckja odpowiada za budowe grafu - listy list*/
void budujGraf(maratonczyk *& pMaratonczyk, const string &nazwisko, const string &data, const string &nazwaMaratonu, const string &czas)
{
	maratonczyk *tmp = pMaratonczyk;

	//jesli nie ma jeszcze zadnego maratonczyka w liscie, to dodajemy nowego wraz z maratonem, w ktorym uczestniczyl
	if (!pMaratonczyk)
	{
		pMaratonczyk = new maratonczyk{ nazwisko, nullptr, nullptr };
		pMaratonczyk->pMaraton = new maraton{ nazwaMaratonu, data, czas, nullptr };

	}
	/*w innym wypadku nalezy sprawdzic, czy odczytywany maratonczyk juz istnieje. Jesli tak - znajdujemy go i dodajemy maraton do jego podlisty
	jesli nie - tworzymy nowego maratonczyka i dodajemy maraton do jego podlisty*/
	else
	{
		maratonczyk *tmpMaratonczyk = znajdzLubDodajMaratonczyka(tmp, nazwisko);
		dodajMaraton(tmpMaratonczyk->pMaraton, nazwaMaratonu, data, czas);
	}

}




/*Funkcja odpowiada za znajdywanie maratonczyka w liscie lub w przypadku, gdy go nie ma - dodaje nowego maratonczyka do listy maratonczykow*/
maratonczyk * znajdzLubDodajMaratonczyka(maratonczyk *&pMaratonczyk, const string &nazwisko)
{
	while (pMaratonczyk)	//dopoki istnieja elementy listy maratonczykow
	{
		//jesli nazwisko maratonczyka, ktory jest obecnie odczytywany z pliku pokrywa sie z nazwiskiem ktoregos elementu listy maratonczykow, znaczy to, ze mamy juz tego maratonczyka w naszej liscie
		if (pMaratonczyk->nazwisko == nazwisko)
		{
			return pMaratonczyk;
		}
		// jesli nie to dodajemy nowego maratonczyka do listy
		else
		{
			if (!pMaratonczyk->pNext)	//przechodzimy na koniec listy maratonczykow
			{
				pMaratonczyk->pNext = new maratonczyk{ nazwisko, nullptr, nullptr };	//dodajemy maratonczyka
				return pMaratonczyk->pNext;
			}
		}
		pMaratonczyk = pMaratonczyk->pNext;
	}
}

/*Funkcja rekurencyjna odpowiada za dodanie maratonu do podlisty maratonow danego maratonczyka.
Funckja wstawia maratony posortowane, zeby pozniej bylo latwiej wypisac je do pliku wyjsciowego*/
void dodajMaraton(maraton *&pMaraton, const string &nazwaMaratonu, const string &data, const string &czas)
{

	if (!pMaraton)	//jesli maratonczyk nie ma zadnego maratonu w podliscie lub przeszlismy do konca podlisty, to dodajemy nowy maraton
	{
		pMaraton = new maraton{ nazwaMaratonu, data, czas, nullptr };
	}
	else if (pMaraton->data > data) //dodajemy nowy element podlisty
	{
		pMaraton = new maraton{ nazwaMaratonu, data, czas, pMaraton };
	}
	else // przechodzimy do kolejnego elementu podlisty
	{
		dodajMaraton(pMaraton->pNext, nazwaMaratonu, data, czas);
	}
}


/*Funckja odpowiada za wypisanie danych z listy list do pliku wyjsciowego*/
bool wypiszDoPliku(maratonczyk *&pMaratonczyk)
{
	string nrString = pMaratonczyk->nazwisko + ".txt";	//wyciagmy nazwisko maratonczyka, aby moc utworzyc plik wynikowy o nazwie takiej samej jak jego nazwisko

	ofstream plikWynikowy;
	plikWynikowy.open(nrString);
	if (plikWynikowy.is_open() && plikWynikowy.good());
	else
	{
		cout << "Blad podczas otwierania pliku wynikowego maratonczyka o nazwisku: " << pMaratonczyk->nazwisko << endl;
		return false;
	}


	plikWynikowy << pMaratonczyk->nazwisko << endl << endl;

	maraton *tmpP = pMaratonczyk->pMaraton;

	while (tmpP)
	{
		plikWynikowy << "- " << tmpP->data << "\t - " << tmpP->nazwa << "\t - " << tmpP->czas << endl;

		tmpP = tmpP->pNext;
	}

	plikWynikowy.close();
	return true;
}



/*Funckja odpowiada za usuniecie grafu - listy list*/
void usun(maratonczyk *&pMaratonczyk)
{
	while (pMaratonczyk)
	{

		while (pMaratonczyk->pMaraton)
		{
			maraton *tmp = pMaratonczyk->pMaraton;
			pMaratonczyk->pMaraton = pMaratonczyk->pMaraton->pNext;
			delete tmp;
		}
		maratonczyk *tmp = pMaratonczyk;

		pMaratonczyk = pMaratonczyk->pNext;
		delete tmp;
	}
}

