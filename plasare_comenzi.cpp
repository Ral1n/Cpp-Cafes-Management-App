#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>

using namespace std;

/*
----------------
    CONTINUT
1. CLASA "Client"
2. FUNCTII
3. MAIN
- Meniul aplicatiei de plasare al unei comenzi
*/

class Client
{
    /*
    -------------------
        INCAPSULARE
    - Numele si prenumele clientului vor fi private
    -------------------
    */
private:
    string nume;
    string prenume;

public:
    /*
    --------------
        SETTER
    --------------
    */
    void setClient(string nume, string prenume)
    {
        this->nume = nume;
        this->prenume = prenume;
    }

    /*
    ---------------
        GETTERE
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

    // Functie prin care se adauga o comanda realizata de client in CSV
    void adaugaClientSiComandaInCSV(vector<string> comanda, float pret_comanda)
    {
        fstream fout;

        fout.open("comenzi_si_clienti.csv", ios::out | ios::app);

        time_t timestamp;
        time(&timestamp);

        fout << getNume() << ","
             << getPrenume() << ",";

        for (int i = 0; i < comanda.size(); i++)
        {
            fout << comanda[i] << " ";
        }

        fout << ","
             << pret_comanda << ","
             << ctime(&timestamp) << "\n";
    }

    // Functie care returneaza numarul de comenzi efectuate de respectivul client
    // si care afiseaza numarul de comenzi pana la urmatoarea reducere
    int nrComenziReducere()
    {
        fstream fin;

        fin.open("comenzi_si_clienti.csv", ios::in);

        string nume, prenume;

        string line, word;
        vector<string> row;
        getline(fin, line);

        int count = 0;

        while (getline(fin, line))
        {
            row.clear();

            stringstream s(line);

            while (getline(s, word, ','))
            {
                row.push_back(word);
            }

            nume = row[0];
            prenume = row[1];

            if (nume == getNume() && prenume == getPrenume())
            {
                count++;
            }
        };

        if (count < 5)
        {
            cout << "\nEfectuati inca " << 5 - count << " comenzi si ve-ti beneficia de 5% reducere la toate produsele noastre!" << endl;
        }
        else if (count >= 5 && count < 10)
        {
            cout << "\n-> Reducere aplicata: 5%" << endl;
            cout << "\nEfectuati inca " << 10 - count << " comenzi si ve-ti beneficia de 7.5% reducere la toate produsele noastre!" << endl;
        }
        else if (count >= 10 && count < 20)
        {
            cout << "\n-> Reducere aplicata: 7.5%" << endl;
            cout << "\nEfectuati inca " << 20 - count << " comenzi si ve-ti beneficia de 10% reducere la toate produsele noastre!" << endl;
        }
        else
        {
            cout << "\n-> Reducere aplicata: 10%" << endl;
            cout << "\nA 50-a comanda a dumneavoastra in cafeneaua noastra va fi gratis! (" << 50 - count << " ramase)" << endl;
        }

        fin.close();

        return count;
    }

    /*
    ------------------
        DESTRUCTOR
    ------------------
    */
    ~Client() {}
};

/*
---------------
    FUNCTII
- "showMenu" -> Functie prin care se afiseaza meniul pentru un anume tip de produs din cafenea
- "plasareComanda" -> Functie prin care se plaseaza o comanda a unui produs din cafenea
                   -> Return type-ul este int deoarece pe langa mesajele afisate,
                      functia returneaza pretul produsului comandat (x cantitate)
- "plasareProdus" -> Functie prin care se returneaza, sub forma de string, cantitatea, marimea si numele unui produs comandat
- "modificareStoc" -> Functie prin care se modifica in CSV stocul unui produs imediat ce acesta a fost comandat
- "adaugareStoc" -> Functie prin care se adauga in CSV o cantitate la stocul produsului selectat
- "startZi" -> Functie prin care se initializeaza o zi calendaristica in CSV
- "verificareStartZi" -> Se verifica daca s-a facut deja initializarea zilei calendaristice
- "adaugareVenitCheltuieliProfituri" -> Se adauga in fisierul CSV urmatoarele:
        -> Venitul in urma unei comenzi (pretul intreg al comenzii)
        -> Cheltuielile de productie ale unei comenzi
        -> Profitul obtinut in urma unei comenzi (venit - cheltuieli de productie)
---------------
*/

