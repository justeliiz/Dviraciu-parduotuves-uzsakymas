#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
using namespace std;

bool tinkamaKategorija(string kategorija) {
    return kategorija == "Dviraciai" ||
           kategorija == "Dalys" ||
           kategorija == "Aksesuarai";
}

double skaiciuotiEilutesSuma(double kaina, int kiekis) {
    return kaina * kiekis;
}

double skaiciuotiNuolaida(double eilutesSuma, int kiekis) {
    if (kiekis >= 10) {
        return eilutesSuma * 0.10;
    }

    return 0.0;
}

int nuskaitytiDuomenis(string failoPavadinimas,
                       string kategorijos[],
                       string pavadinimai[],
                       double kainos[],
                       int kiekiai[],
                       int didziausiasKiekis) {
    ifstream failas(failoPavadinimas);

    if (!failas.is_open()) {
        cout << "Klaida: nepavyko atverti failo "
             << failoPavadinimas << ".\n";
        return -1;
    }

    int prekiuKiekis = 0;

    if (!(failas >> prekiuKiekis)) {
        cout << "Klaida: nepavyko nuskaityti prekiu skaiciaus.\n";
        return -1;
    }

    if (prekiuKiekis < 1 || prekiuKiekis > didziausiasKiekis) {
        cout << "Klaida: prekiu skaicius turi buti nuo 1 iki "
             << didziausiasKiekis << ".\n";
        return -1;
    }

    failas.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < prekiuKiekis; i++) {
        string kainosTekstas;
        string kiekioTekstas;

        if (!getline(failas, kategorijos[i], ';') ||
            !getline(failas, pavadinimai[i], ';') ||
            !getline(failas, kainosTekstas, ';') ||
            !getline(failas, kiekioTekstas)) {
            cout << "Klaida: nepavyko nuskaityti "
                 << i + 1 << " prekes iraso.\n";
            return -1;
        }

        stringstream kainosSrautas(kainosTekstas);
        stringstream kiekioSrautas(kiekioTekstas);

        if (!(kainosSrautas >> kainos[i]) ||
            !(kiekioSrautas >> kiekiai[i])) {
            cout << "Klaida: neteisinga "
                 << i + 1 << " prekes kaina arba kiekis.\n";
            return -1;
        }

        if (!tinkamaKategorija(kategorijos[i])) {
            cout << "Klaida: neteisinga "
                 << i + 1 << " prekes kategorija.\n";
            return -1;
        }

        if (pavadinimai[i].empty() || kainos[i] <= 0 || kiekiai[i] <= 0) {
            cout << "Klaida: neteisingi "
                 << i + 1 << " prekes duomenys.\n";
            return -1;
        }
    }

    return prekiuKiekis;
}

void rodytiVisasPrekes(string kategorijos[],
                       string pavadinimai[],
                       double kainos[],
                       int kiekiai[],
                       int prekiuKiekis) {
    cout << "\n--- VISOS UZSAKYMO PREKES ---\n";
    cout << left
         << setw(14) << "Kategorija"
         << setw(43) << "Pavadinimas"
         << right
         << setw(12) << "Kaina"
         << setw(9) << "Kiekis"
         << setw(14) << "Suma"
         << setw(14) << "Nuolaida"
         << setw(14) << "Moketi" << "\n";

    cout << string(120, '-') << "\n";

    cout << fixed << setprecision(2);

    for (int i = 0; i < prekiuKiekis; i++) {
        double eilutesSuma = skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
        double nuolaida = skaiciuotiNuolaida(eilutesSuma, kiekiai[i]);
        double moketi = eilutesSuma - nuolaida;

        cout << left
             << setw(14) << kategorijos[i]
             << setw(43) << pavadinimai[i]
             << right
             << setw(12) << kainos[i]
             << setw(9) << kiekiai[i]
             << setw(14) << eilutesSuma
             << setw(14) << nuolaida
             << setw(14) << moketi << "\n";
    }
}

void rodytiMeniu() {
    cout << "\n--- DVIRACIU PARDUOTUVES UZSAKYMAS ---\n";
    cout << "1. Perziureti visas uzsakymo prekes\n";
    cout << "2. Perziureti pasirinktos kategorijos prekes\n";
    cout << "3. Perziureti viso uzsakymo sumas\n";
    cout << "4. Perziureti sumas pagal kategorijas\n";
    cout << "5. Parodyti visa uzsakymo ataskaita\n";
    cout << "6. Issaugoti visa ataskaita i faila\n";
    cout << "7. Iseiti\n";
    cout << "Pasirinkite veiksma: ";
}

int main() {
    const int MAX_PREKES = 100;

    string kategorijos[MAX_PREKES];
    string pavadinimai[MAX_PREKES];
    double kainos[MAX_PREKES] = {};
    int kiekiai[MAX_PREKES] = {};

    int prekiuKiekis = nuskaitytiDuomenis("uzsakymas.txt",
                                          kategorijos,
                                          pavadinimai,
                                          kainos,
                                          kiekiai,
                                          MAX_PREKES);

    if (prekiuKiekis == -1) {
        return 1;
    }

    cout << "Duomenys sekmingai nuskaityti. Prekiu irasu skaicius: "
         << prekiuKiekis << ".\n";

    int pasirinkimas = 0;

    do {
        rodytiMeniu();

        if (!(cin >> pasirinkimas)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Klaida: reikia ivesti sveikaji skaiciu.\n";
            continue;
        }

        switch (pasirinkimas) {
            case 1:
                rodytiVisasPrekes(kategorijos,
                                  pavadinimai,
                                  kainos,
                                  kiekiai,
                                  prekiuKiekis);
                break;

            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                cout << "Sis meniu punktas bus pridetas kitame etape.\n";
                break;

            case 7:
                cout << "Programa baigiama.\n";
                break;

            default:
                cout << "Neteisingas pasirinkimas. Iveskite skaiciu nuo 1 iki 7.\n";
                break;
        }
    } while (pasirinkimas != 7);

    return 0;
}
