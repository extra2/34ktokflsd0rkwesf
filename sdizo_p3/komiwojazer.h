#pragma once
class komiwojazer
{
public:
	komiwojazer();
	komiwojazer(int);
	~komiwojazer();
	void zachlanny();
	void przeglad_zupelny(int);
	int ile_drog;
	int obliczono_max;
	int **drogi; // odleglosci miedzy konkretnymi drogami
	int *P; // tablica zawierajaca liczby 0...(ile_drog - 1)
	int *optymalna; // do przegladu zupelnego najlepsza kombinacja
	int globalna_suma; // do przegladu zupelnego
};