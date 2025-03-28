#include <iostream>
#include <vector>
#include <cstdlib>      // srand, rand
#include <ctime>        // time
#include <fstream>      // ofstream
#include <chrono>       // high_resolution_clock, nanoseconds
#include <string>
using namespace std;

// ---------------------------------------------------------------------
//  1) CONSTANTES GLOBALES
// ---------------------------------------------------------------------
const int NUM_PRUEBAS     = 1000;
const int MIN_TAMANO      = 10;
const int MAX_TAMANO      = 1000;
const int INCREMENTO      = 10;
const int NUM_ALGORITMOS  = 7;

static const string NOMBRES_ALGORITMOS[NUM_ALGORITMOS] = {
    "Insercion",
    "Burbuja",
    "Seleccion",
    "Shell",
    "Mezcla",
    "Rapido",
    "Monticulos"
};

// ---------------------------------------------------------------------
//  2) DECLARACIÓN DE FUNCIONES DE ORDENAMIENTO
// ---------------------------------------------------------------------

void insertionSort (int arr[], int n);
void bubbleSort    (int arr[], int n);
void selectionSort (int arr[], int n);
void shellSort     (int arr[], int n);
void mergeSortWrapper(int arr[], int n);
void quickSortWrapper(int arr[], int n);
void heapSort      (int arr[], int n);

// Auxiliares Merge
void mergeSortAux(int arr[], int inicio, int fin);
void merge(int arr[], int inicio, int medio, int fin);

// Auxiliares Quick
void quickSortAux(int arr[], int inicio, int fin);
int  partitionAux (int arr[], int inicio, int fin);

// Auxiliares Heap
void buildMaxHeap (int arr[], int n);
void maxHeapify   (int arr[], int idx, int n);

// ---------------------------------------------------------------------
//  3) IMPLEMENTACIONES
// ---------------------------------------------------------------------

// 3.1) Inserción
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int clave = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > clave) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = clave;
    }
}

// 3.2) Burbuja
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// 3.3) Selección
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            int temp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = temp;
        }
    }
}

// 3.4) Shell
void shellSort(int arr[], int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}

// 3.5) Merge (Wrapper)
void mergeSortWrapper(int arr[], int n) {
    mergeSortAux(arr, 0, n-1);
}
void mergeSortAux(int arr[], int inicio, int fin) {
    if (inicio < fin) {
        int medio = (inicio + fin) / 2;
        mergeSortAux(arr, inicio, medio);
        mergeSortAux(arr, medio + 1, fin);
        merge(arr, inicio, medio, fin);
    }
}
void merge(int arr[], int inicio, int medio, int fin) {
    int n1 = medio - inicio + 1;
    int n2 = fin - medio;
    int* left  = new int[n1];
    int* right = new int[n2];
    for (int i = 0; i < n1; i++)  left[i]  = arr[inicio + i];
    for (int j = 0; j < n2; j++) right[j] = arr[medio + 1 + j];
    int i=0, j=0, k=inicio;
    while (i<n1 && j<n2) {
        if (left[i] <= right[j]) arr[k++] = left[i++];
        else arr[k++] = right[j++];
    }
    while (i < n1) arr[k++] = left[i++];
    while (j < n2) arr[k++] = right[j++];
    delete[] left;
    delete[] right;
}

// 3.6) Quick (Wrapper)
void quickSortWrapper(int arr[], int n) {
    quickSortAux(arr, 0, n-1);
}
void quickSortAux(int arr[], int inicio, int fin) {
    if (inicio < fin) {
        int piv = partitionAux(arr, inicio, fin);
        quickSortAux(arr, inicio, piv - 1);
        quickSortAux(arr, piv + 1, fin);
    }
}
int partitionAux(int arr[], int inicio, int fin) {
    int pivote = arr[fin];
    int i = inicio - 1;
    for (int j = inicio; j < fin; j++) {
        if (arr[j] <= pivote) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i+1];
    arr[i+1] = arr[fin];
    arr[fin] = temp;
    return i + 1;
}

// 3.7) Montículos
void heapSort(int arr[], int n) {
    buildMaxHeap(arr, n);
    for (int i = n-1; i > 0; i--) {
        int temp = arr[0];
        arr[0]   = arr[i];
        arr[i]   = temp;
        maxHeapify(arr, 0, i);
    }
}
void buildMaxHeap(int arr[], int n) {
    for (int i = n/2 - 1; i >= 0; i--) {
        maxHeapify(arr, i, n);
    }
}
void maxHeapify(int arr[], int idx, int n) {
    int izq   = 2*idx + 1;
    int der   = 2*idx + 2;
    int mayor = idx;
    if(izq < n && arr[izq] > arr[mayor]) mayor = izq;
    if(der < n && arr[der] > arr[mayor]) mayor = der;
    if(mayor != idx) {
        int temp = arr[idx];
        arr[idx] = arr[mayor];
        arr[mayor] = temp;
        maxHeapify(arr, mayor, n);
    }
}

