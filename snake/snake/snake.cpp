// snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>     //Para entradas y salidas
#include <windows.h>    //Para poder encontrar las posiciones se usa en gotoxy
#include <conio.h>      //Funcion para los tiempos
#include <stdlib.h>     //Posiciones Aleatorias
#include <fstream>      //Libreria para escribir archivos
#include<string>        //Libreria para usar un getline
#include<chrono>        //Libreria para captar el tiempo
#include <iomanip>      // Para setw y mostrar los valores mas ordenados
using namespace std;    //Libreria para el cout

//Estructuras para la puntuacion
struct Nodo
{
    double puntuacion;
    string nombre;
    Nodo* siguiente;
};

//Definicion de las teclas que sirven para el movimiento y salir
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
#define ESC 27

    //Varaibles Globales
//Variable Globales para iniciar el juego
int cuerpo[200][2];     //Matriz posiciones del snake
char tecla;             //Tecla de entrada
int n = 1;                //Para las posiciones del snake
int x = 10, y = 12;     //Posicion inicial del snake
int tamano = 6;         //Para el tamano del snake
int direccion = 3;      //Variable de la direccion
int velocidad = 80;     //Velocidad del snake
int xc = 30, yc = 15;   //Posicion de la Comida
//Variables Globales para la puntuacion
string nombreJugador;
std::chrono::high_resolution_clock::time_point tiempoInicio; //Variables para el cronometro inicio
std::chrono::high_resolution_clock::time_point tiempoFin;    //Variables para el cronometro fin
double puntuacion = 0;         //Guarda la puntuacion de la persona
string nombreArchivo = "Puntuacions.txt"; //Nombre del archivo con puntuaciones
//Variables Globales para la lista
Nodo* lista = NULL;     //Variables para la lista
double valor;           //Valor de la lista
string jugador;         //Jugador de la lista

//Prototipos
//Prototipos del juego
void goToXY(int, int);
void pintarBordes();
void guardarPosicion();
void dibujarCuerpo();
void borrarCuerpo();
void movimientosTecleado();
void OcultaCursor();
void guardarPuntuacion(string nombre, double valor);
void leerPuntuaciones();
void comidaSnake();
bool finSnake();
void mostrarMenu();
void jugar();
void imprimirGameOver();
void pedirNombre();
void reiniciar();
//Prototipos de la lista
void ingresarPuntuacion(Nodo*& lista, double n, string jugador);
void mostrarValores(Nodo* lista);
void eliminarLista(Nodo*& lista);

//Posicionar el SNAKE en una posicion
void goToXY(int x, int y) {
    HANDLE hCon; //Identificador
    COORD DibujaPosicion; //Variable de posicion x,y
    // Asignacion de la posicion
    DibujaPosicion.X = x;
    DibujaPosicion.Y = y;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon, DibujaPosicion); //Establece la posicion en la consola
}

//Guardar en la matriz las posiciones de acuerdo a su movimento 
void guardarPosicion() {
    cuerpo[n][0] = x;   //Cordenada de x
    cuerpo[n][1] = y;   //Cordenada de y
    n++; //Ir aumentado las posiciones del snake
    if (n == tamano) {
        n = 1;  //Reinciar para guardar los nuevas posiciones del snake
    }
};

//Ir pintado la posicion como se va movilizando
void dibujarCuerpo() {
    for (int i = 1; i < tamano; i++) //Pintar segun tamano del snake
    {
        goToXY(cuerpo[i][0], cuerpo[i][1]);//Cada entrada de la matriz
        printf("*"); //Dar la forma a la snake
    }
}

//Eliminar la parte de atras de la cola
void borrarCuerpo() {
    goToXY(cuerpo[n][0], cuerpo[n][1]);
    printf(" ");
}

//Funcion de los movimientos del snake
void movimientosTecleado()
{
    if (_kbhit()) //Alarma de que se uso el teclado
    {
        tecla = _getch(); //Obtiene el codigo de la letra presionada
        switch (tecla) {
        case ARRIBA:
            if (direccion != 2) //Verificar que no vaya para abajo
            {
                direccion = 1;
            }
            break;

        case ABAJO:
            if (direccion != 1) //Verificar que no vaya para arriba
            {
                direccion = 2;
            }
            break;

        case DERECHA:
            if (direccion != 4) //Verificar que no vaya para la izquierda
            {
                direccion = 3;
            }
            break;

        case IZQUIERDA:
            if (direccion != 3) //Verificar que no vaya para derecha
            {
                direccion = 4;
            }
            break;
        }
    }
}

