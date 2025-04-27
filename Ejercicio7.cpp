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
#include "Queue.h" 

using namespace std;

// Clase para representar una fecha
class Fecha {
private:
    int dia;
    int mes;
    int anio;

public:
    Fecha() : dia(1), mes(1), anio(2025) {}

    Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {}

    // Sobrecarga del operador < para comparar fechas
    bool operator<(const Fecha& otra) const {
        if (anio != otra.anio)
            return anio < otra.anio;
        if (mes != otra.mes)
            return mes < otra.mes;
        return dia < otra.dia;
    }

    // Sobrecarga del operador == para comparar fechas
    bool operator==(const Fecha& otra) const {
        return dia == otra.dia && mes == otra.mes && anio == otra.anio;
    }

    // Método para obtener una representación en string de la fecha
    string toString() const {
        return to_string(dia) + "/" + to_string(mes) + "/" + to_string(anio);
    }

    friend ostream& operator<<(ostream& os, const Fecha& fecha) {
        os << fecha.toString();
        return os;
    }
};

// Clase para representar una caja de productos perecederos
class CajaProducto {
private:
    string nombreProducto;
    Fecha fechaVencimiento;
    int cantidadUnidades;
    int idCaja;
    static int contadorCajas;

public:
    CajaProducto() : nombreProducto(""), cantidadUnidades(0), idCaja(0) {}

    CajaProducto(const string& nombre, const Fecha& fecha, int cantidad)
        : nombreProducto(nombre), fechaVencimiento(fecha), cantidadUnidades(cantidad) {
        idCaja = ++contadorCajas;
    }

    const string& getNombreProducto() const {
        return nombreProducto;
    }

    const Fecha& getFechaVencimiento() const {
        return fechaVencimiento;
    }

    int getCantidadUnidades() const {
        return cantidadUnidades;
    }

    int getIdCaja() const {
        return idCaja;
    }

    void reducirCantidad(int cantidad) {
        if (cantidad > cantidadUnidades)
            cantidadUnidades = 0;
        else
            cantidadUnidades -= cantidad;
    }

    bool estaVacia() const {
        return cantidadUnidades == 0;
    }

    // Sobrecarga del operador << para imprimir información de la caja
    friend ostream& operator<<(ostream& os, const CajaProducto& caja) {
        os << "Caja #" << caja.idCaja
            << " - Producto: " << caja.nombreProducto
            << ", Vence: " << caja.fechaVencimiento
            << ", Unidades: " << caja.cantidadUnidades;
        return os;
    }
};

// Inicialización del contador estático
int CajaProducto::contadorCajas = 0;

// Clase para gestionar el inventario de productos perecederos
class GestorInventario {
private:
    // Usamos Queue porque queremos una política FIFO (First In, First Out)
    // Los productos más antiguos salen primero
    Queue<CajaProducto> estanteria;
    int cajasAgregadas;
    int totalProductosVendidos;

public:
    GestorInventario() : cajasAgregadas(0), totalProductosVendidos(0) {}

    // Método para agregar una nueva caja de productos
    void agregarCaja(const string& nombreProducto, const Fecha& fechaVencimiento, int cantidadUnidades) {
        CajaProducto nuevaCaja(nombreProducto, fechaVencimiento, cantidadUnidades);
        estanteria.push(nuevaCaja);
        cajasAgregadas++;
        cout << "Caja agregada: " << nuevaCaja << endl;
    }

    // Método para vender productos (se venden primero los de las cajas más antiguas)
    bool venderProducto(const string& nombreProducto, int cantidad) {
        // Verificamos si hay cajas con el producto
        if (estanteria.isEmpty()) {
            cout << "No hay cajas en la estanteria." << endl;
            return false;
        }

        // Creamos una cola temporal para mantener el orden adecuado
        Queue<CajaProducto> tempQueue;
        int cantidadRestante = cantidad;
        bool productoEncontrado = false;

        while (!estanteria.isEmpty()) {
            CajaProducto caja;
            estanteria.pop(caja);

            if (caja.getNombreProducto() == nombreProducto && cantidadRestante > 0 && !caja.estaVacia()) {
                productoEncontrado = true;
                int cantidadVendida = min(cantidadRestante, caja.getCantidadUnidades());
                cantidadRestante -= cantidadVendida;
                totalProductosVendidos += cantidadVendida;

                cout << "Vendiendo " << cantidadVendida << " unidades de "
                    << nombreProducto << " de la caja #" << caja.getIdCaja() << endl;

                // Actualizar la cantidad en la caja
                CajaProducto cajaActualizada = caja;
                cajaActualizada.reducirCantidad(cantidadVendida);

                // Si la caja no está vacía, la volvemos a meter en la cola
                if (!cajaActualizada.estaVacia()) {
                    tempQueue.push(cajaActualizada);
                }
                else {
                    cout << "La caja #" << caja.getIdCaja() << " ahora esta vacia y se retira." << endl;
                }
            }
            else {
                // Si no es el producto que buscamos o ya no necesitamos más, 
                // simplemente devolvemos la caja a la cola
                tempQueue.push(caja);
            }
        }

        // Reestablecemos la estantería original con las cajas restantes
        while (!tempQueue.isEmpty()) {
            CajaProducto caja;
            tempQueue.pop(caja);
            estanteria.push(caja);
        }

        if (!productoEncontrado) {
            cout << "No se encontro el producto '" << nombreProducto << "' en inventario." << endl;
            return false;
        }

        if (cantidadRestante > 0) {
            cout << "No hay suficiente stock. Faltan " << cantidadRestante
                << " unidades de " << nombreProducto << "." << endl;
            return false;
        }

        return true;
    }

