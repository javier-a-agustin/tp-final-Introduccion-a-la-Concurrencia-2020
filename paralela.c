// Solucion secuencial n bodies

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include <omp.h>

// Valor de gravedad
#define G (6.67e-11)

// Valor de delta time. Representa 1 segundo
#define DT 1

// Variable utilizada en schedule
#define CHUNK_SIZE 500

struct Position {
    double x;
    double y;
};  

void inicializar_cuerpos(struct Position p[], struct Position v[], struct Position f[], double m[], int size) {
    /* Inicializa los cuerpos con valores aleatorios */

    for (int i = 1; i <= size; i++) {      

        /* Genera posiciones aleatorias */
        double x = drand48() * 1000;            
        double y = drand48() * 1000; 

        /* Genera velocidades aleatorias */
        double velocityx = rand() % 100;   
        double velocityy = rand() % 100; 

        /* Genera masas aleatorias */
        double mass = rand() % 9999999;  

        p[i].x = x;
        p[i].y = y;

        v[i].x = velocityx;
        v[i].y = velocityy;

        m[i] = mass;

    }
}


void calcular_fuerzas(struct Position p[], struct Position f[], double m[], int size) {
    /* Calcula la fuerza para cada uno de los cuerpos */

    double distance;
    double magnitude;
    struct Position direction;

    int i, j;

    #pragma omp parallel for private (j, distance, magnitude, direction) schedule (dynamic, CHUNK_SIZE)
    for (i = 1; i <= size-1; i++) {     
        
        for(j = i + 1; j <= size; j++) {  
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

void mover_cuerpos(struct Position p[], struct Position v[], struct Position f[], double m[], int size) {
    /* Mueve cada uno de los cuerpos */

    struct Position deltav;     // dv = f/m * DT
    struct Position deltap;     // dp = (v + dv/2) * DT

    #pragma omp parallel for private (deltav, deltap) schedule (dynamic, CHUNK_SIZE)
    for (int i = 1; i <= size; i++) {

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

        /* Muestro la posicion actual del cuerpo */
        printf("[CUERPO %d] x: %.2f | y: %.2f \n", i, p[i].x, p[i].y);
    }
}

int main(int argc, char*argv[]){

    double inicio = omp_get_wtime();

    int cant_iteraciones, cant_cuerpos, cant_procesos;

    /* Lee valores de entrada */
    if (argc == 4){

        cant_cuerpos = atoi(argv[1]);         
        cant_iteraciones = atoi(argv[2]);   
        cant_procesos = atoi(argv[3]); 

        printf("Cantidad de cuerpos: %d \n", cant_cuerpos);
        printf("Cantidad de iteraciones: %d \n", cant_iteraciones);
        printf("Cantidad de procesos: %d \n", cant_procesos);

    } else {

        printf("Para ejecutar el programa, debe hacerlo de la siguiente manera: \n");
        printf("%s <cantidad_cuerpos> <cantidad_iteraciones> <cantidad_procesos> \n", argv[0]);

        return 0;
    }
    
    /* Se definen estructuras para almacenar los valores de todos los cuerpos */
    struct Position p[cant_cuerpos+1];    
    struct Position v[cant_cuerpos+1];
    struct Position f[cant_cuerpos+1];
    double m[cant_cuerpos+1];

    inicializar_cuerpos(p, v, f, m, cant_cuerpos);

    /* Selecciono la cantidad de procesos a utilizar */
    omp_set_num_threads(cant_procesos);

    for (int i = 0; i < cant_iteraciones; i++) {

        printf("\n--- ITERACIÓN NRO. %d --- \n", i+1);

        calcular_fuerzas(p, f, m, cant_cuerpos);
        mover_cuerpos(p, v , f, m, cant_cuerpos);
    
    }

    double fin = omp_get_wtime();
    double time_spent = fin - inicio;
 
    printf("Tiempo empleado: %f s. \n", time_spent);
    return 0;
}