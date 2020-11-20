#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void *add_item();
void *remove_item();
void *append_buffer();
void *remove_buffer();
int i;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int pro, cons, buff_size, req;

int buffer[100000];

int tail = 0, head = 0, request = 0, success = 0, count = 0;
clock_t timer_stat, timer_stop ,timeoutp,timeoutp2,timeoutc,timeoutc2;

// Specifies that type of variable the function returns.
// main() must return an integer


void *add_item() {
    buffer[head++] = 1;
    head = head % buff_size;
    printf("Append  Head %d Tail %d Buff %d\n", head , tail , buffer[head]);
}

void *remove_item() {
    buffer[tail++] = 0;
    tail = tail % buff_size;
    printf("Remove  Head %d Tail %d Buff %d\n", head , tail , buffer[tail]);
}

void *append_buffer( void* ) {
    printf("Append thread number %ld\n", pthread_self());
    while(request<req) {
        if(!pthread_mutex_trylock(&mutex) && request<req) {
            if(buffer[head] == 0) {
                add_item();
                request++;
                printf(" ++++ thread %ld append success\n", pthread_self());
            }
            else {
            printf("Buffer overflow\n");
              //  timeoutp = clock();
              //  while (buffer[head] != 0)
               // {
               //     timeoutp2 = clock();
               //     if(buffer[head] == 0) { add_item(); request++; break; }
               //     if((timeoutp2 - timeoutp) > timeproducers) { break; }
               //     printf("Time Out\n");
            //	}
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

void *remove_buffer(void*) {
    printf("Remove thread number %ld\n", pthread_self());
    //count--;
    while(success<req) {
        if(!pthread_mutex_trylock(&mutex) && success<req) {
            if(buffer[tail] == 1) {
                remove_item();
                success++;
                //count++;
                printf(" ---- thread %ld remove success\n", pthread_self());
            }
            else {
            printf("Buffer underflow\n");
            	count++;
               // timeoutc = clock();
              //  while (buffer[tail] != 1)
              //  {
              //      timeoutc2 = clock();
             //       if(buffer[tail] == 1) { remove_item(); success++; break; }
             //       if((timeoutc2 - timeoutc) > timeconsumers) { break; }
             //       printf("Time Out\n");
             //   }      	
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}



int main (int argc, char*argv)
{ 

        printf("Producers : ");  
        scanf("%d", &pro);
        
        printf("Consumers : ");
        scanf("%d", &cons);
        
        printf("Buffer size: ");
        scanf("%d", &buff_size);
        
        printf("Requests: ");
        scanf("%d", &req);
        printf("\n");

    

    pthread_t thread_producer[pro];
    pthread_t thread_consumer[cons];
	timer_stat = clock();   //Pull present cpu clock1
/* Create independent threads each of which will used for producers */
    for( i=0; i<pro; i++){
	
        pthread_create(&thread_producer[i], NULL, &append_buffer, NULL);
}
/* Create independent threads each of which will used for consumers */
    for(i=0; i<cons; i++){
	
        pthread_create(&thread_consumer[i], NULL, &remove_buffer, NULL);
}
/* Wait till threads are complete before main continues. Unless we  */
/* wait we run the risk of executing an exit which will terminate   */
/* the process and all threads before the threads have completed.   */
    for(i=0; i<cons; i++){
	
        pthread_join(thread_consumer[i], NULL);
}
    for(i=0; i<pro; i++){
	
        pthread_join(thread_producer[i], NULL);
}

    timer_stop = clock();   //Pull present cpu clock2
    float time_m = ((float)(timer_stop - timer_stat) / CLOCKS_PER_SEC);

    printf("\n");
    printf("# buff %d %d %d %d\n", pro , cons , buff_size , req);
    printf("Producers %d, Consumers %d\n", pro, cons);
    printf("Buffer size %d\n", buff_size);
    printf("Requests %d\n\n", request);
    //¨0success = success -(count/6);
    printf("Successfully consumed %d requests (%.1f%%)\n", success, (float)success * 100 / request);
    printf("Elapsed Time %.2f s\n", time_m);
    printf("Throughput %.2f successful requests/s\n", (float)(success) / time_m);

    exit(EXIT_SUCCESS);
}
