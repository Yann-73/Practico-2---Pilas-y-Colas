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

// Clase para representar un cliente del banco
class Cliente {
private:
    string nombre;
    string tipoTransaccion;
    int numeroTurno; // Número asignado al cliente para identificarlo

public:
    Cliente() : nombre(""), tipoTransaccion(""), numeroTurno(0) {}

    Cliente(const string& n, const string& transaccion, int num)
        : nombre(n), tipoTransaccion(transaccion), numeroTurno(num) {
    }

    const string& getNombre() const {
        return nombre;
    }

    const string& getTipoTransaccion() const {
        return tipoTransaccion;
    }

    int getNumeroTurno() const {
        return numeroTurno;
    }

    // Sobrecarga del operador << para poder imprimir clientes fácilmente
    friend ostream& operator<<(ostream& os, const Cliente& cliente) {
        os << "Cliente #" << cliente.numeroTurno
            << " - Nombre: " << cliente.nombre
            << ", Transaccion: " << cliente.tipoTransaccion;
        return os;
    }
};

// Clase para gestionar los turnos en el banco
class GestorBanco {
private:
    Queue<Cliente> filaClientes; // Cola para la fila de clientes
    int contadorTurnos; // Contador para asignar números a los clientes
    int clientesAtendidos; // Contador de clientes ya atendidos

public:
    GestorBanco() : contadorTurnos(1), clientesAtendidos(0) {}

    // Método para registrar un nuevo cliente
    void registrarCliente(const string& nombre, const string& tipoTransaccion) {
        Cliente nuevoCliente(nombre, tipoTransaccion, contadorTurnos++);
        filaClientes.push(nuevoCliente);
        cout << "Cliente registrado: " << nuevoCliente << endl;
    }

    // Método para atender al siguiente cliente
    bool atenderCliente() {
        Cliente cliente;
        if (filaClientes.pop(cliente)) {
            cout << "Atendiendo a: " << cliente << endl;
            clientesAtendidos++;
            return true;
        }
        else {
            cout << "No hay clientes en espera." << endl;
            return false;
        }
    }

    // Método para ver quién es el próximo cliente a atender
    bool verProximoCliente() {
        Cliente cliente;
        if (filaClientes.peek(cliente)) {
            cout << "Proximo cliente a atender: " << cliente << endl;
            return true;
        }
        else {
            cout << "No hay clientes en espera." << endl;
            return false;
        }
    }

    // Método para ver todos los clientes en espera
    void verFilaDeEspera() const {
        cout << "Estado actual de la fila de espera:" << endl;
        filaClientes.print();
        cout << "Total de clientes en espera: " << filaClientes.getSize() << endl;
        cout << "Total de clientes atendidos hoy: " << clientesAtendidos << endl;
    }

    // Método para obtener estadísticas
    void estadisticas() const {
        cout << "=== Estadisticas del Banco ===" << endl;
        cout << "Total de turnos emitidos hoy: " << (contadorTurnos - 1) << endl;
        cout << "Clientes atendidos: " << clientesAtendidos << endl;
        cout << "Clientes en espera: " << filaClientes.getSize() << endl;
    }
};

int main() {
    GestorBanco banco;

    banco.registrarCliente("Ana", "Deposito");
    banco.registrarCliente("Luis", "Retiro");
    banco.registrarCliente("Sara", "Consulta");
    banco.registrarCliente("Carlos", "Pago de servicios");
    banco.registrarCliente("Elena", "Deposito");

    cout << "\nEstado inicial de la fila de espera:" << endl;
    banco.verFilaDeEspera();

    cout << "\nVerificando proximo cliente a atender:" << endl;
    banco.verProximoCliente();

    cout << "\nAtendiendo a los dos primeros clientes:" << endl;
    banco.atenderCliente();
    banco.atenderCliente();

    cout << "\nEstado de la fila despues de atender dos clientes:" << endl;
    banco.verFilaDeEspera();

    cout << "\nAtendiendo a todos los clientes restantes:" << endl;
    while (banco.atenderCliente()) {
        // Se atienden todos los clientes hasta que la fila esté vacía
    }

    cout << "\nIntentando atender cuando ya no hay clientes:" << endl;
    banco.atenderCliente();

    cout << "\nEstadisticas finales del dia:" << endl;
    banco.estadisticas();

    return 0;
}