void showMenu(string TIP, string limba)
{
    fstream fin;

    if (limba == "Romana")
    {
        fin.open("produse_ro.csv", ios::in);
    }
    else
    {
        fin.open("produse_eng.csv", ios::in);
    }

    int ID;
    string produs, tip;
    string marime;
    string pret, stoc;

    string line, word;
    vector<string> row;
    getline(fin, line);

    while (getline(fin, line))
    {
        row.clear();

        stringstream s(line);

        while (getline(s, word, ','))
        {
            row.push_back(word);
        }

        ID = stoi(row[0]);
        produs = row[1];
        tip = row[2];
        marime = row[3];
        pret = row[4];
        stoc = row[5];

        vector<string> marimi;
        vector<int> preturi, stocuri;
        string str;
        int nr1, nr2;

        istringstream iss1(marime);
        istringstream iss2(pret);
        istringstream iss3(stoc);

        while (iss1 >> str)
        {
            marimi.push_back(str);
        }

        while (iss2 >> nr1)
        {
            preturi.push_back(nr1);
        }

        while (iss3 >> nr2)
        {
            stocuri.push_back(nr2);
        }

        if (tip == TIP)
        {
            cout << ID << ". " << produs << " - ";

            for (int i = 0; i < marimi.size(); i++)
            {
                if (stocuri[i] > 0)
                {
                    cout << marimi[i] << "(" << preturi[i] << " lei) ";
                }
                else
                {
                    cout << " STOC EPUIZAT ";
                }
            }

            cout << endl;
        }
    };

    fin.close();
}

int plasareComanda(string limba, string TIP, int ID, string MARIME, int cantitate)
{
    fstream fin;

    if (limba == "Romana")
    {
        fin.open("produse_ro.csv", ios::in);
    }
    else
    {
        fin.open("produse_eng.csv", ios::in);
    }

    int id;
    string produs, tip;
    string marime;
    string pret, stoc;

    string line, word;
    vector<string> row;
    getline(fin, line);

    cout << "Ati adaugat pe lista: ";

    while (getline(fin, line))
    {
        row.clear();

        stringstream s(line);

        while (getline(s, word, ','))
        {
            row.push_back(word);
        }

        id = stoi(row[0]);
        produs = row[1];
        tip = row[2];
        marime = row[3];
        pret = row[4];
        stoc = row[5];

        vector<string> marimi;
        vector<int> preturi, stocuri;
        string str;
        int nr1, nr2;

        istringstream iss1(marime);
        istringstream iss2(pret);
        istringstream iss3(stoc);

        while (iss1 >> str)
        {
            marimi.push_back(str);
        }

        while (iss2 >> nr1)
        {
            preturi.push_back(nr1);
        }

        while (iss3 >> nr2)
        {
            stocuri.push_back(nr2);
        }

        if (tip == TIP)
        {
            if (id == ID)
            {
                for (int i = 0; i < marimi.size(); i++)
                {
                    if (marimi[i] == MARIME)
                    {
                        if (stocuri[i] >= cantitate)
                        {
                            cout << cantitate << "x " << MARIME << " " << produs << " (" << cantitate * preturi[i] << " lei)";
                            return cantitate * preturi[i];
                        }
                        else
                        {
                            cout << "Ne pare rau! " << cantitate << "x " << MARIME << " " << produs << " nu se afla in stoc!";
                        }
                    }
                }

                cout << endl;
            }
        }
    };

    fin.close();

    return 0;
}

string plasareProdus(string limba, string TIP, int ID, string MARIME, int cantitate)
{
    fstream fin;

    if (limba == "Romana")
    {
        fin.open("produse_ro.csv", ios::in);
    }
    else
    {
        fin.open("produse_eng.csv", ios::in);
    }

    int id;
    string produs, tip;
    string marime;

    string line, word;
    vector<string> row;
    getline(fin, line);

    while (getline(fin, line))
    {
        row.clear();

        stringstream s(line);

        while (getline(s, word, ','))
        {
            row.push_back(word);
        }

        id = stoi(row[0]);
        produs = row[1];
        tip = row[2];
        marime = row[3];

        vector<string> marimi;
        string str;

        istringstream iss(marime);

        while (iss >> str)
        {
            marimi.push_back(str);
        }

        if (tip == TIP)
        {
            if (id == ID)
            {
                for (int i = 0; i < marimi.size(); i++)
                {
                    if (marimi[i] == MARIME)
                    {
                        string comanda = to_string(cantitate) + "x " + marime + " " + produs;
                        return comanda;
                    }
                }
            }
        }
    };

    fin.close();
    return 0;
}

