#include <chrono>  // biblioteka do std::chrono::high_resolution_clock - Pomiar czasu
#include <fstream> // wczytywanie z pliku
#include <cstring> // memcpy do robienia sortowania na kopi tablicy
#include <type_traits> // sprawdzenie typow czy liczba calkowita czy zmiennoprzecinkowa wygenerujLosowa()
#include <cstdlib> //malloc rand
#include <ctime> // srand()
#include <iostream> // standardowa biblioteka wejcia/wyjscia
using namespace std;

template <typename T>
class Algorytmy {
private:
    T* tablica = nullptr;
    int rozmiar = 0;
public:
    void wczytajzPliku(const string& nazwaPliku) {
        if (tablica) {
            free(tablica); // sprawdzamy czy tablica istnieje, jesli tak zwalniamy
        }
        ifstream plik(nazwaPliku);
        if (!plik.is_open()) {
            perror("Blad otwierania pliku");
            //cout <<" Nie udalo sie wczytac pliku" <<endl;
            return;
        }
        plik >> rozmiar;
        tablica = (T*)malloc(rozmiar * sizeof(T));
        if (!tablica) {
            cout <<" Nie udalo sie zaalokowac pamieci"<<endl;
            return;
        }
        string liczba;
        for(int i = 0; i<rozmiar; i++) {
            plik >> liczba;
            for( char& znak: liczba) {    //zamiana , na . - domyslnie wczytywane jest z kropkami
                if(znak == ',') znak = '.';
            }
            tablica[i] = stof(liczba);  //zamiana stringa na liczbe
        }

        plik.close();
    }
    void wygenerujRosnaco(int rozmiar) {       // Genruje tablice liczb rosnacych od 0 do rozmiar-1
        if(tablica) free(tablica);
        this->rozmiar = rozmiar;
        tablica = (T*)malloc(rozmiar * sizeof(T));
        if (!tablica) {
            cout << "Nie udalo sie zaalokowac pamieci" << endl;
            return;
        }
        for (int i = 0; i < rozmiar; i++) {
            tablica[i] = static_cast<T>(i);
        }
    }
    void wygenerujMalejaco(int rozmiar) { //Generuje tablice liczb malejących od rozmiar do 1
        if(tablica) free(tablica);
        this->rozmiar = rozmiar;
        tablica = (T*)malloc(rozmiar * sizeof(T));
        if (!tablica) {
            cout << "Nie udalo sie zaalokowac pamieci" << endl;
            return;
        }
        for (int i = 0; i < rozmiar; i++) {
            tablica[i] = static_cast<T>(rozmiar - i);
        }
    }
    void wygenerujLosowa(int rozmiar) {  // Generuje tablicę liczb losowych od 0 do 1000
        if(tablica) {
            free(tablica);
        }
        this->rozmiar = rozmiar;
        tablica = (T*)malloc(rozmiar * sizeof(T));
        if (!tablica) {
            cout <<" Nie udalo sie zaalokowac pamieci"<<endl;
            return;
        }
        srand(time(NULL));
        for (int i = 0; i < rozmiar; i++) {
            if (is_floating_point<T>::value) { // sprawdzenie typu danych czy float
                tablica[i] = static_cast<T>(rand() % 1000) + static_cast<T>(rand()%1000/1000.0);
            } else {
                tablica[i] = rand() % 1000;
            }
        }
    }
    void wygenerujCzesciowo(int rozmiar, float procent) { // Generuje czesciowo posortowana tablice
        if(tablica) free(tablica);
        this->rozmiar = rozmiar;
        tablica = (T*)malloc(rozmiar * sizeof(T));
        if (!tablica) {
            cout << "Nie udalo sie zaalokowac pamieci" << endl;
            return;
        }

        int granica = static_cast<int>(rozmiar * procent); // ile elementów ma być posortowanych
        for (int i = 0; i < granica; i++) {
            tablica[i] = static_cast<T>(i);
        }
        srand(time(NULL));
        for (int i = granica; i < rozmiar; i++) {
            if (is_floating_point<T>::value) {
                tablica[i] = static_cast<T>(rand() % 1000) + static_cast<T>(rand() % 1000 / 1000.0);
            } else {
                tablica[i] = rand() % 1000;
            }
        }
    }


