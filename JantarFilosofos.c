#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 5                      // numero de filosofos
#define ESQUERDA (i + N - 1) % N // numero do vizinho a esquerda de i
#define DIREITA (i + 1) % N      // numero do vizinho a direita de i
#define PENSANDO 0               // pensando
#define COM_FOME 1               // tentando pegar garfos
#define COMENDO 2                // comendo

typedef int semaforo;

int state[N];       // estado do filósofo
semaforo mutex = 1; // controla a regiao critica
semaforo s[N];      // semaforo de cada filosofo

/**
 * @brief Libera a região crítica
 *
 * @param mutex
 */
void down(semaforo *mutex)
{
    mutex--;
}

/**
 * @brief Ocupa a região crítica
 *
 * @param mutex
 */
void up(semaforo *mutex)
{
    mutex++;
}

/**
 * @brief Muda o estado do filósofo para pensando
 *
 * @param i índice do filósofo
 */
void pensar(int i)
{
    state[N] = PENSANDO;
    printf("\nFilosofo %d está pensando.\n", i);
}

/**
 * @brief Muda o estado do filósofo para comendo
 *
 * @param i índice do filósofo
 */
void comer(int i)
{
    state[N] = COMENDO;
    printf("Filosofo %d está comendo.\n", i);
}

/**
 * @brief Testa se os filósofos vizinhos estão comendo.
 *
 * @param i índice do filósofo
 */
void testar(int i)
{
    if (state[i] == COM_FOME && state[ESQUERDA] != COMENDO && state[DIREITA] != COMENDO)
    {
        state[i] = COMENDO;
        up(&s[i]);
    }
}

/**
 * @brief Pega os dois garfos
 *
 * @param i  O número do filósofo, de 0 a N–1
 */
void pegarGarfo(int i)
{
    down(&mutex); // entra na regiao critica
    state[i] = COM_FOME;
    testar(i);   // tenta pegar os garfos
    up(&mutex);  // sai da regiao critica
    down(&s[i]); // bloqueia os garfos que não foram pegos
    printf(" O Filosofo %d pegou os garfos.\n", i);
}

/**
 * @brief Devolve os dois garfos
 *
 * @param i O número do filósofo, de 0 a N–1
 */
void devolverGarfo(int i)
{
    down(&mutex); // entra na regiao critica
    state[i] = PENSANDO;
    testar(ESQUERDA); // ve se o vizinho da esquerda pode comer agora
    testar(DIREITA);  // ve se o vizinho da direita pode comer agora
    up(&mutex);       // sai da regiao critica
    printf(" O Filosofo %d devolveu os garfos.\n", i);
}

/**
 * @brief Realização das ações do filósofo no jantar
 *
 * @param i índice do filósofo
 */
void filosofo(int i)
{
    while (true)
    {
        pensar(i);        // filosofo esta pensando
        pegarGarfo(i);    // pega dois garfos ou bloqueia
        comer(i);         // comendo
        devolverGarfo(i); // devolver os garfos a mesa
        if (i<5)
        {
            i++;
        }else
            exit(0);       
    }
}

int main(int argc, char const *argv[])
{
    int i = 1;
    filosofo(i);
    return 0;
}