void modificareStoc(string TIP, int ID, string MARIME, int cantitate)
{
    fstream fin, fout;

    fin.open("produse_ro.csv", ios::in);

    fout.open("produsenew.csv", ios::out);

    int id;
    string produs, tip;
    string marime;
    string pret, stoc;

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

        id = stoi(row[0]);
        produs = row[1];
        tip = row[2];
        marime = row[3];
        pret = row[4];
        stoc = row[5];

        vector<string> marimi;
        vector<int> stocuri;
        string str;
        int nr;

        istringstream iss1(marime);
        istringstream iss2(stoc);

        while (iss1 >> str)
        {
            marimi.push_back(str);
        }

        while (iss2 >> nr)
        {
            stocuri.push_back(nr);
        }

        if (tip == TIP && id == ID)
        {
            // Modificam stocul pentru produsul comandat
            for (int i = 0; i < marimi.size(); i++)
            {
                if (marimi[i] == MARIME)
                {
                    stocuri[i] -= cantitate;
                }
            }

            ostringstream oss;

            for (int i = 0; i < stocuri.size(); i++)
            {
                oss << stocuri[i] << " ";
            }

            // Copiem linia modificata in fisierul nou
            for (int i = 0; i < row.size() - 1; i++)
            {
                fout << row[i] << ",";
            }
            fout << oss.str() << "\n";
        }
        else
        {
            // Copiem liniile nemodificate in fisierul nou
            for (int i = 0; i < row.size() - 1; i++)
            {
                fout << row[i] << ",";
            }
            fout << row[row.size() - 1] << "\n";
        }
    };

    fin.close();
    fout.close();

    remove("produse_ro.csv");

    rename("produsenew.csv", "produse_ro.csv");
}

/*
Codul este la fel ca la functia de "modificareStoc"
doar ca in loc sa eliminam produse din stoc (-= cantitate),
adaugam o cantitate la stocul produsului selectat (+= cantitate)
*/
void adaugareStoc(string TIP, int ID, string MARIME, int cantitate)
{
    fstream fin, fout;

    fin.open("produse_ro.csv", ios::in);

    fout.open("produsenew.csv", ios::out);

    int id;
    string produs, tip;
    string marime;
    string pret, stoc;

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

        id = stoi(row[0]);
        produs = row[1];
        tip = row[2];
        marime = row[3];
        pret = row[4];
        stoc = row[5];

        vector<string> marimi;
        vector<int> stocuri;
        string str;
        int nr;

        istringstream iss1(marime);
        istringstream iss2(stoc);

        while (iss1 >> str)
        {
            marimi.push_back(str);
        }

        while (iss2 >> nr)
        {
            stocuri.push_back(nr);
        }

        if (tip == TIP && id == ID)
        {
            // Adaugam o cantitate la stocul produsului selectat
            for (int i = 0; i < marimi.size(); i++)
            {
                if (marimi[i] == MARIME)
                {
                    stocuri[i] += cantitate;
                }
            }

            ostringstream oss;

            for (int i = 0; i < stocuri.size(); i++)
            {
                oss << stocuri[i] << " ";
            }

            // Copiem linia modificata in fisierul nou
            for (int i = 0; i < row.size() - 1; i++)
            {
                fout << row[i] << ",";
            }
            fout << oss.str() << "\n";
        }
        else
        {
            // Copiem liniile nemodificate in fisierul nou
            for (int i = 0; i < row.size() - 1; i++)
            {
                fout << row[i] << ",";
            }
            fout << row[row.size() - 1] << "\n";
        }
    };

    fin.close();
    fout.close();

    remove("produse_ro.csv");

    rename("produsenew.csv", "produse_ro.csv");
}

