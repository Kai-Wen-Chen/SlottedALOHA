#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#define transNum 100 //#transmitter
int tx_counter = 0; //#successfully transmitted package
int col_counter = 0; //#collision
int t_prb = 100; //transmit probability = 0.01
int receiver[100][transNum]; //receiver
int rec_collide[100] = {0}; //collision record of each slot

void* Transmit(void*);

int main(){
    int i, j;
    srand(time(NULL));

    pthread_t tid[transNum]; //transmitting thread
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    for (i=0; i<transNum; i++) //create thread(transmitter)
    pthread_create(&tid[i], &attr, Transmit, (void*)i);

    for (i=0; i<transNum; i++) //collect the result
    pthread_join(tid[i], NULL);

    for (i=0; i<100; i++) {
        int tmp = 0;
        for (j=0; j<transNum; j++) {
            if (receiver[i][j] == 1) tmp++; //count #package transmitted each slot
        }
        if (tmp > 1) { //if there are more than one package transmitted in the same slot
                rec_collide[i] = 1; //collision occur
                col_counter++;
        }
        else if (tmp == 1) tx_counter++; //only a package transmitted in a slot

        //printf("tmp = %d\n", tmp);
    }

    printf("#transmit: %d\n", tx_counter);
    printf("#collide: %d\n", col_counter);

    pthread_exit(0);

    system("pause");
}

void* Transmit(void* tx){
    int i;
    int transmitter = (int*)tx;

    for (i=0; i<100; i++){
        int tx = rand()%t_prb + 1; //generate a random integer between 1 ~ 100
        if (tx == 1){ //transmit
            receiver[i][transmitter] = 1;
        } else { //don't transmit
            receiver[i][transmitter] = 0;
        }
    }
}
