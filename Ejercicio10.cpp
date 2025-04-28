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

// Clase para representar una consulta de soporte técnico
class Consulta {
private:
    int id;
    string nombreCliente;
    string descripcionProblema;
    string fechaHora;

public:
    Consulta() : id(0), nombreCliente(""), descripcionProblema(""), fechaHora("") {}

    Consulta(int id, const string& nombre, const string& descripcion, const string& fecha)
        : id(id), nombreCliente(nombre), descripcionProblema(descripcion), fechaHora(fecha) {
    }

    int getId() const {
        return id;
    }

    const string& getNombreCliente() const {
        return nombreCliente;
    }

    const string& getDescripcionProblema() const {
        return descripcionProblema;
    }

    const string& getFechaHora() const {
        return fechaHora;
    }

    // Sobrecarga del operador << para poder imprimir consultas fácilmente
    friend ostream& operator<<(ostream& os, const Consulta& consulta) {
        os << "ID: " << consulta.id
            << ", Cliente: " << consulta.nombreCliente
            << ", Problema: " << consulta.descripcionProblema
            << ", Fecha/Hora: " << consulta.fechaHora;
        return os;
    }
};

// Clase para gestionar el soporte técnico
class GestionSoporteTecnico {
private:
    Queue<Consulta> filaConsultas; // Cola para almacenar las consultas
    int contadorId; // Contador para generar IDs únicos

public:
    GestionSoporteTecnico() : contadorId(1) {}

    // Método para registrar una nueva consulta
    void registrarConsulta(const string& nombreCliente, const string& descripcionProblema, const string& fechaHora) {
        Consulta nuevaConsulta(contadorId++, nombreCliente, descripcionProblema, fechaHora);
        filaConsultas.push(nuevaConsulta);
        cout << "Consulta registrada: " << nuevaConsulta << endl;
    }

    // Método para atender la siguiente consulta
    bool atenderConsulta() {
        Consulta consulta;
        if (filaConsultas.pop(consulta)) {
            cout << "Consulta atendida: " << consulta << endl;
            return true;
        }
        else {
            cout << "No hay consultas pendientes por atender." << endl;
            return false;
        }
    }

    // Método para ver la próxima consulta a atender sin retirarla
    bool verProximaConsulta() {
        Consulta consulta;
        if (filaConsultas.peek(consulta)) {
            cout << "Proxima consulta a atender: " << consulta << endl;
            return true;
        }
        else {
            cout << "No hay consultas en la fila." << endl;
            return false;
        }
    }

    // Método para ver todas las consultas en la fila
    void verTodasConsultas() const {
        cout << "Estado actual de la fila de consultas:" << endl;
        filaConsultas.print();
        cout << "Total de consultas pendientes: " << filaConsultas.getSize() << endl;
    }

    // Método para obtener el número de consultas pendientes
    int getConsultasPendientes() const {
        return filaConsultas.getSize();
    }
};

// Programa principal para probar la gestión de soporte técnico
int main() {
    GestionSoporteTecnico soporte;

    cout << "=== SISTEMA DE GESTION DE CONSULTAS DE SOPORTE TECNICO ===" << endl << endl;

    // Registrar varias consultas
    soporte.registrarConsulta("Juan Perez", "No puedo acceder a mi correo", "2025-04-27 09:15");
    soporte.registrarConsulta("María Lopez", "La impresora no responde", "2025-04-27 09:30");
    soporte.registrarConsulta("Carlos Rodriguez", "Error en el sistema de facturacion", "2025-04-27 09:45");
    soporte.registrarConsulta("Ana Garcia", "Necesito restablecer mi contrasena", "2025-04-27 10:00");

    cout << "\nEstado inicial de la fila de consultas:" << endl;
    soporte.verTodasConsultas();

    cout << "\nVerificando proxima consulta a atender:" << endl;
    soporte.verProximaConsulta();

    cout << "\nAtendiendo consultas:" << endl;
    soporte.atenderConsulta();
    soporte.atenderConsulta();

    cout << "\nEstado de la fila despues de atender dos consultas:" << endl;
    soporte.verTodasConsultas();

    // Registrar más consultas
    cout << "\nRegistrando nueva consulta:" << endl;
    soporte.registrarConsulta("Roberto Sanchez", "Problemas con la VPN", "2025-04-27 10:30");

    cout << "\nEstado final de la fila de consultas:" << endl;
    soporte.verTodasConsultas();

    // Atender todas las consultas restantes
    cout << "\nAtendiendo todas las consultas restantes:" << endl;
    while (soporte.getConsultasPendientes() > 0) {
        soporte.atenderConsulta();
    }

    // Verificar que no quedan consultas
    cout << "\nVerificando estado final:" << endl;
    soporte.verTodasConsultas();
    soporte.verProximaConsulta();

    return 0;
}
