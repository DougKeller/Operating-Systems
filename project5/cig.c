#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>		// printf()
#include <stdlib.h>		// srand() and rand()
#include <time.h>		// time()
#include <unistd.h>		// sleep()

#define FALSE 0
#define TRUE 1
#define TOBAC 0
#define PAPER 1
#define MATCH 2

sem_t agentSem,
	  mutex,
	  tobacco,
	  paper,
	  match,
	  tobaccoSem,
	  paperSem,
	  matchSem;
int isTobacco = FALSE,
	isPaper = FALSE,
	isMatch = FALSE,
	numTobacco = 0,
	numPaper = 0,
	numMatch = 0;

void *agent(void* arg) {
	int argVal = (int*)arg;
	int id = argVal + 1;

	printf("Agent %d started\n", id);

	for(int it = 0; it < 6; it++) {
		// Sleep for up to 200ms
		int microsecs = rand() % 1000;
		usleep(microsecs * 1000);
		
		// Wait for agentSem to be available
		sem_wait(&agentSem);

		// Make resources available depending on combination type
		switch(id) {
			case 1:
				numTobacco++;
				numPaper++;
				printf("Agent %d provided TOBAC and PAPER\n", id);
				sem_post(&tobacco);
				sem_post(&paper);
				break;
			case 2:
				numPaper++;
				numMatch++;
				printf("Agent %d provided PAPER and MATCH\n", id);
				sem_post(&paper);
				sem_post(&match);
				break;
			case 3:
				numMatch++;
				numTobacco++;
				printf("Agent %d provided MATCH and TOBAC\n", id);
				sem_post(&match);
				sem_post(&tobacco);
				break;
		}

		// Allow other agents to continue
		sem_post(&agentSem);
	}

	printf("Agent %d complete\n", id);
}

void tobaccoPusher() {
	printf("TOBAC pusher started\n");

	for(int it = 0; it < 12; it++) {
		sem_wait(&tobacco);
		sem_wait(&mutex);

		if(isPaper) {
			isPaper = FALSE;
			printf("TOBAC pusher alerted MATCH smoker\n");
			sem_post(&matchSem);
		} else if(isMatch) {
			isMatch = FALSE;
			printf("TOBAC pusher alerted PAPER smoker\n");
			sem_post(&paperSem);
		} else {
			isTobacco = TRUE;
			printf("TOBAC pusher signals availability\n");
		}

		sem_post(&mutex);
	}

	printf("TOBAC pusher complete\n");
}

void paperPusher() {
	printf("PAPER pusher started\n");

	for(int it = 0; it < 12; it++) {
		sem_wait(&paper);
		sem_wait(&mutex);

		if(isMatch) {
			isMatch = FALSE;
			printf("PAPER pusher alerted TOBAC smoker\n");
			sem_post(&tobaccoSem);
		} else if(isTobacco) {
			isTobacco = FALSE;
			printf("PAPER pusher alerted MATCH smoker\n");
			sem_post(&matchSem);
		} else {
			isPaper = TRUE;
			printf("PAPER pusher signals availability\n");
		}

		sem_post(&mutex);
	}

	printf("PAPER pusher complete\n");
}

void matchPusher() {
	printf("MATCH pusher started\n");

	for(int it = 0; it < 12; it++) {
		sem_wait(&match);
		sem_wait(&mutex);

		if(isTobacco) {
			isTobacco = FALSE;
			printf("MATCH pusher alerted PAPER smoker\n");
			sem_post(&paperSem);
		} else if(isPaper) {
			isPaper = FALSE;
			printf("MATCH pusher alerted TOBAC smoker\n");
			sem_post(&tobaccoSem);
		} else {
			isMatch = TRUE;
			printf("MATCH pusher signals availability\n");
		}

		sem_post(&mutex);
	}

	printf("MATCH pusher complete\n");
}

void *pusher(void* arg) {
	int type = (int*)arg;

	switch(type) {
		case TOBAC:
			tobaccoPusher();
			break;
		case PAPER:
			paperPusher();
			break;
		case MATCH:
			matchPusher();
			break;
	}
}

void tobaccoSmoker(int id) {
	printf("TOBAC smoker %d started\n", id);

	for(int smoked = 0; smoked < 3; smoked++) {
		sem_wait(&tobaccoSem);
		sem_wait(&agentSem);

		numPaper -= 1;
		numMatch -= 1;

		printf("TOBAC smoker %d created and smoked cigarette #%d\n", id, smoked + 1);

		sem_post(&agentSem);
	}

	printf("TOBAC smoker %d left to go eat\n", id);
}

void paperSmoker(int id) {
	printf("PAPER smoker %d started\n", id);

	for(int smoked = 0; smoked < 3; smoked++) {
		sem_wait(&paperSem);
		sem_wait(&agentSem);

		numMatch -= 1;
		numTobacco -= 1;

		printf("PAPER smoker %d created and smoked cigarette #%d\n", id, smoked + 1);

		sem_post(&agentSem);
	}

	printf("PAPER smoker %d left to go eat\n", id);
}

void matchSmoker(int id) {
	printf("MATCH smoker %d started\n", id);

	for(int smoked = 0; smoked < 3; smoked++) {
		sem_wait(&matchSem);
		sem_wait(&agentSem);

		numTobacco -= 1;
		numPaper -= 1;

		printf("MATCH smoker %d created and smoked cigarette #%d\n", id, smoked + 1);

		sem_post(&agentSem);
	}

	printf("MATCH smoker %d left to go eat\n", id);
}

void *smoker(void* arg) {
	int argVal = (int*)arg;
	int id = argVal / 3 + 1;
	int type = argVal % 3;

	switch(type) {
		case TOBAC:
			tobaccoSmoker(id);
			break;
		case PAPER:
			paperSmoker(id);
			break;
		case MATCH:
			matchSmoker(id);
			break;
	}
}

int main() {
	// Generate seed for rand()
	srand(time(NULL));

	pthread_t agentThreads[3],
			  pusherThreads[3],
			  smokerThreads[6];

	sem_init(&agentSem, 0, 1);
	sem_init(&mutex, 0, 1);

	sem_init(&tobacco, 0, 0);
	sem_init(&paper, 0, 0);
	sem_init(&match, 0, 0);
	sem_init(&tobaccoSem, 0, 0);
	sem_init(&paperSem, 0, 0);
	sem_init(&matchSem, 0, 0);

	// Start agents, pushers, and smokers
	for(int i = 0; i < 3; i++)
		pthread_create(&agentThreads[i], NULL, agent, (void*)i);
	for(int i = 0; i < 3; i++)
		pthread_create(&pusherThreads[i], NULL, pusher, (void*)i);
	for(int i = 0; i < 6; i++)
		pthread_create(&smokerThreads[i], NULL, smoker, (void*)i);

	// Wait for all threads to complete
	for(int i = 0; i < 3; i++)
		pthread_join(agentThreads[i], NULL);
	for(int i = 0; i < 3; i++)
		pthread_join(pusherThreads[i], NULL);
	for(int i = 0; i < 6; i++)
		pthread_join(smokerThreads[i], NULL);

	return 0;
}