    // Método para verificar productos próximos a vencer
    void verificarVencimientos(const Fecha& fechaActual) {
        if (estanteria.isEmpty()) {
            cout << "No hay cajas en la estanteria para verificar." << endl;
            return;
        }

        cout << "\n=== Productos proximos a vencer ===" << endl;

        Queue<CajaProducto> tempQueue;
        bool hayProductosPorVencer = false;

        // Verificamos todas las cajas
        while (!estanteria.isEmpty()) {
            CajaProducto caja;
            estanteria.pop(caja);
            tempQueue.push(caja);

            // Si la fecha de vencimiento es menor o igual a la fecha actual, está vencido
            if (caja.getFechaVencimiento() < fechaActual || caja.getFechaVencimiento() == fechaActual) {
                cout << "ALERTA! Caja #" << caja.getIdCaja()
                    << " de " << caja.getNombreProducto()
                    << " ya está vencida o vence hoy (" << caja.getFechaVencimiento() << ")." << endl;
                hayProductosPorVencer = true;
            }
        }

        if (!hayProductosPorVencer) {
            cout << "No hay productos proximos a vencer." << endl;
        }

        // Restauramos la estantería
        while (!tempQueue.isEmpty()) {
            CajaProducto caja;
            tempQueue.pop(caja);
            estanteria.push(caja);
        }
    }

    // Método para mostrar todo el inventario
    void mostrarInventario() const {
        if (estanteria.isEmpty()) {
            cout << "El inventario está vacio." << endl;
            return;
        }

        cout << "\n=== Inventario de Productos Perecederos ===" << endl;
        estanteria.print();
        cout << "Total de cajas en inventario: " << estanteria.getSize() << endl;
    }

    // Método para mostrar estadísticas
    void mostrarEstadisticas() const {
        cout << "\n=== Estadisticas de Inventario ===" << endl;
        cout << "Total cajas agregadas: " << cajasAgregadas << endl;
        cout << "Cajas actuales en inventario: " << estanteria.getSize() << endl;
        cout << "Total productos vendidos: " << totalProductosVendidos << endl;
    }
};

int main() {
    GestorInventario inventario;

    inventario.agregarCaja("Leche", Fecha(28, 4, 2025), 20);
    inventario.agregarCaja("Yogur", Fecha(30, 4, 2025), 15);
    inventario.agregarCaja("Queso", Fecha(10, 5, 2025), 10);
    inventario.agregarCaja("Mantequilla", Fecha(5, 5, 2025), 8);

    cout << "\nEstado inicial del inventario:" << endl;
    inventario.mostrarInventario();

    cout << "\nVendiendo productos:" << endl;
    inventario.venderProducto("Leche", 12);      // Vender parte de la leche
    inventario.venderProducto("Yogur", 20);      // Intentar vender más yogur del que hay
    inventario.venderProducto("Queso", 5);       // Vender parte del queso

    cout << "\nInventario despues de algunas ventas:" << endl;
    inventario.mostrarInventario();

    cout << "\nVerificando productos proximos a vencer (fecha actual: 30/4/2025):" << endl;
    inventario.verificarVencimientos(Fecha(30, 4, 2025));

    cout << "\nVendiendo el resto de la leche:" << endl;
    inventario.venderProducto("Leche", 10);      // Vender el resto de la leche

    cout << "\nInventario final:" << endl;
    inventario.mostrarInventario();

    cout << "\nEstadisticas finales del inventario:" << endl;
    inventario.mostrarEstadisticas();

    return 0;
}
