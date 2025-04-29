#include <iostream>
#include <string>
#include "Stack.h" // Incluimos la clase Stack proporcionada

using namespace std;

class Navegador {
private:
    Stack<string> historial; // Pila para guardar el historial de sitios
    string sitioActual;      // Sitio que se está mostrando actualmente

public:
    // Constructor
    Navegador() {
        sitioActual = "Página de inicio";
        historial.push(sitioActual);
    }

    // Navegar a un nuevo sitio
    void navegar(const string& url) {
        historial.push(url);
        sitioActual = url;
        cout << "Navegando a: " << sitioActual << endl;
    }

    // Volver al sitio anterior
    bool atras() {
        string sitioEliminado;
        // Eliminamos el sitio actual
        if (historial.pop(sitioEliminado)) {
            // Verificamos si quedó algún sitio en el historial
            if (historial.peek(sitioActual)) {
                cout << "Volviendo a: " << sitioActual << endl;
                return true;
            } else {
                // Si no queda ningún sitio, volvemos a la página de inicio
                sitioActual = "Página de inicio";
                historial.push(sitioActual);
                cout << "Volviendo a: " << sitioActual << endl;
                return true;
            }
        }
        cout << "No hay sitios anteriores disponibles." << endl;
        return false;
    }

    // Obtener el sitio actual
    string getSitioActual() const {
        return sitioActual;
    }

    // Mostrar el historial completo
    void mostrarHistorial() const {
        cout << "Historial de navegación (más reciente primero):" << endl;
        historial.print();
    }
};

// Programa principal para probar la clase Navegador
int main() {
    Navegador miNavegador;
    
    cout << "Sitio inicial: " << miNavegador.getSitioActual() << endl;
    
    miNavegador.navegar("www.google.com");
    miNavegador.navegar("www.wikipedia.org");
    miNavegador.navegar("www.github.com");
    
    cout << "\nHistorial actual:" << endl;
    miNavegador.mostrarHistorial();
    
    cout << "\nVolviendo atrás:" << endl;
    miNavegador.atras();
    
    cout << "\nHistorial después de volver atrás:" << endl;
    miNavegador.mostrarHistorial();
    
    cout << "\nNavegando a un nuevo sitio:" << endl;
    miNavegador.navegar("www.stackoverflow.com");
    
    cout << "\nHistorial final:" << endl;
    miNavegador.mostrarHistorial();
    
    return 0;
}
