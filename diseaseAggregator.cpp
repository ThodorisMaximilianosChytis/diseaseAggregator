#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h> 
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<signal.h>

//#include "diseaseAggregatorClasses.h"
// #include "diseaseAggregatorDS.h"
#include "diseaseAggregatorfuncts.h"

using namespace std;


//global flags to deal with sig int, quit and chld signals 

volatile sig_atomic_t aflag_int_q=0;
volatile sig_atomic_t sigchld_pid=-1;




int main(int argc, char **argv){
//initialize siactions struct	
	struct sigaction action;
	action.sa_handler=sig_handler_parent;
	action.sa_flags = SA_RESTART;
	sigemptyset (&action.sa_mask);
	
//link handlers	
	sigaction(SIGINT,&action,NULL);
	sigaction(SIGQUIT,&action,NULL);
	sigaction(SIGCHLD,&action,NULL);
  		//printf("\ncan't catch SIGINT\n");

	char input_dir[50];
	int numWorkers;
	int bufferSize;

	if (argc!=7){
		cout << "Input error" << endl;
		exit(1);
	}else{			
		int i;
		for(i=1;i<=5;i+=2){
			if(strcmp(argv[i],"-i")==0){
				strcpy(input_dir,argv[i+1]);
			}else if(strcmp(argv[i],"-b")==0){
				bufferSize = atoi(argv[i+1]);
			}else if(strcmp(argv[i],"-w")==0){
				numWorkers = atoi(argv[i+1]);
			}else exit(1);			
		}

		 if(bufferSize<1) exit(1);
		 if(numWorkers<1) exit(1);

		 cout << "number of workers is: " << numWorkers << endl;
		 cout << "bufferSize is: " <<bufferSize << endl;
		 cout << "input_dir is: " << input_dir << endl;
	}

//Create directories table
	DIR *dp;
	struct dirent * e;

	int dircount = 0; 		//by default two directories . and ..
	dp = opendir(input_dir);

	if (dp==NULL){
		cout << "problem" << endl;
		exit(1);
	}

	while ( (e=readdir(dp)) != NULL){
		if (e->d_type == DT_DIR && strcmp(e->d_name,".")!=0 &&  strcmp(e->d_name,"..")!=0){
			dircount++;	
		}
	} 
	rewinddir(dp);

	
	char ** directories = new char*[dircount];
	int i=0;

	//while(1);


	while ( (e=readdir(dp)) != NULL){

		if (e->d_type == DT_DIR && strcmp(e->d_name,".")!=0 &&  strcmp(e->d_name,"..")!=0){
			directories[i]= new char[50];
			strcpy(directories[i],e->d_name);
			// cout << directories[i] << endl;
			i++;
		}
	} 
	rewinddir(dp);
	closedir(dp);



	cout << input_dir << " has "<< dircount << " directories" <<endl;
	int nworkdir = (dircount/numWorkers);
	int dirleft = (dircount%numWorkers); 
	cout << nworkdir << "||" << dirleft << endl;


	if (numWorkers>dircount){				//no need for more workers than directories
		numWorkers = dircount;
	}


	int pid;
	int stat;
	int j=0; //direcotireis index

	Child ** CHT = new Child*[numWorkers];

	for(int i=0; i<numWorkers;i++){

		if( (pid = fork()) == -1){		//fork children
			exit(1);
		}

		if ( pid==0 ){		//child breaks free
			break;
		}
		CHT[i] = new Child(pid);		//store data about each child


		CHT[i]->AssignDirectories(nworkdir,directories,dirleft,j);
 //create fifo files		
		if(mkfifo(CHT[i]->pathr,0666) < 0){
			exit(1);
		}
		if(mkfifo(CHT[i]->pathw,0666) < 0){
			exit(1);
		}

	}
	
//if parent
		if (pid != 0){
			string * message = new string();
			for (int i=0; i<numWorkers ; i++){	
//open fifo files
				if( (CHT[i]->wfd = open(CHT[i]->pathw,O_WRONLY)) < 0 ){
					cout << "problem parent rfile" << endl;
					//cout << i << endl;
					exit(1);
				}

				if( (CHT[i]->rfd = open(CHT[i]->pathr,O_RDONLY)) < 0 ){
					cout << "problem parent rfile" << endl;
					exit(1);
				}				


//send direcotires to each child
				char buff[bufferSize];
				int n;
				
				message->assign("");
				for(int j=0; j<CHT[i]->numdir+1; j++){
				
					if (j!=CHT[i]->numdir){	//isws den xreiazetai
						message->append(CHT[i]->directories[j]);
						message->append("@");

					}			
					
				}
			

				Write(CHT[i]->pathw,CHT[i]->wfd,bufferSize,message);
	//receive summary statistics			
				 for(int j=0; j<CHT[i]->numdir; j++){
				 	Read(CHT[i]->pathr,CHT[i]->rfd,bufferSize,message);
				 	cout << *message << endl;

				 }

			

			}
//sigchld is the value off the pid of passed child i can only catch one signal before the interface for terminated children			//sleep(30);
			if(sigchld_pid>0){
				for(int i=0;i<numWorkers;i++){
					if(CHT[i]->pid==sigchld_pid){
						CHT[i]->SIG_INT_Q=1;
						ReforkChild(CHT, numWorkers, bufferSize,argv);
						break;
					}
				}
				sigchld_pid=-1;
			}

///call interface
			int SUCCESS=0;
			int FAIL=0;
			if(aflag_int_q==0){
				UserQueries(CHT,bufferSize,numWorkers,argv,SUCCESS,FAIL);
			}

//parent received sig int or quit
			if(aflag_int_q==1){
				message->assign("");
				for(int i=0;i<numWorkers;i++){
					for(int j=0;j<CHT[i]->numdir;j++){
							message->append(CHT[i]->directories[j]);
							message->append("\n");

					}
//kill children					
					if(kill(CHT[i]->pid,SIGKILL)==-1){
						cout << "error" << endl;
					}	
				}
//create the logfile
			ACreateLogfile(SUCCESS,FAIL,message);
			}



			cout << "parent process waiting, pid is : " << getpid() << endl;
			for(int i=0; i<numWorkers;i++){
				wait(&stat);
				close(CHT[i]->wfd);
				close(CHT[i]->rfd);

			}
			cout << "parent waited for children exit status is : "<< WEXITSTATUS(stat) << endl;
			 	
			delete message;
		// 	 // delete the names of the paths to the fifo files
			 for (int i=0; i<numWorkers; i++){
			 	delete CHT[i];

			 }
			 delete[] CHT;

			 for(int i=0; i<dircount; i++){
			 	delete[] directories[i]; 
			 }
			delete[] directories;

		}else{		//worker
			sleep(2); //the fifo files must be opened.
//run the different code
			 if(execv("./diseaseAggregatorWorker",argv)==-1){
			 	cout << "Error exec the worker executable" << endl;
			 }


		}

}


