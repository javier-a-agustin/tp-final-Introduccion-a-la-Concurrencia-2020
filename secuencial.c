// Solucion secuencial n bodies

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Cantidad de cuerpos
#define N 5

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

    srand48(time(NULL));   // Para que los valores random varien en cada ejecucion
    //srand(time(NULL));

    for (int i = 1; i <= N; i++) {  // Limites modificados

        long double x = rand() % 20;             // Puede que sea una posicion muy baja
        long double y = rand() % 20; 

        printf("TEST: x = %.2f ; y = %.2f \n", x, y);

        double velocityx = rand() % 100;    // Puede que 100 sea una velocidad muy baja
        double velocityy = rand() % 100; 

        //double forcex = rand() % 100;         // Si le damos fuerzas random lanza -nan
        //double forcey = rand() % 100; 

        //double mass = rand() % 10000;         // Estos valores son muy chicos para la masa de un cuerpo

        double decimal = drand48() * (9.0 - 1.0) + 1.0;
        double exp = rand() % (28-22+1) + 22; // No se por que me da 0

        double mass = decimal * pow (10, exp);      // La masa de Marte es 6,4 * 10^23

        printf("TEST: Decimal aleatorio es igual a %f \n", decimal); // Calculo el 6,4 de Marte
        printf("TEST: Exponente aleatorio es igual a %d \n", exp);   // Calculo el 23 de Marte   
        printf("TEST: Masa aleatoria es igual a %f \n", mass);       // Calculo el valor de la masa

        p[i].x = x;
        p[i].y = y;

        v[i].x = velocityx;
        v[i].y = velocityy;

        //f[i].x = forcex;
        //f[i].y = forcey;

        m[i] = mass;

        // printf("Cuerpo %d: posicion: x=%d y=%d | velocidad=%.2f | fuerza=%.2f | masa=%.2f\n",i,x,y,velocity,force,mass);
    }

}

void calcular_fuerzas() {
// Calcula las fuerzas para cada uno de los cuerpos
    double distance;
    double magnitude;
    struct Position direction;

    for (int i = 1; i <= N-1; i++) { // Limites modificados
        
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

            // printf("distance: %.2f, magnitude: %.2f, direccion x: %d direcciony: %d\n", distance, magnitude, direction.x, direction.y);
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

        //f[i].x = f[i].y = 0.0; // resetea el vector fuerza porque? nose 

        f[i].x = 0.0;   // Lo mismo que arriba
        f[i].y = 0.0;

        printf("[CUERPO %d] x: %.2f | y: %.2f \n", i, p[i].x, p[i].y);
    }

}

int main(int argc, char*argv[]){
    /*
        initialize bodies;

        for [time = start to finish by DT] {
            calculate forces;
            move bodies;
        }
    */

    int cant_iteraciones = atoi(argv[1]);      // Cantidad de iteraciones | Valor de entrada

    // if (cant_iteraciones % 2 != 0) {
    // No aplica porque no depende de la cantidad de iteraciones
    //     printf("Entrada incorrecta: Cantidad_Iteraciones debe ser entero\n");
    //     return 0;
    // }

    inicializar_cuerpos();

    for (int i = 0; i < cant_iteraciones; i++) {
        // printf("Calculo de fuerza de iteracion %d\n", i);
        printf("\n--- ITERACIÓN NRO. %d --- \n", i+1);

        //mover_cuerpos();
        calcular_fuerzas();
        mover_cuerpos();
    }

    /* for (int i = 0; i < cant_iteraciones; i++) {
        printf("\n--- ITERACIÓN NRO. %d --- \n", i+1);
        mover_cuerpos();

    }*/

    return 0;
}