/*
 * Author: Gao Chang
 * Development platform: gcc (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4
 * Last modified date: 17 Nov 2016
 * Compilation: gcc DPP.c -o DPP -Wall -pthread
                ./DPP <N> <S> <T>
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define Thinking 0
#define Waiting 1
#define Eating 2
#define Terminated 3

int N, S, T;
int terminate_flag = 0;

typedef struct {
  int pos;        // position of the philosopher
  int status;     // Thinking, Waiting, Eating, or Terminated
} philosopher;

sem_t *forks;         // semaphores of each fork, should be of size N in total
sem_t *statuslock;    // the semaphore of changing information of philosopher status 
sem_t *forklock;      // the semaphore of changing information of fork holders
int *fork_holders;    // the position of fork holders
pthread_t *threads;   // pthreads, should be of size N (philosopher) + 1 (watcher)
philosopher* philosophers;

/* allocate memory */
void init_mem();
/* initialize semaphores */
void init_sems();
/* initialize threads */
void init_threads();
/* randome usleep time for philosophers to think or eat */
void think_or_eat();
/* run the watcher */
void *run_watcher();
/* run the philosophers */
void *run_philosopher(void *);

int main(int argc, char const *argv[]){
  N = atoi(argv[1]);
  S = atoi(argv[2]);
  T = atoi(argv[3]);

  srandom(S);

  init_mem();
  init_sems();
  init_threads();

  usleep(1000*1000*T);
  terminate_flag = 1;

  pthread_exit(NULL);
  return 0;
}

void init_mem(){
  forks = (sem_t*)malloc(N*sizeof(sem_t));
  statuslock = (sem_t*)malloc(sizeof(sem_t));
  forklock = (sem_t*)malloc(sizeof(sem_t));
  fork_holders = (int*)malloc(N*sizeof(int));
  threads = (pthread_t*)malloc((N+1)*sizeof(pthread_t));
  philosophers = (philosopher*)malloc(N*sizeof(philosopher));
}

void init_sems(){
  int i;
  for(i = 0; i < N; ++i) sem_init(&forks[i], 0, 1);
  sem_init(&statuslock[0], 0, 1);
  sem_init(&forklock[0], 0, 1);
  memset(fork_holders, -1, N*sizeof(int));
}

void init_threads(){
  int i;
  for(i = 0; i < N; ++i) {
    (philosophers+i)->pos = i;
    (philosophers+i)->status = Thinking;
  }
  pthread_create(&threads[N], NULL, run_watcher, NULL);
  for(i = 0; i < N; ++i) {
    pthread_create(&threads[i], NULL, run_philosopher, (void *)(philosophers+i));
  }
}

void think_or_eat(){
  usleep(random()%10000000+1); // one microsecond to ten seconds
}

void *run_philosopher(void *param){

  philosopher* phi = (philosopher*)param;

  int first_fork, second_fork;

  // philosophers with odd number position pick the left fork first
  if (phi->pos%2) {
    first_fork = phi->pos;
    second_fork = (phi->pos+1)%N;
  }
  // philosophers with even number position pick the right fork first
  else {
    first_fork = (phi->pos+1)%N;
    second_fork = phi->pos;
  }

  // start think and eat loop
  while (!terminate_flag) {
    think_or_eat(); // think

    sem_wait(statuslock);
    phi->status = Waiting;
    sem_post(statuslock);

    sem_wait(forks+first_fork); // wait first fork

    sem_wait(forklock);
    fork_holders[first_fork] = phi->pos;
    sem_post(forklock);

    sem_wait(forks+second_fork); // wait second fork

    sem_wait(forklock);
    fork_holders[second_fork] = phi->pos;
    sem_post(forklock);

    sem_wait(statuslock);
    phi->status = Eating;
    sem_post(statuslock);

    think_or_eat(); // eat
    
    // Notice that we need to lock the fork status changing and the status changing altogether
    // otherwise the watcher may found a "Eating" philosopher holding one or no fork
    sem_wait(forklock);
    sem_wait(statuslock);

    sem_post(forks+first_fork); // drop first fork

    fork_holders[first_fork] = -1;

    sem_post(forks+second_fork); // drop second fork

    fork_holders[second_fork] = -1;

    phi->status = Thinking;

    sem_post(statuslock);
    sem_post(forklock);
  }

  sem_wait(statuslock);
  phi->status = Terminated;
  sem_post(statuslock);

  pthread_exit(NULL);
}

void *run_watcher(){
  int tot_think, tot_wait, tot_eat, total_term, tot_inuse;

  // start watching untill all N philosopher threads terminated
  while (total_term != N){
    tot_think = tot_wait = tot_eat = total_term = tot_inuse = 0;
    usleep(500*1000);     // 500 milliseconds
    fprintf(stdout, "Philo\tState\t\t\tFork\tHeld by\n");
    int i;
    sem_wait(forklock);   // lock all fork status semaphores (but not fork semaphores)
    sem_wait(statuslock); // lock all philosopher status semaphores
    for (i = 0; i < N; ++i){
      fprintf(stdout, "[%2d]:\t", i);
      if ((philosophers+i)->status == Thinking) {
        fprintf(stdout, "Thinking\t\t");
        ++tot_think;
      }
      else if ((philosophers+i)->status == Waiting) {
        fprintf(stdout, "Waiting\t\t\t");
        ++tot_wait;
      }
      else if ((philosophers+i)->status == Eating){
        fprintf(stdout, "Eating\t\t\t");
        ++tot_eat;
      }
      else{
        fprintf(stdout, "Terminated\t\t");
        ++total_term;
      }
      fprintf(stdout, "[%2d]:\t", i);
      if(fork_holders[i] != -1){
        fprintf(stdout, "%4d\n", fork_holders[i]);
        ++tot_inuse;
      }
      else fprintf(stdout, "Free\n");
    }
    sem_post(statuslock);
    sem_post(forklock);
    fprintf(stdout, "Th=%2d Wa=%2d Ea=%2d\t\tUse=%2d\tAvail=%2d\n\n\n", tot_think, tot_wait, tot_eat, tot_inuse, N-tot_inuse);
  }
  pthread_exit(NULL);
}
