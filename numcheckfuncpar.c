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

pthread_mutex_t lock_all, lock_sem;
sem_t semaphore;
int semCounter = 0;
int all = 0;

struct threadArgs
{
    long maxnum;
    int ndigits;
};

// Contadores para cada uma das condições testadas
long match_some_test = 0,
     palindromes = 0,
     repeated_seqs = 0,
     sums_are_ap = 0,
     have_tripled_digits = 0,
     have_four_repetitions = 0;

void *process_is_palindrome(void *input);
void *process_has_repeated_seq(void *input);
void *process_sum_is_ap(void *input);
void *process_has_tripled_digits(void *input);
void *process_has_four_repetitions(void *input);

int main(int argc, char *argv[])
{
    int ndigits; // núm. de dígitos para representar até o maior número
    long tmp, maxnum;
    struct timeval t1, t2; // marcação do tempo de execução
    struct threadArgs *threadData = (struct threadArgs *)malloc(sizeof(struct threadArgs));
    pthread_t funcThreads[NUM_THREADS];
    pthread_mutex_init(&lock_all, NULL);
    pthread_mutex_init(&lock_sem, NULL);
    sem_init(&semaphore, 0, 0);

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

    threadData->maxnum = maxnum;
    threadData->ndigits = ndigits;

    // Marca o tempo e checa cada número na faixa definida.
    // Note que o valor do parâmetro maxnum é considerado inclusive (<=)
    gettimeofday(&t1, NULL);

    pthread_create(&(funcThreads[0]), NULL, process_is_palindrome, (void *)threadData);
    pthread_create(&(funcThreads[1]), NULL, process_has_repeated_seq, (void *)threadData);
    pthread_create(&(funcThreads[2]), NULL, process_sum_is_ap, (void *)threadData);
    pthread_create(&(funcThreads[3]), NULL, process_has_tripled_digits, (void *)threadData);
    pthread_create(&(funcThreads[4]), NULL, process_is_palindrome, (void *)threadData);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(funcThreads[i], NULL);

    gettimeofday(&t2, NULL);

    // Escrita das estatísticas ao final da execução
    // printf("%ld match_some_test (%d%%)\n", match_some_test, (int)((100.0 * match_some_test) / maxnum));
    // printf("%ld palindromes\n", palindromes);
    // printf("%ld repeated_seqs\n", repeated_seqs);
    // printf("%ld sums_are_ap\n", sums_are_ap);
    // printf("%ld have_tripled_digits\n", have_tripled_digits);
    // printf("%ld have_four_repetitions\n", have_four_repetitions);
    // print_max(ndigits);
    // printf("\ntempo: %lf\n", timediff(&t2, &t1));
}

void *process_is_palindrome(void *input)
{
    long maxnum = ((struct threadArgs *)input)->maxnum;
    int ndigits = ((struct threadArgs *)input)->ndigits;
    int pal;
    digit_t num;

    for (long i = 0; i <= maxnum; ++i)
    {
        // check_num(i, ndigits);
        long orign = i;

        // Transforma número (n) em vetor de dígitos (num)
        break_into_digits(i, num, ndigits);

        pal = is_palindrome(num, ndigits);

        palindromes += pal;

        pthread_mutex_lock(&lock_all);
        all += pal;
        semCounter++;
        pthread_mutex_unlock(&lock_all);

        pthread_mutex_lock(&lock_sem);
        if (semCounter == NUM_THREADS) // Última thread a terminar a execução
        {
            // Atualiza valores globais
            if (all > 0)
            {
                match_some_test += 1;
            }
            printf("round %ld | value %ld\n", i, orign);
            update_max(orign, all);

            pthread_mutex_unlock(&lock_sem);

            all = 0;
            semCounter--;
            sem_post(&semaphore);
        }
        else
        {
            pthread_mutex_unlock(&lock_sem);
            sem_wait(&semaphore);
            if (semCounter > 1)
            { // Existem threads em espera
                semCounter--;
                sem_post(&semaphore);
            }
            else
            { // Não existem mais threads em espera
                semCounter--;
            }
        }
    }

    return 0;
}

void *process_has_repeated_seq(void *input)
{
    long maxnum = ((struct threadArgs *)input)->maxnum;
    int ndigits = ((struct threadArgs *)input)->ndigits;
    int rep;
    digit_t num;

    for (long i = 0; i <= maxnum; ++i)
    {
        // check_num(i, ndigits);
        long orign = i;

        // Transforma número (n) em vetor de dígitos (num)
        break_into_digits(i, num, ndigits);

        rep = has_repeated_seq(num, ndigits);

        repeated_seqs += rep;

        pthread_mutex_lock(&lock_all);
        all += rep;
        semCounter++;
        pthread_mutex_unlock(&lock_all);

        pthread_mutex_lock(&lock_sem);
        if (semCounter == NUM_THREADS) // Última thread a terminar a execução
        {
            // Atualiza valores globais
            if (all > 0)
            {
                match_some_test += 1;
            }
            printf("round %ld | value %ld\n", i, orign);
            update_max(orign, all);

            pthread_mutex_unlock(&lock_sem);

            all = 0;
            semCounter--;
            sem_post(&semaphore);
        }
        else
        {
            pthread_mutex_unlock(&lock_sem);
            sem_wait(&semaphore);
            if (semCounter > 1)
            { // Existem threads em espera
                semCounter--;
                sem_post(&semaphore);
            }
            else
            { // Não existem mais threads em espera
                semCounter--;
            }
        }
    }

    return 0;
}

