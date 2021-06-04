/* numcheckseq.c - versão sequencial do programa para verificação de números
 * segundo um conjunto de regras pré-definidas
 * A PRINCIPIO, TODO O TRABALHO DE PARALELIZAÇÃO PODE SER FEITO A PARTIR
 * DESTE AQUIVO, SEM PRECISAR ALTERAR OS DEMAIS ARQUIVOS FORNECIDOS
 * Você pode criar, alterar ou remover funções dentro do que considerar
 * necessário para a criação das versões paralelas.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "timediff.h"   // calcula tempo decorrido
#include "numchecks.h"  // conta números com mais condições válidas
#include "conditions.h" // verifica cada condição

// Contadores para cada uma das condições testadas
long match_some_test = 0,
     palindromes = 0,
     repeated_seqs = 0,
     sums_are_ap = 0,
     have_tripled_digits = 0,
     have_four_repetitions = 0;


/* check_num: concentra todos os testes a serem aplicados a cada número.
 */
void check_num(long n, int ndigits)
{
    int all, pal, rep, sum, dou, fou;
    digit_t num;
    long orign = n;

    // Transforma número (n) em vetor de dígitos (num)
    break_into_digits(n, num, ndigits);

    // Aplica os diversos testes a um dado número
    pal = is_palindrome( num, ndigits );
    rep = has_repeated_seq( num, ndigits );
    sum = sum_is_ap( num, ndigits );
    dou = has_tripled_digits( num, ndigits );
    fou = has_four_repetitions( num, ndigits );

    // Para processar número de condições satisfeitas
    all = pal + rep + sum + dou + fou;
    if (all>0) {
        match_some_test += 1;
    }
    update_max( orign, all );

    // Atualiza os contadores por condição
    palindromes += pal;
    repeated_seqs += rep;
    sums_are_ap += sum;
    have_tripled_digits += dou;
    have_four_repetitions += fou;
}

int
main( int argc, char* argv[] )
{
    int  ndigits; // núm. de dígitos para representar até o maior número

    long i, tmp, maxnum;

    struct timeval t1, t2; // marcação do tempo de execução

    // tratamento da linha de comando
    if (argc!=2) {
        fprintf(stderr,"usage: %s maxnum\n",argv[0]);
        exit(1);
    }
    maxnum = atol(argv[1]);

    // determinação de ndigits em função do maxnum
    tmp = maxnum;
    ndigits=0;
    do {
        ndigits++;
        tmp=tmp/10;
    } while (tmp>0);


    // Marca o tempo e checa cada número na faixa definida.
    // Note que o valor do parâmetro maxnum é considerado inclusive (<=)
    gettimeofday(&t1,NULL);
    for (i=0;i<=maxnum;++i) {
        check_num(i,ndigits);
    }
    gettimeofday(&t2,NULL);

    // Escrita das estatísticas ao final da execução
    printf("%ld match_some_test (%d%%)\n", match_some_test, (int)((100.0*match_some_test)/maxnum));
    printf("%ld palindromes\n", palindromes);
    printf("%ld repeated_seqs\n", repeated_seqs);
    printf("%ld sums_are_ap\n", sums_are_ap);
    printf("%ld have_tripled_digits\n", have_tripled_digits);
    printf("%ld have_four_repetitions\n", have_four_repetitions);
    print_max( ndigits );
    printf("\ntempo: %lf\n",timediff(&t2,&t1));
}
