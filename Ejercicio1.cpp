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

// Clase para representar un paquete de ropa
class Paquete {
private:
    int identificador;
    string tipoPrenda;

public:
    Paquete() : identificador(0), tipoPrenda("") {}
    
    Paquete(int id, const string& tipo) : identificador(id), tipoPrenda(tipo) {}
    
    int getIdentificador() const {
        return identificador;
    }
    
    const string& getTipoPrenda() const {
        return tipoPrenda;
    }
    
    // Sobrecarga del operador << para poder imprimir paquetes fácilmente
    friend ostream& operator<<(ostream& os, const Paquete& paquete) {
        os << "ID: " << paquete.identificador << ", Tipo: " << paquete.tipoPrenda;
        return os;
    }
};

// Clase para gestionar el inventario de la tienda
class GestionInventario {
private:
    Queue<Paquete> almacen; // Cola para almacenar los paquetes
    int contadorId; // Contador para generar IDs únicos

public:
    GestionInventario() : contadorId(1) {}
    
    // Método para recibir un nuevo paquete
    void recibirPaquete(const string& tipoPrenda) {
        Paquete nuevoPaquete(contadorId++, tipoPrenda);
        almacen.push(nuevoPaquete);
        cout << "Paquete recibido: " << nuevoPaquete << endl;
    }
    
    // Método para procesar el siguiente paquete
    bool procesarPaquete() {
        Paquete paquete;
        if (almacen.pop(paquete)) {
            cout << "Paquete procesado: " << paquete << endl;
            return true;
        } else {
            cout << "No hay paquetes para procesar." << endl;
            return false;
        }
    }
    
    // Método para ver el próximo paquete a procesar sin retirarlo
    bool verProximoPaquete() {
        Paquete paquete;
        if (almacen.peek(paquete)) {
            cout << "Próximo paquete a procesar: " << paquete << endl;
            return true;
        } else {
            cout << "No hay paquetes en el almacén." << endl;
            return false;
        }
    }
    
    // Método para ver todos los paquetes en el almacén
    void verAlmacen() const {
        cout << "Estado actual del almacén (primero en entrar -> primero en salir):" << endl;
        almacen.print();
        cout << "Total de paquetes: " << almacen.getSize() << endl;
    }
};

// Programa principal para probar la gestión de inventario
int main() {
    GestionInventario tienda;
    
    // Recibir varios paquetes
    tienda.recibirPaquete("Camisas");
    tienda.recibirPaquete("Pantalones");
    tienda.recibirPaquete("Vestidos");
    tienda.recibirPaquete("Chaquetas");
    
    cout << "\nEstado inicial del almacén:" << endl;
    tienda.verAlmacen();
    
    cout << "\nVerificando próximo paquete a procesar:" << endl;
    tienda.verProximoPaquete();
    
    cout << "\nProcesando paquetes:" << endl;
    tienda.procesarPaquete();
    tienda.procesarPaquete();
    
    cout << "\nEstado del almacén después de procesar dos paquetes:" << endl;
    tienda.verAlmacen();
    
    // Recibir más paquetes
    cout << "\nRecibiendo más paquetes:" << endl;
    tienda.recibirPaquete("Zapatos");
    
    cout << "\nEstado final del almacén:" << endl;
    tienda.verAlmacen();
    
    return 0;
}
