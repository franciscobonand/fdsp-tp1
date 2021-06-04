/* timediff.h - define a função timediff para facilitar a medição do tempo
 * de execução. A marcação do tempo será com a função gettimeofday.
 */

/* timediff recebe as structs timeval que registram o tempo final e o tempo
 * inicial e retorna um número de ponto flutuante com os segundos
 * decorridos entre as duas marcas de tempo.
 */
double timediff( struct timeval* tf, struct timeval* ti);
