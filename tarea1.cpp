#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

const int PAGE_SIZE = 1024; // Tamaño de página en bytes
const int ELEMENT_SIZE = sizeof(long long); // Tamaño del elemento (64 bits)

class LinearHashing {
private:
    int p, t; // Número de páginas y parámetro t
    int maxAccess; // Costo promedio máximo permitido
    std::vector<std::vector<long long>> table; // Tabla de hash con listas de páginas
    std::vector<int> overflowPages; // Páginas de desbordamiento

    // Función de hash que devuelve un valor en el rango [0, 2^t+1)
    int hash(long long y) {
        return y % (1 << (t + 1));
    }

public:
    LinearHashing(int maxAccess) : p(1), t(0), maxAccess(maxAccess) {
        table.resize(1); // Inicia con una sola página
    }

    // Inserción de un nuevo elemento 'y' en la tabla
    void insert(long long y) {
        int k = hash(y);

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

// Función para generar números aleatorios de 64 bits
long long generateRandom64() {
    static std::random_device rd;
    static std::mt19937_64 eng(rd());
    static std::uniform_int_distribution<long long> distr;
    return distr(eng);
}

int main() {
    // Configuración de la tabla de hashing lineal
    int maxAccess = 4; // Hiperparámetro para el máximo costo promedio permitido
    LinearHashing hashTable(maxAccess);

    // Generación de una secuencia N de números aleatorios (por ejemplo, 2^10 números)
    int numElements = 1024; // 2^10 elementos
    for (int i = 0; i < numElements; ++i) {
        long long element = generateRandom64();
        hashTable.insert(element);
    }

    // Mostrar la tabla de hash
    hashTable.display();

    return 0;
}
