#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//#include <Windows.h>

// #ifdef _WIN32 || _WIN64
//     #include <Windows.h>
// #else
//     #include <unistd.h>
// #endif

#define N 5                      // numero de filosofos
#define ESQUERDA (i + N - 1) % N // numero do vizinho a esquerda de i
#define DIREITA (i + 1) % N      // numero do vizinho a direita de i
#define PENSANDO 0               // pensando
#define FAMINTO 1                // tentando pegar garfos
#define COMENDO 2                // comendo

typedef pthread_mutex_t semaforo;

int state[N];        // estado do filósofo
semaforo mutex;      // controla a regiao critica
semaforo s[N];       // semaforo de cada filosofo
pthread_t jantar[N]; // Todos os filósofos

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
    printf("\nFilosofo %d esta pensando.\n", i);
    usleep(1000 * 1000);
}

/**
 * @brief Muda o estado do filósofo para comendo
 *
 * @param i índice do filósofo
 */
void comer(int i)
{
    state[N] = COMENDO;
    printf("Filosofo %d esta comendo.\n", i);
    usleep(2000 * 1000);
}

/**
 * @brief Testa se os filósofos vizinhos estão comendo.
 *
 * @param i índice do filósofo
 */
void testar(int i)
{
    if (state[i] == FAMINTO && state[ESQUERDA] != COMENDO && state[DIREITA] != COMENDO)
    {
        state[i] = COMENDO;
        pthread_mutex_unlock(&(s[i]));
    }
}

/**
 * @brief Pega os dois garfos
 *
 * @param i  O número do filósofo, de 0 a N–1
 */
void pegarGarfo(int i)
{
    pthread_mutex_lock(&(mutex)); // entra na regiao critica
    printf(" O Filosofo %d pegou os garfos.\n", i);
    state[i] = FAMINTO;
    testar(i);                      // tenta pegar os garfos
    up(&mutex);                     // sai da regiao critica
    pthread_mutex_unlock(&(mutex)); // Sai na região crítica
    pthread_mutex_lock(&(s[i]));    // Bloqueia os hashis
    usleep(1000 * 1000);
}

/**
 * @brief Devolve os dois garfos
 *
 * @param i O número do filósofo, de 0 a N–1
 */
void devolverGarfo(int i)
{
    pthread_mutex_lock(&(mutex)); // entra na regiao critica
    printf(" O Filosofo %d devolveu os garfos.\n", i);
    state[i] = PENSANDO;
    testar(ESQUERDA);               // ve se o vizinho da esquerda pode comer agora
    testar(DIREITA);                // ve se o vizinho da direita pode comer agora
    pthread_mutex_unlock(&(mutex)); // Sai da regiсo crítica
    usleep(1000);
}

/**
 * @brief Realização das ações do filósofo no jantar
 *
 * @param i índice do filósofo
 */
void *filosofo(void *vparam)
{
    int *i = (int *)(vparam); // Repassa o id do filósofo

    while (true)
    {
        pensar(*(i));        // filosofo esta pensando
        pegarGarfo(*(i));    // pega dois garfos ou bloqueia
        comer(*(i));         // comendo
        devolverGarfo(*(i)); // devolver os garfos a mesa
        // if (i < N)
        // {
        //     i++;
        // }
        // else
        //     i = 1;
    }

    pthread_exit((void *)0); // Legado do retorno
}

int main(int argc, char const *argv[])
{
    // int i = 1;
    // filosofo(i);
    // return 0;
    int cont;   // Contador auxiliar
    int status; // Criação da thread

    // Inicia os mutexes
    pthread_mutex_init(&(mutex), NULL);
    for (cont = 0; cont < N; cont++)
    {
        pthread_mutex_init(&(s[cont]), NULL);
    }

    // Inicia threads (filósofos)
    for (cont = 0; cont < N; cont++)
    {
        // verifica se ocorreu erro
        status = pthread_create(&(jantar[cont]), NULL, filosofo, (void *)&(cont));
        if (status)
        {
            printf("Erro criando thread %d, retornou codigo %d\n", cont, status);
            return EXIT_FAILURE;
        }
    }

    // Destroi antes de sair
    pthread_mutex_destroy(&(mutex));
    for (cont = 0; cont < N; cont++)
    {
        pthread_mutex_destroy(&(s[cont]));
    }
    pthread_exit(NULL);

    return EXIT_SUCCESS;    
}