/*
Prin aceasta functie se adauga in CSV la inceputul fiecarei zile:
Data zilei, 0 Venituri, 0 Cost produse, 1360 Salarii/zi, 100 Chirie/zi, 50 Intretinere/zi, -1510 Profituri
    - Am calculat salariile angajatilor pe zi
    - Am presupus chiria fiecarei locatii de 3000 lei/luna
    - Am presupus cheltuielile pe intretinere pt. fiecare locatie de 1500 lei/luna
*/
void startZi(char data[], string oras)
{
    fstream fout;

    fout.open("rapoarte_financiare.csv", ios::out | ios::app);

    fout << data << ","
         << oras << ","
         << 0 << ","
         << 0 << ","
         << 1360 << ","
         << 100 << ","
         << 50 << ","
         << -1510 << "\n";
}

/*
Prin aceasta functie se verifica daca datele specifice inceputului unei zi
au fost adaugate deja in fisierul CSV
*/
bool verificareStartZi(char data[])
{
    fstream fin;

    fin.open("rapoarte_financiare.csv", ios::in);

    string date;

    string line, word;
    vector<string> row;
    getline(fin, line);

    while (getline(fin, line))
    {
        row.clear();

        stringstream s(line);

        while (getline(s, word, ','))
        {
            row.push_back(word);
        }

        date = row[0];

        if (date == data)
        {
            return true;
        }
    };

    fin.close();

    return false;
}

void adaugareVenitCheltuieliProfituri(char DATA[], string ORAS, float venit, float cost)
{
    fstream fin, fout;

    fin.open("rapoarte_financiare.csv", ios::in);

    fout.open("rapoarte.csv", ios::out);

    string data;
    string oras;
    float venituri;
    float cost_productie;
    float profituri;

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

        data = row[0];
        oras = row[1];
        venituri = stof(row[2]);
        cost_productie = stof(row[3]);
        profituri = stof(row[7]);

        if (data == DATA && oras == ORAS)
        {
            // Modificam veniturile si cheltuielile
            venituri += venit;
            cost_productie += cost;
            profituri += (venituri - cost_productie);

            // Copiem linia modificata in fisierul nou
            for (int i = 0; i < row.size() - 1; i++)
            {
                if (i == 2)
                {
                    fout << venituri << ",";
                }
                else if (i == 3)
                {
                    fout << cost_productie << ",";
                }
                else
                {
                    fout << row[i] << ",";
                }
            }
            fout << profituri << "\n";
        }
        else
        {
            // Copiem liniile nemodificate in fisierul nou
            for (int i = 0; i < row.size() - 1; i++)
            {
                fout << row[i] << ",";
            }
            fout << row[row.size() - 1] << "\n";
        }
    };

    fin.close();
    fout.close();

    remove("rapoarte_financiare.csv");

    rename("rapoarte.csv", "rapoarte_financiare.csv");
}