    void wyswietl() { // wyswietla tablice
        if (!tablica) {
            cout << "Brak tablicy"<< endl;
            return;
        }
        for (int i = 0; i<rozmiar;i++) {
            cout << tablica[i] << " ";
        }
        cout<< endl;
    }

    void przezWstawianie() { // Sortowanie przez wstawianie
        if (!tablica) {
            return;
        }
        T* kopia = (T*)malloc(rozmiar*sizeof(T));
        memcpy(kopia, tablica, rozmiar*sizeof(T));

        for (int i = 1; i < rozmiar; i++) {
            T x = kopia[i];
            int j = i - 1;
            while (j >= 0 && kopia[j] > x) {
                kopia[j + 1] = kopia[j];
                j--;
            }
            kopia[j + 1] = x;
        }
       // for (int i = 0; i<rozmiar; i++) {
         //   cout << kopia[i] <<" ";
        //}
        cout << endl;
        free(kopia);
    }
    void heapify(T* tab,int n, int i) {
        int rodzic = i;
        int lewy = 2 * i + 1;
        int prawy = 2 * i + 2;

        if (lewy < n && tab[lewy] > tab[rodzic])
            rodzic = lewy;

        if (prawy < n && tab[prawy] > tab[rodzic])
            rodzic = prawy;

        if (rodzic != i) {  // jezeli najwiekszy element nie jest rodzicem to zamieniamy i wywolujemy rekurencyjnie ponownie
            swap(tab[i], tab[rodzic]);
            heapify(tab ,n, rodzic);
        }
    }
    void przezKopcowanie() { // Sortowanie przez kopcowanie
        if(!tablica) {
            return;
        }
        T* kopia = (T*)malloc(rozmiar*sizeof(T*));
        memcpy(kopia, tablica, rozmiar*sizeof(T));

        for (int i = rozmiar / 2 - 1; i >= 0; i--) //budowa kopca maksymalnego (kazdy rodzic >= potomkowie)
            heapify(kopia,rozmiar, i);

        for (int i = rozmiar - 1; i > 0; i--) {
            swap(kopia[0], kopia[i]);

            heapify(kopia,i, 0);
        }
        //for (int i = 0;i<rozmiar;i++) {
          //  cout<< kopia[i] <<" ";
        //}
        cout << endl;

        free(kopia);
    }
    void shellSortKlasyczny() {
        if (!tablica) return;
        T* kopia = (T*)malloc(rozmiar * sizeof(T));
        memcpy(kopia, tablica, rozmiar * sizeof(T));

        for (int gap = rozmiar / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < rozmiar; i++) {
                T temp = kopia[i];
                int j;
                for (j = i; j >= gap && kopia[j - gap] > temp; j -= gap) {
                    kopia[j] = kopia[j - gap];
                }
                kopia[j] = temp;
            }
        }
       // for (int i = 0; i < rozmiar; i++) {
         //   cout << kopia[i] << " ";
        //}
        cout << endl;
        free(kopia);
    }

    void shellSortKnuth() {
        if (!tablica) return;
        T* kopia = (T*)malloc(rozmiar * sizeof(T));
        memcpy(kopia, tablica, rozmiar * sizeof(T));

        int gap = 1;
        while (gap < rozmiar / 3) {
            gap = gap * 3 + 1; // Wzor Knutha
        }

        while (gap > 0) {
            for (int i = gap; i < rozmiar; i++) {
                T temp = kopia[i];
                int j;
                for (j = i; j >= gap && kopia[j - gap] > temp; j -= gap) {
                    kopia[j] = kopia[j - gap];
                }
                kopia[j] = temp;
            }
            gap = (gap - 1) / 3;
        }

        //for (int i = 0; i < rozmiar; i++) {
          //  cout << kopia[i] << " ";
        //}
        cout << endl;
        free(kopia);
    }

