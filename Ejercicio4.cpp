#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <stdexcept>

using namespace std;

template<typename T>
class Queue {
private:
    class Node {
    public:
        T data;
        Node* next;

        Node(const T& value, Node* next = nullptr) : data(value), next(next) {}
    };

    Node* front;
    Node* back;
    int count;

public:
    Queue() : front(nullptr), back(nullptr), count(0) {}

    ~Queue() {
        while (front != nullptr) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }

    void push(const T& value) {
        Node* newNode = new Node(value);
        if (back == nullptr) {
            front = back = newNode;
        }
        else {
            back->next = newNode;
            back = newNode;
        }
        count++;
    }

    bool pop(T& value) {
        if (front == nullptr) {
            return false;
        }
        value = front->data;
        Node* temp = front;
        front = front->next;
        if (front == nullptr) {
            back = nullptr;
        }
        delete temp;
        count--;
        return true;
    }

    bool peek(T& value) const {
        if (front == nullptr) {
            return false;
        }
        value = front->data;
        return true;
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    int getSize() const {
        return count;
    }

    void print() const {
        Node* current = front;
        while (current != nullptr) {
            cout << current->data << " -> ";
            current = current->next;
        }
        cout << "NULL" << endl;
    }
};

#endif // QUEUE_H#pragma once


#include <iostream>
#include <string>
#include "Queue.h" // Incluimos la clase Queue proporcionada

using namespace std;

// Clase para representar un trabajo de impresión
class TrabajoImpresion {
private:
    string nombreArchivo;
    int numeroPaginas;
    string usuario;
    int idTrabajo; // Identificador único del trabajo

public:
    TrabajoImpresion() : nombreArchivo(""), numeroPaginas(0), usuario(""), idTrabajo(0) {}

    TrabajoImpresion(const string& archivo, int paginas, const string& user, int id)
        : nombreArchivo(archivo), numeroPaginas(paginas), usuario(user), idTrabajo(id) {
    }

    const string& getNombreArchivo() const {
        return nombreArchivo;
    }

    int getNumeroPaginas() const {
        return numeroPaginas;
    }

    const string& getUsuario() const {
        return usuario;
    }

    int getIdTrabajo() const {
        return idTrabajo;
    }

    // Sobrecarga del operador << para poder imprimir trabajos fácilmente
    friend ostream& operator<<(ostream& os, const TrabajoImpresion& trabajo) {
        os << "ID: " << trabajo.idTrabajo
            << " - Archivo: " << trabajo.nombreArchivo
            << " (" << trabajo.numeroPaginas << " paginas)"
            << " - Usuario: " << trabajo.usuario;
        return os;
    }
};

// Clase para gestionar la cola de impresión
class GestorImpresion {
private:
    Queue<TrabajoImpresion> colaImpresion; // Cola para los trabajos
    int contadorTrabajos; // Contador para asignar IDs a los trabajos
    int paginasImpresas; // Contador de páginas impresas
    int trabajosCompletados; // Contador de trabajos completados

public:
    GestorImpresion() : contadorTrabajos(1), paginasImpresas(0), trabajosCompletados(0) {}

    // Método para añadir un nuevo trabajo a la cola
    void agregarTrabajo(const string& nombreArchivo, int numeroPaginas, const string& usuario) {
        TrabajoImpresion nuevoTrabajo(nombreArchivo, numeroPaginas, usuario, contadorTrabajos++);
        colaImpresion.push(nuevoTrabajo);
        cout << "Trabajo adicionado a la cola: " << nuevoTrabajo << endl;
    }

    // Método para imprimir el siguiente trabajo
    bool imprimirSiguiente() {
        TrabajoImpresion trabajo;
        if (colaImpresion.pop(trabajo)) {
            cout << "Imprimiendo: " << trabajo << endl;
            cout << "Progreso: ";

            // Simulación del proceso de impresión
            for (int i = 1; i <= trabajo.getNumeroPaginas(); i++) {
                cout << "Pagina " << i << " de " << trabajo.getNumeroPaginas() << " impresa\r";
                cout.flush();
                // En un sistema real, aquí habría un delay
            }

            cout << endl << "Trabajo completado!" << endl;

            paginasImpresas += trabajo.getNumeroPaginas();
            trabajosCompletados++;
            return true;
        }
        else {
            cout << "No hay trabajos pendientes en la cola." << endl;
            return false;
        }
    }

    // Método para ver el siguiente trabajo sin procesarlo
    bool verSiguienteTrabajo() {
        TrabajoImpresion trabajo;
        if (colaImpresion.peek(trabajo)) {
            cout << "Siguiente trabajo a imprimir: " << trabajo << endl;
            return true;
        }
        else {
            cout << "No hay trabajos en la cola de impresion." << endl;
            return false;
        }
    }

    // Método para ver todos los trabajos en la cola
    void verColaImpresion() const {
        cout << "Estado actual de la cola de impresion:" << endl;
        colaImpresion.print();
        cout << "Total de trabajos en cola: " << colaImpresion.getSize() << endl;
    }

    // Método para mostrar estadísticas
    void mostrarEstadisticas() const {
        cout << "=== Estadisticas de la Impresora ===" << endl;
        cout << "Trabajos completados: " << trabajosCompletados << endl;
        cout << "Paginas impresas: " << paginasImpresas << endl;
        cout << "Trabajos pendientes: " << colaImpresion.getSize() << endl;
    }
};

// Programa principal para probar el gestor de impresión
int main() {
    GestorImpresion impresora;

    // Añadir varios trabajos de impresión
    impresora.agregarTrabajo("Informe.pdf", 5, "Juan");
    impresora.agregarTrabajo("Presentacion.pptx", 12, "Maria");
    impresora.agregarTrabajo("Tesis.docx", 50, "Carlos");
    impresora.agregarTrabajo("Factura.pdf", 2, "Ana");

    cout << "\nEstado inicial de la cola de impresion:" << endl;
    impresora.verColaImpresion();

    cout << "\nVerificando siguiente trabajo a imprimir:" << endl;
    impresora.verSiguienteTrabajo();

    cout << "\nImprimiendo trabajos:" << endl;
    impresora.imprimirSiguiente();
    impresora.imprimirSiguiente();

    cout << "\nEstado de la cola despues de imprimir dos trabajos:" << endl;
    impresora.verColaImpresion();

    // Añadir más trabajos
    cout << "\nAdicionando mas trabajos:" << endl;
    impresora.agregarTrabajo("Imagen.jpg", 1, "Pedro");

    cout << "\nEstadisticas actuales:" << endl;
    impresora.mostrarEstadisticas();

    return 0;
}
