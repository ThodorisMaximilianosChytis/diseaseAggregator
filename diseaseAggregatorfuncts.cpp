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



#include "diseaseAggregatorfuncts.h"
using namespace std;



//signal handler

void sig_handler_parent(int signo)
{
  if (signo == SIGINT || signo == SIGQUIT){
		aflag_int_q=1;
		write(0,"SIGINT OR SIGQUIT press enter\0",50);
  }
  if(signo == SIGCHLD){
    	int   status;
		write(1,"caught termindated kid\0 ",50);
//change global variable tp dead pid
		sigchld_pid = waitpid(-1, &status, WNOHANG);	//rtreice passes away kiddo id
		cout << sigchld_pid << endl;
  }	
}


void Read(char * fifopath,int &rfd,int bufferSize,string* message){


	char buff[bufferSize];
	int n;

//message ends at #
	
		message->assign("");
		do{
			if ( (n=read(rfd,buff,bufferSize))<0){
				cout << "problem" << endl;
				exit(1);
			}
			buff[n]='\0';
			message->append(buff);
			
		}while(message->back()!='#');



}


void Write(char * fifopath,int &wfd,int bufferSize,string* message){
	

	char buff[bufferSize];
	int n;


//wite #biffersize bytes at a time unti message is sent
	if (message->back()!='#')
		message->append("#");
	int length = message->length();
	for(int j=0;1;j++){
		strcpy(buff,message->substr(j*bufferSize,bufferSize).c_str());

		n=strlen(buff);	
		
		if(write(wfd,buff,n) !=n){
			//exit(1);
			cout << "problem" << endl;
		}
		
		length-=bufferSize;
		if (length<=0){
			break;
		}
	}
	

}






//user interface