//Quick Sort

void quickSortPivotLeft() {
    if (!tablica) return;
    T* kopia = (T*)malloc(rozmiar * sizeof(T));
    memcpy(kopia, tablica, rozmiar * sizeof(T));
    quickSortLeft(kopia, 0, rozmiar - 1);
    //for (int i = 0; i < rozmiar; i++) cout << kopia[i] << " ";
    cout << endl;
    free(kopia);
}

void quickSortLeft(T* arr, int low, int high) {
    if (low < high) {
        T pivot = arr[low];
        int i = low + 1, j = high;
        while (i <= j) {
            while (i <= j && arr[i] <= pivot) i++;
            while (i <= j && arr[j] > pivot) j--;
            if (i < j) swap(arr[i], arr[j]);
        }
        swap(arr[low], arr[j]);
        quickSortLeft(arr, low, j - 1);
        quickSortLeft(arr, j + 1, high);
    }
}

void quickSortPivotRight() {
    if (!tablica) return;
    T* kopia = (T*)malloc(rozmiar * sizeof(T));
    memcpy(kopia, tablica, rozmiar * sizeof(T));
    quickSortRight(kopia, 0, rozmiar - 1);
    //for (int i = 0; i < rozmiar; i++) cout << kopia[i] << " ";
    cout << endl;
    free(kopia);
}

void quickSortRight(T* arr, int low, int high) {
    if (low < high) {
        T pivot = arr[high];
        int i = low, j = high - 1;
        while (i <= j) {
            while (i <= j && arr[i] < pivot) i++;
            while (i <= j && arr[j] >= pivot) j--;
            if (i < j) swap(arr[i], arr[j]);
        }
        swap(arr[i], arr[high]);
        quickSortRight(arr, low, i - 1);
        quickSortRight(arr, i + 1, high);
    }
}

void quickSortPivotMiddle() {
    if (!tablica) return;
    T* kopia = (T*)malloc(rozmiar * sizeof(T));
    memcpy(kopia, tablica, rozmiar * sizeof(T));
    quickSortMiddle(kopia, 0, rozmiar - 1);
    //for (int i = 0; i < rozmiar; i++) cout << kopia[i] << " ";
    cout << endl;
    free(kopia);
}

void quickSortMiddle(T* arr, int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        T pivot = arr[mid];
        int i = low, j = high;
        while (i <= j) {
            while (arr[i] < pivot) i++;
            while (arr[j] > pivot) j--;
            if (i <= j) {
                swap(arr[i], arr[j]);
                i++;
                j--;
            }
        }
        if (low < j) quickSortMiddle(arr, low, j);
        if (i < high) quickSortMiddle(arr, i, high);
    }
}

void quickSortPivotRandom() {
    if (!tablica) return;
    T* kopia = (T*)malloc(rozmiar * sizeof(T));
    memcpy(kopia, tablica, rozmiar * sizeof(T));
    srand(time(NULL));
    quickSortRandom(kopia, 0, rozmiar - 1);
   //1
   //for (int i = 0; i < rozmiar; i++) cout << kopia[i] << " ";
    cout << endl;
    free(kopia);
}

void quickSortRandom(T* arr, int low, int high) {
    if (low < high) {
        int pivotIndex = low + rand() % (high - low + 1);
        swap(arr[pivotIndex], arr[high]);
        T pivot = arr[high];
        int i = low, j = high - 1;
        while (i <= j) {
            while (i <= j && arr[i] < pivot) i++;
            while (i <= j && arr[j] >= pivot) j--;
            if (i < j) swap(arr[i], arr[j]);
        }
        swap(arr[i], arr[high]);
        quickSortRandom(arr, low, i - 1);
        quickSortRandom(arr, i + 1, high);
    }
}





};



