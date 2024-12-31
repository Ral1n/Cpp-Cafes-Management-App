#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

/*
----------------
    CONTINUT
1. CLASA abstracta "Angajat"
2. CLASELE mostenite "Barista", "Manager", "Ospatar"
3. FUNCTII
- "stergeAngajat" -> Functie prin care se sterge un angajat deja existent din CSV
- "statusAngajati" -> Functie prin care se afiseaza statusului angajatilor fiecarei cafenele din tara
4. MAIN
- Meniul aplicatiei de gestionare al angajatilor
*/

/*
---------------------
    ABSTRACTIZARE
- Clasa Angajat este o clasa abstracta
- Va servi ca model pentru toate functiile angajatilor
- Se va folosi Factory Design Pattern pentru a crea functiile angajatilor
---------------------
*/
class Angajat
{
    /*
    -------------------
        INCAPSULARE
    - Unele dintre atributele clasei Angajat vor fi private (informatiile personale)
    -------------------
    */
private:
    string nume;
    string prenume;
    int varsta;
    string sex;
    int experienta; // ani
    int salariu;    // lei/luna

protected:
    /*
    -----------------------------
        CONSTRUCTOR protected
    - Se atribuie valori atributelor private ale clasei
    -----------------------------
    */
    Angajat(string nume, string prenume, int varsta, string sex, int experienta, int salariu)
    {
        this->nume = nume;
        this->prenume = prenume;
        this->varsta = varsta;
        this->sex = sex;
        this->experienta = experienta;
        this->salariu = salariu;
    }

public:
    int start_tura;   // ora la care incepe tura unui angajat
    int stop_tura;    // ora la care se termina tura unui angajat
    string zile_tura; // zilele in care angajatul va lucra

    /*
    ---------------
        GETTERE
    - Functii de get pentru toate atributele private
    ---------------
    */
    string getNume()
    {
        return this->nume;
    }

    string getPrenume()
    {
        return this->prenume;
    }

    int getVarsta()
    {
        return this->varsta;
    }

    string getSex()
    {
        return this->sex;
    }

    int getExperienta()
    {
        return this->experienta;
    }

    int getSalariu()
    {
        return this->salariu;
    }

    /*
    ------------------------
        FUNCTII VIRTUALE
    - Clasa abstracta contine o functie virtuala care va fi suprascrisa ulterior
    - Functia prin care se adauga un angajat in fisierul CSV
    ------------------------
    */
    virtual void adaugaAngajat(string oras) = 0;

    /*
    --------------------------
        DESTRUCTOR VIRTUAL
    --------------------------
    */
    virtual ~Angajat() = default;
};

// Functie prin care se sterge un angajat din fisierul CSV
void stergeAngajat(string nume, string prenume)
{
    fstream fin, fout;

    fin.open("angajati.csv", ios::in);

    fout.open("angajatinew.csv", ios::out);

    string last_name, first_name;
    bool deleted = false;

    string line, word;
    vector<string> row;

    // Copiem prima linie in fisierul nou
    getline(fin, line);
    row.clear();
    stringstream s(line);

    while (getline(s, word, ','))
    {
        row.push_back(word);
    }

    for (int i = 0; i < row.size() - 1; i++)
    {
        fout << row[i] << ",";
    }

    fout << row[row.size() - 1] << "\n";

    while (getline(fin, line))
    {
        row.clear();

        stringstream s(line);

        while (getline(s, word, ','))
        {
            row.push_back(word);
        }

        last_name = row[0];
        first_name = row[1];

        // Copiem, intr-un fisier nou, toti angajatii
        // in afara de cel care trebuie sa fie sters
        if (last_name != nume && first_name != prenume)
        {
            for (int i = 0; i < row.size() - 1; i++)
            {
                fout << row[i] << ",";
            }
            fout << row[row.size() - 1] << "\n";
        }
        else
        {
            deleted = true;
        }
    };

    fin.close();
    fout.close();

    remove("angajati.csv");

    rename("angajatinew.csv", "angajati.csv");
}

/*
------------------------------------------
    MOSTENIRE + FACTORY DESIGN PATTERN
- Clasele Barista, Manager si Ospatar mostenesc clasa abstracta Angajat
- Vor fi create folosind Factory Design Pattern
------------------------------------------
*/
class Barista : public Angajat
{
public:
    string task;

