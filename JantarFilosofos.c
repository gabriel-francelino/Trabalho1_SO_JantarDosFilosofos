#include <stdio.h>
#include <stdbool.h>

#define N 5                  // numero de filosofos
#define LEFT (i + N - 1) % N // numero do vizinho a esquerda de i
#define RIGHT (i + 1) % N    // numero do vizinho a direita de i
#define THINKING 0           // pensando
#define HUNGRY 1             // tentando pegar garfos
#define EATING 2             // comendo
#define COND true            // condição

typedef int semaphore;

int state[N];        // estado do filósofo
semaphore mutex = 1; // controla a regiao critica
semaphore s[N];      // semaforo de cada filosofo

void down(semaphore *mutex)
{
    if (*mutex == 1)
    {
        *mutex = 0;
    }
}

void up(semaphore *mutex)
{
    if (*mutex == 0)
    {
        *mutex = 1;
    }
}

/**
 * @brief Muda o estado do filósofo para pensando
 *
 */
void think()
{
    state[N] = THINKING;
    printf("Filosofo está pensando.\n");
}

/**
 * @brief Muda o estado do filósofo para comendo
 *
 */
void eat()
{
    state[N] = EATING;
    printf("Filosofo está comendo.\n");
}

void test(int i)
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
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
    state[i] = HUNGRY;
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
    state[i] = THINKING;
    test(LEFT);  // ve se o vizinho da esquerda pode comer agora
    test(RIGHT); // ve se o vizinho da direita pode comer agora
    up(&mutex);  // sai da regiao critica
    printf(" O Filosofo %d devolveu os garfos.\n", i);
}

void philosopher(int i)
{
    while (COND)
    {
        think();       // filosofo esta pensando
        take_forks(i); // pega dois garfos ou bloqueia
        eat();         // comendo
        put_forks(i);  // devolver os garfos a mesa
    }
}

int main(int argc, char const *argv[])
{
    for (int i = 1; i <= 5; i++)
    {
        philosopher(i);
    }

    return 0;
}
