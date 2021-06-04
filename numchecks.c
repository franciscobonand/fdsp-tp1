/* numchecks.c - implementação das condições a serem testadas para cada número
 * A PRINCÍPIO ESTE ARQUIVO NÃO PRECISA SER ALTERADO.
*/ 
#include <stdio.h>
#include <stdlib.h>

#include "numchecks.h"

/* break_into_digits: dado n, preenche vetor dígitos com cada algarismo */
void break_into_digits(long n, digit_t digits, int ndigits)
{
    // no main já se definiu o número de dígitos a serem usados (ndigits)
    for (int i=0;i<ndigits;++i) {
        digits[ndigits-i-1] = n % 10; // %10 dá o dígito mais à direita
        n = n / 10;                   // /10 descarta aquele dígito
    }
}

/* retorna 1 se representação com n dígitos é um palíndromo, 0 caso contrário */
int is_palindrome( digit_t n, int ndigits )
{
    int lim = ndigits/2;
    for (int i=0;i<lim;++i) {
        if (n[i]!=n[ndigits-i-1]) { // condição padrão de teste de palíndromos
            return 0;
        }
    }
    return 1;
}

/* retorna 1 se representação com n dígitos tem duas partes iguais, 0 caso contrário */
int has_repeated_seq( digit_t n, int ndigits )
{
    /* Nesse caso, estamos considerando que se o número de dígitos é ímpar,
     * o último dígito é ignorado; poderia ser outra forma, mas essa foi
     * escolhida.
     */
    int lim = ndigits/2;
    for (int i=0;i<lim;++i) {
        if (n[i]!=n[lim+i]) {
            return 0;
        }
    }
    return 1;
}

/* retorna 1 se a soma dos dígitos é igual à PA com n elementos, 0 caso contrário */
int sum_is_ap( digit_t n, int ndigits )
{
    int sum = 0;
    // sum armazena a soma de todos os dígitos
    for (int i=0;i<ndigits;++i) {
        sum+=n[i];
    }
    // estamos considerando a soma da PA incluindo o zero
    return (sum==(ndigits*(ndigits-1)/2))?1:0;
}

/* retorna 1 se representação com n dígitos tem um dígito repetido três
 * vezes em sequência, 0 caso contrário */
int has_tripled_digits( digit_t n, int ndigits )
{
    for (int i=0;i<ndigits-2;++i) {
        // código simples para fazer o teste, não precisa ser mais eficiente
        if ((n[i]==n[i+1])&&(n[i]==n[i+2])) {
            return 1;
        }
    }
    return 0;
}

/* retorna 1 se representação com n dígitos tem um dígito repetido quatro
 * vezes em qualquer lugar, 0 caso contrário */
int has_four_repetitions( digit_t n, int ndigits )
{
    int reps[10] = { 0,0,0,0,0,0,0,0,0,0 };
    int i;

    // vai somando as ocorrências de cada valor nos dígitos em reps
    for (i=0;i<ndigits;++i) {
         reps[n[i]]++;
         // como só precisamos saber se tem pelo menos 4, podemos voltar
         // assim que a condição é satisfeita, não é preciso terminar.
         if (reps[n[i]]==4) {
            return 1;
         }
    }
    return 0;
}
