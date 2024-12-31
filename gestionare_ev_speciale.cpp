#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>

using namespace std;

/*
----------------
    CONTINUT
1. CLASA "EvenimentSpecial"
2. CLASA "Date"
- Clasa pentru extragerea datei curente
3. MAIN
- Meniul aplicatiei de gestionare al evenimentelor speciale
*/

class EvenimentSpecial
{
    /*
    -------------------
        INCAPSULARE
    - Pretul evenimentului special va fi un atribut privat
    -------------------
    */
private:
    int pret; // Costul evenimentului

public:
    string eveniment; // Titlul evenimentului
    string data;      // Data in care va avea loc evenimentul
    float durata;     // Durata evenimentului in ore

    /*
   -------------------
       CONSTRUCTOR
   -------------------
   */
    EvenimentSpecial(string eveniment, string data, float durata)
    {
        this->eveniment = eveniment;
        this->data = data;
        this->durata = durata;
    }

    /*
    --------------
        SETTER
    --------------
    */
    void setPret(int pret)
    {
        this->pret = pret;
    }

    /*
    --------------
        GETTER
    --------------
    */
    int getPret()
    {
        return this->pret;
    }

    // Functie prin care se adauga evenimentul special in fisierul CSV
    void adaugaEvenimentLaCSV(string oras)
    {
        fstream fout;

        fout.open("evenimente_speciale.csv", ios::out | ios::app);

        fout << this->eveniment << ","
             << oras << ","
             << this->data << ","
             << this->durata << ","
             << getPret() << "\n";
    }

    /*
    ------------------
        DESTRUCTOR
    ------------------
    */
    ~EvenimentSpecial() {}
};

class Date
{
public:
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;

    Date()
    {
        time_t current_time = time(NULL);
        tm *ltm = localtime(&current_time);

        year = 1900 + ltm->tm_year;
        month = 1 + ltm->tm_mon;
        day = ltm->tm_mday;
        hour = ltm->tm_hour;
        min = ltm->tm_min;
        sec = ltm->tm_sec;
    }
};

/*
------------
    MAIN
- Aici este meniul "aplicatiei" de gestionare al evenimentelor speciale
- Se alege orasul unde este situata cafeneaua
- Se introduc datele referitoare la evenimentul special care va fi organizat in cafenea
- Se introduce evenimentul special in baza de date (fisierul CSV)
------------
*/
int main()
{
    cout << "--- ORGANIZARE EVENIMENTE SPECIALE ---" << endl;
    cout << "Bun venit in meniul de organizare a unui eveniment special!" << endl;
    cout << "Din ce oras este cafeneaua dumneavoastra?" << endl;
    cout << "1. Bucuresti" << endl;
    cout << "2. Cluj-Napoca" << endl;
    cout << "3. Timisoara" << endl;
    cout << "4. Iasi" << endl;
    cout << "5. Brasov" << endl;

    int nr_oras;
    cin >> nr_oras;
    vector<string> orase = {"Bucuresti", "Cluj-Napoca", "Timisoara", "Iasi", "Brasov"};

    cout << "\n---- CAFENEA " << orase[nr_oras - 1] << " ----" << endl;
    cout << "Introduceti datele referitoare la eveniment: " << endl;
    cout << "Titlu evniment: ";
    string eveniment;
    getline(cin >> ws, eveniment);
    cout << "Data in care evenimentul va avea loc: " << endl;
    cout << "Zi: ";
    int date_day;
    cin >> date_day;
    cout << "Luna: ";
    int date_month;
    cin >> date_month;
    cout << "An: ";
    int date_year;
    cin >> date_year;

    Date today;

    /*
    ------------------
        EXCEPTIONS
    - Format data eveniment invalida
    - Data eveniment in trecut
    ------------------
    */
    while (true)
    {
        try
        {
            if (date_day < 1 || date_day > 31 || date_month < 1 || date_month > 12)
            {
                throw "Format data incorect!";
            }

            if (date_year < today.year)
            {
                throw "Data in trecut!";
            }
            else if (date_month < today.month && date_year < today.year)
            {
                throw "Data in trecut!";
            }
            else if (date_day < today.day && date_month < today.month && date_year < today.year)
            {
                throw "Data in trecut!";
            }

            break;
        }
        catch (const char *msg)
        {
            cerr << msg << endl;
            cout << "Introduceti o data valida: ";
            cout << "Zi: ";
            cin >> date_day;
            cout << "Luna: ";
            cin >> date_month;
            cout << "An: ";
            cin >> date_year;
        }
    }

    string data = to_string(date_day) + "/" + to_string(date_month) + "/" + to_string(date_year);

    cout << "Durata evenimentului: ";
    float durata;
    cin >> durata;

    cout << "Costul evenimentului: ";
    int pret;
    cin >> pret;

    EvenimentSpecial evenimentSpecial(eveniment, data, durata);
    evenimentSpecial.setPret(pret);
    evenimentSpecial.adaugaEvenimentLaCSV(orase[nr_oras - 1]);

    cout << "\nIn data de " << data << " va avea loc in cafeneaua din " << orase[nr_oras - 1] << " un eveniment special!" << endl;
    cout << "---- " << eveniment << " ----" << endl;
    cout << "---- Durata: " << durata << "h" << " ----" << endl;

    return 0;
}