// ---------------------------------------------------------------------
//   4) FUNCIÓN PRINCIPAL
// ---------------------------------------------------------------------
int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    // Verificación: cada algoritmo ordena correctamente un arreglo de prueba
    cout << "Verificando algoritmos con un arreglo de prueba [5,2,4,1,3]...\n";
    {
        int prueba[5] = {5,2,4,1,3};
        auto checarOrden = [&](void(*f)(int[],int), const string& name) {
            int tmp[5];
            for(int i=0; i<5; i++) tmp[i] = prueba[i];
            f(tmp, 5);
            for(int i=1; i<5; i++){
                if(tmp[i]<tmp[i-1]){
                    cerr << "ERROR: Algoritmo " << name << " no ordenó correctamente.\n";
                }
            }
        };
        checarOrden(insertionSort,    "Insercion");
        checarOrden(bubbleSort,       "Burbuja");
        checarOrden(selectionSort,    "Seleccion");
        checarOrden(shellSort,        "Shell");
        checarOrden(mergeSortWrapper, "Mezcla");
        checarOrden(quickSortWrapper, "Rapido");
        checarOrden(heapSort,         "Monticulos");
    }
    cout << "Verificación completada.\n\n";

    // Calculamos cuántos tamaños
    int cantidadTamanios = 0;
    for(int t = MIN_TAMANO; t <= MAX_TAMANO; t+=INCREMENTO) {
        cantidadTamanios++;
    }

    // Creamos un vector con los tamaños (10,20,30...1000)
    vector<int> listaTamanios;
    listaTamanios.reserve(cantidadTamanios);
    for(int t = MIN_TAMANO; t <= MAX_TAMANO; t+=INCREMENTO) {
        listaTamanios.push_back(t);
    }

    // Array de punteros a funciones
    // Mismo orden que NOMBRES_ALGORITMOS
    void (*algoritmos[NUM_ALGORITMOS])(int[], int) = {
        insertionSort,
        bubbleSort,
        selectionSort,
        shellSort,
        mergeSortWrapper,
        quickSortWrapper,
        heapSort
    };

    // Para cada algoritmo, guardaremos un vector de promedios (uno por cada tamaño)
    // Y generaremos un CSV con la forma:
    // Tamanio,Promedio(ns)
    for(int idxAlg=0; idxAlg<NUM_ALGORITMOS; idxAlg++) {
        // Nombre del archivo CSV:  "n_[Nombre]_Ordenamiento.csv"
        // (idxAlg+1 para que sea 1,2,3..., no 0,1,2.)
        string filename = to_string(idxAlg+1) + "_" + NOMBRES_ALGORITMOS[idxAlg] + "_Ordenamiento.csv";

        // Preparamos un vector para los promedios
        vector<long long> promedios(cantidadTamanios, 0);

        cout << "=== Analizando algoritmo: " << NOMBRES_ALGORITMOS[idxAlg] << " ===" << endl;

        // Recorremos cada tamaño
        for(int iT=0; iT<cantidadTamanios; iT++){
            int tam = listaTamanios[iT];

            // Generar 1000 arreglos base
            vector<vector<int>> arreglosBase(NUM_PRUEBAS, vector<int>(tam));
            for(int iP=0; iP<NUM_PRUEBAS; iP++){
                for(int k=0; k<tam; k++){
                    arreglosBase[iP][k] = rand() % 100000;
                }
            }

            // Sumar tiempos
            long long sumaTiempo = 0;

            // Para cada prueba, medimos
            for(int iP=0; iP<NUM_PRUEBAS; iP++){
                // Copiamos arreglosBase[iP] a arr
                int* arr = new int[tam];
                for(int k=0; k<tam; k++){
                    arr[k] = arreglosBase[iP][k];
                }

                auto inicio = std::chrono::high_resolution_clock::now();
                algoritmos[idxAlg](arr, tam);
                auto fin    = std::chrono::high_resolution_clock::now();

                long long duracion_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio).count();
                sumaTiempo += duracion_ns;

                delete[] arr;
            }

            // Calcular promedio
            long long promedio_ns = sumaTiempo / NUM_PRUEBAS;
            promedios[iT] = promedio_ns;

            // Imprimir en consola
            cout << "Tamaño: " << tam
                 << " => Promedio: " << promedio_ns << " ns" << endl;
        }

        // Al finalizar, generamos el CSV con promedios
        ofstream archivoCSV(filename.c_str());
        if(!archivoCSV.is_open()) {
            cerr << "Error al crear " << filename << endl;
            continue;
        }

        // Primera línea: "Tamanio,Promedio(ns)"
        archivoCSV << "Tamanio,Promedio(ns)\n";

        // Siguientes lineas:  tam, promedios[i]
        for(int iT=0; iT<cantidadTamanios; iT++){
            archivoCSV << listaTamanios[iT] << ","
                       << promedios[iT] << "\n";
        }

        archivoCSV.close();
        cout << "Archivo CSV generado: " << filename << "\n\n";
    }

    cout << "Programa finalizado correctamente.\n";
    return 0;
}
