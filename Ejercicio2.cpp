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

// Clase para representar un libro
class Libro {
private:
    string titulo;
    string autor;
    string editorial;

public:
    Libro() : titulo(""), autor(""), editorial("") {}

    Libro(const string& t, const string& a, const string& e)
        : titulo(t), autor(a), editorial(e) {
    }

    const string& getTitulo() const {
        return titulo;
    }

    const string& getAutor() const {
        return autor;
    }

    const string& getEditorial() const {
        return editorial;
    }

    // Sobrecarga del operador << para poder imprimir libros fácilmente
    friend ostream& operator<<(ostream& os, const Libro& libro) {
        os << "Titulo: " << libro.titulo
            << ", Autor: " << libro.autor
            << ", Editorial: " << libro.editorial;
        return os;
    }
};

// Clase para gestionar donaciones de libros en la biblioteca
class GestionDonaciones {
private:
    Stack<Libro> estanteriaTemporal; // Pila para almacenar los libros donados
    int totalProcesados; // Contador de libros procesados

public:
    GestionDonaciones() : totalProcesados(0) {}

    // Método para recibir un nuevo libro donado
    void recibirLibro(const string& titulo, const string& autor, const string& editorial) {
        Libro nuevoLibro(titulo, autor, editorial);
        estanteriaTemporal.push(nuevoLibro);
        cout << "Libro donado recibido: " << nuevoLibro << endl;
    }

    // Método para registrar el siguiente libro (el último que llegó)
    bool registrarLibro() {
        Libro libro;
        if (estanteriaTemporal.pop(libro)) {
            cout << "Libro registrado: " << libro << endl;
            totalProcesados++;
            return true;
        }
        else {
            cout << "No hay libros pendientes para registrar." << endl;
            return false;
        }
    }

    // Método para ver el próximo libro a registrar sin retirarlo
    bool verProximoLibro() {
        Libro libro;
        if (estanteriaTemporal.peek(libro)) {
            cout << "Proximo libro a registrar: " << libro << endl;
            return true;
        }
        else {
            cout << "No hay libros en la estanteria temporal." << endl;
            return false;
        }
    }

    // Método para ver todos los libros en la estantería temporal
    void verEstanteriaTemporal() const {
        cout << "Estado actual de la estanteria temporal (LIFO):" << endl;
        estanteriaTemporal.print();
        cout << "Total de libros pendientes: " << estanteriaTemporal.getSize() << endl;
        cout << "Total de libros procesados: " << totalProcesados << endl;
    }

    // Método para procesar todos los libros pendientes
    void procesarTodosLosLibros() {
        cout << "Procesando todos los libros pendientes..." << endl;
        Libro libro;
        while (estanteriaTemporal.pop(libro)) {
            cout << "Libro registrado: " << libro << endl;
            totalProcesados++;
        }
        cout << "No quedan libros pendientes." << endl;
    }
};

// Programa principal para probar la gestión de donaciones
int main() {
    GestionDonaciones biblioteca;

    // Recibir varios libros donados
    biblioteca.recibirLibro("Don Quijote de la Mancha", "Miguel de Cervantes", "Editorial Planeta");
    biblioteca.recibirLibro("Cien anios de soledad", "Gabriel Garcia Marquez", "Editorial Sudamericana");
    biblioteca.recibirLibro("El principito", "Antoine de Saint-Exupery", "Salamandra");
    biblioteca.recibirLibro("1984", "George Orwell", "Debolsillo");

    cout << "\nEstado inicial de la estanteria temporal:" << endl;
    biblioteca.verEstanteriaTemporal();

    cout << "\nVerificando proximo libro a registrar:" << endl;
    biblioteca.verProximoLibro();

    cout << "\nRegistrando libros:" << endl;
    biblioteca.registrarLibro();
    biblioteca.registrarLibro();

    cout << "\nEstado de la estanteria despues de registrar dos libros:" << endl;
    biblioteca.verEstanteriaTemporal();

    // Recibir más libros
    cout << "\nRecibiendo mas libros:" << endl;
    biblioteca.recibirLibro("Harry Potter", "J.K. Rowling", "Salamandra");

    cout << "\nEstado actual de la estanteria:" << endl;
    biblioteca.verEstanteriaTemporal();

    cout << "\nProcesando todos los libros restantes:" << endl;
    biblioteca.procesarTodosLosLibros();

    cout << "\nEstado final de la estanteria:" << endl;
    biblioteca.verEstanteriaTemporal();

    return 0;
}