void UserQueries(Child** CHT,int bufferSize,int numwork,char * argv[],int & SUCCESS,int & FAIL){
	 string Request,disease,country,date1,date2,record_id;
 	int error,k,w_s_q;
 	while(1){
 		
 		error=0;
//this is a flag for a terminated child. I dont use the global one to catch multiple terminated kids 		
 		w_s_q=-1;				// /Start the user interface
 		cout << "____________________________________________________________________________________________________" << endl;
		cout << "|Please Choose Between Applications:																 " << endl;
		cout << "----------------------------------------------------------------------------------------------------" << endl;
		cout << "|1)  /diseaseFrequency virusName date1 date2 [country]												 " << endl;
		cout << "|2)  /topk-AgeRanges k country disease date1 date2													 " << endl;
		cout << "|3)  /searchPatientRecord recordID																	 " << endl;
		cout << "|4)  /numPatientAdmissions disease date1 date2 [country]									    	 " << endl;
		cout << "|5)  /numPatientDischarges disease date1 date2	[country]									         " << endl;
		cout << "|6)  /listCountries																	             " << endl;
		cout << "|7)  /exit 																						 " << endl;
		cout << "| Note: Dates entered must follow the format dd-mm-yyyy and date1 must be earlier than date2        " << endl;
		cout << "____________________________________________________________________________________________________"	<< endl;
			

			getline(cin,Request);			//get choice from user
			cout << "You chose: " << Request << endl;
			stringstream wordcount(Request);
			stringstream srequest(Request);

			int count = 0;
			string temp;
			while(wordcount>>temp)			//count number of words given
				count++;
			
			 string app;
			 srequest>>app;				//get app name
			
			//cout << count << endl;
			 ////Use the num of words given to choose how to call each function (optional arguments)

			if (app=="/listCountries"){
				
				if (count!=1){			
					cout << "Wrong input, try again" << endl;
					error=1;
				}else
					listCountries(CHT,numwork,bufferSize,w_s_q);
			}else if(app=="/diseaseFrequency"){
				srequest >> disease;
				srequest >> date1;
				srequest >> date2;
				
				if (count==5){
					srequest >> country;
				}else if (count==4){
					country.assign("");
				}else{
					cout << "Wrong input, try again" << endl;
					error=1;
				}
				if (error!=1 && DateCompareEarlier(&date2,&date1)==1){
					cout << "Error: exit date given earlier than entry date" << endl;
					error=1;
				}
				if(error!=1)
					diseaseFrequency(CHT,app,disease,country,date1,date2,bufferSize,numwork,w_s_q);
				
				

			}else if(app=="/topk-AgeRanges"){

				if (count!=6){
					cout << "Wrong input, try again" << endl;
					error=1;
				}
					srequest >> k;
					if(k>4) k=4;
					if (k<1) k=1;
					
					srequest >>country;
					srequest >> disease;
					srequest >>date1;
					srequest >> date2;

				if (error!=1 && DateCompareEarlier(&date2,&date1)==1 ){
					cout << "Error: exit date given earlier than entry date" << endl;
					error=1;
				}						
			 
				if(error!=1)
			 		TopKAgeRanges(CHT,k,disease, country, date1, date2, bufferSize, numwork,w_s_q);

			

			}else if(app=="/searchPatientRecord"){

				if(count!=2){
					cout << "Wrong input,try again" << endl;
					error=1;
				}
				srequest >> record_id;

				if(error!=1)
					searchPatientRecord(CHT,record_id,bufferSize,numwork,w_s_q);
				


			}else if(app=="/numPatientAdmissions"){
				srequest >> disease;
				srequest >> date1;
				srequest >> date2;
				
				if (count==5){
					srequest >> country;
				}else if (count==4){
					country.assign("");
				}else{
					cout << "Wrong input, try again" << endl;
					error=1;
				}
				if (error!=1 && DateCompareEarlier(&date2,&date1)==1){
					cout << "Error: exit date given earlier than entry date" << endl;
					error=1;
				}
				if(error!=1)
					diseaseFrequency(CHT,app,disease,country,date1,date2,bufferSize,numwork,w_s_q);
				

			}else if(app=="/numPatientDischarges"){
				srequest >> disease;
				srequest >> date1;
				srequest >> date2;
				if (DateCompareEarlier(&date2,&date1)==1){
					cout << "Error: exit date given earlier than entry date" << endl;
					error=1;
				}
				if (count==5){
					srequest >> country;
				}else if (count==4){
					country.assign("");
				}else{
					cout << "Wrong input, try again" << endl;
					error=1;
				}
				if(error!=1)
					diseaseFrequency(CHT,app,disease,country,date1,date2,bufferSize,numwork,w_s_q);
				

			}else if (app=="/exit"){
				exit(CHT,bufferSize,numwork,w_s_q);
				break;							//exit loop and user interface
			}else{
				error=1;
			}
			if(error==1){
				SendError(CHT,bufferSize,numwork,w_s_q);
				FAIL++;
			}else{
				SUCCESS++;
			}
//here has been a signal //if siganl received during command line reading it is terminated after the completion of query			
			if(w_s_q>-1){
				ReforkChild(CHT, numwork, bufferSize,argv);

			}
//Parent has received a sig int or q signal.this signal is receivied during completing a query so complete it to move on
		if(aflag_int_q==1){
			break;
		}
 	}
}


void SendError(Child** T, int bufferSize, int numwork,int& w_s_q){
	string * message = new string("ERROR@");
	string Query=*message;
	cout << "ERROR" << endl;
	for(int i=0; i<numwork; i++){
		Write(T[i]->pathw,T[i]->wfd,bufferSize,&Query);
	}
	for(int i=0; i<numwork; i++){

		Read(T[i]->pathr,T[i]->rfd,bufferSize,message);
		int length=message->length();
		// cout << *message << endl;
//sig int or q from worker code $		
		if (length>=2 && message->substr(length-2,length)=="$#"){
			w_s_q=T[i]->pid;		 		
				T[i]->SIG_INT_Q=1;

		}
	}
	delete message;
}


void exit(Child** T,int bufferSize,int numwork,int& w_s_q){
	string message;
	message.assign("/exit@");
	string Query=message;

	for(int i=0; i<numwork; i++){
		Write(T[i]->pathw,T[i]->wfd,bufferSize,&Query);
	}
	for(int i=0; i<numwork; i++){
		
		Read(T[i]->pathr,T[i]->rfd,bufferSize,&message);
		int length=message.length();
//sig int or q from worker code $		

		if (length>=2 && message.substr(length-2,length)=="$#"){
				w_s_q=T[i]->pid;		 		
				T[i]->SIG_INT_Q=1;
		}
	}
}


