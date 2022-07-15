#include <stdio.h>
#include <stdbool.h>

#define N 5                      // numero de filosofos
#define ESQUERDA (i + N - 1) % N // numero do vizinho a esquerda de i
#define DIREITA (i + 1) % N      // numero do vizinho a direita de i
#define PENSANDO 0               // pensando
#define COM_FOME 1               // tentando pegar garfos
#define COMENDO 2                // comendo
bool cond = true;                // condição

typedef int semaphore;

int state[N];        // estado do filósofo
semaphore mutex = 1; // controla a regiao critica
semaphore s[N];      // semaforo de cada filosofo

void down(semaphore *mutex)
{
}

void up(semaphore *mutex)
{
}

/**
 * @brief Muda o estado do filósofo para pensando
 *
 */
void think()
{
    state[N] = PENSANDO;
    printf("Filosofo está pensando.\n");
}

/**
 * @brief Muda o estado do filósofo para comendo
 *
 */
void eat()
{
    state[N] = COMENDO;
    printf("Filosofo está comendo.\n");
}

void test(int i)
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
void take_forks(int i)
{
    down(&mutex); // entra na regiao critica
    state[i] = COM_FOME;
    test(i);     // tenta pegar os garfos
    up(&mutex);  // sai da regiao critica
    down(&s[i]); // bloqueia os garfos que não foram pegos
    printf(" O Filosofo %d pegou os garfos.\n", i);
}

/**
 * @brief Devolve os dois garfos
 *
 * @param i O número do filósofo, de 0 a N–1
 */
void put_forks(int i)
{
    down(&mutex); // entra na regiao critica
    state[i] = PENSANDO;
    test(ESQUERDA); // ve se o vizinho da esquerda pode comer agora
    test(DIREITA);  // ve se o vizinho da direita pode comer agora
    up(&mutex);     // sai da regiao critica
    printf(" O Filosofo %d devolveu os garfos.\n", i);
}

void philosopher(int i)
{
    while (cond)
    {
        think();       // filosofo esta pensando
        take_forks(i); // pega dois garfos ou bloqueia
        eat();         // comendo
        put_forks(i);  // devolver os garfos a mesa
    }
}

int main(int argc, char const *argv[])
{
    int i=0;
    while (i < N)
    {
        philosopher(5);
    }
    return 0;
}
