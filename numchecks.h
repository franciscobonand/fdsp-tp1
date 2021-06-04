/* numchecks.h - definição das condições a serem testadas para cada número
 * A PRINCÍPIO ESTE ARQUIVO NÃO PRECISA SER ALTERADO.
*/ 

#define MAXDIGITS 10 // número máximo de dígitos a serem considerados

typedef int digit_t[MAXDIGITS]; // vetor para separar cada algarismo (dígito) de um número

/* break_into_digits: dado n, preenche vetor dígitos com cada algarismo */
void break_into_digits(long n, digit_t digits, int ndigits);

/* retorna 1 se representação com n dígitos é um palíndromo, 0 caso contrário */
int is_palindrome( digit_t n, int ndigits );

/* retorna 1 se representação com n dígitos tem duas partes iguais, 0 caso contrário */
int has_repeated_seq( digit_t n, int ndigits );

/* retorna 1 se a soma dos dígitos é igual à PA com n elementos, 0 caso contrário */
int sum_is_ap( digit_t n, int ndigits );

/* retorna 1 se representação com n dígitos tem um dígito repetido três
 * vezes em sequência, 0 caso contrário */
int has_tripled_digits( digit_t n, int ndigits );

/* retorna 1 se representação com n dígitos tem um dígito repetido quatro
 * vezes em qualquer lugar, 0 caso contrário */
int has_four_repetitions( digit_t n, int ndigits );