/*
------------
    MAIN
- Aici este meniul "aplicatiei" de plasare a unei comenzi de catre un client
- Se alege orasul unde se afla clientul
- Se cer numele si prenumele clientului pentru a fi introduse in baza de date (fisierul CSV),
ca ulterior sa se verifice fidelitatea acestuia pentru a se aplica reducerea corespunzatoare:
    - sub 5 comenzi -> 0% reducere
    - intre 5 si 10 comenzi -> 5% reducere
    - intre 10 si 20 comenzi -> 7.5% reducere
    - peste 20 de comenzi -> 10% reducere
    - a 50-a comanda -> gratis
- Se afiseaza meniul pentru tipul de produs selectat si se comanda
- Se afiseaza pretul comenzii dupa fiecare produs adaugat pe lista
- Dupa finalizarea comenzii, pretul total este adaugat in fisierul CSV la categoria "Venituri"
- Costul de productie al fiecarei bauturi este de jumatate din pretul acesteia (Ex: produs->12lei => cost productie->6lei)
- Se adauga comanda si clientul in baza de date (fisierul CSV)
- Se adauga veniturile, cheltuielile de productie si profiturile in urma unei comenzi
------------
*/
int main()
{
    char data[9];
    _strdate_s(data, 9);

    if (!verificareStartZi(data))
    {
        startZi(data, "Bucuresti");
        startZi(data, "Cluj-Napoca");
        startZi(data, "Timisoara");
        startZi(data, "Iasi");
        startZi(data, "Brasov");
    }

    cout << "Bine ati venit in meniul de plasare a unei comenzi!" << endl;
    cout << "Selectati limba: " << endl;
    cout << "1. Romana" << endl;
    cout << "2. Engleza" << endl;

    int nr_limba;
    cin >> nr_limba;
    string limba;
    if (nr_limba == 1)
    {
        limba = "Romana";
    }
    else
    {
        limba = "Engleza";
    }

    cout << "Selectati orasul in care va aflati: " << endl;
    cout << "1. Bucuresti" << endl;
    cout << "2. Cluj-Napoca" << endl;
    cout << "3. Timisoara" << endl;
    cout << "4. Iasi" << endl;
    cout << "5. Brasov" << endl;

    int nr_oras;
    cin >> nr_oras;
    vector<string> orase = {"Bucuresti", "Cluj-Napoca", "Timisoara", "Iasi", "Brasov"};

    cout << "Care este numele dumneavoastra?" << endl;
    cout << "Nume: ";
    string nume;
    cin >> nume;
    cout << "Prenume: ";
    string prenume;
    cin >> prenume;

    Client client;
    client.setClient(nume, prenume);
    int nr_comenzi = client.nrComenziReducere();

    vector<string> comanda;

    if (limba == "Romana")
    {
        cout << "\n---- CAFENEA " << orase[nr_oras - 1] << " ----" << endl;
        cout << "Ce doriti sa comandati?" << endl;
        cout << "1. Cafea" << endl;
        cout << "2. Ciocolata calda" << endl;
        cout << "3. Ceai" << endl;
        cout << "4. Limonada" << endl;
        cout << "5. Desert" << endl;
        cout << "6. Sandwich" << endl;

        int tip_comanda;
        cin >> tip_comanda;

        int pret_inainte_comanda = 0;
        int pret_total = 0;
        float pret_total_redus = 0;

        while (tip_comanda != 7)
        {
            int ID, cantitate;
            string MARIME;

            if (tip_comanda == 1)
            {
                cout << "\n---- MENIU CAFEA ----" << endl;
                showMenu("Cafea", limba);
                cout << endl;
                cout << "Ce cafea doriti sa comandati?" << endl;
                cout << "ID: ";
                cin >> ID;
                cout << "Marime: ";
                cin >> MARIME;
                cout << "Cantitate: ";
                cin >> cantitate;

                cout << endl;
                pret_total += plasareComanda(limba, "Cafea", ID, MARIME, cantitate);
                comanda.push_back(plasareProdus(limba, "Cafea", ID, MARIME, cantitate));

                if (pret_total != pret_inainte_comanda)
                {
                    modificareStoc("Cafea", ID, MARIME, cantitate);
                    pret_inainte_comanda = pret_total;
                }
            }
            else if (tip_comanda == 2)
            {
                cout << "\n---- MENIU CIOCOLATA CALDA ----" << endl;
                showMenu("Ciocolata Calda", limba);
                cout << endl;
                cout << "Ce ciocolata calda doriti sa comandati?" << endl;
                cout << "ID: ";
                cin >> ID;
                cout << "Marime: ";
                cin >> MARIME;
                cout << "Cantitate: ";
                cin >> cantitate;

                cout << endl;

                pret_total += plasareComanda(limba, "Ciocolata Calda", ID, MARIME, cantitate);
                comanda.push_back(plasareProdus(limba, "Ciocolata Calda", ID, MARIME, cantitate));

                if (pret_total != pret_inainte_comanda)
                {
                    modificareStoc("Ciocolata Calda", ID, MARIME, cantitate);
                    pret_inainte_comanda = pret_total;
                }
            }
            else if (tip_comanda == 3)
            {
                cout << "\n---- MENIU CEAI ----" << endl;
                showMenu("Ceai", limba);
                cout << endl;
                cout << "Ce ceai doriti sa comandati?" << endl;
                cout << "ID: ";
                cin >> ID;
                cout << "Marime: ";
                cin >> MARIME;
                cout << "Cantitate: ";
                cin >> cantitate;

                cout << endl;
                pret_total += plasareComanda(limba, "Ceai", ID, MARIME, cantitate);
                comanda.push_back(plasareProdus(limba, "Ceai", ID, MARIME, cantitate));

                if (pret_total != pret_inainte_comanda)
                {
                    modificareStoc("Ceai", ID, MARIME, cantitate);
                    pret_inainte_comanda = pret_total;
                }
            }
            else if (tip_comanda == 4)
            {
                cout << "\n---- MENIU LIMONADA ----" << endl;
                showMenu("Limonada", limba);
                cout << endl;
                cout << "Ce limonada doriti sa comandati?" << endl;
                cout << "ID: ";
                cin >> ID;
                cout << "Marime: ";
                cin >> MARIME;
                cout << "Cantitate: ";
                cin >> cantitate;

                cout << endl;
                pret_total += plasareComanda(limba, "Limonada", ID, MARIME, cantitate);
                comanda.push_back(plasareProdus(limba, "Limonada", ID, MARIME, cantitate));

                if (pret_total != pret_inainte_comanda)
                {
                    modificareStoc("Limonada", ID, MARIME, cantitate);
                    pret_inainte_comanda = pret_total;
                }
            }
            else if (tip_comanda == 5)
            {
                cout << "\n---- MENIU DESERT ----" << endl;
                showMenu("Desert", limba);
                cout << endl;
                cout << "Ce desert doriti sa comandati?" << endl;
                cout << "ID: ";
                cin >> ID;
                cout << "Marime: ";
                cin >> MARIME;
                cout << "Cantitate: ";
                cin >> cantitate;

                cout << endl;
                pret_total += plasareComanda(limba, "Desert", ID, MARIME, cantitate);
                comanda.push_back(plasareProdus(limba, "Desert", ID, MARIME, cantitate));

                if (pret_total != pret_inainte_comanda)
                {
                    modificareStoc("Desert", ID, MARIME, cantitate);
                    pret_inainte_comanda = pret_total;
                }
            }
            else
            {
                cout << "\n---- MENIU SANDWICH ----" << endl;
                showMenu("Sandwich", limba);
                cout << endl;
                cout << "Ce sandwich doriti sa comandati?" << endl;
                cout << "ID: ";
                cin >> ID;
                cout << "Marime: ";
                cin >> MARIME;
                cout << "Cantitate: ";
                cin >> cantitate;

                cout << endl;
                pret_total += plasareComanda(limba, "Sandwich", ID, MARIME, cantitate);
                comanda.push_back(plasareProdus(limba, "Sandwich", ID, MARIME, cantitate));

                if (pret_total != pret_inainte_comanda)
                {
                    modificareStoc("Sandwich", ID, MARIME, cantitate);
                    pret_inainte_comanda = pret_total;
                }
            }

            if (nr_comenzi < 5)
            {
                pret_total_redus = pret_total;
                // cout << "\nPRET COMANDA: " << pret_total_redus << " lei" << endl;
            }
            else if (nr_comenzi >= 5 && nr_comenzi < 10)
            {
                pret_total_redus = pret_total - pret_total * 0.05;
                // cout << "\nPRET COMANDA: " << pret_total_redus << " lei" << endl;
            }
            else if (nr_comenzi >= 10 && nr_comenzi < 20)
            {
                pret_total_redus = pret_total - pret_total * 0.075;
                // cout << "\nPRET COMANDA: " << pret_total_redus << " lei" << endl;
            }
            else if (nr_comenzi >= 20)
            {
                pret_total_redus = pret_total - pret_total * 0.1;
                // cout << "\nPRET COMANDA: " << pret_total_redus << " lei" << endl;
            }

            if (nr_comenzi == 50)
            {
                cout << "Felicitari!" << endl;
                cout << "In semn de recunostinta pentru ca ne-ati fost alaturi si ati efectuat 50 de comenzi in cafeneaua noastra, va oferim comanda gratis!" << endl;
                cout << "\nPRET COMANDA: 0 lei" << endl;
            }

            cout << endl;
            cout << "\nCe doriti sa mai comandati?" << endl;
            cout << "1. Cafea" << endl;
            cout << "2. Ciocolata calda" << endl;
            cout << "3. Ceai" << endl;
            cout << "4. Limonada" << endl;
            cout << "5. Desert" << endl;
            cout << "6. Sandwich" << endl;
            cout << "7. Nimic altceva" << endl;

            cin >> tip_comanda;
        }

        cout << "\nComanda dumneavoastra: " << endl;
        for (int i = 0; i < comanda.size(); i++)
        {
            cout << comanda[i] << endl;
        }
        cout << "Pret: " << pret_total_redus << " lei" << endl;
        cout << "\nMultumim ca ati ales cafeneaua noastra! O zi buna in continuare" << endl;

        client.adaugaClientSiComandaInCSV(comanda, pret_total_redus);

        adaugareVenitCheltuieliProfituri(data, orase[nr_oras - 1], pret_total_redus, pret_total / 2);
    }
    else
    {
        cout << "\n---- COFFEE SHOP " << orase[nr_oras - 1] << " ----" << endl;
        cout << "What would you like to order?" << endl;
        cout << "1. Coffee" << endl;
        cout << "2. Hot Chocolate" << endl;
        cout << "3. Tea" << endl;
        cout << "4. Lemonade" << endl;
        cout << "5. Dessert" << endl;
        cout << "6. Sandwich" << endl;

        int tip_comanda;
        cin >> tip_comanda;

        int pret_inainte_comanda = 0;
        int pret_total = 0;
        float pret_total_redus = 0;

        while (tip_comanda != 7)
        {
            int ID, cantitate;
            string MARIME;

            if (tip_comanda == 1)
            {
                cout << "\n---- COFFEE MENU ----" << endl;
                showMenu("Coffee", limba);
                cout << endl;
                cout << "What coffee would you like to order?" << endl;
                cout << "ID: ";
                cin >> ID;
                cout << "Size: ";
                cin >> MARIME;
                cout << "Quantity: ";
                cin >> cantitate;

                cout << endl;
                pret_total += plasareComanda(limba, "Coffee", ID, MARIME, cantitate);
                comanda.push_back(plasareProdus(limba, "Coffee", ID, MARIME, cantitate));

                if (pret_total != pret_inainte_comanda)
                {
                    modificareStoc("Cafea", ID, MARIME, cantitate);
                    pret_inainte_comanda = pret_total;
                }
            }
            else if (tip_comanda == 2)
            {
                cout << "\n---- HOT CHOCOLATE MENU ----" << endl;
                showMenu("Hot Chocolate", limba);
                cout << endl;
                cout << "What hot chocolate would you like to order?" << endl;
                cout << "ID: ";
                cin >> ID;
                cout << "Size: ";
                cin >> MARIME;
                cout << "Quantity: ";
                cin >> cantitate;

                cout << endl;

                pret_total += plasareComanda(limba, "Hot Chocolate", ID, MARIME, cantitate);
                comanda.push_back(plasareProdus(limba, "Hot Chocolate", ID, MARIME, cantitate));

                if (pret_total != pret_inainte_comanda)
                {
                    modificareStoc("Ciocolata Calda", ID, MARIME, cantitate);
                    pret_inainte_comanda = pret_total;
                }
            }
            else if (tip_comanda == 3)
            {
                cout << "\n---- TEA MENU ----" << endl;
                showMenu(limba, "Tea");
                cout << endl;
                cout << "What tea would you like to order?" << endl;
                cout << "ID: ";
                cin >> ID;
                cout << "Size: ";
                cin >> MARIME;
                cout << "Quantity: ";
                cin >> cantitate;

                cout << endl;
                pret_total += plasareComanda(limba, "Tea", ID, MARIME, cantitate);
                comanda.push_back(plasareProdus(limba, "Tea", ID, MARIME, cantitate));

                if (pret_total != pret_inainte_comanda)
                {
                    modificareStoc("Ceai", ID, MARIME, cantitate);
                    pret_inainte_comanda = pret_total;
                }
            }
            else if (tip_comanda == 4)
            {
                cout << "\n---- LEMONADE MENU ----" << endl;
                showMenu("Lemonade", limba);
                cout << endl;
                cout << "What lemonade would you like to order?" << endl;
                cout << "ID: ";
                cin >> ID;
                cout << "Size: ";
                cin >> MARIME;
                cout << "Quantity: ";
                cin >> cantitate;

                cout << endl;
                pret_total += plasareComanda(limba, "Lemonade", ID, MARIME, cantitate);
                comanda.push_back(plasareProdus(limba, "Lemonade", ID, MARIME, cantitate));

                if (pret_total != pret_inainte_comanda)
                {
                    modificareStoc("Limonada", ID, MARIME, cantitate);
                    pret_inainte_comanda = pret_total;
                }
            }
            else if (tip_comanda == 5)
            {
                cout << "\n---- DESSERT MENU ----" << endl;
                showMenu("Dessert", limba);
                cout << endl;
                cout << "What dessert would you like to order?" << endl;
                cout << "ID: ";
                cin >> ID;
                cout << "Size: ";
                cin >> MARIME;
                cout << "Quantity: ";
                cin >> cantitate;

                cout << endl;
                pret_total += plasareComanda(limba, "Dessert", ID, MARIME, cantitate);
                comanda.push_back(plasareProdus(limba, "Dessert", ID, MARIME, cantitate));

                if (pret_total != pret_inainte_comanda)
                {
                    modificareStoc("Desert", ID, MARIME, cantitate);
                    pret_inainte_comanda = pret_total;
                }
            }
            else
            {
                cout << "\n---- SANDWICH MENU ----" << endl;
                showMenu("Sandwich", limba);
                cout << endl;
                cout << "What sandwich would you like to order?" << endl;
                cout << "ID: ";
                cin >> ID;
                cout << "Size: ";
                cin >> MARIME;
                cout << "Quantity: ";
                cin >> cantitate;

                cout << endl;
                pret_total += plasareComanda(limba, "Sandwich", ID, MARIME, cantitate);
                comanda.push_back(plasareProdus(limba, "Sandwich", ID, MARIME, cantitate));

                if (pret_total != pret_inainte_comanda)
                {
                    modificareStoc("Sandwich", ID, MARIME, cantitate);
                    pret_inainte_comanda = pret_total;
                }
            }

            if (nr_comenzi < 5)
            {
                pret_total_redus = pret_total;
                // cout << "\nPRET COMANDA: " << pret_total_redus << " lei" << endl;
            }
            else if (nr_comenzi >= 5 && nr_comenzi < 10)
            {
                pret_total_redus = pret_total - pret_total * 0.05;
                // cout << "\nPRET COMANDA: " << pret_total_redus << " lei" << endl;
            }
            else if (nr_comenzi >= 10 && nr_comenzi < 20)
            {
                pret_total_redus = pret_total - pret_total * 0.075;
                // cout << "\nPRET COMANDA: " << pret_total_redus << " lei" << endl;
            }
            else if (nr_comenzi >= 20)
            {
                pret_total_redus = pret_total - pret_total * 0.1;
                // cout << "\nPRET COMANDA: " << pret_total_redus << " lei" << endl;
            }

            if (nr_comenzi == 50)
            {
                cout << "Congratulations!" << endl;
                cout << "As a token of appreciation for placing 50 orders in our coffee shop, we offer you a free order!" << endl;
                cout << "\nORDER PRICE: 0 RON" << endl;
            }

            cout << endl;
            cout << "\nWhat else would you like to order?" << endl;
            cout << "1. Coffee" << endl;
            cout << "2. Hot Chocolate" << endl;
            cout << "3. Tea" << endl;
            cout << "4. Lemonade" << endl;
            cout << "5. Dessert" << endl;
            cout << "6. Sandwich" << endl;
            cout << "7. Nothing else" << endl;

            cin >> tip_comanda;
        }

        cout << "\nYour order: " << endl;
        for (int i = 0; i < comanda.size(); i++)
        {
            cout << comanda[i] << endl;
        }
        cout << "Price: " << pret_total_redus << " lei" << endl;
        cout << "\nThank you for choosing our coffee shop! Have a great day!" << endl;

        client.adaugaClientSiComandaInCSV(comanda, pret_total_redus);

        adaugareVenitCheltuieliProfituri(data, orase[nr_oras - 1], pret_total_redus, pret_total / 2);
    }
    return 0;
}