void listCountries(Child** T,int numwork,int bufferSize,int& w_s_q){
	string message;
	message.assign("/listCountries@");
	string Query=message;
	int length;

	for(int i=0;i<numwork;i++){
		Write(T[i]->pathw,T[i]->wfd,bufferSize,&Query);
	}
	for(int i=0;i<numwork;i++){
		
		Read(T[i]->pathr,T[i]->rfd,bufferSize,&message);
		length=message.length();
//sig int or q from worker code $		

		if (length>=2 && message.substr(length-2,length)=="$#"){
				T[i]->SIG_INT_Q=1;
				w_s_q=T[i]->pid;
		}
//print countries and pids		
		for (int j=0;j < T[i]->numdir;j++){
			cout << T[i]->directories[j] << " " << T[i]->pid <<endl;
		}
	}
}



void searchPatientRecord(Child ** T,string record_id,int bufferSize,int numwork,int& w_s_q){
	string* message = new string("/searchPatientRecord");

	message->append("@");
	message->append(record_id);
	message->append("@");
	string Query = *message;
	for(int i=0; i<numwork; i++){
		Write(T[i]->pathw,T[i]->wfd,bufferSize,&Query);
	}
	for(int i=0; i<numwork; i++){

		Read(T[i]->pathr,T[i]->rfd,bufferSize,message);
		//cout << *message << endl;
		if(*message=="Found#"){
			cout << "job done" << endl;
		}
		int length=message->length();
		if (length>=2 && message->substr(length-2,length)=="$#"){
			T[i]->SIG_INT_Q=1;
			w_s_q=T[i]->pid;		 		
		}

	}

	if(*message!="Found#" || *message!="Found$#"){
		cout << "no record with id: " << record_id << endl;
	}
	//cout << *wholemess << endl;
	delete message;
	//delete wholemess;

}

void diseaseFrequency(Child ** Table,string app,string virusName,string country,string date1,string date2,int bufferSize,int numwork,int& w_s_q){
	int process=-1;
	int freq=0;

	string* message = new string(app);
	string* wholemess = new string();
	message->append("@");
	message->append(virusName);
	message->append("@");
	message->append(date1);
	message->append("@");
	message->append(date2);
	message->append("@");

//with country
	if (country!=""){
		// int dir=-1;

		for (int i=0; i<numwork; i++){
			if(Table[i]->Find((char *) country.c_str())>-1){
				process = i;
				break;
			}

		}
		if (process!=-1){
			int optimize=1;
			message->append(country);
			message->append("@");
			Write(Table[process]->pathw,Table[process]->wfd,bufferSize,message);
			Read(Table[process]->pathr,Table[process]->rfd,bufferSize,message);
			
			int length=message->length();
			if (length>=2 && message->substr(length-2,length)=="$#"){
				Table[process]->SIG_INT_Q=1;
				w_s_q=Table[process]->pid;
				optimize=2;
			}
			if (app=="/diseaseFrequency"){ 
				freq+=stoi(message->substr(0,message->length()-optimize));
			}
			else{
				wholemess->append(message->substr(0,message->length()-optimize));
			}
		}else
			cout << "country not found" << endl;
	}else{
		//no country given go through all of them
		string Query = *message;
		for(int i=0; i<numwork; i++){
			Write(Table[i]->pathw,Table[i]->wfd,bufferSize,&Query);
		}
		for(int i=0; i<numwork; i++){
			int optimize=1;

			Read(Table[i]->pathr,Table[i]->rfd,bufferSize,message);
			
			int length=message->length();
			if (length>=2 && message->substr(length-2,length)=="$#"){
				Table[i]->SIG_INT_Q=1;
				w_s_q=Table[i]->pid;
				optimize=2;
			}
//count freq			
			if (app=="/diseaseFrequency"){ 
				freq+=stoi(message->substr(0,message->length()-optimize));
			}
///list for numPatient Queries			
			else
				wholemess->append(message->substr(0,message->length()-optimize));
		}
	}
	if (app=="/diseaseFrequency")
		cout << "freq is " <<freq << endl; 
	else
		cout << *wholemess << endl;

	//cout << "Frequencies are :" << endl;
	//HT->PrintdiseaseFrequency(&virusName,&country,&date1,&date2);
	delete message;
	delete wholemess;

	return;
}