//Funcion para pintar los bordes
void pintarBordes() {
    // Obtener el ancho y alto de la consola
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int anchoConsola = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int altoConsola = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // Pintar lineas horizontales
    for (int i = 2; i < anchoConsola - 2; i++) {
        goToXY(i, 3);  // ir a la posicion para pintar arriba
        printf("%c", 205);  // Caracter 205 bordes lineas horizontales
        goToXY(i, altoConsola - 2);  // Ir a la posicion para pintar abajo
        printf("%c", 205);  // Caracter 205 bordes lineas horizontales
    }

    // Pintar lineas verticales
    for (int i = 3; i < altoConsola - 1; i++) {
        goToXY(2, i);  // ir a la posicion para pintar izquierda
        printf("%c", 186);  // Caracter 186 bordes lineas verticales
        goToXY(anchoConsola - 3, i);  // ir a la posicion para pintar derecha
        printf("%c", 186);  // Caracter 205 bordes lineas verticales
    }

    // Esquinas
    goToXY(2, 3);  // esquina noroeste
    printf("%c", 201);
    goToXY(2, altoConsola - 2);  // esquina noreste
    printf("%c", 200);
    goToXY(anchoConsola - 3, 3);  // esquina suroeste
    printf("%c", 187);
    goToXY(anchoConsola - 3, altoConsola - 2);  // esquina suroeste
    printf("%c", 188);
}

