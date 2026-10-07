#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>

using namespace std;

void wypisz_tablice(int * tab)
{
    for (int i = 0; i < 256; ++i)
    {
        cout << setw(6) << static_cast<int>(tab[i]) << " ";
        if (i % 10 == 9)
            cout << endl;
    }
}

void wypisz(unsigned char * tab)
{
    for (int i = 0; i < 256; ++i)
    {
        cout << setw(6) << static_cast<int>(tab[i]) << " ";
        if (i % 10 == 9)
            cout << endl;
    }
}

int main()
{
    ifstream plik;
    plik.open("muzg.pgm", ios::binary);    

    if (plik.is_open()) {
        cout << "Plik został otwart pomyślnie" << endl;
    } else {
        cout << "Plik NIE został otwart" << endl;
        return 1;
    }

    string format;
    int szerokosc, wysokosc, jasnosc;

    plik >> format;
    plik >> szerokosc >> wysokosc;
    plik >> jasnosc;
    plik.ignore();

    cout << "Format: " << format << endl;
    cout << "Szerokość: " << szerokosc << endl;
    cout << "Wysokość: " << wysokosc << endl;
    cout << "Jasność: " << jasnosc << endl;

    int histogram[256]{0};
    unsigned char **tab = new unsigned char*[wysokosc];
    for (int i = 0; i < wysokosc; i++) {
        tab[i] = new unsigned char[szerokosc];
    }

    for (int i = 0; i < wysokosc; i++) {
        for (int k = 0; k < szerokosc; k++) {
            unsigned char wartosc = plik.get();
            tab[i][k] = wartosc;
            histogram[wartosc]++;
        }
    }

    int dystrybuanta[256]{0};
    int suma = 0;
    for (int i = 0; i < 256; i++) {
        suma += histogram[i];
        dystrybuanta[i] = suma;
    }

    plik.close();

    cout << "HISTOGRAM: \n";
    wypisz_tablice(histogram);
    cout << "\n \n DYSTRYBUANTA: \n";
    wypisz_tablice(dystrybuanta);

    int min_dyst = 0;
    for (int i = 0; i < 256; ++i) {
        if (dystrybuanta[i] > 0) {
            min_dyst = dystrybuanta[i];
            break;
        }
    }

    cout << "\n\nMinimalna dystrybuanta: " << min_dyst << endl;

    unsigned char nowe_wartosci[256]{};
    int liczba_pikseli = szerokosc * wysokosc;
    for (int i = 0; i < 256; ++i) {
        nowe_wartosci[i] = static_cast<unsigned char>(
            round(255.0 * (dystrybuanta[i] - min_dyst) / (liczba_pikseli - min_dyst))
        );
    }

    cout << "\nNowe wartości:\n";
    wypisz(nowe_wartosci);

    ofstream plik_wyj("wyjscie.pgm", ios::binary);
    if (!plik_wyj) {
        cout << "Błąd otwarcia pliku do zapisu: wyjscie.pgm" << endl;
        return 1;
    }

    plik_wyj << format << "\n" << szerokosc << " " << wysokosc << "\n" << jasnosc << "\n";
    for (int b = 0; b < wysokosc; ++b) {
        for (int k = 0; k < szerokosc; ++k) {
            unsigned char nowa_wartosc = nowe_wartosci[tab[b][k]];
            plik_wyj.put(static_cast<char>(nowa_wartosc));
        }
    }
    plik_wyj.close();

    for (int i = 0; i < wysokosc; i++) {
        delete[] tab[i];
    }
    delete[] tab;

    return 0;
}
