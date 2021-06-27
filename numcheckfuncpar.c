/* Você deve implementar uma versão usando paralelismo de funções (também
 * chamado decomposição por funções (function decomposition)
 * Para simplificar, você pode considerar que seu programa criará as
 * threads que você julgar necessárias para processar todos os números,
 * onde cada thread executará uma função diferente, com parte das ações
 * esperadas do programa final.
 * 
 * O programa deve aceitar o mesmo parâmetro de linha de comando da versão
 * sequencial - e nenhum outro. 
 * A saída deve sequir exatamente o mesmo formato da versão sequencial.
 */

// TODO: manipulação dos semáforos é uma zona crítica e necessita de tratamento para tal
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdint.h>
#include <semaphore.h>

#include "timediff.h"   // calcula tempo decorrido
#include "numchecks.h"  // conta números com mais condições válidas
#include "conditions.h" // verifica cada condição

#define NUM_THREADS 5
#define ISPALINDROME 0
#define HASREPSEQ 1
#define SUMISAP 2
#define HASTRPLDIGITS 3
#define HASFOURREP 4

pthread_mutex_t lock_all;
long maxnum;
int ndigits; // núm. de dígitos para representar até o maior número

struct calcThreadArgs
{
    int value;
    int count;
};

struct overallThreadArgs
{
    int method;
    struct calcThreadArgs *values;
};

// Contadores para cada uma das condições testadas
long match_some_test = 0,
     palindromes = 0,
     repeated_seqs = 0,
     sums_are_ap = 0,
     have_tripled_digits = 0,
     have_four_repetitions = 0;

void *process_method(void *input);
void *process_result(void *input);

int main(int argc, char *argv[])
{
    long tmp;
    struct timeval t1, t2; // marcação do tempo de execução
    struct overallThreadArgs threadData[NUM_THREADS];
    pthread_t funcThreads[NUM_THREADS];
    pthread_t calcThread;
    pthread_mutex_init(&lock_all, NULL);

    // tratamento da linha de comando
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s maxnum\n", argv[0]);
        exit(1);
    }
    maxnum = atol(argv[1]);

    // determinação de ndigits em função do maxnum
    tmp = maxnum;
    ndigits = 0;
    do
    {
        ndigits++;
        tmp = tmp / 10;
    } while (tmp > 0);

    struct calcThreadArgs *threadValues = (struct calcThreadArgs *)malloc(sizeof(struct calcThreadArgs) * (maxnum + 1));
    for (long i = 0; i < (maxnum + 1); i++)
    {
        threadValues[i].count = 0;
        threadValues[i].value = 0;
    }

    // Marca o tempo e checa cada número na faixa definida.
    // Note que o valor do parâmetro maxnum é considerado inclusive (<=)
    gettimeofday(&t1, NULL);

    pthread_create(&calcThread, NULL, process_result, threadValues);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        threadData[i].method = i;
        threadData[i].values = threadValues;
        pthread_create(&(funcThreads[i]), NULL, process_method, (void *)&threadData[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(funcThreads[i], NULL);

    pthread_join(calcThread, NULL);

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
}

void *process_method(void *input)
{
    int method = (((struct overallThreadArgs *)input)->method);
    struct calcThreadArgs *values = (((struct overallThreadArgs *)input)->values);
    int val = 0;
    digit_t num;

    for (long i = 0; i <= maxnum; ++i)
    {
        long orign = i;

        // Transforma número (n) em vetor de dígitos (num)
        break_into_digits(i, num, ndigits);

        if (method == ISPALINDROME)
        {
            val = is_palindrome(num, ndigits);
            palindromes += val;
        }
        else if (method == HASREPSEQ)
        {
            val = has_repeated_seq(num, ndigits);
            repeated_seqs += val;
        }
        else if (method == SUMISAP)
        {
            val = sum_is_ap(num, ndigits);
            sums_are_ap += val;
        }
        else if (method == HASTRPLDIGITS)
        {
            val = has_tripled_digits(num, ndigits);
            have_tripled_digits += val;
        }
        else if (method == HASFOURREP)
        {
            val = has_four_repetitions(num, ndigits);
            have_four_repetitions += val;
        }
        else
        {
            fprintf(stderr, "Invalid method.\n");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&lock_all);
        values[orign].count = values[orign].count + 1;
        values[orign].value = values[orign].value + val;
        pthread_mutex_unlock(&lock_all);
    }

    return 0;
}

void *process_result(void *input)
{
    struct calcThreadArgs *values = (((struct calcThreadArgs *)input));
    long valuesSize = (maxnum + 1);

    for (long i = 0; i < valuesSize; i++)
    {
        while (values[i].count < 5)
            ;

        // Atualiza valores globais
        if (values[i].value > 0)
        {
            match_some_test += 1;
        }
        update_max(i, values[i].value);
        values[i].value = 0;
        values[i].count = 0;
    }

    return 0;
}