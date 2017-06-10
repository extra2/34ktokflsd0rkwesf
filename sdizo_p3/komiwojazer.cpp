#include "komiwojazer.h"
#include <iostream>

using namespace std;

komiwojazer::komiwojazer(){
	ile_drog = 0;
}

komiwojazer::komiwojazer(int ile_drog){ // inicjuje tablice 'drogi'
	drogi = new int*[ile_drog];
	for (int i = 0; i < ile_drog; i++) {
		drogi[i] = new int[ile_drog];
	}
	this->ile_drog = ile_drog;
}

komiwojazer::~komiwojazer(){
	for (int i = 0; i < ile_drog; i++) {
		delete drogi[i];
	}
	delete[] drogi;
	delete[] P;
	delete[] optymalna;
}

void komiwojazer::zachlanny() {
	// W każdym kroku idziemy do najbliższego nieodwiedzonego miasta.
	int *best_combination = new int[ile_drog];
	int *super_best_combination = new int[ile_drog];
	int megasuma = -1; // megasuma czyli suma najmniejsza ze wszystkich (w zaleznosci od wybranego rzedu)
	for (int x = 0; x < ile_drog; x++) { // tutaj jest petla ktora wybiera wiersz od ktorego zaczynam (wybieram kazdy, zaczynajac od pierwszego)
		int aktualny = x, aktualne_min = -1, index = 0, suma = 0;
		bool *czy_aktywny; // lista miast ktore odwiedzilem
		czy_aktywny = new bool[ile_drog];
		for (int i = 0; i < ile_drog; i++) czy_aktywny[i] = true;// ustawianie tablicy
		for (int j = 0; j < ile_drog; j++) { // dla kazdego wiersza
			// wyszukuje minimalny element w aktualnej tablicy
			best_combination[j] = aktualny;
			aktualne_min = -1;
			for (int i = 0; i < ile_drog; i++) { // dla kazdego pola w aktualnym wierszu
				if (aktualne_min == -1) { // jesli aktualne_min nie zostalo jeszcze wybrane (wynosi -1)
					if (drogi[aktualny][i] != -1 && czy_aktywny[i]) {
						aktualne_min = drogi[aktualny][i];
						index = i;
					}
					else continue; // trafilismy na samego siebie
				}
				else { // sprawdzamy, czy aktualna droga < aktualne_min
					if (drogi[aktualny][i] != -1 && drogi[aktualny][i] < aktualne_min && czy_aktywny[i]) {
						aktualne_min = drogi[aktualny][i];
						index = i;
					}
				}
			}
			if (j == x) czy_aktywny[x] = false; // dla zerowego elementu nalezy dac false (w nim zaczynam i do niego wracam, ale na samym koncu!)
			if (aktualne_min != -1) suma += aktualne_min;
			aktualny = index;
			czy_aktywny[index] = false;
			
			// cout << "\nA_min: " << aktualne_min << " s: " << suma;
		}
		suma += drogi[aktualny][x];
		if (megasuma == -1) {
			megasuma = suma;
			for(int i = 0; i < ile_drog; i++) super_best_combination[i] = best_combination[i];
		}
		else if (megasuma > suma) {
			megasuma = suma;
			for (int i = 0; i < ile_drog; i++) super_best_combination[i] = best_combination[i];
		}
		delete[] czy_aktywny;
	}
	/*cout << "\nAlgorytm zachlanny:\nDroga: ";
	for (int i = 0; i < ile_drog; i++) cout << super_best_combination[i] << " ";
	cout << endl << "Suma: " << megasuma << endl;*/
	delete[] best_combination;
	delete[] super_best_combination;
}

void komiwojazer::przeglad_zupelny(int k) { // bruteforce
	if (k == 1) { // sprawdzam rekurencyjnie wszystkie mozliwosci, jesli k == 1 to mamy konkretna mozliwosc
		int local_sum = 0;
		for (int i = 0; i < ile_drog-1; i++) { // przechodze z 0 na 1 itd, na koncu z indexu i=ile_drog - 1 przechodze na index i=0 (to juz poza petla)
			local_sum += drogi[P[i]][P[i+1]];
		}
		local_sum += drogi[P[ile_drog - 1]][P[0]]; // przechodze z ostatniej na pierwsza, dodaje koszt drogi
		if (globalna_suma == -1) {
			globalna_suma = local_sum;
			for (int i = 0; i < ile_drog; i++) optymalna[i] = P[i];
		}
		else if (local_sum < globalna_suma && globalna_suma > 0) {
			globalna_suma = local_sum;
			for (int i = 0; i < ile_drog; i++) optymalna[i] = P[i];
		}
	}
	else {
		for (int i = 0; i < k; i++) {
			// swapuje P[i] i P[k-1]
			int x = P[i];
			P[i] = P[k-1];
			P[k-1] = x;
			przeglad_zupelny(k - 1);
			// przywracam
			x = P[i];
			P[i] = P[k-1];
			P[k-1] = x;
		}
	}
}