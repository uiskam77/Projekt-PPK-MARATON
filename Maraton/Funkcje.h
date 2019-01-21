#pragma once
#include <string>
#include <iostream>

using namespace std;

struct maraton;


/*Struktura maratonczyk tworzy liste maratonczykow. kazdy element posiada wskaznika na jego liste z wynikami z danego maratanu*/
struct maratonczyk
{
	string nazwisko;
	maraton *pMaraton;	//wskaznik na podliste
	maratonczyk *pNext;
};
/*Struktura maraton tworzy podliste maratonow oraz przechowuje czasy,
jakie uzyskal maratonczyk
*/
struct maraton
{
	string nazwa;
	string data;
	string czas;
	maraton *pNext;
};


/*Funckja odpowiada za budowe grafu - listy list*/
void budujGraf(maratonczyk *& pMaratonczyk, const string &nazwisko, const string &data, const string &nazwaMaratonu, const string &czas);


/*Funkcja odpowiada za znajdywanie maratonczyka w liscie lub w przypadku, gdy go nie ma - dodaje nowego maratonczyka do listy maratoñczyków*/
maratonczyk * znajdzLubDodajMaratonczyka(maratonczyk *&pMaratonczyk, const string &nazwisko);


/*Funkcja rekurencyjna odpowiada za dodanie maratonu do podlisty maratonow danego maratonczyka.
Funckja wstawia maratony posortowane, zeby pozniej bylo latwiej wypisac je do pliku wyjsciowego*/
void dodajMaraton(maraton *&pMaraton, const string &nazwaMaratonu, const string &data, const string &czas);


/*Funckja sprawdzajaca poprawnosc podanych argumentow w konsoli oraz
odczytujaca nazwy plikow z maratonami*/
bool sprawdzanie_argumentow(int argc, char **argv, string plikiMaratonowNazwy[]);


/*Funckja odczytuje dane z pliku i wysyla je do funckji budujGraf,
ktora tworzy z nich liste list*/
bool czytajDane(string nazwaPliku, maratonczyk *& pMaratonczyk);


/*Funckja odpowiada za wypisanie danych z listy list do pliku wyjsciowego*/
bool wypiszDoPliku(maratonczyk *&pMaratonczyk);


/*Funckja odpowada za usuniecie grafu - listy list*/
void usun(maratonczyk *&pMaratonczyk);
