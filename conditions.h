/* count_conditions.h: acompanha o máximo de condições que algum número atinge.
 * A PRINCÍPIO ESTE ARQUIVO NÃO PRECISA SER ALTERADO.
 */

extern int max_conditions, // número máximo de condições em um só número
           max_count;      // contage de números com o máximo de condições

extern long smallest_max,  // menor número que tem o máximo de condições
            largest_max;   // maior número que tem o máximo de condições


/* update_max recebe um número e o número de condições nele verificadas
 * e atualiza as variáveis acima, caso seja necessário.
 */
void update_max( long n, int conds );

/* print_max escreve na saída as informações mantidas nas variáveis acima.
 */
void print_max( int ndigits );
