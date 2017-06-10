// sdizo_p3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono> // do pomiaru czasu
#include <time.h>
#include "plecak.h"
#include "komiwojazer.h"

using namespace std;

// do pomiaru czasu, zmienne i funkcje:
chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
void get_time_in_seconds_start(); // wywolaj przed wywolaniem funkcji
double get_time_in_seconds_stop(); // wywolaj po wywolaniu funkcji, zwraca wynik wykonania w [ms]

void dla_plecaka();
void dla_komiwojazera();
void testuj();

int main(){
	srand(time(NULL));
	int wybor = 0;
	while (wybor != 4) {
		while (wybor > 5 || wybor < 1) {
			cout << "1. Problem plecakowy\n2. Problem komiwojazera\n3. Testy\n4.Wyjscie\n->";
			cin >> wybor;
		}
		if (wybor == 1) {
			dla_plecaka();
		}
		if (wybor == 2) {
			dla_komiwojazera();
		}
		if (wybor == 3) {
			testuj();
		}
		wybor = -1;
	}
    return 0;
}

void dla_plecaka() {
	string fileName = "";
	fstream file;
	plecak *nowy_plecak = new plecak(0, 0);
	int pojemnosc, ilosc_przedmiotow, rozmiar, wartosc;

	cout << "Podaj nazwe pliku: ";
	cin >> fileName;
	file.open(fileName, ios::in);

	if (file.good()) {// wczytuje dane
			file >> pojemnosc >> ilosc_przedmiotow;
			nowy_plecak = new plecak(pojemnosc, ilosc_przedmiotow);
			for (int i = 0; i < ilosc_przedmiotow; i++) {
				file >> wartosc >> rozmiar;
				nowy_plecak->dodaj_przedmiot(rozmiar, wartosc);
			}
			int option = -1;
			while (option < 0 || option > 3) { // menu - ktory algorytm (wiem, slabe miejsce na menu, ale narazie niech zostanie)
				cout << "\n1.Algorytm zachlanny\n2.Algorytm - programowanie dynamiczne\n3.Wyjscie\n >";
				cin >> option;
			}
			if(option == 1)nowy_plecak->zachlanny();
			if(option == 2)nowy_plecak->dynamiczny();
	}
	delete nowy_plecak;
}

void dla_komiwojazera() {
	string fileName = "";
	fstream file;
	komiwojazer * komi;
	int ile_miast;
	cout << "Podaj nazwe pliku: ";
	cin >> fileName;
	file.open(fileName, ios::in);

	if (file.good()) {
		file >> ile_miast;
		komi = new komiwojazer(ile_miast);
		for (int i = 0; i < ile_miast; i++) { // wczytanie danych
			for (int j = 0; j < ile_miast; j++) {
				file >> komi->drogi[i][j];
			}
		}
		int option = -1;
		komi->P = new int[ile_miast];
		komi->optymalna = new int[ile_miast];
		komi->globalna_suma = -1;
		for (int i = 0; i < ile_miast; i++) komi->P[i] = i;
		while (option < 0 || option > 3) { // menu
			cout << "\n1.Algorytm zachlanny\n2.Przeglad zupelny\n3.Wyjscie\n >";
			cin >> option;
		}
		if (option == 1)komi->zachlanny();
		if (option == 2) {
			komi->przeglad_zupelny(ile_miast);
			// --------------- komiwojazer zupelny wypisanie -----------
			//cout << "\nKomiwojazer - przeglad zupelny:\nDroga: ";
			//for (int i = 0; i < ile_miast; i++) {
			//	cout << komi->optymalna[i] << " ";
			//}
			//cout << "\nSuma: " << komi->globalna_suma << endl;
		}
	}
	delete komi;
}