    Barista(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task)
        : Angajat(nume, prenume, varsta, sex, experienta, salariu)
    {
        this->start_tura = start_tura;
        this->stop_tura = stop_tura;
        this->zile_tura = zile_tura;
        this->task = task;
    }

    /*
    -------------------
        POLIMORFISM
    - Functia se suprascrie diferit in fiecare subclasa
    -------------------
    */
    void adaugaAngajat(string oras) override
    {
        fstream fout;

        fout.open("angajati.csv", ios::out | ios::app);

        fout << getNume() << ","
             << getPrenume() << ","
             << getVarsta() << ","
             << getSex() << ","
             << getExperienta() << ","
             << "Barista" << ","
             << oras << ","
             << getSalariu() << ","
             << this->start_tura << ","
             << this->stop_tura << ","
             << this->zile_tura << "\n";
    }
};

class Manager : public Angajat
{
public:
    string task;

    Manager(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task)
        : Angajat(nume, prenume, varsta, sex, experienta, salariu)
    {
        this->start_tura = start_tura;
        this->stop_tura = stop_tura;
        this->zile_tura = zile_tura;
        this->task = task;
    }

    void adaugaAngajat(string oras) override
    {
        fstream fout;

        fout.open("angajati.csv", ios::out | ios::app);

        fout << getNume() << ","
             << getPrenume() << ","
             << getVarsta() << ","
             << getSex() << ","
             << getExperienta() << ","
             << "Manager" << ","
             << oras << ","
             << getSalariu() << ","
             << this->start_tura << ","
             << this->stop_tura << ","
             << this->zile_tura << "\n";
    }
};

class Ospatar : public Angajat
{
public:
    string task;

    Ospatar(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task)
        : Angajat(nume, prenume, varsta, sex, experienta, salariu)
    {
        this->start_tura = start_tura;
        this->stop_tura = stop_tura;
        this->zile_tura = zile_tura;
        this->task = task;
    }

    void adaugaAngajat(string oras) override
    {
        fstream fout;

        fout.open("angajati.csv", ios::out | ios::app);

        fout << getNume() << ","
             << getPrenume() << ","
             << getVarsta() << ","
             << getSex() << ","
             << getExperienta() << ","
             << "Ospatar" << ","
             << oras << ","
             << getSalariu() << ","
             << this->start_tura << ","
             << this->stop_tura << ","
             << this->zile_tura << "\n";
    }
};

/*
----------------------------------------------
    CLASE specifice FACTORY DESIGN PATTERN
----------------------------------------------
*/
class AngajatFactory
{
public:
    virtual Angajat *createBarista(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task) = 0;
    virtual Angajat *createManager(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task) = 0;
    virtual Angajat *createOspatar(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task) = 0;
    virtual ~AngajatFactory() = default;
};

class BaristaFactory : public AngajatFactory
{
    Angajat *createBarista(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task) override
    {
        return new Barista(nume, prenume, varsta, sex, experienta, salariu, start_tura, stop_tura, zile_tura, task);
    }

    Angajat *createManager(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task) override
    {
        return nullptr;
    }

    Angajat *createOspatar(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task) override
    {
        return nullptr;
    }
};

class ManagerFactory : public AngajatFactory
{
    Angajat *createManager(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task) override
    {
        return new Manager(nume, prenume, varsta, sex, experienta, salariu, start_tura, stop_tura, zile_tura, task);
    }

    Angajat *createBarista(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task) override
    {
        return nullptr;
    }

    Angajat *createOspatar(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task) override
    {
        return nullptr;
    }
};

class OspatarFactory : public AngajatFactory
{
    Angajat *createOspatar(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task) override
    {
        return new Ospatar(nume, prenume, varsta, sex, experienta, salariu, start_tura, stop_tura, zile_tura, task);
    }

    Angajat *createBarista(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task) override
    {
        return nullptr;
    }

    Angajat *createManager(string nume, string prenume, int varsta, string sex, int experienta, int salariu, int start_tura, int stop_tura, string zile_tura, string task) override
    {
        return nullptr;
    }
};

