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
#include <iomanip> // Para formatear la salida decimal
#include "LinkedList.h" // Incluimos la clase LinkedList proporcionada
using namespace std;

// Clase para representar un examen entregado
class Examen {
private:
    int idEstudiante;
    string nombreEstudiante;
    double nota;

public:
    Examen() : idEstudiante(0), nombreEstudiante(""), nota(0.0) {}

    Examen(int id, const string& nombre, double calificacion)
        : idEstudiante(id), nombreEstudiante(nombre), nota(calificacion) {
        // Aseguramos que la nota esté en el rango válido
        if (nota < 0.0) nota = 0.0;
        if (nota > 10.0) nota = 10.0;
    }

    int getIdEstudiante() const {
        return idEstudiante;
    }

    const string& getNombreEstudiante() const {
        return nombreEstudiante;
    }

    double getNota() const {
        return nota;
    }

    // Sobrecarga del operador << para poder imprimir exámenes fácilmente
    friend ostream& operator<<(ostream& os, const Examen& examen) {
        os << "ID: " << examen.idEstudiante
            << ", Estudiante: " << examen.nombreEstudiante
            << ", Nota: " << fixed << setprecision(2) << examen.nota;
        return os;
    }
};

// Clase para gestionar los exámenes y calcular promedios
class GestionExamenes {
private:
    LinkedList<Examen> examenes; // Lista enlazada para almacenar los exámenes
    int contadorId; // Contador para generar IDs únicos

public:
    GestionExamenes() : contadorId(1) {}

    // Método para registrar un nuevo examen
    void registrarExamen(const string& nombreEstudiante, double nota) {
        Examen nuevoExamen(contadorId++, nombreEstudiante, nota);
        examenes.pushBack(nuevoExamen); // Añadimos al final para mantener el orden de entrega
        cout << "Examen registrado: " << nuevoExamen << endl;
    }

    // Método para calcular el promedio de todas las notas
    double calcularPromedio() {
        if (examenes.getSize() == 0) {
            cout << "No hay examenes registrados para calcular el promedio." << endl;
            return 0.0;
        }

        double sumaNotas = 0.0;
        int cantidadExamenes = examenes.getSize();

        // Recorremos todos los exámenes para calcular la suma
        for (int i = 0; i < cantidadExamenes; i++) {
            sumaNotas += examenes.at(i).getNota();
        }

        // Calculamos y devolvemos el promedio
        return sumaNotas / cantidadExamenes;
    }

    // Método para ver todos los exámenes registrados
    void verTodosExamenes() const {
        cout << "Examenes registrados:" << endl;
        examenes.print();
        cout << "Total de examenes: " << examenes.getSize() << endl;
    }

    // Método para obtener el número de exámenes registrados
    int getNumeroExamenes() const {
        return examenes.getSize();
    }

    // Método para encontrar la nota más alta
    Examen encontrarMejorNota() {
        if (examenes.getSize() == 0) {
            return Examen(); // Devolver un examen vacío
        }

        int mejorIndice = 0;
        double mejorNota = examenes.at(0).getNota();

        for (int i = 1; i < examenes.getSize(); i++) {
            if (examenes.at(i).getNota() > mejorNota) {
                mejorNota = examenes.at(i).getNota();
                mejorIndice = i;
            }
        }

        return examenes.at(mejorIndice);
    }

    // Método para encontrar la nota más baja
    Examen encontrarPeorNota() {
        if (examenes.getSize() == 0) {
            return Examen(); // Devolver un examen vacío
        }

        int peorIndice = 0;
        double peorNota = examenes.at(0).getNota();

        for (int i = 1; i < examenes.getSize(); i++) {
            if (examenes.at(i).getNota() < peorNota) {
                peorNota = examenes.at(i).getNota();
                peorIndice = i;
            }
        }

        return examenes.at(peorIndice);
    }

    // Método para generar un reporte detallado con el promedio
    void generarReporte() {
        if (examenes.getSize() == 0) {
            cout << "No hay examenes para generar el reporte." << endl;
            return;
        }

        double promedio = calcularPromedio();
        Examen mejorExamen = encontrarMejorNota();
        Examen peorExamen = encontrarPeorNota();

        cout << "\n===== REPORTE DE CALIFICACIONES =====" << endl;
        cout << "Total de examenes evaluados: " << getNumeroExamenes() << endl;
        cout << "Promedio de la clase: " << fixed << setprecision(2) << promedio << endl;
        cout << "Nota mas alta: " << fixed << setprecision(2) << mejorExamen.getNota()
            << " (" << mejorExamen.getNombreEstudiante() << ")" << endl;
        cout << "Nota mas baja: " << fixed << setprecision(2) << peorExamen.getNota()
            << " (" << peorExamen.getNombreEstudiante() << ")" << endl;

        // Contador para exámenes por encima y por debajo del promedio
        int encimaDel7 = 0;
        int porDebajoPromedio = 0;
        int porEncimaPromedio = 0;

        for (int i = 0; i < examenes.getSize(); i++) {
            double nota = examenes.at(i).getNota();

            if (nota >= 7.0) {
                encimaDel7++;
            }

            if (nota < promedio) {
                porDebajoPromedio++;
            }
            else if (nota > promedio) {
                porEncimaPromedio++;
            }
        }

        cout << "Estudiantes con nota >= 7.0: " << encimaDel7 << " ("
            << fixed << setprecision(1) << (encimaDel7 * 100.0 / getNumeroExamenes()) << "%)" << endl;
        cout << "Estudiantes por encima del promedio: " << porEncimaPromedio << " ("
            << fixed << setprecision(1) << (porEncimaPromedio * 100.0 / getNumeroExamenes()) << "%)" << endl;
        cout << "Estudiantes por debajo del promedio: " << porDebajoPromedio << " ("
            << fixed << setprecision(1) << (porDebajoPromedio * 100.0 / getNumeroExamenes()) << "%)" << endl;
        cout << "===================================" << endl;
    }
};

// Programa principal para probar el cálculo de promedio de notas
int main() {
    GestionExamenes profesor;

    cout << "=== SISTEMA DE CALCULO DE PROMEDIO DE NOTAS ===" << endl << endl;

    // Registrar exámenes de varios estudiantes
    profesor.registrarExamen("Ana Garcia", 8.5);
    profesor.registrarExamen("Luis Hernandez", 6.7);
    profesor.registrarExamen("Maria Lopez", 9.2);
    profesor.registrarExamen("Juan Tapia", 7.8);
    profesor.registrarExamen("Carlos Rodriguez", 5.6);
    profesor.registrarExamen("Sofia Martinez", 9.0);
    profesor.registrarExamen("Pablo Torres", 4.3);
    profesor.registrarExamen("Elena Castro", 7.5);

    cout << "\nExamenes registrados:" << endl;
    profesor.verTodosExamenes();

    // Calcular y mostrar el promedio
    double promedio = profesor.calcularPromedio();
    cout << "\nEl promedio de todas las notas es: " << fixed << setprecision(2) << promedio << endl;

    // Generar un reporte más detallado
    profesor.generarReporte();

    // Registrar un examen adicional y recalcular
    cout << "\nRegistrando un examen adicional:" << endl;
    profesor.registrarExamen("Roberto Reyes", 8.8);

    cout << "\nNuevo promedio despues de agregar un examen: "
        << fixed << setprecision(2) << profesor.calcularPromedio() << endl;

    // Generar reporte final
    profesor.generarReporte();

    return 0;
}
