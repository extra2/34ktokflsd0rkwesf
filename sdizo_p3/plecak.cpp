#include "plecak.h"
#include <iostream>

using namespace std;

int suma_wartosci(vector<przedmiot>);
int suma_rozmiarow(vector<przedmiot>);

plecak::plecak(int rozmiar, int ile_przedmiotow)
{
	this->rozmiar = rozmiar;
	this->ile_przedmiotow = ile_przedmiotow;
	max_cena = 0;
}

plecak::~plecak()
{
	przedmioty.empty();
	przedmioty_buffer.empty();
	przedmioty_dla_max.empty();
}

przedmiot::przedmiot()
{
}

przedmiot::przedmiot(int wartosc, int rozmiar)
{
	this->wartosc = wartosc;
	this->rozmiar = rozmiar;
}

void plecak::sprawdz_max_bruteforce() // chwilowo nieczynne, zapraszamy za jakis czas
{
	int suma = 0;
	for (int j = 0; j < ile_przedmiotow; j++)
	{
		przedmioty_buffer.clear();
		suma = 0;
		for (int i = j; i >= 0; i--)
		{
			if (przedmioty[i].wartosc == rozmiar)
			{
				przedmioty_buffer.push_back(przedmioty[i]);
			}
			if (przedmioty[i].wartosc > rozmiar) continue;
			if (suma + przedmioty[i].wartosc > rozmiar)
				continue;
			suma += przedmioty[i].wartosc;
			przedmioty_buffer.push_back(przedmioty[i]);
		}
		if (suma == rozmiar)
			break;
		if (/*suma poprzednia < suma nastepna*/true)
		{
			 //kopiowanie wektora                       
		}
	}
		/*if (suma != rozmiar)
		{
				cout << "Nie ma mozliwosci takiego spakowania elementow " << endl;
				cout << "Aby zapakowane elementy mialy najwieksza mase naleze spakowac elementy: ";
				iter(optymalny.size())
				cout << optymalny[i] << " ";
		}
		else
		{
				cout << "Istnieje taka  mozliwosc, nalezy spakowac elementy o masach: ";
				iter(mozliwosc.size())
			{
					cout << mozliwosc[i] << " ";
			}
		}*/

}

void plecak::zachlanny() { // dziala i nie dziala; bo tak ma byc, nie pokazuje najoptymalniejszego wyniku, ale zlozonosc czasowa jest niewielka
	sortuj(); // sortuje przedmioty, trafiaja do vectora przedmiotow przedmioty_rosnaco, znajdujacego sie w klasie plecak
	int aktualna_waga = 0;
	int max_wartosc = 0;
	for (int i = 0; i < ile_przedmiotow; i++) { // wybieram kolejne obiekty
		if (przedmioty_rosnaco[i].rozmiar + aktualna_waga <= rozmiar) { // i sprawdzam, czy zmieszcza sie w plecaku
			przedmioty_dla_max.push_back(przedmioty_rosnaco[i]);
			max_wartosc += przedmioty_rosnaco[i].wartosc;
			aktualna_waga += przedmioty_rosnaco[i].rozmiar;
		}
	}
	// --------------- wypisanie ---------------
	cout << "\nPrzedmioty: \n";
	for (unsigned int i = 0; i < przedmioty_dla_max.size(); i++) {
		cout << przedmioty_dla_max[i].rozmiar << " " << przedmioty_dla_max[i].wartosc << endl;
	}
	cout << "Waga = " << aktualna_waga << ", wartosc = " << max_wartosc << endl;
	// --------------- wypisanie end -----------
	przedmioty_rosnaco.empty();
	przedmioty_dla_max.empty();
}

void plecak::sortuj() { // sortowanie dla zachlannego
	przedmioty_rosnaco = przedmioty;
	for (unsigned int i = 0; i < przedmioty_rosnaco.size() - 1; i++) {
		for (unsigned int j = i + 1; j < przedmioty_rosnaco.size(); j++) {
			// rzutuje na double, gdyz na intach ucina precyzje i nie zawsze dokladnie sortuje
			if ((double)przedmioty_rosnaco[i].wartosc/(double)przedmioty_rosnaco[i].rozmiar < (double)przedmioty_rosnaco[j].wartosc/ (double)przedmioty_rosnaco[j].rozmiar) {
				przedmiot x = przedmioty_rosnaco[i];
				przedmioty_rosnaco[i] = przedmioty_rosnaco[j];
				przedmioty_rosnaco[j] = x;
			}
		}
	}
}

