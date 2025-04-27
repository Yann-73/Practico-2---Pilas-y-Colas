#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>

using namespace std;

template<typename T>
class Stack {
private:
    class Node {
    public:
        T data;
        Node* next;

        Node(const T& value, Node* next = nullptr) : data(value), next(next) {}
    };

    Node* top;
    int count;

public:
    Stack() : top(nullptr), count(0) {}

    ~Stack() {
        while (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(const T& value) {
        Node* newNode = new Node(value, top);
        top = newNode;
        count++;
    }

    bool pop(T& value) {
        if (isEmpty()) {
            return false;
        }
        value = top->data;
        Node* temp = top;
        top = top->next;
        delete temp;
        count--;
        return true;
    }

    bool peek(T& value) const {
        if (isEmpty()) {
            return false;
        }
        value = top->data;
        return true;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    int getSize() const {
        return count;
    }

    void print() const {
        Node* current = top;
        while (current != nullptr) {
            cout << current->data << " -> ";
            current = current->next;
        }
        cout << "NULL" << endl;
    }
};

#endif // STACK_H


#include <iostream>
#include <string>
#include "Stack.h" // Incluimos la clase Stack 

using namespace std;

// Clase para representar un plato
class Plato {
private:
    int identificador;
    string tipo;
    bool estaSucio;

public:
    Plato() : identificador(0), tipo(""), estaSucio(true) {}

    Plato(int id, const string& t) : identificador(id), tipo(t), estaSucio(true) {}

    int getIdentificador() const {
        return identificador;
    }

    const string& getTipo() const {
        return tipo;
    }

    bool getEstaSucio() const {
        return estaSucio;
    }

    void limpiar() {
        estaSucio = false;
    }

    void ensuciar() {
        estaSucio = true;
    }

    // Sobrecarga del operador << para poder imprimir platos fácilmente
    friend ostream& operator<<(ostream& os, const Plato& plato) {
        os << "Plato #" << plato.identificador
            << " - Tipo: " << plato.tipo
            << " - Estado: " << (plato.estaSucio ? "Sucio" : "Limpio");
        return os;
    }
};

// Clase para gestionar el lavado de platos
class LavadoPlatos {
private:
    Stack<Plato> pilaPlatosSucios; // Pila para platos sucios
    Stack<Plato> pilaplatosLimpios; // Pila para platos limpios
    int contadorPlatos; // Contador para generar IDs únicos de platos
    int platosLavados; // Contador de platos lavados

public:
    LavadoPlatos() : contadorPlatos(1), platosLavados(0) {}

    // Método para añadir un nuevo plato sucio a la pila
    void agregarPlatoSucio(const string& tipo) {
        Plato nuevoPlato(contadorPlatos++, tipo);
        pilaPlatosSucios.push(nuevoPlato);
        cout << "Plato agregado a la pila: " << nuevoPlato << endl;
    }

    // Método para lavar el plato que está en la cima
    bool lavarPlato() {
        Plato plato;
        if (pilaPlatosSucios.pop(plato)) {
            cout << "Lavando: " << plato << endl;
            plato.limpiar();
            pilaplatosLimpios.push(plato);
            platosLavados++;
            cout << "Plato lavado y agregado a la pila de limpios: " << plato << endl;
            return true;
        }
        else {
            cout << "No hay platos sucios para lavar." << endl;
            return false;
        }
    }

    // Método para ver el próximo plato a lavar sin retirarlo
    bool verProximoPlatoALavar() {
        Plato plato;
        if (pilaPlatosSucios.peek(plato)) {
            cout << "Proximo plato a lavar: " << plato << endl;
            return true;
        }
        else {
            cout << "No hay platos sucios en la pila." << endl;
            return false;
        }
    }

    // Método para usar un plato limpio (lo retira de la pila de limpios)
    bool usarPlatoLimpio() {
        Plato plato;
        if (pilaplatosLimpios.pop(plato)) {
            cout << "Plato limpio utilizado: " << plato << endl;
            return true;
        }
        else {
            cout << "No hay platos limpios disponibles." << endl;
            return false;
        }
    }

    // Método para ver el próximo plato limpio a usar
    bool verProximoPlatoLimpio() {
        Plato plato;
        if (pilaplatosLimpios.peek(plato)) {
            cout << "Proximo plato limpio a usar: " << plato << endl;
            return true;
        }
        else {
            cout << "No hay platos limpios disponibles." << endl;
            return false;
        }
    }

    // Método para ver todos los platos en la pila de sucios
    void verPilaSucios() const {
        cout << "Estado actual de la pila de platos sucios:" << endl;
        pilaPlatosSucios.print();
        cout << "Total de platos sucios: " << pilaPlatosSucios.getSize() << endl;
    }

    // Método para ver todos los platos en la pila de limpios
    void verPilaLimpios() const {
        cout << "Estado actual de la pila de platos limpios:" << endl;
        pilaplatosLimpios.print();
        cout << "Total de platos limpios: " << pilaplatosLimpios.getSize() << endl;
    }

    // Método para mostrar estadísticas
    void mostrarEstadisticas() const {
        cout << "=== Estadisticas de Lavado de Platos ===" << endl;
        cout << "Total de platos en el sistema: " << (contadorPlatos - 1) << endl;
        cout << "Platos lavados: " << platosLavados << endl;
        cout << "Platos sucios pendientes: " << pilaPlatosSucios.getSize() << endl;
        cout << "Platos limpios disponibles: " << pilaplatosLimpios.getSize() << endl;
    }

    // Método para lavar todos los platos sucios
    void lavarTodos() {
        cout << "Lavando todos los platos sucios..." << endl;
        while (lavarPlato()) {
            // El método lavarPlato() ya imprime los mensajes necesarios
        }
        cout << "Todos los platos han sido lavados." << endl;
    }
};

// Programa principal para probar el sistema de lavado de platos
int main() {
    LavadoPlatos restaurante;

    // Añadir varios platos sucios
    restaurante.agregarPlatoSucio("Hondo");
    restaurante.agregarPlatoSucio("Llano");
    restaurante.agregarPlatoSucio("Postre");
    restaurante.agregarPlatoSucio("Hondo");

    cout << "\nEstado inicial de la pila de platos sucios:" << endl;
    restaurante.verPilaSucios();

    cout << "\nVerificando proximo plato a lavar:" << endl;
    restaurante.verProximoPlatoALavar();

    cout << "\nLavando platos:" << endl;
    restaurante.lavarPlato();
    restaurante.lavarPlato();

    cout << "\nEstado despues de lavar dos platos:" << endl;
    restaurante.verPilaSucios();
    restaurante.verPilaLimpios();

    cout << "\nUsando un plato limpio:" << endl;
    restaurante.usarPlatoLimpio();

    cout << "\nEstado de la pila de platos limpios despues de usar uno:" << endl;
    restaurante.verPilaLimpios();

    // Añadir más platos sucios
    cout << "\nAgregando más platos sucios:" << endl;
    restaurante.agregarPlatoSucio("Llano");

    cout << "\nLavando todos los platos restantes:" << endl;
    restaurante.lavarTodos();

    cout << "\nEstadisticas finales:" << endl;
    restaurante.mostrarEstadisticas();

    return 0;
}