int main() {
    int wyborDanych;
    cout << "Wybierz typ danych:"<<endl;
    cout << "1 - int" << endl;
    cout << "2 - float" << endl;
    cout << "Wybor: ";
    cin >> wyborDanych;

    if (wyborDanych == 1) { // Menu z wyborem typu danych Int czy float bez pworotu.
        Algorytmy<int> algorytmy;
        for (;;) {  // Nieskonczona petla
            string plik;
            int wybor, rozmiar;
            cout << "=============MENU==========:\n 1 - Wczytaj tablice z pliku\n";
            cout << "2 - Wygeneruj rosnaca tablice\n";
            cout << "3 - Wygeneruj malejaca tablice\n";
            cout << "4 - Wygeneruj 33% posortowana tablice\n";
            cout << "5 - Wygeneruj 66% posortowana tablice\n";
            cout << "6 - Wygeneruj losowa tablice\n";
            cout << "7 - Wyswietl tablice\n";
            cout << "8 - Sortowanie przez Wstawianie\n";
            cout << "9 - Sortowanie przez Kopcowanie\n";
            cout << "10 - Sortowanie Shella (klasyczne)\n";
            cout << "11 - Sortowanie Shella (Knuth)\n";
            cout << "12 - QuickSort (lewy pivot)\n";
            cout << "13 - QuickSort (prawy pivot)\n";
            cout << "14 - QuickSort (srodkowy pivot)\n";
            cout << "15 - QuickSort (losowy pivot)\n";
            cout << "16 - Koniec\n";

            cout << "Wybor: ";

            cin>> wybor;
            switch (wybor) {
                case 1: {
                    cout << " Wpisz nazwe pliku: ";
                    cin >> plik;
                    algorytmy.wczytajzPliku(plik);
                    break;
                }
                case 2: {
                    cout << "Wpisz rozmiar tablicy: ";
                    cin >> rozmiar;
                    algorytmy.wygenerujRosnaco(rozmiar);
                    break;
                }
                case 3: {
                    cout << "Wpisz rozmiar tablicy: ";
                    cin >> rozmiar;
                    algorytmy.wygenerujMalejaco(rozmiar);
                    break;
                }
                case 4: {
                    cout << "Wpisz rozmiar tablicy: ";
                    cin >> rozmiar;
                    algorytmy.wygenerujCzesciowo(rozmiar,0.33);
                    break;
                }
                case 5: {
                    cout << "Wpisz rozmiar tablicy: ";
                    cin >> rozmiar;
                    algorytmy.wygenerujCzesciowo(rozmiar,0.66);
                    break;
                }
                case 6: {
                    cout << "Wpisz rozmiar tablicy: ";
                    cin >> rozmiar;
                    algorytmy.wygenerujLosowa(rozmiar);
                    break;
                }
                case 7: {
                    algorytmy.wyswietl();
                    break;
                }
                case 8: {
                    using namespace std::chrono; // pomiar czasu
                    auto start = high_resolution_clock::now();
                    algorytmy.przezWstawianie();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana!" << endl;
                    break;
                }
                case 9: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.przezKopcowanie();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout<< "Tablica posortowana" <<endl;
                    break;
                }
                case 10: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.shellSortKlasyczny();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana (Shell Classic)!" << endl;
                    break;
                }
                case 11: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.shellSortKnuth();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana (Shell Knuth)" << endl;
                    break;
                }
                case 12: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.quickSortPivotLeft();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana (QuickSort - lewy pivot)" << endl;
                    break;
                }
                case 13: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.quickSortPivotRight();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana (QuickSort - prawy pivot)" << endl;
                    break;
                }
                case 14: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.quickSortPivotMiddle();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana (QuickSort - srodkowy pivot)" << endl;
                    break;
                }
                case 15: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.quickSortPivotRandom();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana (QuickSort - losowy pivot)" << endl;
                    break;
                }
                case 16: {
                    exit(0);
                }

                default: cout << "Nie ma takiej opcji w menu!" <<endl;
            }
        }
    } else if(wyborDanych == 2) {
        Algorytmy<float> algorytmy;
        for(;;) {
            string plik;
            int wybor, rozmiar;
            cout << "=============MENU==========:\n 1 - Wczytaj tablice z pliku\n";
            cout << "2 - Wygeneruj rosnaca tablice\n";
            cout << "3 - Wygeneruj malejaca tablice\n";
            cout << "4 - Wygeneruj 33% posortowana tablice\n";
            cout << "5 - Wygeneruj 66% posortowana tablice\n";
            cout << "6 - Wygeneruj losowa tablice\n";
            cout << "7 - Wyswietl tablice\n";
            cout << "8 - Sortowanie przez Wstawianie\n";
            cout << "9 - Sortowanie przez Kopcowanie\n";
            cout << "10 - Sortowanie Shella (klasyczne)\n";
            cout << "11 - Sortowanie Shella (Knuth)\n";
            cout << "12 - QuickSort (lewy pivot)\n";
            cout << "13 - QuickSort (prawy pivot)\n";
            cout << "14 - QuickSort (srodkowy pivot)\n";
            cout << "15 - QuickSort (losowy pivot)\n";
            cout << "16 - Koniec\n";

            cout << "Wybor: ";

            cin>> wybor;
            switch (wybor) {
                case 1: {
                    cout << " Wpisz nazwe pliku: ";
                    cin >> plik;
                    algorytmy.wczytajzPliku(plik);
                    break;
                }
                case 2: {
                    cout << "Wpisz rozmiar tablicy: ";
                    cin >> rozmiar;
                    algorytmy.wygenerujRosnaco(rozmiar);
                    break;
                }
                case 3: {
                    cout << "Wpisz rozmiar tablicy: ";
                    cin >> rozmiar;
                    algorytmy.wygenerujMalejaco(rozmiar);
                    break;
                }
                case 4: {
                    cout << "Wpisz rozmiar tablicy: ";
                    cin >> rozmiar;
                    algorytmy.wygenerujCzesciowo(rozmiar,0.33);
                    break;
                }
                case 5: {
                    cout << "Wpisz rozmiar tablicy: ";
                    cin >> rozmiar;
                    algorytmy.wygenerujCzesciowo(rozmiar,0.66);
                    break;
                }
                case 6: {
                    cout << "Wpisz rozmiar tablicy: ";
                    cin >> rozmiar;
                    algorytmy.wygenerujLosowa(rozmiar);
                    break;
                }
                case 7: {
                    algorytmy.wyswietl();
                    break;
                }
                case 8: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.przezWstawianie();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana" << endl;
                    break;
                }
                case 9: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.przezKopcowanie();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout<< "Tablica posortowana" <<endl;
                    break;
                }
                case 10: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.shellSortKlasyczny();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana (Shell Classic)" << endl;
                    break;
                }
                case 11: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.shellSortKnuth();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana (Shell Knuth)" << endl;
                    break;
                }
                case 12: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.quickSortPivotLeft();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana (QuickSort - lewy pivot)" << endl;
                    break;
                }
                case 13: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.quickSortPivotRight();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana (QuickSort - prawy pivot)" << endl;
                    break;
                }
                case 14: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.quickSortPivotMiddle();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana (QuickSort - srodkowy pivot)" << endl;
                    break;
                }
                case 15: {
                    using namespace std::chrono;
                    auto start = high_resolution_clock::now();
                    algorytmy.quickSortPivotRandom();
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout << "Czas sortowania: " << duration.count() <<" milisekund"<<endl;
                    cout << "Tablica posortowana (QuickSort - losowy pivot)" << endl;
                    break;
                }
                case 16: {
                    exit(0);
                }
                default: cout << "Nie ma takiej opcji w menu!" <<endl;
            }
        }
    } else {
        cout << "Nie ma takiej opcji w menu !" <<endl;
    }
    return 0;
}