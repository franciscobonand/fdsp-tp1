/* count_conditions.c: acompanha o máximo de condições que algum número atinge.
 * A PRINCÍPIO ESTE ARQUIVO NÃO PRECISA SER ALTERADO.
 */
#include <stdio.h>
#include "conditions.h"

int  max_conditions = 0; // número máximo de condições em um só número
int  max_count = 0;      // contage de números com o máximo de condições
long smallest_max = -1;  // menor número que tem o máximo de condições
long largest_max = -1;   // maior número que tem o máximo de condições

/* update_max recebe um número e o número de condições nele verificadas
 * e atualiza as variáveis acima, caso seja necessário.
 */
void update_max( long n, int conds )
{
    if (conds>max_conditions) {
        // se o número tem satisfaz mais condições que o máximo já visto,
        // atualiza as variáveis para começar a registrar outros.
        max_count = 1;
        max_conditions = conds;
        largest_max = n;
        smallest_max = n;
    } else if (conds==max_conditions) {
        // senão, se ele satisfaz tantas condições quan do o máximo,
        // conta-o e verifica se o número é o maior ou menor deles.
        max_count ++;
        if (n>largest_max)  largest_max = n;
        if (n<smallest_max) smallest_max = n;
    }
}

/* print_max escreve na saída as informações mantidas nas variáveis acima.
 */
void print_max( int ndigits )
{
    printf( "%d numbers had %d conditions; smallest: %0*ld; largest: %0*ld\n",
            max_count, max_conditions,
            ndigits, smallest_max, ndigits, largest_max );
}
