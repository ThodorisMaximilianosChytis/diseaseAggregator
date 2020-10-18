#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h> 
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "diseaseAggregatorWorkerfuncts.h"






// global flag to handle the sig int and sig quit signal
volatile sig_atomic_t wflag_int_q=0;
// int wflag_int_q=0;
volatile sig_atomic_t flag_usr1=0;



int main(int argc,char ** argv){
	
	//set up sigaction struct and link handlers
	struct sigaction action;
	action.sa_handler=sig_handler_worker;
	action.sa_flags = SA_RESTART;
	sigemptyset (&action.sa_mask);
	
	sigaction(SIGINT,&action,NULL);
	sigaction(SIGQUIT,&action,NULL);
	sigaction(SIGUSR1,&action,NULL);


//nBflah shows if the worker is reforked or not
	int nBflag=0;
	// cout << "getpid is " << getpid() << endl;
	
//$ as argv means reforked
	if(strcmp(argv[0],"$")==0){
		nBflag=1;
	}

	char input_dir[100];
	int numWorkers;
	int bufferSize;
	

	if (argc!=7){
		
			 cout << "Input error" << endl;
			 exit(1);
		
	}else{	
	// for(int i=0;i<7;i++){
	// 	cout << argv[i] << endl;
	// }		

		for(int i=1;i<=5;i+=2){
			if(strcmp(argv[i],"-i")==0){
				strcpy(input_dir,argv[i+1]);
			}else if(strcmp(argv[i],"-b")==0){
				bufferSize = atoi(argv[i+1]);
			}else if(strcmp(argv[i],"-w")==0){
				numWorkers = atoi(argv[i+1]);
			}else {
			 cout << "Input error" << endl;
			}			
		}
		 if(bufferSize<1) exit(1);
		 if(numWorkers<1) exit(1);

		 // cout << "number of workers is: " << numWorkers << endl;
		 // cout << "bufferSize is: " <<bufferSize << endl;
		 // cout << "input_dir is: " << input_dir << endl;
	}




	
			cout << "child process, pid is : " << getpid() << endl;
			cout << "parent is : " << getppid() << endl;
			 

//create fifofile paths
			 char temp[100];
			 char path1[50];
			 char path2[50];
			 int readfd;
			 int writefd;
			strcpy(path1,"./NamedPipes/fifo1.");
			strcpy(path2,"./NamedPipes/fifo2.");
			sprintf(temp, "%d", getpid());
			// cout << temp << endl;
			strcat(path1 ,temp);
			strcat(path2 ,temp);
			 // cout << path1 << endl;
			 // cout << path2 << endl;


		// // cout << "fifo files open" << endl;

			if( (readfd = open(path2,O_RDONLY)) < 0 ){
				cout << "problem worker rfile" << endl;
				exit(1);
			}
			if( (writefd = open(path1,O_WRONLY)) < 0 ){
				cout << "problem worker wfile" << endl;
				exit(1);
			}

//read the directories worker is responsible for
				string* message = new string();
				Read(path2,readfd,bufferSize,message);
				// cout << *message << endl;
//decode message from aggregator @ means end of word # means end of message
				int pos=0;
				SSList * dirs = new SSList();
				for(int j=0;j<message->length();j++){
					if (message->at(j)=='@'){
						dirs->Insert(message->substr(pos,j-pos));
						pos = j+1;
					}
				}
				// dirs->Print();
//Create and fill data structures for each worker 
				LList *  Records = new LList();		
				Hashtable 	* ChildHT = new Hashtable(3,4);
				SSLNode* tempN=dirs->SSLHead;
				while (tempN!=NULL){
					
					DIR *dp;
					struct dirent * e;
//List of date file names			
					SSList* files = new SSList();

					int filecount = 0;
					strcpy(temp,input_dir);
					strcat(temp,"/");
					strcat(temp,tempN->data->c_str());
					dp = opendir(temp);

					if (dp==NULL){
						cout << "problem" << endl;
						exit(1);
					}

					while ( (e=readdir(dp)) != NULL){
						if (e->d_type == DT_REG){
							filecount++;	
							files->Insert(e->d_name);
						}
					} 
					rewinddir(dp);				
					closedir(dp);

					//files->Print();
					//cout << "--------------" << endl;
//Sort datefiles to be in correct chronological order for insertion					
					files->Sort(1);
					//files->Print();

					InsertChildFileRecords(Records, ChildHT, tempN, files,temp);
//Message of summary statistics for each file of the same directory
					message->assign(*tempN->data);
					message->append("\n");

					SSLNode* idate=files->SSLHead;				
					while(idate!=NULL){
						message->append(*idate->data);
						message->append("\n");
						//cout << "ola kala" << endl;
						ChildHT->SummaryStatistics(idate->data,message);


						idate = idate->Next;

					}
//summary statistics ready send only if worker origianl and not reborn
					if(nBflag!=1){
						Write(path1,writefd,bufferSize,message);
					}

					delete files;
					tempN= tempN->Next;
				}
			
				

//start interface to parent
			int SUCCESS=0;
			int FAIL=0;
			if (wflag_int_q==0){
				QueriesAnswer(Records,ChildHT,dirs,path1,path2,writefd,readfd,bufferSize,SUCCESS,FAIL,input_dir);				
			}

//if child terminated violentrly create logfile 
			if (wflag_int_q==1){
				CreateLogfile(dirs,SUCCESS,FAIL);

			}







			//	Records->Print();

			delete dirs;
			
			
			delete message;
			delete Records;
			delete ChildHT;







//close and delete fifo files

			close(readfd);
			close(writefd);

			if(unlink(path1) < 0){
				cout << "problem" << endl;
				exit(1);
			}
			if(unlink(path2) < 0){
				cout << "problem" << endl;
				exit(1);
			}

}