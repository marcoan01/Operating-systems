#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

//Prototypes for our three functions out of main 
void * philosopher(void *);  
int eat(int, float);
int think(int, float);

sem_t forks[5];        //Semaphore to control every single fork on the table
sem_t room;            //Semaphore to control the capacity of the room
bool cond = true;      //Condition to terminate the philoopher threads
int e[5] = {0};        //int array to count the eating activities
int t[5] = {0};        //int array to count the thinking activities
float ce[5] = {0};     //float array to keep track of the time for eating
float ct[5] = {0};     //float array to keep track of the time for thinking

int main(int argc, char *argv[])
{
  //variable declaration and initialization
  int ph[5];                        //the value for every philosopher
  pthread_t phil[5];                //5 threads for every philosopher 
  sem_init(&room, 0, 4);            //room semaphore with capacity of 4

  //for loop to initialize every fork 
  for(int i = 0; i < 5; i++)    
    {
      sem_init(&forks[i], 0, 1);    //every fork is initialized as 1
    }

  //for loop to create all the threads
  for(int i = 0; i < 5; i++)
    {
      ph[i] = i;                    //Storing the value for every philosopher
      pthread_create(&phil[i], NULL, philosopher,(void *)&ph[i]);
    }

  sleep(300);                       //Make the main thread to sleep for 5 five                                    minutes so that the other threads can run                                     for that time
  cond = false;                     //set condition to false after sleeping
  
  //for loop to join all the threads to be terminated 
  for(int i = 0; i < 5; i++)
    {
      pthread_join(phil[i], NULL);
     
    }

  printf("\n\n");                   //Printing two new lines for better displa                                      y
  //for loop to print the results for count and time 
  for(int i = 0; i < 5; i++)
    {
      //prints the count of activities for every philosopher 
      printf("\n The eating and thinking count for %d is %d ", i, e[i]);
      //prints the total eating time for every philosopher
      printf("\n The total eating time for %d is %f ", i, ce[i] / 100);
      //prints the total thinking time for every philosopher 
      printf("\n The total thinking time for %d is %f ", i, ct[i] / 100); 
    }
}

void * philosopher (void * p)
{
  int ph =* (int *)p;                     //this int will represent the # for                                           each philosopher 
  float ran;                              //random number in float format
  int random;                             //random number in int format

  //This while loop will keep the threads busy until main is done sleeping
  while(cond == true)
  {

  random = (rand() % (49 - 25 + 1)) + 25; //create a random number (25 to 49)
  ran = (float) random;                   //convert random to a float 
  ran = ran / 1000;                       //divide by 1000 for miliseconds 

  //When there is room availability, the philosopher entering will be printed
  printf("\nThe philosopher %d just entered the room ", ph);
  sem_wait(&room);                        //room semaphore waits until there i                                          s capacity
  t[ph] = think(ph, ran);                 //the philosopher will start thinkin                                          g and time will be stored in t[ph]
  sleep(ran);                             //let the philosopher sleep for that                                          period of random time

  random = (rand() % (49 - 25 + 1)) + 25; //create another random value
  ran = (float) random;                   //convert to float
  ran = ran / 1000;                       //divide it by 1000 for miliseconds
  
  sem_wait(&forks[ph]);                   //wait for the fork to the right to                                           be available for the philosopher 
  sem_wait(&forks[(ph+1)%5]);             //wait for the fork to the left to b                                          e available for the philosopher 

  e[ph] = eat(ph, ran);                   //the philosopher prepares to eat an                                          d the amount of time spent will be s                                          tored in e[ph]
  sleep(ran);                             //let the thread sleep for that time

  
  sem_post(&forks[(ph+1)%5]);             //signaling the fork to the left to                                           be used for the next philosopher 
  sem_post(&forks[ph]);                   //signaling the fork to the right to                                          be ready
  sem_post(&room);                        //signaling availability of the room

  }
  return NULL;
}
int eat (int r, float ra)
{
  //printing eating statement
  printf("\nThe philosopher number %d is eating for %f seconds ", r, ra);
  ce[r] = ce[r] + ra;             //storing the time in the ce[] array
  return e[r] + 1;                //return the sum of the counting of eating
}

int think(int s, float sa)
{
  //printing thinking statement
  printf("\nThe philosopher number %d is thinking for %f seconds ", s, sa);
  ct[s] = ct[s] + sa;               //storing the time in the ct array
  return t[s] + 1;                  //return the sum of the counting of thinki                                    ng
}
