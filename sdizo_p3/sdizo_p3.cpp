// sdizo_p3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono> // do pomiaru czasu
#include "plecak.h"

using namespace std;

// do pomiaru czasu, zmienne i funkcje:
chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
void get_time_in_seconds_start(); // wywolaj przed wywolaniem funkcji
double get_time_in_seconds_stop(); // wywolaj po wywolaniu funkcji, zwraca wynik wykonania w [ms]

void dla_plecaka(plecak *);
void testuj();

int main(){
	int wybor = 0;
	plecak *nowy_plecak = new plecak(0,0);
	while (wybor != 3) {
		while (wybor > 4 || wybor < 1) {
			cout << "1. Problem plecakowy\n2.\n3. Wyjscie\n->";
			cin >> wybor;
		}
		if (wybor == 1) {
			dla_plecaka(nowy_plecak);
		}
		if (wybor == 2) {
			//dla_komi();
		}
	}
	cin >> wybor;
    return 0;
}

void dla_plecaka(plecak *nowy_plecak) {
	string fileName = "";
	fstream file;
	
	int pojemnosc, ilosc_przedmiotow, rozmiar, wartosc;

	cout << "Podaj nazwe pliku: ";
	cin >> fileName;
	file.open(fileName, ios::in);

	if (file.good()) {
			file >> pojemnosc >> ilosc_przedmiotow;
			nowy_plecak = new plecak(pojemnosc, ilosc_przedmiotow);
			for (int i = 0; i < ilosc_przedmiotow; i++) {
				file >> wartosc >> rozmiar;
				nowy_plecak->dodaj_przedmiot(rozmiar, wartosc);
			}
			// wiem, ze nie powinno byc tu, ale narazie XD
			int option = -1;
			while (option < 0 || option > 3) {
				cout << "\n1.Algorytm zachlanny\n2.Algorytm - programowanie dynamiczne\n3.Wyjscie\n >";
				cin >> option;
			}
			if(option == 1)nowy_plecak->zachlanny();
			if(option == 2)nowy_plecak->dynamiczny();
	}
}

void testuj() {
	plecak *nowy_plecak = new plecak(0, 0);

}

void get_time_in_seconds_start() {
	t1 = chrono::high_resolution_clock::now();
	// liczę czas wykonania - start
}

double get_time_in_seconds_stop() {
	t2 = chrono::high_resolution_clock::now();
	// liczę czas wykonania - stop i zwracam różnicę w [ms]
	return (chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000.0);
}