void TopKAgeRanges(Child ** Table,int k,string disease,string country,string date1,string date2,int bufferSize,int numwork,int& w_s_q){
	int process=-1;
	int freq=0;
	char numk[2];
	string* message = new string("/topk-AgeRanges");
	// string* wholemess = new string();
	message->append("@");
	sprintf(numk,"%d",k);
	message->append(numk);
	message->append("@");
	message->append(country);
	message->append("@");
	message->append(disease);
	message->append("@");
	message->append(date1);
	message->append("@");
	message->append(date2);
	message->append("@");
	

	for (int i=0; i<numwork; i++){
			if(Table[i]->Find((char *) country.c_str())>-1){
				process = i;
				break;
			}
	}
	if (process!=-1){
		Write(Table[process]->pathw,Table[process]->wfd,bufferSize,message);
		Read(Table[process]->pathr,Table[process]->rfd,bufferSize,message);
		int length=message->length();
		if (length>=2 && message->substr(length-2,length)=="$#"){
			Table[process]->SIG_INT_Q=1;
			w_s_q=Table[process]->pid;
		}
		cout << *message << endl;
	}else
			cout << "country not found" << endl;

	delete message;
}

//create log file
void ACreateLogfile(int SUCCESS,int FAIL,string * &text){
	char pid[50];
	char logfile[100];
	int logfilefd;
	strcpy(logfile,"./logfiles/Aggregatorlogfile.");
	sprintf(pid,"%d",getpid());
	strcat(logfile,pid);
	// cout << logfile << endl;
	if((logfilefd=open(logfile,O_WRONLY | O_CREAT | O_APPEND,0666))<0){
		cout << "ERROR opening logfile" << endl;
		exit(1);
	}


	text->append("TOTAL ");
	text->append(to_string(SUCCESS+FAIL));
	text->append("\n");
	text->append("SUCCESS ");
	text->append(to_string(SUCCESS));
	text->append("\n");
	text->append("FAIL ");
	text->append(to_string(FAIL));
	text->append("\n");
	int n=text->length();

	if(write(logfilefd,text->c_str(),n) !=n){
		exit(1);
		cout << "problem" << endl;
	}
}




void ReforkChild(Child** CHT, int numwork,int bufferSize,char** argv){
	cout << "a little patience" << endl;
	int pid;
	int status;
	for(int i=0;i<numwork;i++){
		if(CHT[i]->SIG_INT_Q==1){
			if(waitpid(CHT[i]->pid,&status,0)<0){
				cout << "waitpid problem" << endl;
			}
			// cout<<"nai edw ftanw ola komple" << endl;
	//wait for the children that received and int q signal to terminate the refork	
			if( (pid = fork()) == -1){
				exit(1);
			}
			if(pid!=0){
				Child * nC=new Child(pid);
				if(mkfifo(nC->pathr,0666) < 0){
					exit(1);
				}
				if(mkfifo(nC->pathw,0666) < 0){
					exit(1);
				}
				nC->numdir=CHT[i]->numdir;
				nC->directories=CHT[i]->directories;
				CHT[i]->directories=NULL;		//trick so that the directories are not erased but juts moved

				

				
				if( (nC->wfd = open(nC->pathw,O_WRONLY)) < 0 ){
					cout << "problem parent wfile" << endl;
					exit(1);
				}
				if( (nC->rfd = open(nC->pathr,O_RDONLY)) < 0 ){
					cout << "problem parent rfile" << endl;
					exit(1);
				}
	//send drectories			
				char buff[bufferSize];
				int n;
				string send;
				send.assign("");
				for(int j=0; j<nC->numdir; j++){
				
						send.append(nC->directories[j]);
						send.append("@");
				}
				
				Write(nC->pathw,nC->wfd,bufferSize,&send);
				
				delete CHT[i];
				CHT[i]=nC;				

			}else{
				sleep(1); //have some patience to refork and open fifos
				strcpy(argv[0],"$"); //sign for worker to understand that he is reforked
				// cout << argv[0] << endl;
				// cout << getpid() << endl;
				if(execv("./diseaseAggregatorWorker",argv)==-1){
 					cout << "Error exec the worker executable" << endl;
 				}
			}
		}
	}	
}