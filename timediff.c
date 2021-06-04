/* timediff.c - define a função timediff para facilitar a medição do tempo
 * de execução. A marcação do tempo será com a função gettimeofday.
 */
#include <sys/time.h>

#include "timediff.h"

/* timediff recebe as structs timeval que registram o tempo final e o tempo
 * inicial e retorna um número de ponto flutuante com os segundos
 * decorridos entre as duas marcas de tempo.
 */
double timediff( struct timeval* tvf, struct timeval* tvi)
{   
    double dtf, dti;
    dtf = tvf->tv_sec + (tvf->tv_usec/1.0e6); // converte para ponto flutuante
    dti = tvi->tv_sec + (tvi->tv_usec/1.0e6);
    return dtf - dti; // retorna a diferença entre os tempos
}
