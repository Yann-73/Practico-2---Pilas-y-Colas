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

// Clase para representar un paciente
class Paciente {
private:
    string nombre;
    int edad;
    string motivoConsulta;
    int numeroPaciente; // Número asignado al paciente para identificarlo

public:
    Paciente() : nombre(""), edad(0), motivoConsulta(""), numeroPaciente(0) {}

    Paciente(const string& n, int e, const string& motivo, int num)
        : nombre(n), edad(e), motivoConsulta(motivo), numeroPaciente(num) {
    }

    const string& getNombre() const {
        return nombre;
    }

    int getEdad() const {
        return edad;
    }

    const string& getMotivoConsulta() const {
        return motivoConsulta;
    }

    int getNumeroPaciente() const {
        return numeroPaciente;
    }

    // Sobrecarga del operador << para poder imprimir pacientes fácilmente
    friend ostream& operator<<(ostream& os, const Paciente& paciente) {
        os << "Paciente #" << paciente.numeroPaciente
            << " - Nombre: " << paciente.nombre
            << ", Edad: " << paciente.edad
            << ", Motivo: " << paciente.motivoConsulta;
        return os;
    }
};

// Clase para gestionar la sala de espera de la clínica
class SalaDeEspera {
private:
    Queue<Paciente> filaPacientes; // Cola para la fila de pacientes
    int contadorPacientes; // Contador para asignar números a los pacientes
    int pacientesAtendidos; // Contador de pacientes ya atendidos

public:
    SalaDeEspera() : contadorPacientes(1), pacientesAtendidos(0) {}

    // Método para registrar un nuevo paciente
    void registrarPaciente(const string& nombre, int edad, const string& motivoConsulta) {
        Paciente nuevoPaciente(nombre, edad, motivoConsulta, contadorPacientes++);
        filaPacientes.push(nuevoPaciente);
        cout << "Paciente registrado: " << nuevoPaciente << endl;
    }

    // Método para atender al siguiente paciente
    bool atenderPaciente() {
        Paciente paciente;
        if (filaPacientes.pop(paciente)) {
            cout << "Atendiendo a: " << paciente << endl;
            pacientesAtendidos++;
            return true;
        }
        else {
            cout << "No hay pacientes en espera." << endl;
            return false;
        }
    }

    // Método para ver quién es el próximo paciente a atender
    bool verProximoPaciente() {
        Paciente paciente;
        if (filaPacientes.peek(paciente)) {
            cout << "Proximo paciente a atender: " << paciente << endl;
            return true;
        }
        else {
            cout << "No hay pacientes en espera." << endl;
            return false;
        }
    }

    // Método para ver todos los pacientes en espera
    void verSalaDeEspera() const {
        cout << "Estado actual de la sala de espera (FIFO):" << endl;
        filaPacientes.print();
        cout << "Total de pacientes en espera: " << filaPacientes.getSize() << endl;
        cout << "Total de pacientes atendidos hoy: " << pacientesAtendidos << endl;
    }

    // Método para obtener estadísticas
    void estadisticas() const {
        cout << "=== Estadisticas de la Clinica ===" << endl;
        cout << "Total de pacientes registrados hoy: " << (contadorPacientes - 1) << endl;
        cout << "Pacientes atendidos: " << pacientesAtendidos << endl;
        cout << "Pacientes en espera: " << filaPacientes.getSize() << endl;
    }
};

// Programa principal para probar la sala de espera
int main() {
    SalaDeEspera clinica;

    // Registrar varios pacientes
    clinica.registrarPaciente("Juan Perez", 45, "Dolor de espalda");
    clinica.registrarPaciente("Maria Garcia", 35, "Revision rutinaria");
    clinica.registrarPaciente("Carlos Lopez", 28, "Dolor de cabeza");
    clinica.registrarPaciente("Ana Martinez", 60, "Problemas respiratorios");

    cout << "\nEstado inicial de la sala de espera:" << endl;
    clinica.verSalaDeEspera();

    cout << "\nVerificando proximo paciente a atender:" << endl;
    clinica.verProximoPaciente();

    cout << "\nAtendiendo pacientes:" << endl;
    clinica.atenderPaciente();
    clinica.atenderPaciente();

    cout << "\nEstado de la sala despues de atender dos pacientes:" << endl;
    clinica.verSalaDeEspera();

    // Registrar más pacientes
    cout << "\nRegistrando mas pacientes:" << endl;
    clinica.registrarPaciente("Laura Sanchez", 52, "Control de diabetes");

    cout << "\nEstado final de la sala de espera:" << endl;
    clinica.verSalaDeEspera();

    cout << "\nEstadisticas finales:" << endl;
    clinica.estadisticas();

    return 0;
}
