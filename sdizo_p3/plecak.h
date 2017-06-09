#pragma once
#include <vector>

class przedmiot
{
public:
	przedmiot();
	przedmiot(int rozmiar, int wartosc);
	int wartosc;
	int rozmiar;
};

class plecak
{
public:
	plecak(int rozmiar, int ile_przedmiotow);
	void sprawdz_max_bruteforce();
	void zachlanny();
	void dynamiczny();
	void dodaj_przedmiot(int rozmiar, int wartosc);
	void sortuj();
	~plecak();
//private:
	int rozmiar;
	int ile_przedmiotow;
	int max_cena;
	std::vector <przedmiot> przedmioty, przedmioty_buffer, przedmioty_dla_max, przedmioty_rosnaco;
};