//Ocular cursor del teclado
void OcultaCursor() {
    CONSOLE_CURSOR_INFO cci = { 100, FALSE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

//Mostrar la comida y calcular la puntuacion
void comidaSnake()
{
    if (x == xc && y == yc) //Si toca la comida
    {
        // Obtener el ancho y alto de la consola
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int anchoConsola = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int altoConsola = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        //Calcular la puntuacion
        tiempoFin = std::chrono::high_resolution_clock::now(); //Finalizar el cronometro
        auto duracion = std::chrono::duration_cast<std::chrono::seconds>(tiempoFin - tiempoInicio); //Calcular la duracion
        if (duracion.count() == 0)
        {
            duracion = std::chrono::seconds(1);
        }
        puntuacion += (20 / duracion.count()); //5 Puntos en 1 segundo
        tiempoInicio = std::chrono::high_resolution_clock::now(); //Reiniciar el cronometro

        //Posicionar la comida nuevamente
        xc = (rand() % (anchoConsola - 10)) + 4; // Posicion random X dentro de los bordes
        yc = (rand() % (altoConsola - 8)) + 4;   // Posicion random Y dentro de los bordes

        tamano++; //Hacer crecer el snake
        goToXY(xc, yc);
        printf("%c", 254); //Mostrar la comida
        velocidad--;
    }
}

//Finalizar la partida
bool finSnake()
{
    // Obtener el ancho y alto de la consola
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int anchoConsola = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int altoConsola = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    if (y == 3 || y == altoConsola - 2 || x == 2 || x == anchoConsola - 3) //si toca un borde
    {
        return false;//SALIR
    }

    for (int j = tamano - 1; j > 0; j--) {
        if (cuerpo[j][0] == x && cuerpo[j][1] == y) //Si se golpea a si misma
            return false;//SALIR
    }
    return true;//Continuar
}

//Función para mostrar el menú
void mostrarMenu() {
    // Limpiar la pantalla
    system("cls");

    // Obtener el ancho y alto de la consola
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int anchoConsola = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int altoConsola = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // Calcular la posición para imprimir en el centro
    int xCentro = anchoConsola / 2;
    int yCentro = altoConsola / 2 - 2; // Ajustar la posición vertical

    // Posicionar el cursor en el centro
    goToXY(xCentro, yCentro);

    // Imprimir el menú
    cout << "===============" << endl;
    goToXY(xCentro, yCentro + 1);
    cout << "1. Jugar" << endl;
    goToXY(xCentro, yCentro + 2);
    cout << "2. Puntuaciones" << endl;
    goToXY(xCentro, yCentro + 3);
    cout << "3. Salir" << endl;
    goToXY(xCentro, yCentro + 4);
    cout << "===============" << endl;
}

//Funcion para el nivel uno
void jugar() {
    pintarBordes(); //Pintar los bordes
    goToXY(xc, yc);
    tiempoInicio = std::chrono::high_resolution_clock::now(); //Iniciar el cronometro
    printf("%c", 254);

    while (tecla != ESC && finSnake())
    {
        borrarCuerpo();
        guardarPosicion();
        dibujarCuerpo();
        comidaSnake();
        movimientosTecleado();
        movimientosTecleado();

        //Verificacion de quien cambia en la matriz
        if (direccion == 1) {
            y--;
        }
        if (direccion == 2) {
            y++;
        }
        if (direccion == 3) {
            x++;
        }
        if (direccion == 4) {
            x--;
        }
        Sleep(velocidad);
    }

}

//Funcion para guardar las puntuaciones en un txt
void guardarPuntuacion(string nombre, double valor) {
    //Variables
    ofstream archivo;

    archivo.open(nombreArchivo.c_str(), ios::app); //Abrir el archivo

    if (archivo.fail()) //Verificacion de si se pudo abrir el archivo
    {
        cout << "No se puede abir el archivo" << endl;
        exit(1);
    }
    archivo << nombre << " " << valor << endl; //Formato de nombre y puntuacion

    archivo.close(); //Guardar y cierra
}

//Funcion para leer las puntuaciones del txt
void leerPuntuaciones() {
    //Variables
    string name;
    double punt;
    ifstream archivo;

    // Variables
    archivo.open(nombreArchivo.c_str(), ios::in); //Abrir el archivo

    if (archivo.fail()) //Verificar de si se pudo abrir el archivo
    {
        cout << "No se puede abir el archivo" << endl;
        exit(1);
    }

    // Mostrar puntuaciones como tabla
    cout << "\n";
    cout << setw(20) << left << "Nombre" << setw(15) << left << "Puntuacion" << endl;
    cout << setfill('-') << setw(35) << "-" << setfill(' ') << endl;

    while (archivo >> name >> punt) {
        ingresarPuntuacion(lista, punt, name);
    }

    archivo.close(); // Cerrar el archivo
}

//Menu principal
int main()
{
    OcultaCursor();
    int opcion;

    do {
        mostrarMenu();
        cin >> opcion;
        system("cls");
        switch (opcion) {

        case 1: //Jugar el primer nivel
            //Solicitar el nombre del jugador
            pedirNombre();

            //Jugar
            jugar();
            guardarPuntuacion(nombreJugador, puntuacion);
            reiniciar();
            imprimirGameOver();
            system("pause");
            system("cls");

            break;

        case 2: //Mostrar las puntuaciones guardadas
            eliminarLista(lista);
            leerPuntuaciones();
            mostrarValores(lista);
            system("pause");
            break;

        case 3: //Salir del juego
            cout << "Saliendo del juego. ¡Hasta luego!" << endl;
            break;

        default:
            cout << "Opción no válida. Inténtalo de nuevo." << endl;
        }

    } while (opcion != 3); // El bucle continuará hasta que el usuario


    return 0;
}

//Ingresar la puntuacion y el nombre en la lista
void ingresarPuntuacion(Nodo*& lista, double n, string jugador) {
    //Nuevo nodo y auxiliares
    Nodo* nuevoNodo = new Nodo();
    nuevoNodo->puntuacion = n;
    nuevoNodo->nombre = jugador;
    Nodo* auxIni = new Nodo();
    Nodo* auxFin = new Nodo();

    auxIni = NULL;
    auxFin = lista;

    //Movilizar los auxilliares para colocarlo en orden
    while ((auxFin != NULL) && (auxFin->puntuacion > n))
    {
        auxIni = auxFin;
        auxFin = auxFin->siguiente;
    }
    //Asignacion del nodo
    if (lista == auxFin)
    {
        lista = nuevoNodo; //En caso de ser el menor
    }
    else
    {
        auxIni->siguiente = nuevoNodo; //El anterior apunta a el
    }
    nuevoNodo->siguiente = auxFin; //El apunta al siguiente
}

//Mostrar puntuaciones
void mostrarValores(Nodo* lista) {
    //Auxiliares para mostrar valores
    Nodo* auxiliar = new Nodo();
    auxiliar = lista;
    int contador = 1;

    cout << "Posicion  " << "  Nombre  " << "  Puntaje" << endl;
    while (auxiliar != NULL && contador < 11)//Siempre que no sea el ultimo
    {

        cout << setw(12) << left << contador;  // Alineación a la izquierda, ancho de 9 caracteres
        cout << setw(12) << left << auxiliar->nombre;
        cout << auxiliar->puntuacion << endl;
        auxiliar = auxiliar->siguiente; // Mover al siguiente
        contador++;
    }
    cout << endl;
    system("pause");
    mostrarMenu();
}

//Limpiar la lista
void eliminarLista(Nodo*& lista) {
    Nodo* aux;
    while (lista != NULL) {
        aux = lista;
        lista = lista->siguiente;
        delete aux;
    }
}

//Imprimir el Game Over
void imprimirGameOver() {
    // Limpiar la pantalla
    system("cls");

    // Obtener el ancho y alto de la consola
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int anchoConsola = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int altoConsola = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // Calcular la posición para imprimir en el centro
    int xCentro = anchoConsola / 2;
    int yCentro = altoConsola / 2;

    // Posicionar el cursor en el centro
    goToXY(xCentro, yCentro);

    // Imprimir el mensaje "Game Over"
    cout << "Game Over" << endl;
}

//Perdir el nombre al usuario
void pedirNombre() {
    // Limpiar la pantalla
    system("cls");

    // Obtener el ancho y alto de la consola
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int anchoConsola = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int altoConsola = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // Calcular la posición para imprimir en el centro
    int xCentro = anchoConsola / 2 - 8; // Ajuste para centrar
    int yCentro = altoConsola / 2 - 2;

    // Posicionar el cursor en el centro
    goToXY(xCentro, yCentro);

    cout << "Dijite su nombre" << endl;

    // Posicionar el cursor en el centro para la entrada del nombre
    int xCentroNombre = anchoConsola / 2 - 4;
    goToXY(xCentroNombre, yCentro + 1);

    // Leer el nombre del jugador
    while (getchar() != '\n');
    getline(cin, nombreJugador);
    system("cls");
}

//Reiniciar a los valores iniciales
void reiniciar() {
    n = 1;
    x = 10;
    y = 12;     //Posicion inicial del snake
    tamano = 6;         //Para el tamano del snake
    direccion = 3;      //Variable de la direccion
    velocidad = 60;     //Velocidad del snake
    xc = 30, yc = 15;   //PosicionComida
    puntuacion = 0;
}