// Functie prin care se afiseaza statusului angajatilor fiecarei cafenele din tara
// Cati angajati lipsa are cafeneaua conform schemei prezentate mai jos
void statusAngajati(string oras)
{
    fstream fin;

    fin.open("angajati.csv", ios::in);

    string oras_cafenea, functie;
    int start_tura;
    int count_manager = 0;
    int count_barista_tura1 = 0, count_ospatar_tura1 = 0;
    int count_barista_tura2 = 0, count_ospatar_tura2 = 0;

    vector<string> row;
    string line, word, temp;

    getline(fin, line);

    while (getline(fin, line))
    {
        row.clear();

        stringstream s(line);

        while (getline(s, word, ','))
        {
            row.push_back(word);
        }

        oras_cafenea = row[6];
        functie = row[5];
        start_tura = stoi(row[8]);

        if (oras_cafenea == oras)
        {
            if (functie == "Manager")
            {
                count_manager++;
            }
            else if (functie == "Barista" && start_tura == 9)
            {
                count_barista_tura1++;
            }
            else if (functie == "Barista" && start_tura == 13)
            {
                count_barista_tura2++;
            }
            else if (functie == "Ospatar" && start_tura == 9)
            {
                count_ospatar_tura1++;
            }
            else
            {
                count_ospatar_tura2++;
            }
        }
    }

    if (count_manager < 2)
    {
        cout << "Cafeneaua are nevoie de " << 2 - count_manager << " manageri!!!" << endl;
    }
    else
    {
        cout << "Cafeneaua are 2 manageri." << endl;
    }

    if (count_barista_tura1 < 2)
    {
        cout << "Cafeneaua are nevoie de " << 2 - count_barista_tura1 << " barista in intervalul orar 9:00-13:00!!!" << endl;
    }
    else
    {
        cout << "Cafeneaua are 2 barista in intervalul orar 9:00-13:00" << endl;
    }

    if (count_barista_tura2 < 2)
    {
        cout << "Cafeneaua are nevoie de " << 2 - count_barista_tura2 << " barista in intervalul orar 13:00-17:00!!!" << endl;
    }
    else
    {
        cout << "Cafeneaua are 2 barista in intervalul orar 13:00-17:00" << endl;
    }

    if (count_ospatar_tura1 < 2)
    {
        cout << "Cafeneaua are nevoie de " << 2 - count_ospatar_tura1 << " ospatari in intervalul orar 9:00-13:00!!!" << endl;
    }
    else
    {
        cout << "Cafeneaua are 2 ospatari in intervalul orar 9:00-13:00" << endl;
    }

    if (count_ospatar_tura2 < 2)
    {
        cout << "Cafeneaua are nevoie de " << 2 - count_ospatar_tura2 << " ospatari in intervalul orar 13:00-17:00!!!" << endl;
    }
    else
    {
        cout << "Cafeneaua are 2 ospatari in intervalul orar 13:00-17:00" << endl;
    }

    fin.close();
}

