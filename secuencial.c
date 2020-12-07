// Solucion secuencial n bodies

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

// Cantidad de cuerpos
#define N 3

// Valor de gravedad
#define G (6.67e-11)

// Valor de delta time. Representa 1 segundo
#define DT 1

struct Position {
    double x;
    double y;
};  

struct Position p[N+1];       // La posicion esta compuesta del eje x y del eje y por eso es un arreglo de struct
struct Position v[N+1];
struct Position f[N+1];
double m[N+1];            // posicion, velocidad, fuerza y masa para los cuerpos

void inicializar_cuerpos() {
    // Inicializacion de los cuerpos con valores random

    //srand(time(NULL));  Para que siempre se generen los mismos valores, esto tiene que estar comentado

    for (int i = 1; i <= N; i++) {  // Limites modificados

        double x = drand48() * 1000;            
        double y = drand48() * 1000; 

        printf("TEST: x = %f ; y = %f \n", x, y);

        double velocityx = rand() % 100;   
        double velocityy = rand() % 100; 

        double mass = rand() % 9999999;  

        printf("TEST: Masa aleatoria es igual a %f \n", mass);       

        p[i].x = x;
        p[i].y = y;

        v[i].x = velocityx;
        v[i].y = velocityy;

        m[i] = mass;

    }
}

void calcular_fuerzas() {
// Calcula las fuerzas para cada uno de los cuerpos

    double distance;
    double magnitude;
    struct Position direction;

    for (int i = 1; i <= N-1; i++) {        // Limites modificados
        
        for(int j = i + 1; j <= N; j++) {   // Limites modificados
            distance = sqrt(
                pow( p[i].x - p[j].x, 2 ) +
                pow( p[i].y - p[j].y, 2 )
            );

            magnitude = ( (G*m[i]*m[j]) / pow(distance, 2) );

            direction.x = p[j].x - p[i].x;
            direction.y = p[j].y - p[i].y;

            f[i].x = f[i].x + magnitude*direction.x/distance;
            f[j].x = f[j].x - magnitude*direction.x/distance;
            f[i].y = f[i].y + magnitude*direction.y/distance;
            f[j].y = f[j].y - magnitude*direction.y/distance;

        }
    }
}

void mover_cuerpos() {
// Mueve cada uno de los cuerpos
    struct Position deltav;     // dv = f/m * DT
    struct Position deltap;     // dp = (v + dv/2) * DT

    for (int i = 1; i <= N; i++) {

        deltav.x = (f[i].x/m[i]) * DT;
        deltav.y = (f[i].y/m[i]) * DT;

        deltap.x = (v[i].x + deltav.x/2) * DT;
        deltap.y = (v[i].y + deltav.y/2) * DT;

        v[i].x = v[i].x + deltav.x;
        v[i].y = v[i].y + deltav.y;

        p[i].x = p[i].x + deltap.x;
        p[i].y = p[i].y + deltap.y;

        f[i].x = 0.0;  
        f[i].y = 0.0;

        printf("[CUERPO %d] x: %.2f | y: %.2f \n", i, p[i].x, p[i].y);
    }

}

int main(int argc, char*argv[]){

    int cant_iteraciones, cant_cuerpos;

    clock_t inicio = clock();

    if (argc == 3){
        cant_cuerpos = atoi(argv[1]);          // Cantidad de cuerpos (deberia reemplazar a la constante N)
        cant_iteraciones = atoi(argv[2]);      // Cantidad de iteraciones | Valor de entrada
    } else {
        printf("Para ejecutar el programa, debe ingresar la cantidad de cuerpos e iteraciones. \n");
        return 0;
    }
    
    //generar_estructuras();        Deberiamos tener un metodo que defina las estrucuras con cant_cuerpos

    inicializar_cuerpos();

    for (int i = 0; i < cant_iteraciones; i++) {

        printf("\n--- ITERACIÓN NRO. %d --- \n", i+1);

        calcular_fuerzas();
        mover_cuerpos();
    }

    clock_t fin = clock();
    double time_spent = 0.0;
    time_spent += (double)(fin - inicio) / CLOCKS_PER_SEC;
 
    printf("Tiempo empleado: %f s. \n", time_spent);
    return 0;
}