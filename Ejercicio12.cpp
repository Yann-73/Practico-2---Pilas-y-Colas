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
#include <iomanip> // Para formatear la fecha
#include "Stack.h" // Incluimos la clase Stack proporcionada
using namespace std;

// Clase para representar un documento
class Documento {
private:
    int id;
    string titulo;
    string fechaRecepcion;
    string departamento;
    int prioridad; // 1-5 (5 es la más alta)

public:
    Documento() : id(0), titulo(""), fechaRecepcion(""), departamento(""), prioridad(1) {}

    Documento(int id, const string& titulo, const string& fecha, const string& depto, int prioridad)
        : id(id), titulo(titulo), fechaRecepcion(fecha), departamento(depto), prioridad(prioridad) {
        // Validación de prioridad
        if (this->prioridad < 1) this->prioridad = 1;
        if (this->prioridad > 5) this->prioridad = 5;
    }

    int getId() const {
        return id;
    }

    const string& getTitulo() const {
        return titulo;
    }

    const string& getFechaRecepcion() const {
        return fechaRecepcion;
    }

    const string& getDepartamento() const {
        return departamento;
    }

    int getPrioridad() const {
        return prioridad;
    }

    string getPrioridadTexto() const {
        switch (prioridad) {
        case 1: return "Muy baja";
        case 2: return "Baja";
        case 3: return "Media";
        case 4: return "Alta";
        case 5: return "Urgente";
        default: return "Desconocida";
        }
    }

    // Sobrecarga del operador << para poder imprimir documentos fácilmente
    friend ostream& operator<<(ostream& os, const Documento& doc) {
        os << "ID: " << doc.id
            << ", Titulo: " << doc.titulo
            << ", Fecha: " << doc.fechaRecepcion
            << ", Departamento: " << doc.departamento
            << ", Prioridad: " << doc.getPrioridadTexto();
        return os;
    }
};

// Clase para gestionar la bandeja de documentos
class BandejaDocumentos {
private:
    Stack<Documento> pila; // Pila para almacenar los documentos
    int contadorId; // Contador para generar IDs únicos
    int documentosRevisados; // Contador de documentos revisados

public:
    BandejaDocumentos() : contadorId(1), documentosRevisados(0) {}

    // Método para añadir un nuevo documento a la bandeja
    void recibirDocumento(const string& titulo, const string& fecha, const string& departamento, int prioridad) {
        Documento nuevoDoc(contadorId++, titulo, fecha, departamento, prioridad);
        pila.push(nuevoDoc);
        cout << "Documento recibido: " << nuevoDoc << endl;
    }

    // Método para revisar el último documento recibido
    bool revisarDocumento() {
        Documento doc;
        if (pila.pop(doc)) {
            cout << "Documento revisado: " << doc << endl;
            documentosRevisados++;
            return true;
        }
        else {
            cout << "No hay documentos pendientes para revisar." << endl;
            return false;
        }
    }

    // Método para ver el próximo documento a revisar sin retirarlo
    bool verProximoDocumento() {
        Documento doc;
        if (pila.peek(doc)) {
            cout << "Proximo documento a revisar: " << doc << endl;
            return true;
        }
        else {
            cout << "No hay documentos en la bandeja." << endl;
            return false;
        }
    }

    // Método para ver todos los documentos pendientes en la bandeja
    void verBandeja() const {
        cout << "Estado actual de la bandeja:" << endl;
        pila.print();
        cout << "Total de documentos pendientes: " << pila.getSize() << endl;
    }

    // Método para ver estadísticas de la bandeja
    void mostrarEstadisticas() const {
        cout << "\n=== ESTADISTICAS DE LA BANDEJA ===" << endl;
        cout << "Total de documentos recibidos: " << contadorId - 1 << endl;
        cout << "Documentos revisados: " << documentosRevisados << endl;
        cout << "Documentos pendientes: " << pila.getSize() << endl;

        if (contadorId > 1) { // Si hay al menos un documento registrado
            double porcentajeRevisado = (documentosRevisados * 100.0) / (contadorId - 1);
            cout << "Porcentaje completado: " << fixed << setprecision(1) << porcentajeRevisado << "%" << endl;
        }
        cout << "=================================" << endl;
    }

    // Método para revisar todos los documentos pendientes
    void revisarTodos() {
        cout << "\nRevisando todos los documentos pendientes:" << endl;
        while (!pila.isEmpty()) {
            revisarDocumento();
        }
        cout << "Todos los documentos han sido revisados." << endl;
    }

    // Método para obtener el número de documentos pendientes
    int getDocumentosPendientes() const {
        return pila.getSize();
    }
};

// Programa principal para probar la gestión de documentos
int main() {
    BandejaDocumentos bandeja;

    cout << "=== SISTEMA DE GESTION DE DOCUMENTOS ===" << endl << endl;

    // Recibir varios documentos
    bandeja.recibirDocumento("Informe Trimestral", "2025-04-20", "Contabilidad", 3);
    bandeja.recibirDocumento("Contrato Nuevo Cliente", "2025-04-22", "Legal", 4);
    bandeja.recibirDocumento("Solicitud de Vacaciones", "2025-04-24", "RR.HH.", 2);
    bandeja.recibirDocumento("Presupuesto Anual", "2025-04-25", "Finanzas", 5);
    bandeja.recibirDocumento("Propuesta de Marketing", "2025-04-26", "Marketing", 3);

    cout << "\nEstado inicial de la bandeja:" << endl;
    bandeja.verBandeja();

    cout << "\nVerificando proximo documento a revisar:" << endl;
    bandeja.verProximoDocumento();

    cout << "\nRevisando documentos:" << endl;
    bandeja.revisarDocumento(); // Revisar el último documento recibido (Propuesta de Marketing)
    bandeja.revisarDocumento(); // Revisar el siguiente (Presupuesto Anual)

    cout << "\nEstado de la bandeja despues de revisar dos documentos:" << endl;
    bandeja.verBandeja();
    bandeja.mostrarEstadisticas();

    // Recibir más documentos
    cout << "\nRecibiendo mas documentos:" << endl;
    bandeja.recibirDocumento("Reporte de Incidencias", "2025-04-27", "IT", 4);

    cout << "\nEstado final de la bandeja:" << endl;
    bandeja.verBandeja();

    // Revisar todos los documentos pendientes
    bandeja.revisarTodos();

    // Verificar estadísticas finales
    bandeja.mostrarEstadisticas();

    return 0;
}
