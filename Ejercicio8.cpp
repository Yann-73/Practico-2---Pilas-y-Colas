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
#include "Stack.h" 

using namespace std;

// Enumeración para los tipos de acciones disponibles
enum TipoAccion {
    ESCRIBIR,
    BORRAR,
    COPIAR,
    PEGAR,
    REEMPLAZAR
};

// Función auxiliar para convertir TipoAccion a string
string tipoAccionToString(TipoAccion tipo) {
    switch (tipo) {
    case ESCRIBIR: return "Escribir";
    case BORRAR: return "Borrar";
    case COPIAR: return "Copiar";
    case PEGAR: return "Pegar";
    case REEMPLAZAR: return "Reemplazar";
    default: return "Desconocido";
    }
}

// Clase para representar una acción en el editor de texto
class Accion {
private:
    TipoAccion tipo;
    string contenido;
    int posicion;  // Posición donde se realizó la acción

public:
    Accion() : tipo(ESCRIBIR), contenido(""), posicion(0) {}

    Accion(TipoAccion t, const string& c, int pos)
        : tipo(t), contenido(c), posicion(pos) {
    }

    TipoAccion getTipo() const {
        return tipo;
    }

    const string& getContenido() const {
        return contenido;
    }

    int getPosicion() const {
        return posicion;
    }

    // Sobrecarga del operador << para imprimir información de la acción
    friend ostream& operator<<(ostream& os, const Accion& accion) {
        os << "Accion: " << tipoAccionToString(accion.tipo)
            << ", Contenido: \"" << accion.contenido << "\""
            << ", Posicion: " << accion.posicion;
        return os;
    }
};

// Clase para simular un editor de texto básico
class EditorTexto {
private:
    string texto;             // El texto actual del editor
    string portapapeles;      // El contenido del portapapeles
    Stack<Accion> acciones;   // Pila para almacenar las acciones realizadas
    Stack<Accion> deshechas;  // Pila para almacenar las acciones deshechas (para rehacer)

public:
    EditorTexto() : texto(""), portapapeles("") {}

    // Método para obtener el texto actual
    const string& getTexto() const {
        return texto;
    }

    // Método para obtener el contenido del portapapeles
    const string& getPortapapeles() const {
        return portapapeles;
    }

    // Método para ejecutar la acción de escribir
    void escribir(const string& contenido, int posicion) {
        if (posicion < 0 || posicion > texto.length()) {
            cout << "Posicion invalida para escribir." << endl;
            return;
        }

        // Guardar la acción (para deshacer, necesitamos saber qué se agregó)
        Accion accion(ESCRIBIR, contenido, posicion);
        acciones.push(accion);

        // Limpiar la pila de deshechas al realizar una nueva acción
        while (!deshechas.isEmpty()) {
            Accion temp;
            deshechas.pop(temp);
        }

        // Realizar la acción de escritura
        texto.insert(posicion, contenido);
        cout << "Accion realizada: " << accion << endl;
    }

    // Método para ejecutar la acción de borrar
    void borrar(int posicion, int longitud) {
        if (posicion < 0 || posicion >= texto.length() || longitud <= 0) {
            cout << "Parametros invalidos para borrar." << endl;
            return;
        }

        // Ajustar la longitud si excede el final del texto
        if (posicion + longitud > texto.length()) {
            longitud = texto.length() - posicion;
        }

        // El contenido que se va a borrar (para poder deshacer)
        string contenidoBorrado = texto.substr(posicion, longitud);

        // Guardar la acción
        Accion accion(BORRAR, contenidoBorrado, posicion);
        acciones.push(accion);

        // Limpiar la pila de deshechas
        while (!deshechas.isEmpty()) {
            Accion temp;
            deshechas.pop(temp);
        }

        // Realizar la acción de borrado
        texto.erase(posicion, longitud);
        cout << "Accion realizada: " << accion << endl;
    }

