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

        kainosSrautas >> ws;
        kiekioSrautas >> ws;

        if (!kainosSrautas.eof() || !kiekioSrautas.eof()) {
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

void rodytiLentelesAntraste() {
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
}

void rodytiPrekesEilute(string kategorija,
                        string pavadinimas,
                        double kaina,
                        int kiekis) {
    double eilutesSuma = skaiciuotiEilutesSuma(kaina, kiekis);
    double nuolaida = skaiciuotiNuolaida(eilutesSuma, kiekis);
    double moketi = eilutesSuma - nuolaida;

    cout << left
         << setw(14) << kategorija
         << setw(43) << pavadinimas
         << right
         << setw(12) << kaina
         << setw(9) << kiekis
         << setw(14) << eilutesSuma
         << setw(14) << nuolaida
         << setw(14) << moketi << "\n";
}

void rodytiVisasPrekes(string kategorijos[],
                       string pavadinimai[],
                       double kainos[],
                       int kiekiai[],
                       int prekiuKiekis) {
    cout << "\n--- VISOS GALIMOS PREKES ---\n";
    cout << fixed << setprecision(2);
    rodytiLentelesAntraste();

    for (int i = 0; i < prekiuKiekis; i++) {
        rodytiPrekesEilute(kategorijos[i],
                          pavadinimai[i],
                          kainos[i],
                          kiekiai[i]);
    }
}

string pasirinktiKategorija() {
    int pasirinkimas = 0;

    while (true) {
        cout << "\nPasirinkite kategorija:\n";
        cout << "1. Dviraciai\n";
        cout << "2. Dalys\n";
        cout << "3. Aksesuarai\n";
        cout << "0. Grizti i pagrindini meniu\n";
        cout << "Jusu pasirinkimas: ";

        if (!(cin >> pasirinkimas)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Klaida: reikia ivesti sveikaji skaiciu.\n";
            continue;
        }

        switch (pasirinkimas) {
            case 1:
                return "Dviraciai";
            case 2:
                return "Dalys";
            case 3:
                return "Aksesuarai";
            case 0:
                return "";
            default:
                cout << "Neteisingas pasirinkimas. Iveskite skaiciu nuo 0 iki 3.\n";
        }
    }
}

void rodytiKategorijosPrekes(string pasirinktaKategorija,
                             string kategorijos[],
                             string pavadinimai[],
                             double kainos[],
                             int kiekiai[],
                             int prekiuKiekis) {
    cout << "\n--- KATEGORIJA: " << pasirinktaKategorija << " ---\n";
    cout << fixed << setprecision(2);
    rodytiLentelesAntraste();

    bool rasta = false;

    for (int i = 0; i < prekiuKiekis; i++) {
        if (kategorijos[i] == pasirinktaKategorija) {
            rodytiPrekesEilute(kategorijos[i],
                              pavadinimai[i],
                              kainos[i],
                              kiekiai[i]);
            rasta = true;
        }
    }

    if (!rasta) {
        cout << "Sioje kategorijoje prekiu nerasta.\n";
    }
}

double skaiciuotiBendraSuma(double kainos[],
                            int kiekiai[],
                            int prekiuKiekis) {
    double bendraSuma = 0.0;

    for (int i = 0; i < prekiuKiekis; i++) {
        bendraSuma += skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
    }

    return bendraSuma;
}

double skaiciuotiBendraNuolaida(double kainos[],
                                int kiekiai[],
                                int prekiuKiekis) {
    double bendraNuolaida = 0.0;

    for (int i = 0; i < prekiuKiekis; i++) {
        double eilutesSuma = skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
        bendraNuolaida += skaiciuotiNuolaida(eilutesSuma, kiekiai[i]);
    }

    return bendraNuolaida;
}

double skaiciuotiKategorijosSuma(string pasirinktaKategorija,
                                 string kategorijos[],
                                 double kainos[],
                                 int kiekiai[],
                                 int prekiuKiekis) {
    double kategorijosSuma = 0.0;

    for (int i = 0; i < prekiuKiekis; i++) {
        if (kategorijos[i] == pasirinktaKategorija) {
            double eilutesSuma = skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
            double nuolaida = skaiciuotiNuolaida(eilutesSuma, kiekiai[i]);
            kategorijosSuma += eilutesSuma - nuolaida;
        }
    }

    return kategorijosSuma;
}

void rodytiUzsakymoSumas(double kainos[],
                         int kiekiai[],
                         int prekiuKiekis) {
    double bendraSuma = skaiciuotiBendraSuma(kainos, kiekiai, prekiuKiekis);
    double bendraNuolaida = skaiciuotiBendraNuolaida(kainos, kiekiai, prekiuKiekis);
    double galutineSuma = bendraSuma - bendraNuolaida;

    cout << fixed << setprecision(2);
    cout << "\n--- VISO UZSAKYMO SUMOS ---\n";
    cout << "Suma be nuolaidos: " << bendraSuma << " Eur\n";
    cout << "Bendra nuolaida:   " << bendraNuolaida << " Eur\n";
    cout << "Galutine suma:     " << galutineSuma << " Eur\n";
}

void rodytiKategorijuSumas(string kategorijos[],
                           double kainos[],
                           int kiekiai[],
                           int prekiuKiekis) {
    cout << fixed << setprecision(2);
    cout << "\n--- SUMOS PAGAL KATEGORIJAS ---\n";
    cout << "Dviraciai:  "
         << skaiciuotiKategorijosSuma("Dviraciai", kategorijos, kainos, kiekiai, prekiuKiekis)
         << " Eur\n";
    cout << "Dalys:      "
         << skaiciuotiKategorijosSuma("Dalys", kategorijos, kainos, kiekiai, prekiuKiekis)
         << " Eur\n";
    cout << "Aksesuarai: "
         << skaiciuotiKategorijosSuma("Aksesuarai", kategorijos, kainos, kiekiai, prekiuKiekis)
         << " Eur\n";
}

void rodytiVisaAtaskaita(string kategorijos[],
                         string pavadinimai[],
                         double kainos[],
                         int kiekiai[],
                         int prekiuKiekis) {
    cout << "\n=============================================\n";
    cout << "      DVIRACIU PARDUOTUVES ATASKAITA\n";
    cout << "=============================================\n";

    rodytiVisasPrekes(kategorijos,
                      pavadinimai,
                      kainos,
                      kiekiai,
                      prekiuKiekis);

    rodytiUzsakymoSumas(kainos, kiekiai, prekiuKiekis);
    rodytiKategorijuSumas(kategorijos, kainos, kiekiai, prekiuKiekis);
}

bool issaugotiAtaskaita(string failoPavadinimas,
                        string kategorijos[],
                        string pavadinimai[],
                        double kainos[],
                        int kiekiai[],
                        int prekiuKiekis) {
    ofstream failas(failoPavadinimas);

    if (!failas.is_open()) {
        cout << "Klaida: nepavyko sukurti failo "
             << failoPavadinimas << ".\n";
        return false;
    }

    failas << fixed << setprecision(2);
    failas << "DVIRACIU PARDUOTUVES UZSAKYMO ATASKAITA\n\n";

    failas << left
           << setw(14) << "Kategorija"
           << setw(43) << "Pavadinimas"
           << right
           << setw(12) << "Kaina"
           << setw(9) << "Kiekis"
           << setw(14) << "Suma"
           << setw(14) << "Nuolaida"
           << setw(14) << "Moketi" << "\n";

    failas << string(120, '-') << "\n";

    for (int i = 0; i < prekiuKiekis; i++) {
        double eilutesSuma = skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
        double nuolaida = skaiciuotiNuolaida(eilutesSuma, kiekiai[i]);
        double moketi = eilutesSuma - nuolaida;

        failas << left
               << setw(14) << kategorijos[i]
               << setw(43) << pavadinimai[i]
               << right
               << setw(12) << kainos[i]
               << setw(9) << kiekiai[i]
               << setw(14) << eilutesSuma
               << setw(14) << nuolaida
               << setw(14) << moketi << "\n";
    }

    double bendraSuma = skaiciuotiBendraSuma(kainos, kiekiai, prekiuKiekis);
    double bendraNuolaida = skaiciuotiBendraNuolaida(kainos, kiekiai, prekiuKiekis);
    double galutineSuma = bendraSuma - bendraNuolaida;

    failas << "\nVISO UZSAKYMO SUMOS\n";
    failas << "Suma be nuolaidos: " << bendraSuma << " Eur\n";
    failas << "Bendra nuolaida:   " << bendraNuolaida << " Eur\n";
    failas << "Galutine suma:     " << galutineSuma << " Eur\n";

    failas << "\nSUMOS PAGAL KATEGORIJAS\n";
    failas << "Dviraciai:  "
           << skaiciuotiKategorijosSuma("Dviraciai", kategorijos, kainos, kiekiai, prekiuKiekis)
           << " Eur\n";
    failas << "Dalys:      "
           << skaiciuotiKategorijosSuma("Dalys", kategorijos, kainos, kiekiai, prekiuKiekis)
           << " Eur\n";
    failas << "Aksesuarai: "
           << skaiciuotiKategorijosSuma("Aksesuarai", kategorijos, kainos, kiekiai, prekiuKiekis)
           << " Eur\n";

    failas.close();

    if (!failas) {
        cout << "Klaida: nepavyko irasyti ataskaitos i faila "
             << failoPavadinimas << ".\n";
        return false;
    }

    return true;
}

void rodytiMeniu() {
    cout << "\n--- DVIRACIU PARDUOTUVES UZSAKYMAS ---\n";
    cout << "1. Perziureti visas galimas prekes\n";
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

            case 2: {
                string pasirinktaKategorija = pasirinktiKategorija();

                if (!pasirinktaKategorija.empty()) {
                    rodytiKategorijosPrekes(pasirinktaKategorija,
                                            kategorijos,
                                            pavadinimai,
                                            kainos,
                                            kiekiai,
                                            prekiuKiekis);

                    cout << fixed << setprecision(2);
                    cout << "Kategorijos moketina suma: "
                         << skaiciuotiKategorijosSuma(pasirinktaKategorija,
                                                     kategorijos,
                                                     kainos,
                                                     kiekiai,
                                                     prekiuKiekis)
                         << " Eur\n";
                }
                break;
            }

            case 3:
                rodytiUzsakymoSumas(kainos, kiekiai, prekiuKiekis);
                break;

            case 4:
                rodytiKategorijuSumas(kategorijos,
                                      kainos,
                                      kiekiai,
                                      prekiuKiekis);
                break;

            case 5:
                rodytiVisaAtaskaita(kategorijos,
                                    pavadinimai,
                                    kainos,
                                    kiekiai,
                                    prekiuKiekis);
                break;

            case 6:
                if (issaugotiAtaskaita("ataskaita.txt",
                                       kategorijos,
                                       pavadinimai,
                                       kainos,
                                       kiekiai,
                                       prekiuKiekis)) {
                    cout << "Ataskaita sekmingai issaugota faile ataskaita.txt.\n";
                }
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