void plecak::dynamiczny() {
	// vector ktory przechowuje informacje o przedmiotach w 3 wymiarze
	// (tj: mam 2 wymiary: (ilosc przedmiotow) x (rozmiar) i 3 wymiar przechowuje przedmioty wykorzystywane w tym polu (jako zawartosc plecaka)
	// jest on potrzebny do wypisania listy przedmiotow, ktorych uzylem do danej konfiguracji (nadwyreza on nieco zlozonosc czasowa i obliczeniowa)
	// bez wykorzystania vectora mozna sprawdzic rozmiar (upakowanie) plecaka, ale nie wypisze wykorzystanych elementow
	vector <vector <vector <przedmiot>>> buff_przedm(ile_przedmiotow + 1); 
	
	for (int i = 0; i < ile_przedmiotow + 1; i++) 
		for (int j = 0; j < rozmiar + 1; j++)
		buff_przedm[i].push_back(vector<przedmiot>()); // dodaję 2 wymiar do vectora (tak, aby mozliwe bylo dodawanie przedmiotow do 3 wymiaru)
	
	int **A = new int*[ile_przedmiotow+1]; // przechowuje najoptymalniejszy WYNIK dla konktretnego rozmiaru przy wyborze n pierwszych elementow
	for (int i = 0; i < ile_przedmiotow+1; i++) A[i] = new int[rozmiar+1]; // robie dynamiczna tablice 2-wymiarowa

	// zeruje pierwszy wiersz oraz pierwsza kolumne:
	for (int i = 0; i < rozmiar+1; i++) A[0][i] = 0; 
	for (int i = 0; i < ile_przedmiotow+1; i++)A[i][0] = 0;

	for (int i = 1; i < ile_przedmiotow+1; i++) { // dla kazdego przedmiotu
		for (int j = 0; j < rozmiar+1; j++) { // dla rozmiaru plecaka (idac od rozmiaru 0)
			if (przedmioty[i-1].rozmiar > j) { // jesli rozmiar aktualnie sprawdzanego elementu nie zmiesci sie w danym rozmiarze plecaka
				A[i][j] = A[i - 1][j]; // kopiuje najlepszy wynik dla tego rozmiaru (dla poprzedniego przedmiotu)
				buff_przedm[i][j] = buff_przedm[i - 1][j]; // to samo, tylko dla vectora przedmiotow
			}
			else { // tu wybieram wieksza wartosc i przypisuje do A[i][j]
				if (A[i - 1][j] > A[i - 1][j - przedmioty[i - 1].rozmiar] + przedmioty[i - 1].wartosc) { // poprzedni wynik wiekszy
					A[i][j] = A[i - 1][j]; // kopiuje poprzedni wynik
					buff_przedm[i][j] = buff_przedm[i - 1][j];
				}
				else { // wynik po dodaniu nowego obiektu jest wiekszy
					A[i][j] = A[i - 1][j - przedmioty[i - 1].rozmiar] + przedmioty[i - 1].wartosc; // stare dane + nowy obiekt
					buff_przedm[i][j] = buff_przedm[i - 1][j - przedmioty[i - 1].rozmiar]; // kopiuje stare dane
					buff_przedm[i][j].push_back(przedmioty[i - 1]); // dodaje nowy obiekt do 3 wymiaru vectora
				}
			}
		}
	}
	// ---- wypisz: ----
	cout << "\nROZMIAR | WARTOSC\n";
	for (unsigned int k = 0; k < buff_przedm[ile_przedmiotow][rozmiar].size(); k++) {
			cout << buff_przedm[ile_przedmiotow][rozmiar][k].rozmiar << " " << buff_przedm[ile_przedmiotow][rozmiar][k].wartosc << endl;
		}
	cout << "Waga: " << suma_rozmiarow(buff_przedm[ile_przedmiotow][rozmiar]) << ", wartosc: " << suma_wartosci(buff_przedm[ile_przedmiotow][rozmiar]) << endl;
		// ---- wypisz end ----
	//sprzatam:
	for (int i = 0; i < ile_przedmiotow;  i++) {
		delete A[i];
		}
		delete[] A;
	buff_przedm.clear();
}

int suma_wartosci(vector<przedmiot> przedmioty_do_sumowania) {
	int suma = 0;
	for (unsigned int i = 0; i < przedmioty_do_sumowania.size(); i++) {
		suma += przedmioty_do_sumowania[i].wartosc;
	}
	return suma;
}

int suma_rozmiarow(vector<przedmiot> przedmioty_do_sumowania) {
	int suma = 0;
	for (unsigned int i = 0; i < przedmioty_do_sumowania.size(); i++) {
		suma += przedmioty_do_sumowania[i].rozmiar;
	}
	return suma;
}

void plecak::dodaj_przedmiot(int rozmiar, int wartosc)
{
	przedmioty.push_back(przedmiot(rozmiar, wartosc));
}