/*
------------
    MAIN
- Aici este meniul "aplicatiei" de gestionare al angajatilor
- Se alege orasul unde este situata cafeneaua
- Fiecare cafenea trebuie sa aiba:
    - 1 manager de cafenea
        - 1: Luni - Vineri: 9 - 17 -> 8000 lei/luna -> 50 lei/ora
    - 4 barista
        - 2: Luni, Miercuri, Vineri, Duminica: 9 - 19 -> 4900 lei/luna/barista -> 35 lei/ora/barista
        - 2: Marti, Joi, Sambata: 9 - 19 -> 4900 lei/luna/barista -> 35 lei/ora/barista
        - Saptamana urmatoare va fi invers => odata la 2 saptamani toti lucreaza 7 zile => 14 zile pe luna
    - 2 ospatari (am presupus ca multi clienti achizitioneaza cafea si pleaca, deci nu au nevoie de servire)
        - 1: Luni, Miercuri, Vineri, Duminica: 9 - 19 -> 3640 lei/luna -> 26 lei/ora
        - 1: Marti, Joi, Sambata: 9 - 19 -> 3640 lei/luna -> 26 lei/ora
        - Saptamana urmatoare va fi invers => odata la 2 saptamani toti lucreaza 7 zile => 14 zile pe luna
    => 1360 lei/zi salarile angajatilor
- Se afiseaza statusul fiecarei cafenele
    - Angajati lipsa
- Se afiseaza meniul de gestionare:
    - Adaugare angajat
    - Stergere angajat
    - Exit
- Se realizeaza actiunile
------------
*/
int main()
{
    cout << "---- GESTIONARE ANGAJATI ----" << endl;
    cout << "Bun venit in meniul de gestionare al angajatilor!" << endl;
    cout << "Din ce oras este cafeneaua dumneavoastra?" << endl;
    cout << "1. Bucuresti" << endl;
    cout << "2. Cluj-Napoca" << endl;
    cout << "3. Timisoara" << endl;
    cout << "4. Iasi" << endl;
    cout << "5. Brasov" << endl;

    int nr_oras;
    cin >> nr_oras;
    vector<string> orase = {"Bucuresti", "Cluj-Napoca", "Timisoara", "Iasi", "Brasov"};

    cout << "\n---- STATUS ANGAJATI ----" << endl;
    statusAngajati(orase[nr_oras - 1]);
    cout << "-------------------------\n"
         << endl;

    cout << "Ce doriti sa faceti?" << endl;
    cout << "1. Adaugare angajat in sistem" << endl;
    cout << "2. Stergere angajat din sistem" << endl;
    cout << "3. Iesire" << endl;

    int actiune;
    cin >> actiune;

    while (actiune != 3)
    {
        if (actiune == 1)
        {
            string nume, prenume, sex, zile_tura, task;
            int varsta, experienta, salariu, start_tura, stop_tura;

            cout << "Ati accesat meniul de adaugare a unui angajat in sistem." << endl;
            cout << "Ce functie va ocupa angajatul?" << endl;
            cout << "1. Manager" << endl;
            cout << "2. Barista" << endl;
            cout << "3. Ospatar" << endl;

            int tip_angajat;
            cin >> tip_angajat;

            cout << "Completati urmatoarele date referitoare la angajat" << endl;
            cout << "Nume: ";
            cin >> nume;
            cout << "Prenume: ";
            cin >> prenume;
            cout << "Varsta: ";
            cin >> varsta;
            cout << "Sex: ";
            cin >> sex;
            cout << "Experienta: ";
            cin >> experienta;
            cout << "Salariu brut lunar: ";
            cin >> salariu;
            cout << "Program: " << endl;
            cout << "Ora de incepere a turei: ";
            cin >> start_tura;
            cout << "Ora de sfarsire a turei: ";
            cin >> stop_tura;
            cout << "Zilele de lucru: ";
            getline(cin, zile_tura);
            cout << "Task zilnic: ";
            cin >> task;

            if (tip_angajat == 1)
            {
                AngajatFactory *managerFactory = new ManagerFactory();
                Angajat *manager = managerFactory->createManager(nume, prenume, varsta, sex, experienta, salariu, start_tura, stop_tura, zile_tura, task);
                manager->adaugaAngajat(orase[nr_oras - 1]);
            }
            else if (tip_angajat == 2)
            {
                AngajatFactory *baristaFactory = new BaristaFactory();
                Angajat *barista = baristaFactory->createBarista(nume, prenume, varsta, sex, experienta, salariu, start_tura, stop_tura, zile_tura, task);
                barista->adaugaAngajat(orase[nr_oras - 1]);
            }
            else
            {
                AngajatFactory *ospatarFactory = new OspatarFactory();
                Angajat *ospatar = ospatarFactory->createOspatar(nume, prenume, varsta, sex, experienta, salariu, start_tura, stop_tura, zile_tura, task);
                ospatar->adaugaAngajat(orase[nr_oras - 1]);
            }
        }
        else if (actiune == 2)
        {
            string nume, prenume;

            cout << "Ati accesat meniul de stergere a unui angajat din sistem." << endl;
            cout << "Ce functie ocupa angajatul?" << endl;
            cout << "1. Manager" << endl;
            cout << "2. Barista" << endl;
            cout << "3. Ospatar" << endl;

            int tip_angajat;
            cin >> tip_angajat;

            cout << "Care este numele angajatului?" << endl;
            cout << "Nume: ";
            cin >> nume;
            cout << "Prenume: ";
            cin >> prenume;

            stergeAngajat(nume, prenume);
        }

        cout << "Ce doriti sa faceti?" << endl;
        cout << "1. Adaugare angajat in sistem" << endl;
        cout << "2. Stergere angajat din sistem" << endl;
        cout << "3. Iesire" << endl;

        cin >> actiune;
    }
    return 0;
}