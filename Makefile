all: secuencial paralela

secuencial: secuencial.c
	gcc secuencial.c -o secuencial -lm

paralela: paralela.c
	gcc paralela.c -o paralela -lm -fopenmp