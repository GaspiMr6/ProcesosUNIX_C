#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void alarma(int s){

}

void ejecuta(int s){

	printf("Señal recibida soy %d \n",getpid());
}

void ejecutaA(int n){

	printf("Soy el proceso A con pid %d, he recibido la señal.\n",getpid());
	system("pstree");
}
void ejecutaB(int n){

	printf("Soy el proceso B con pid %d, he recibido la señal.\n",getpid());
	system("pstree");
}
void ejecutaX(int n){

	printf("Soy el proceso X con pid %d, he recibido la señal.\n",getpid());
	system("ls");
}
void ejecutaY(int n){

	printf("Soy el proceso Y con pid %d, he recibido la señal.\n",getpid());
	system("ls");
}

void falso(int s){

}

int main(int argc, char *argv[]){

	if (argc >= 2) {

		int vert = 2;
		int hor = 3;
		int segs = (unsigned int)atoi(argv[2]);

		pid_t* mispids;

		int shmid = shmget(IPC_PRIVATE,sizeof(pid_t),IPC_CREAT|0666); //crea la mc
		mispids=(int *)shmat(shmid,0,0);

		printf("Soy el proceso ejec: mi pid es %d \n",getpid());
		*(mispids)=getpid();

		for(int i=0;i<vert;i++){

			pid_t pidy = fork();

			if (pidy != 0 && getpid()==(*(mispids))) { //ejec

				wait(NULL);
				break;
			}
			
			else if(*(mispids)==getppid() && i == 0){ // A
				
					mispids=(int *)shmat(shmid,0,0); 
					*(mispids+1)=getpid();
					printf("Soy el proceso A: mi pid es %d. Mi padre es %d \n", *(mispids+1), *(mispids));
					
					wait(NULL);
			}

			if (pidy != 0 && i==1){

				if (*argv[1] == 'A') signal(SIGUSR1, ejecutaA);
				else signal(SIGUSR1,falso);
				pause();
				wait(NULL);
			}


			if (pidy == 0 && i == vert-1){

				if(getppid() == *(mispids)+1){ 

					mispids=(int *)shmat(shmid,0,0); //***
					*(mispids+2) = getpid();
					printf("Soy el proceso B: mi pid es %d. Mi padre es %d. Mi abuelo es %d \n", *(mispids+2), *(mispids+1), *(mispids));

					for(int j=0; j<hor;j++){

						pidy = fork();

						if(pidy != 0 && j == hor-1){

							if (*argv[1] == 'B') signal(SIGUSR1, ejecutaB);
							else signal(SIGUSR1,falso);
							pause();
						}

						if(getpid() == *(mispids) + 3) {

							mispids=(int *)shmat(shmid,0,0); 
							*(mispids + 3) = getpid();
							printf("Soy el proceso X: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d \n", *(mispids+3), *(mispids+2), *(mispids+1),*(mispids));

							if (*argv[1] == 'X') signal(SIGUSR1, ejecutaX);
							else signal(SIGUSR1,falso);
							pause();

							sleep(3);
							break;

						}
						else if(getpid() == *(mispids) + 4) {

							mispids=(int *)shmat(shmid,0,0); 
							*(mispids + 4) = getpid();
							printf("Soy el proceso Y: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d \n", *(mispids+4), *(mispids+2), *(mispids+1),*(mispids));

							if (*argv[1] == 'Y') signal(SIGUSR1, ejecutaY);
							else signal(SIGUSR1,falso);
							pause();

							sleep(2);
							break;
						}
						else if(getpid() == *(mispids) + 5) {

							mispids=(int *)shmat(shmid,0,0); 
							*(mispids + 5) = getpid();
							printf("Soy el proceso Z: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d \n", *(mispids+5), *(mispids+2), *(mispids+1),*(mispids));
							
							signal(SIGALRM, alarma); 
							alarm(segs);
							pause();

							kill((int)*(mispids + 1),SIGUSR1);
							kill((int)*(mispids + 2),SIGUSR1);
							kill((int)*(mispids + 3),SIGUSR1);
							kill((int)*(mispids + 4),SIGUSR1);
							
							sleep(1);
							break;
						}
					}
					
					
					//pause();
					wait(NULL);
					wait(NULL);
					wait(NULL);

				}
			}
		}

		if(getpid() == (*(mispids))) printf("Soy ejec (%d) y muero\n", getpid());
		if(getpid() == (*(mispids+1))) printf("Soy A (%d) y muero\n", getpid());
		if(getpid() == (*(mispids+2))) printf("Soy B (%d) y muero\n", getpid());
		if(getpid() == (*(mispids+3))) printf("Soy X (%d) y muero\n", getpid());
		if(getpid() == (*(mispids+4))) printf("Soy Y (%d) y muero\n", getpid());
		if(getpid() == (*(mispids+5))) printf("Soy Z (%d) y muero\n", getpid());

		if(getpid() == *(mispids)) {

			shmdt((pid_t *) mispids);
			shmctl(shmid, IPC_RMID,0); //borra
		}
		else shmdt((pid_t *)mispids); //desvincula
		

	}
	else printf("Faltan los parametros\n");
	
}
