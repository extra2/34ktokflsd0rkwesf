#include "plecak.h"
#include <iostream>

using namespace std;

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
/*
wikipedia:
posortuj nierosnąco przedmioty według wartości c[j]/w[j]
aktualna_waga:=0

for i:=1 to n do
  if w[i] + aktualna_waga <= W then
    dodaj i-ty przedmiot do plecaka
    aktualna_waga := aktualna_waga + w[i]
	*/
	sortuj();
	int aktualna_waga = 0;
	int max_wartosc = 0;
	for (int i = 0; i < ile_przedmiotow; i++) {
		if (przedmioty_rosnaco[i].rozmiar + aktualna_waga <= rozmiar) {
			przedmioty_dla_max.push_back(przedmioty_rosnaco[i]);
			max_wartosc += przedmioty_rosnaco[i].wartosc;
			aktualna_waga += przedmioty_rosnaco[i].rozmiar;
		}
		//------ Generalnie algorytm nie zawsze pokazuje poprawny wynik, jesli uznamy, ze obiektem jest "substancja" 
		//------ tzn ze mozemy wziac "kawalek" obiektu (gdy sie nie zmiesci), trzeba odkomantowac ponizszy kod else. 
		//------ tylko to jest tzw: ciagly problem plecakowy, ale go nie rozwazamy
		/*
		else {
			int cena_jednostkowa = przedmioty_rosnaco[i].wartosc / przedmioty_rosnaco[i].rozmiar; // teoretycznie powinien byc double, ale dane maja byc ostatecznie w incie
			int pozostaly_rozmiar = rozmiar - aktualna_waga;
			przedmioty_dla_max.push_back(przedmiot(pozostaly_rozmiar, cena_jednostkowa*pozostaly_rozmiar));
			max_wartosc += cena_jednostkowa*pozostaly_rozmiar;
			aktualna_waga = rozmiar;
			break;
		}
		*/
	}
	// --------------- wypisanie ---------------
	cout << "\nPrzedmioty: \n";
	for (int i = 0; i < przedmioty_dla_max.size(); i++) {
		cout << przedmioty_dla_max[i].rozmiar << " " << przedmioty_dla_max[i].wartosc << endl;
	}
	cout << "Waga = " << aktualna_waga << ", wartosc = " << max_wartosc << endl;
	// --------------- wypisanie end -----------
	przedmioty_rosnaco.empty();
	przedmioty_dla_max.empty();
}

void plecak::sortuj() { // sortowanie dla zachlannego
	przedmioty_rosnaco = przedmioty;
	for (int i = 0; i < przedmioty_rosnaco.size() - 1; i++) {
		for (int j = i + 1; j < przedmioty_rosnaco.size(); j++) {
			// rzutuje na double, gdyz na intach ucina precyzje i nie zawsze dokladnie sortuje
			if ((double)przedmioty_rosnaco[i].wartosc/(double)przedmioty_rosnaco[i].rozmiar < (double)przedmioty_rosnaco[j].wartosc/ (double)przedmioty_rosnaco[j].rozmiar) {
				przedmiot x = przedmioty_rosnaco[i];
				przedmioty_rosnaco[i] = przedmioty_rosnaco[j];
				przedmioty_rosnaco[j] = x;
			}
		}
	}
	// testowo
	/*for (int i = 0; i < ile_przedmiotow; i++) {
		cout << (double)przedmioty_rosnaco[i].wartosc / (double)przedmioty_rosnaco[i].rozmiar << endl;
	}*/
}

void plecak::dynamiczny() {
	// -------- rozwiazanie wikipedia
	/*
	 for i:=0 to n do
    A[i,0]:= 0
  for j:=0 to W do
    A[0,j]:= 0

  for i:=1 to n do           //rozważanie kolejno i pierwszych przedmiotów
    for j:=0 to W do
      if ( w[i] > j ) then   //sprawdzenie czy i-ty element mieści się w plecaku o rozmiarze j
        A[i,j] = A[i-1,j]
      else
        A[i,j] =  max(A[i-1,j], A[i-1,j-w[i]] + c[i])
	*/

	// cos tu jest nie tak, ten algorytm tez nie dziala...
	// dobra on nie ma dzialac dokladnie
	// czyli jest ok, on znajduje wynik "okolo"
	// robie tabelke i (w pionie - numer przedmiotu) i j (w poziomie - rozmiar plecaka)
	int **A = new int*[ile_przedmiotow+1]; // przechowuje najoptymalniejszy WYNIK dla konktretnego rozmiaru przy wyborze n pierwszych elementow
	for (int i = 0; i < ile_przedmiotow+1; i++) A[i] = new int[rozmiar+1]; // robie dynamiczna tablice 2-wymiarowa

	for (int i = 0; i < rozmiar+1; i++) A[0][i] = 0; // zeruje, jak w algorytmie z wiki

	for (int i = 0; i < ile_przedmiotow+1; i++)A[i][0] = 0; // zeruje, jak w algorytmie z wiki

	for (int i = 1; i < ile_przedmiotow+1; i++) {
		for (int j = 0; j < rozmiar+1; j++) { // bez +1 do rozmiaru sprawdzalibysmy do rzeczywistego rozmiaru - 1
			if (przedmioty[i-1].rozmiar > j) {
				A[i][j] = A[i - 1][j]; // kopiuje sobie poprzedni wynik, bo nie zmiesci sie ten przedmiot
			}
			else { // tu wybieram wieksza wartosc i przypisuje do A[i][j] (wieksza wartosc - czy dodac nowy obiekt czy wynik dla poprzedniego przedmiotu byl wiekszy)
				if (A[i - 1][j] > A[i - 1][j - przedmioty[i-1].rozmiar] + przedmioty[i-1].wartosc) A[i][j] = A[i - 1][j];
				else A[i][j] = A[i - 1][j - przedmioty[i-1].rozmiar] + przedmioty[i-1].wartosc;
			}
		}
	}
	// wypisz:
	//cout << endl << endl << "A[ile_przedm - 1][rozmiar - 1] = " << A[ile_przedmiotow - 1][rozmiar - 1] << endl;
		for (int i = 1; i < ile_przedmiotow+1; i++) { // wypisanie danych - testowo
			for (int j = 0; j < rozmiar + 1; j++) {
				cout << i << " " << j << " " << A[i][j] << endl;
			}
		}
		for (int i = 0; i < ile_przedmiotow;  i++) {
			delete A[i];
		}
		delete[] A;
}
void plecak::dodaj_przedmiot(int rozmiar, int wartosc)
{
	przedmioty.push_back(przedmiot(rozmiar, wartosc));
}