    // Método para ejecutar la acción de copiar
    void copiar(int posicion, int longitud) {
        if (posicion < 0 || posicion >= texto.length() || longitud <= 0) {
            cout << "Parámetros inválidos para copiar." << endl;
            return;
        }

        // Ajustar la longitud si excede el final del texto
        if (posicion + longitud > texto.length()) {
            longitud = texto.length() - posicion;
        }

        // Copiar el contenido al portapapeles
        portapapeles = texto.substr(posicion, longitud);

        // Guardar la acción
        Accion accion(COPIAR, portapapeles, posicion);
        acciones.push(accion);

        // Limpiar la pila de deshechas
        while (!deshechas.isEmpty()) {
            Accion temp;
            deshechas.pop(temp);
        }

        cout << "Accion realizada: " << accion << endl;
    }

    // Método para ejecutar la acción de pegar
    void pegar(int posicion) {
        if (posicion < 0 || posicion > texto.length()) {
            cout << "Posicion invalida para pegar." << endl;
            return;
        }

        if (portapapeles.empty()) {
            cout << "El portapapeles esta vacio." << endl;
            return;
        }

        // Guardar la acción
        Accion accion(PEGAR, portapapeles, posicion);
        acciones.push(accion);

        // Limpiar la pila de deshechas
        while (!deshechas.isEmpty()) {
            Accion temp;
            deshechas.pop(temp);
        }

        // Realizar la acción de pegar
        texto.insert(posicion, portapapeles);
        cout << "Accion realizada: " << accion << endl;
    }

    // Método para ejecutar la acción de reemplazar
    void reemplazar(int posicion, int longitud, const string& nuevoTexto) {
        if (posicion < 0 || posicion >= texto.length() || longitud <= 0) {
            cout << "Parametros invalidos para reemplazar." << endl;
            return;
        }

        // Ajustar la longitud si excede el final del texto
        if (posicion + longitud > texto.length()) {
            longitud = texto.length() - posicion;
        }

        // El contenido que se va a reemplazar (para poder deshacer)
        string contenidoReemplazado = texto.substr(posicion, longitud);

        // Guardar la acción (guardamos el texto original para deshacer)
        Accion accion(REEMPLAZAR, contenidoReemplazado + "|" + nuevoTexto, posicion);
        acciones.push(accion);

        // Limpiar la pila de deshechas
        while (!deshechas.isEmpty()) {
            Accion temp;
            deshechas.pop(temp);
        }

        // Realizar la acción de reemplazo
        texto.replace(posicion, longitud, nuevoTexto);
        cout << "Accion realizada: " << accion << endl;
    }

    // Método para deshacer la última acción
    bool deshacer() {
        if (acciones.isEmpty()) {
            cout << "No hay acciones para deshacer." << endl;
            return false;
        }

        Accion ultimaAccion;
        acciones.pop(ultimaAccion);
        deshechas.push(ultimaAccion);

        // Deshacer la acción según su tipo
        switch (ultimaAccion.getTipo()) {
        case ESCRIBIR:
            // Para deshacer una escritura, borramos el texto que se escribió
            texto.erase(ultimaAccion.getPosicion(), ultimaAccion.getContenido().length());
            break;

        case BORRAR:
            // Para deshacer un borrado, volvemos a insertar el texto borrado
            texto.insert(ultimaAccion.getPosicion(), ultimaAccion.getContenido());
            break;

        case COPIAR:
            // Para deshacer una copia, no hacemos nada al texto
            // (podríamos restaurar el portapapeles anterior si lo guardáramos)
            break;

        case PEGAR:
            // Para deshacer un pegado, borramos el texto que se pegó
            texto.erase(ultimaAccion.getPosicion(), ultimaAccion.getContenido().length());
            break;

        case REEMPLAZAR: {
            // Para deshacer un reemplazo, restauramos el texto original
            // El formato es "textoOriginal|textoNuevo"
            size_t separador = ultimaAccion.getContenido().find("|");
            string textoOriginal = ultimaAccion.getContenido().substr(0, separador);
            string textoNuevo = ultimaAccion.getContenido().substr(separador + 1);
            texto.replace(ultimaAccion.getPosicion(), textoNuevo.length(), textoOriginal);
            break;
        }
        }

        cout << "Accion deshecha: " << ultimaAccion << endl;
        return true;
    }

