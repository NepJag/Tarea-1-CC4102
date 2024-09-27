#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <limits>
using namespace std; // Para evitar escribir "std::"

const int PAGE_SIZE = 1024; // Tamaño de página en bytes
const int ELEMENT_SIZE = sizeof(long long); // Tamaño del elemento (64 bits)
const size_t ELEMENTS_PER_PAGE = PAGE_SIZE / ELEMENT_SIZE; // Elementos por página


// int main() {
//     // Configuración de la tabla de hashing lineal
//     int maxInsert = 4; // Hiperparámetro para el máximo costo promedio permitido
//     LinearHashingTable hashTable(maxInsert);

//     // Generación de una secuencia N de números aleatorios (por ejemplo, 2^10 números)
//     int numElements = 1024; // 2^10 elementos
//     for (int i = 0; i < numElements; ++i) {
//         long long element = generateRandom64();
//         hashTable.insert(element);
//     }

//     // Mostrar la tabla de hash
//     hashTable.display();

//     return 0;
// }


// Función de hash que devuelve un valor entre 0 y 2^64 - 1 aleatorio
unsigned long long h(long long y) {
    // Inicializar el generador con la semilla basada en el valor de y
    mt19937_64 generator(y);
    
    // Generar el valor hash en el rango [0, 2^64 - 1]
    uniform_int_distribution<unsigned long long> distribution(0, (1ULL << 63)*2 - 1);
    
    return distribution(generator);
}

// Función para generar una secuencia de números secuenciales long long
vector<long long> generateSequence(size_t N) {
    vector<long long> sequence;
    sequence.reserve(N); // Reservar espacio para evitar redimensionamiento

    // Generar la secuencia secuencialmente
    for (long long i = 0; i < N; ++i) {
        sequence.push_back(i);
    }

    return sequence;
}

class LinearHashingTable {
private:

    int p, t; // Número de páginas y parámetro t ("instante")
    int maxInsert; // Costo promedio máximo permitido para la operación de inserción
    vector<vector<long long>> table; // Tabla de hash lineal (lista de páginas (listas) de elementos)
    

public:
    LinearHashingTable(int maxInsert) : p(1), t(0), maxInsert(maxInsert) {
        table.resize(1); // Inicia con una sola página
    }

    // Inserción de un nuevo elemento 'y' en la tabla
    void insert(long long y) {
        int k = h(y);

        if (k < p) {
            // La página ya está expandida
            table[k].push_back(y);
        } else {
            // La página aún no ha sido creada, usar página desbordada
            table[k - (1 << t)].push_back(y);
        }

        // Verificar si es necesario expandir la tabla
        if (table[k].size() * ELEMENT_SIZE > PAGE_SIZE) {
            expand();
        }
    }

    // Expansión de la tabla
    void expand() {
        std::vector<long long> toRehash = table[p - (1 << t)];
        table[p - (1 << t)].clear();

        table.push_back({}); // Añadir nueva página
        p++;

        if (p == (1 << (t + 1))) {
            t++;
        }

        // Rehash de los elementos
        for (long long elem : toRehash) {
            insert(elem);
        }
    }

    // Función para mostrar la tabla de hash
    void display() {
        for (int i = 0; i < table.size(); ++i) {
            std::cout << "Página " << i << ": ";
            for (auto &elem : table[i]) {
                std::cout << elem << " ";
            }
            std::cout << std::endl;
        }
    }
};

// Estructura de la tabla de hashing
class HashTable {
private:
    // Cada página contiene hasta ELEMENTS_PER_PAGE elementos
    using Page = std::vector<long long>;

    // Cada bucket (índice de la tabla de hash) contiene una lista de páginas
    using Index = std::vector<Page>;

    // La tabla de hash es una lista de buckets
    std::vector<Index> table;

public:
    // Constructor: inicializa la tabla con un tamaño determinado
    HashTable(size_t size) : table(size) {}

    // Inserta un elemento y en la tabla de hash
    void insert(long long y) {
        unsigned long long index = h(y) % table.size(); // Indexado con el hash
        Index & bucket = table[index]; // Obtener el bucket correspondiente

        // Si el bucket está vacío o la última página está llena, agrega una nueva página
        if (bucket.empty() || bucket.back().size() == ELEMENTS_PER_PAGE) {
            bucket.push_back(Page());
        }

        // Insertar el elemento en la última página del bucket
        bucket.back().push_back(y);
    }

}

int main() {
    size_t N = 1ULL << 24; // Ejemplo con 2^24 elementos
    vector<long long> sequence = generateSequence(N);

    // Imprimir algunos de los números generados
    // for (size_t i = 15999990; i < 16000000; ++i) {
    //     cout << sequence[i] << endl;
    // }

    cout <<  "(1ULL << 63)*2 -1 = " << (1ULL << 63)*2 -1 << endl;

    long long y = 1; // Ejemplo de elemento y
    unsigned long long hashValue = h(y);
    
    cout << "Hash de " << y << " es: " << hashValue << endl;

    return 0;
}

