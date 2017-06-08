// sdizo_p3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "plecak.h"

using namespace std;

void read_from_file(bool, plecak *);

int main(){
	int wybor = 0;
	plecak *nowy_plecak = new plecak(0,0);
	while (wybor > 4 || wybor < 1) {
		cout << "1. Problem plecakowy\n2.\n3. Wyjscie\n->";
		cin >> wybor;
	}
	if (wybor == 1) {
		read_from_file(true, nowy_plecak);
		// nowy_plecak->przedmioty.size() == 0 ?? to jebne wywolanie w read_from_file chwilowo
	}
	cin >> wybor;
    return 0;
}

void read_from_file(bool ktory_problem, plecak *nowy_plecak) {
	string fileName = "";
	fstream file;
	
	int pojemnosc, ilosc_przedmiotow, rozmiar, wartosc;

	cout << "Podaj nazwe pliku: ";
	cin >> fileName;
	file.open(fileName, ios::in);

	if (file.good()) {
		if (ktory_problem) { // problem plecakowy
			file >> pojemnosc >> ilosc_przedmiotow;
			nowy_plecak = new plecak(pojemnosc, ilosc_przedmiotow);
			for (int i = 0; i < ilosc_przedmiotow; i++) {
				file >> wartosc >> rozmiar;
				nowy_plecak->dodaj_przedmiot(rozmiar, wartosc);
			}
			nowy_plecak->zachlanny();
			nowy_plecak->dynamiczny();
		}
		else {} // drugi problem
	}
}