void testuj() {
	// -------------------------------------------- KOMIWOJAZER ---------------------------------------------------------
	komiwojazer * komi;
	int N, ile_powtorzen;
	double time_zachlanny = 0, time_zupelny = 0; // w double mam 10^12 max sekund, czyli raczej nie przeleci i mogę sumować w nim wyniki
	cout << "Ilosc miast: ";
	cin >> N;
	cout << "Ilosc powtorzen: ";
	cin >> ile_powtorzen;
	for (int powt = 0; powt < ile_powtorzen; powt++) {
		cout << (double)powt / (double)ile_powtorzen * 100 << "%\r"; // liczałka procentow (zeby bylo wiadomo na jakim etapie sie jest)
		komi = new komiwojazer(N);
		for (int k = 0; k < N; k++) {
			for (int i = k; i < N; i++) {
				komi->drogi[k][i] = rand() % (N * 10); // generuje dane (ustawiam max odleglosc na N * 10
				komi->drogi[i][k] = komi->drogi[k][i]; // tu wpisuje to samo co wyzej (macierz symetryczna)
				if (k == i) { // jetem na przekatnej, ustawiam -1
					komi->drogi[k][i] = -1;
				}
			}
		}
		//zachlanny
		get_time_in_seconds_start();
		komi->zachlanny();
		time_zachlanny += get_time_in_seconds_stop(); // TO DAJE W MS NIE W S!
		// ------ zupelny ------
		// najpierw inicjuje dane:
		komi->P = new int[N];
		komi->optymalna = new int[N];
		komi->globalna_suma = -1;
		for (int i = 0; i < N; i++) komi->P[i] = i;
		// teraz licze czas:
		get_time_in_seconds_start();
		komi->przeglad_zupelny(N);
		time_zupelny += get_time_in_seconds_stop();// TO DAJE W MS NIE W S!
		// ------ zupelny koniec ------
		delete komi;
	}
	cout << "\nZachlanny czas: " << time_zachlanny / (double)ile_powtorzen;
	cout << "\nprzeglad zupelny czas: " << time_zupelny / (double)ile_powtorzen << endl;
	// -------------------------------------------- KOMIWOJAZER END -----------------------------------------------------

	// ----------------------------------------------- PLACEK -----------------------------------------------------------
	/*
	 Badania należy wykonać dla 5 różnych (reprezentatywnych) liczb przedmiotów N. Dla każdej wartości N badania trzeba 
	 wykonać dla 3 różnych pojemności plecaka B
	*/
	int przedmioty[5], pojemnosci[3], ile_powt;
	double time_zachlanny_plecak , time_dynamiczny;
	plecak *plec;
	cout << endl;
	for (int i = 0; i < 5; i++) { // wczytaj ilosci przedmiotow
		cout << "Ile przedmiotow (" << i + 1 << "): ";
		cin >> przedmioty[i];
	}
	for (int i = 0; i < 3; i++) { // wczytaj pojemnosci plecaka
		cout << "Rozmiar plecaka (" << i + 1 << "): ";
		cin >> pojemnosci[i];
	}
	cout << "Ilosc powtorzen: ";
	cin >> ile_powt;
	for (int i = 0; i < 5; i++) { // dla kazdego przedmiotu
		for (int j = 0; j < 3; j++) { // i kazdej pojemnosci
			// --------- GENERATOR ------------
			time_zachlanny_plecak = 0;
			time_dynamiczny = 0;
			for (int k = 0; k < ile_powt; k++) { // tyle powtorzen
				bool repeat = true;
				while (repeat) {
					plec = new plecak(pojemnosci[j], przedmioty[i]);
					int suma = 0, rozmiar, wartosc;
					for (int k = 0; k < przedmioty[i]; k++) { // generuje przedmioty
						wartosc = rand() % pojemnosci[j];
						rozmiar = rand() % (pojemnosci[j] / 2);
						suma += rozmiar;
						plec->dodaj_przedmiot(wartosc, rozmiar);
					}
					if (suma <= pojemnosci[j]) { // suma wag mniejsza niz pojemnosc plecaka
						delete plec;
					}
					else repeat = false;
				}
				// dobra, mam wygenerowany plecak i przedmioty
				// ---------- POMIAR CZASU ----------
				get_time_in_seconds_start();
				plec->zachlanny();
				time_zachlanny_plecak += get_time_in_seconds_stop();

				get_time_in_seconds_start();
				plec->dynamiczny();
				time_dynamiczny += get_time_in_seconds_stop();
				
				delete plec;
			}
			cout << "\nZachlanny:\nPrzedmiotow: " << przedmioty[i] << ", pojemnosc: " << pojemnosci[j] << ", powtorzen: " << ile_powt << endl;
			cout << "Czas: " << time_zachlanny_plecak / (double)ile_powt << endl;
			cout << "\Dynamiczny:\nPrzedmiotow: " << przedmioty[i] << ", pojemnosc: " << pojemnosci[j] << ", powtorzen: " << ile_powt << endl;
			cout << "Czas: " << time_dynamiczny / (double)ile_powt << endl;
		}
	}
	cout << endl;
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