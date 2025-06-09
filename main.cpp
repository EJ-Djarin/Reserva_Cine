#include <windows.h>     ///< Para manipular la consola (colores y codificación en Windows)
#include <iostream>      ///< Entrada y salida estándar
#include <vector>        ///< Usamos vector para representar los espacios de cine

using namespace std;

/**
 * @class Cine
 * @brief Clase que representa un cine con espacios numerados que pueden reservarse o liberarse.
 *
 * Esta clase administra un conjunto de espacios (asientos) en un cine y permite
 * mostrar su estado (reservado o libre) usando colores en consola. También permite
 * reservar o cancelar una reserva en un espacio específico.
 */
class Cine {
private:
    vector<bool> reservados;  ///< Vector que indica si cada espacio está reservado (true) o libre (false)
    HANDLE hConsole;          ///< Manejador de consola para cambiar colores de texto en Windows

public:
    /**
     * @brief Constructor que inicializa el vector de espacios y obtiene el manejador de consola
     * @param cantidadEspacios Número total de espacios (asientos) en el cine
     *
     * Inicializa el vector reservados con 'false' para indicar que inicialmente todos los asientos están libres.
     * También obtiene el handle de la consola para cambiar colores posteriormente.
     */
    Cine(int cantidadEspacios) : reservados(cantidadEspacios, false) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE); ///< Obtener manejador de consola para cambiar colores
    }

    /**
     * @brief Muestra en consola el estado de los espacios (libres o reservados) usando colores
     *
     * Los espacios reservados se muestran en color rojo, los libres en verde.
     * Imprime 5 espacios por línea para mejor visualización.
     */
    void mostrarEspacios() {
        system("cls"); ///< Limpia la consola para mostrar el estado actualizado

        cout << "\nEspacios del cine:\n";
        for (size_t i = 0; i < reservados.size(); ++i) {
            // Cambiar color de texto según el estado del espacio
            if (reservados[i])
                SetConsoleTextAttribute(hConsole, 12); ///< Rojo (reservado)
            else
                SetConsoleTextAttribute(hConsole, 10); ///< Verde (libre)

            // Imprimir número del asiento con alineación para facilitar lectura
            printf("[%2d] ", (int)i + 1);

            // Hacer salto de línea cada 5 espacios para formato tabla
            if ((i + 1) % 5 == 0)
                cout << endl;
        }

        // Restaurar color predeterminado (blanco/gris)
        SetConsoleTextAttribute(hConsole, 7);
        cout << endl;
    }

    /**
     * @brief Intenta reservar o cancelar la reserva de un espacio dado su número
     * @param numero Número del espacio (1-indexado)
     *
     * Si el espacio está libre, lo reserva.
     * Si está reservado, pregunta al usuario si desea cancelar la reserva.
     * Muestra mensajes de confirmación o error según corresponda.
     * Al final, actualiza la visualización de los espacios.
     */
    void reservarOCancelar(int numero) {
        // Validar rango del espacio
        if (numero < 1 || numero > (int)reservados.size()) {
            cout << "Número de espacio inválido.\n";
            return;
        }

        int index = numero - 1; ///< Ajustar número de asiento (1-based) a índice vector (0-based)

        if (reservados[index]) {
            // Espacio ya reservado, confirmar cancelación
            cout << "Ese espacio ya está reservado. ¿Deseas cancelar la reserva? (s/n): ";
            char respuesta;
            cin >> respuesta;
            if (respuesta == 's' || respuesta == 'S') {
                reservados[index] = false; ///< Cancelar reserva
                cout << "Reserva del espacio " << numero << " cancelada.\n";
            } else {
                cout << "Reserva no modificada.\n";
            }
        } else {
            // Reservar espacio libre
            reservados[index] = true;
            cout << "Espacio " << numero << " reservado con éxito.\n";
        }

        mostrarEspacios(); ///< Actualizar pantalla con estado actualizado
    }
};

/**
 * @brief Función principal que ejecuta el sistema de reservas de cine.
 *
 * Configura la consola para UTF-8, crea un objeto Cine con 30 espacios,
 * y presenta un menú para reservar o cancelar reservas hasta que el usuario decida salir.
 *
 * @return int Código de salida del programa (0 para éxito)
 */
int main() {
    // Configurar consola para mostrar correctamente caracteres especiales (UTF-8)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int cantidadEspacios = 30;       ///< Número total de asientos en el cine
    Cine cine(cantidadEspacios);     ///< Crear objeto Cine con la cantidad especificada

    int opcion;
    do {
        cine.mostrarEspacios();      ///< Mostrar los espacios y su estado actual

        // Mostrar opciones que el usuario puede realizar
        cout << "\nOpciones:\n";
        cout << "1. Reservar/cancelar espacio\n";
        cout << "2. Salir\n";
        cout << "Elige una opción: ";
        cin >> opcion;

        if (opcion == 1) {
            int espacio;
            cout << "Ingrese el número del espacio (1-" << cantidadEspacios << "): ";
            cin >> espacio;
            cine.reservarOCancelar(espacio);
        } else if (opcion != 2) {
            cout << "Opción inválida.\n";
        }
    } while (opcion != 2);

    cout << "Gracias por usar el sistema. ¡Hasta pronto!\n";
    return 0;
}