    // Método para rehacer la última acción deshecha
    bool rehacer() {
        if (deshechas.isEmpty()) {
            cout << "No hay acciones para rehacer." << endl;
            return false;
        }

        Accion accionDeshecha;
        deshechas.pop(accionDeshecha);
        acciones.push(accionDeshecha);

        // Rehacer la acción según su tipo
        switch (accionDeshecha.getTipo()) {
        case ESCRIBIR:
            // Para rehacer una escritura, volvemos a insertar el texto
            texto.insert(accionDeshecha.getPosicion(), accionDeshecha.getContenido());
            break;

        case BORRAR:
            // Para rehacer un borrado, volvemos a borrar el texto
            texto.erase(accionDeshecha.getPosicion(), accionDeshecha.getContenido().length());
            break;

        case COPIAR:
            // Para rehacer una copia, solo actualizamos el portapapeles
            portapapeles = accionDeshecha.getContenido();
            break;

        case PEGAR:
            // Para rehacer un pegado, volvemos a insertar el texto del portapapeles
            texto.insert(accionDeshecha.getPosicion(), accionDeshecha.getContenido());
            break;

        case REEMPLAZAR: {
            // Para rehacer un reemplazo, volvemos a aplicar el texto nuevo
            // El formato es "textoOriginal|textoNuevo"
            size_t separador = accionDeshecha.getContenido().find("|");
            string textoOriginal = accionDeshecha.getContenido().substr(0, separador);
            string textoNuevo = accionDeshecha.getContenido().substr(separador + 1);
            texto.replace(accionDeshecha.getPosicion(), textoOriginal.length(), textoNuevo);
            break;
        }
        }

        cout << "Accion rehecha: " << accionDeshecha << endl;
        return true;
    }

    // Método para mostrar el historial de acciones
    void mostrarHistorial() const {
        cout << "\n=== Historial de Acciones ===" << endl;
        if (acciones.isEmpty()) {
            cout << "No hay acciones registradas." << endl;
        }
        else {
            Stack<Accion> tempStack = acciones; // Crear una copia de la pila
            Stack<Accion> reverseStack; // Para mostrar en orden cronológico

            // Invertimos el orden para mostrar de más antigua a más reciente
            while (!tempStack.isEmpty()) {
                Accion accion;
                tempStack.pop(accion);
                reverseStack.push(accion);
            }

            cout << "Mostrando desde la mas antigua a la mas reciente:" << endl;
            int i = 1;
            while (!reverseStack.isEmpty()) {
                Accion accion;
                reverseStack.pop(accion);
                cout << i++ << ". " << accion << endl;
            }
        }
    }

    // Método para mostrar el estado actual del editor
    void mostrarEstado() const {
        cout << "\n=== Estado Actual del Editor ===" << endl;
        cout << "Texto actual: \"" << texto << "\"" << endl;
        cout << "Contenido del portapapeles: \"" << portapapeles << "\"" << endl;
        cout << "Longitud del texto: " << texto.length() << " caracteres" << endl;
        cout << "Acciones registradas: " << acciones.getSize() << endl;
        cout << "Acciones para rehacer: " << deshechas.getSize() << endl;
    }
};

int main() {
    EditorTexto editor;

    // Escribir texto inicial
    editor.escribir("Hola", 0);
    editor.escribir(" mundo", 4);

    // Mostrar estado actual
    editor.mostrarEstado();

    // Copiar "Hola"
    editor.copiar(0, 4);

    // Pegar lo copiado al final
    editor.pegar(editor.getTexto().length());

    // Borrar " mundo"
    editor.borrar(4, 6);

    // Reemplazar "Hola" por "Hi"
    editor.reemplazar(0, 4, "Hi");

    // Mostrar historial de acciones
    editor.mostrarHistorial();

    // Deshacer dos acciones
    editor.deshacer();
    editor.deshacer();

    // Mostrar estado después de deshacer
    editor.mostrarEstado();

    // Rehacer una acción
    editor.rehacer();

    // Mostrar estado final
    editor.mostrarEstado();

    return 0;
}
