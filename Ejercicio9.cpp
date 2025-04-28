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
#include "Queue.h" // Se incluye la clase Queue

using namespace std;

// Clase para representar un pasajero
class Pasajero {
private:
    string nombre;
    string destino;
    int numeroFila; // Número asignado al pasajero en la fila

public:
    Pasajero() : nombre(""), destino(""), numeroFila(0) {}

    Pasajero(const string& n, const string& d, int num)
        : nombre(n), destino(d), numeroFila(num) {
    }

    const string& getNombre() const {
        return nombre;
    }

    const string& getDestino() const {
        return destino;
    }

    int getNumeroFila() const {
        return numeroFila;
    }

    // Sobrecarga del operador << para poder imprimir pasajeros fácilmente
    friend ostream& operator<<(ostream& os, const Pasajero& pasajero) {
        os << "Pasajero #" << pasajero.numeroFila
            << " - Nombre: " << pasajero.nombre
            << ", Destino: " << pasajero.destino;
        return os;
    }
};

// Clase para gestionar la cola de pasajeros en la estación de tren
class EstacionTren {
private:
    Queue<Pasajero> filaPasajeros; // Cola para la fila de pasajeros
    int contadorFilas; // Contador para asignar números a los pasajeros
    int pasajerosAtendidos; // Contador de pasajeros ya atendidos  

public:
    EstacionTren() : contadorFilas(1), pasajerosAtendidos(0) {
    }

    // Método para que un pasajero se una a la fila
    void unirseAFila(const string& nombre, const string& destino) {
        Pasajero nuevoPasajero(nombre, destino, contadorFilas++);
        filaPasajeros.push(nuevoPasajero);
        cout << "Pasajero registrado en la fila: " << nuevoPasajero << endl;
    }

    // Método para atender al siguiente pasajero
    bool atenderPasajero() {
        Pasajero pasajero;
        if (filaPasajeros.pop(pasajero)) {
            cout << "Atendiendo a: " << pasajero << endl;
            cout << "Vendiendo boleto para " << pasajero.getDestino() << "..." << endl;
            cout << "Transaccion completada." << endl;
            pasajerosAtendidos++;
            return true;
        }
        else {
            cout << "No hay pasajeros en la fila de espera." << endl;
            return false;
        }
    }

    // Método para ver quién es el próximo pasajero en la fila
    bool verProximoPasajero() {
        Pasajero pasajero;
        if (filaPasajeros.peek(pasajero)) {
            cout << "Proximo pasajero a atender: " << pasajero << endl;
            return true;
        }
        else {
            cout << "No hay pasajeros en la fila de espera." << endl;
            return false;
        }
    }


    // Método para ver toda la fila de pasajeros
    void verFilaCompleta() const {
        cout << "Estado actual de la fila de pasajeros:" << endl;
        filaPasajeros.print();
        cout << "Total de pasajeros en la fila: " << filaPasajeros.getSize() << endl;
    }

    // Método para obtener estadísticas
    void estadisticas() const {
        cout << "=== Estadisticas de la Estacion de Tren ===" << endl;
        cout << "Total de pasajeros que han llegado hoy: " << (contadorFilas - 1) << endl;
        cout << "Pasajeros atendidos: " << pasajerosAtendidos << endl;
        cout << "Pasajeros en fila de espera: " << filaPasajeros.getSize() << endl;
    }
};

int main() {
    EstacionTren estacion;

    // Registrar varios pasajeros automáticamente
    estacion.unirseAFila("Ana", "Cochabamba");
    estacion.unirseAFila("Luis", "La Paz");
    estacion.unirseAFila("Sofia", "Oruro");
    estacion.unirseAFila("Carlos", "Santa Cruz");
    estacion.unirseAFila("Elena", "Sucre");

    cout << "\nEstado inicial de la fila de pasajeros:" << endl;
    estacion.verFilaCompleta();

    cout << "\nVerificando proximo pasajero a atender:" << endl;
    estacion.verProximoPasajero();

    cout << "\nAtendiendo a los dos primeros pasajeros:" << endl;
    estacion.atenderPasajero();
    estacion.atenderPasajero();

    cout << "\nEstado de la fila despues de atender dos pasajeros:" << endl;
    estacion.verFilaCompleta();


    cout << "\nAtendiendo a todos los pasajeros restantes:" << endl;
    while (estacion.atenderPasajero()) {
        // Se atienden todos los pasajeros hasta que la fila esté vacía
    }

    cout << "\nIntentando atender cuando ya no hay pasajeros:" << endl;
    estacion.atenderPasajero();

    cout << "\nEstadisticas finales del dia:" << endl;
    estacion.estadisticas();

    return 0;
}