void *process_sum_is_ap(void *input)
{
    long maxnum = ((struct threadArgs *)input)->maxnum;
    int ndigits = ((struct threadArgs *)input)->ndigits;
    int sum;
    digit_t num;

    for (long i = 0; i <= maxnum; ++i)
    {
        // check_num(i, ndigits);
        long orign = i;

        // Transforma número (n) em vetor de dígitos (num)
        break_into_digits(i, num, ndigits);

        sum = sum_is_ap(num, ndigits);

        repeated_seqs += sum;

        pthread_mutex_lock(&lock_all);
        all += sum;
        semCounter++;
        pthread_mutex_unlock(&lock_all);

        pthread_mutex_lock(&lock_sem);
        if (semCounter == NUM_THREADS) // Última thread a terminar a execução
        {
            // Atualiza valores globais
            if (all > 0)
            {
                match_some_test += 1;
            }
            printf("round %ld | value %ld\n", i, orign);
            update_max(orign, all);

            pthread_mutex_unlock(&lock_sem);

            all = 0;
            semCounter--;
            sem_post(&semaphore);
        }
        else
        {
            pthread_mutex_unlock(&lock_sem);
            sem_wait(&semaphore);
            if (semCounter > 1)
            { // Existem threads em espera
                semCounter--;
                sem_post(&semaphore);
            }
            else
            { // Não existem mais threads em espera
                semCounter--;
            }
        }
    }

    return 0;
}

void *process_has_tripled_digits(void *input)
{
    long maxnum = ((struct threadArgs *)input)->maxnum;
    int ndigits = ((struct threadArgs *)input)->ndigits;
    int dou;
    digit_t num;

    for (long i = 0; i <= maxnum; ++i)
    {
        // check_num(i, ndigits);
        long orign = i;

        // Transforma número (n) em vetor de dígitos (num)
        break_into_digits(i, num, ndigits);

        dou = has_tripled_digits(num, ndigits);

        repeated_seqs += dou;

        pthread_mutex_lock(&lock_all);
        all += dou;
        semCounter++;
        pthread_mutex_unlock(&lock_all);

        pthread_mutex_lock(&lock_sem);
        if (semCounter == NUM_THREADS) // Última thread a terminar a execução
        {
            // Atualiza valores globais
            if (all > 0)
            {
                match_some_test += 1;
            }
            printf("round %ld | value %ld\n", i, orign);
            update_max(orign, all);

            pthread_mutex_unlock(&lock_sem);

            all = 0;
            semCounter--;
            sem_post(&semaphore);
        }
        else
        {
            pthread_mutex_unlock(&lock_sem);
            sem_wait(&semaphore);
            if (semCounter > 1)
            { // Existem threads em espera
                semCounter--;
                sem_post(&semaphore);
            }
            else
            { // Não existem mais threads em espera
                semCounter--;
            }
        }
    }

    return 0;
}

void *process_has_four_repetitions(void *input)
{
    long maxnum = ((struct threadArgs *)input)->maxnum;
    int ndigits = ((struct threadArgs *)input)->ndigits;
    int fou;
    digit_t num;

    for (long i = 0; i <= maxnum; ++i)
    {
        // check_num(i, ndigits);
        long orign = i;

        // Transforma número (n) em vetor de dígitos (num)
        break_into_digits(i, num, ndigits);

        fou = has_four_repetitions(num, ndigits);

        repeated_seqs += fou;

        pthread_mutex_lock(&lock_all);
        all += fou;
        semCounter++;
        pthread_mutex_unlock(&lock_all);

        pthread_mutex_lock(&lock_sem);
        if (semCounter == NUM_THREADS) // Última thread a terminar a execução
        {
            // Atualiza valores globais
            if (all > 0)
            {
                match_some_test += 1;
            }
            printf("round %ld | value %ld\n", i, orign);
            update_max(orign, all);

            pthread_mutex_unlock(&lock_sem);

            all = 0;
            semCounter--;
            sem_post(&semaphore);
        }
        else
        {
            pthread_mutex_unlock(&lock_sem);
            sem_wait(&semaphore);
            if (semCounter > 1)
            { // Existem threads em espera
                semCounter--;
                sem_post(&semaphore);
            }
            else
            { // Não existem mais threads em espera
                semCounter--;
            }
        }
    }

    return 0;
}
