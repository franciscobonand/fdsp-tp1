/* Você deve implementar uma versão usando paralelismo de dados (também
 * chamado decomposição/partição de domínio (domain partition/decomposition)
 * Para simplificar, você pode considerar que seu programa criará oito
 * threads para processar todos os números. Todas as threads devem executar
 * o mesmo código, já que estarão executando as mesmas operações para
 * partes diferentes do conjunto de dados.
 * 
 * O programa deve aceitar o mesmo parâmetro de linha de comando da versão
 * sequencial - e nenhum outro. 
 * A saída deve sequir exatamente o mesmo formato da versão sequencial.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#include "timediff.h"   // calcula tempo decorrido
#include "numchecks.h"  // conta números com mais condições válidas
#include "conditions.h" // verifica cada condição

#define NUM_THREADS 4

struct threadArgs
{
    long begin;
    long end;
    long max;
    int ndigits;
};

// Variáveis globais
pthread_mutex_t lock;

// Contadores para cada uma das condições testadas
long match_some_test = 0,
     palindromes = 0,
     repeated_seqs = 0,
     sums_are_ap = 0,
     have_tripled_digits = 0,
     have_four_repetitions = 0;

// check_num: concentra todos os testes a serem aplicados a cada número.
void check_num(long n, int ndigits)
{
    int all, pal, rep, sum, dou, fou;
    digit_t num;
    long orign = n;

    // Transforma número (n) em vetor de dígitos (num)
    break_into_digits(n, num, ndigits);

    // Aplica os diversos testes a um dado número
    pal = is_palindrome(num, ndigits);
    rep = has_repeated_seq(num, ndigits);
    sum = sum_is_ap(num, ndigits);
    dou = has_tripled_digits(num, ndigits);
    fou = has_four_repetitions(num, ndigits);

    // Para processar número de condições satisfeitas
    all = pal + rep + sum + dou + fou;

    // Seção crítica
    pthread_mutex_lock(&lock);
    if (all > 0)
    {
        match_some_test += 1;
    }
    update_max(orign, all);

    // Atualiza os contadores por condição
    palindromes += pal;
    repeated_seqs += rep;
    sums_are_ap += sum;
    have_tripled_digits += dou;
    have_four_repetitions += fou;
    pthread_mutex_unlock(&lock);
}

void *processData(void *input)
{
    long begin = ((struct threadArgs *)input)->begin;
    long end = ((struct threadArgs *)input)->end;
    long max = ((struct threadArgs *)input)->max;
    int ndigits = ((struct threadArgs *)input)->ndigits;

    if (begin < max && end <= max)
    {
        // printf("%ld %ld %d\n", begin, end, ndigits);
        for (long i = begin; i <= end; ++i)
        {
            check_num(i, ndigits);
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int ndigits;      // núm. de dígitos para representar até o maior número
    int intervalSize; // tamanho de cada partição dos dados
    int currInterval = 0;
    long tmp, maxnum;
    struct timeval t1, t2; // marcação do tempo de execução
    struct threadArgs threadData[NUM_THREADS];
    pthread_t dataThreads[NUM_THREADS];
    pthread_mutex_init(&lock, NULL);

    // tratamento da linha de comando
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s maxnum\n", argv[0]);
        exit(1);
    }
    maxnum = atol(argv[1]);

    intervalSize = maxnum / NUM_THREADS;
    // determinação de ndigits em função do maxnum
    tmp = maxnum;
    ndigits = 0;
    do
    {
        ndigits++;
        tmp = tmp / 10;
    } while (tmp > 0);

    // Marca o tempo e checa cada número na faixa definida.
    // Note que o valor do parâmetro maxnum é considerado inclusive (<=)
    gettimeofday(&t1, NULL);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        threadData[i].begin = currInterval;
        threadData[i].ndigits = ndigits;
        threadData[i].max = maxnum;
        if (i == NUM_THREADS - 1)
        {
            threadData[i].end = maxnum;
        }
        else
        {
            threadData[i].end = currInterval + intervalSize;
        }

        pthread_create(&(dataThreads[i]), NULL, processData, (void *)&threadData[i]);

        currInterval = currInterval + intervalSize + 1;
    }

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(dataThreads[i], NULL);

    gettimeofday(&t2, NULL);

    // Escrita das estatísticas ao final da execução
    printf("%ld match_some_test (%d%%)\n", match_some_test, (int)((100.0 * match_some_test) / maxnum));
    printf("%ld palindromes\n", palindromes);
    printf("%ld repeated_seqs\n", repeated_seqs);
    printf("%ld sums_are_ap\n", sums_are_ap);
    printf("%ld have_tripled_digits\n", have_tripled_digits);
    printf("%ld have_four_repetitions\n", have_four_repetitions);
    print_max(ndigits);
    printf("\ntempo: %lf\n", timediff(&t2, &t1));

    return 0;
}
