#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5                      // numero de filosofos
#define ESQUERDA (i + N - 1) % N // numero do vizinho a esquerda de i
#define DIREITA (i + 1) % N      // numero do vizinho a direita de i
#define PENSANDO 0               // pensando
#define FAMINTO 1                // tentando pegar garfos
#define COMENDO 2                // comendo

int estado[N]; // estado do filósofo
sem_t mutex;   // controla a regiao critica
sem_t s[N];    // semaforo de cada filosofo
// pthread_t jantar[N];   // Todos os filósofos

void *filosofo(void *param);
void pegarGarfo(int i);
void devolverGarfo(int i);
void testar(int i);
void comer(int i);
void pensar(int i);

/**
 * @brief Mostra o estado de cada filósofo
 *
 */
void mostrar()
{
    for (int i = 1; i <= N; i++)
    {
        if (estado[i - 1] == PENSANDO)
        {
            printf("O Filosofo %d esta pensando.\n", i);
        }
        if (estado[i - 1] == FAMINTO)
        {
            printf("O Filosofo %d esta faminto.\n", i);
        }
        if (estado[i - 1] == COMENDO)
        {
            printf("O Filosofo %d esta comendo.\n", i);
        }
    }
    printf("\n");
}

/**
 * @brief Realização das ações do filósofo no jantar
 *
 * @param i índice do filósofo
 */
void *filosofo(void *id)
{
    int i = *(int *)id; // Repassa o id do filósofo
    while (true)
    {
        pensar(i);        // filosofo esta pensando
        pegarGarfo(i);    // pega dois garfos ou bloqueia
        comer(i);         // comendo
        devolverGarfo(i); // devolver os garfos a mesa
    }
}

/**
 * @brief Pega os dois garfos
 *
 * @param i  O número do filósofo, de 0 a N–1
 */
void pegarGarfo(int i)
{
    sem_wait(&mutex);    // entra na regiao critica
    estado[i] = FAMINTO; // altera o estado do filósofo
    mostrar();
    testar(i);        // tenta pegar os garfos
    sem_post(&mutex); // Sai na região crítica
    sem_wait(&s[i]);  // Bloqueia os hashis
}

/**
 * @brief Devolve os dois garfos
 *
 * @param i O número do filósofo, de 0 a N–1
 */
void devolverGarfo(int i)
{
    sem_wait(&mutex); // entra na regiao critica
    estado[i] = PENSANDO;
    mostrar();
    testar(ESQUERDA); // ve se o vizinho da esquerda pode comer agora
    testar(DIREITA);  // ve se o vizinho da direita pode comer agora
    sem_post(&mutex); // Sai da regiсo crítica
}

/**
 * @brief Testa se os filósofos vizinhos estão comendo.
 *
 * @param i índice do filósofo
 */
void testar(int i)
{
    if (estado[i] == FAMINTO && estado[ESQUERDA] != COMENDO && estado[DIREITA] != COMENDO)
    {
        estado[i] = COMENDO;
        mostrar();
        sem_post(&s[i]); // libera os garfos
    }
}

/**
 * @brief Muda o estado do filósofo para pensando
 *
 * @param i índice do filósofo
 */
void pensar(int i)
{
    //int r = (rand() % 10 + 1);
    float r = 0.001*random();
    mostrar();
    usleep((int)r);
}

/**
 * @brief Muda o estado do filósofo para comendo
 *
 * @param i índice do filósofo
 */
void comer(int i)
{
    //int r = (rand() % 10 + 1); // gerar valores de tempo de 1 a 10
    float r = 0.001*random();
    mostrar();
    usleep((int)r);
}

void excecao(int e)
{
    if (e != 0)
    {
        perror("Erro na inicializacao do semaforo");
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    for (int i = 0; i < N; i++)
    {
        estado[i] = 0;
    }
    mostrar();
    int res;
    pthread_t thread[N];
    void *thread_result;

    // Inicia os semaforos
    res = sem_init(&mutex, 0, 1);
    excecao(res);

    for (int i = 0; i < N; i++)
    {
        res = sem_init(&s[i], 0, 0);
        excecao(res);
    }

    // cria as threads(filosofos)
    for (int i = 0; i < N; i++)
    {
        res = pthread_create(&thread[i], NULL, filosofo, &i);
        excecao(res);
    }

    // faz um join nas threads
    for (int i = 0; i < N; i++)
    {
        res = pthread_join(thread[i], &thread_result);
        excecao(res);
    }
}
