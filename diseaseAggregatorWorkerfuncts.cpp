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



#include "diseaseAggregatorWorkerfuncts.h"
using namespace std;


// sig int and quit handler change value of global flag
void sig_handler_worker(int signo){


	if (signo == SIGINT || signo == SIGQUIT){
		wflag_int_q=1;
	}
	if (signo == SIGUSR1){
		flag_usr1=1;
	}
}





void Read(char * fifopath,int &rfd,int bufferSize,string* message){


	char buff[bufferSize];
	int n;


//end of message receivred is #	
		message->assign("");
		do{
			if ( (n=read(rfd,buff,bufferSize))<0){
				cout << "problem" << endl;
				//close(rfd);
				break;
			}
			buff[n]='\0';
			message->append(buff);
		}while(message->back()!='#');



}


void Write(char * fifopath,int &wfd,int bufferSize,string* message){
	
//send only #buffersize bytes as long ass needed to complete the message

	char buff[bufferSize];
	int n;

	if (message->back()!='#')
		message->append("#");
	int length = message->length();
	for(int j=0;1;j++){
		strcpy(buff,message->substr(j*bufferSize,bufferSize).c_str());

		n=strlen(buff);	
		
		if(write(wfd,buff,n) !=n){
			//exit(1);
			cout << "wproblem" << endl;
		}
		
		length-=bufferSize;
		if (length<=0){
			break;
		}
	}


}




void InsertChildFileRecords(LList* Records,Hashtable * ChildHT,SSLNode* dirnode,SSList* files,char* path){
	
//for a country directory read throught its files' content	

	SSLNode * iter = files->SSLHead;
	while (iter!=NULL){
		char filepath[100];
		ifstream infile;
		
		strcpy(filepath,path);
		strcat(filepath, "/");						
		strcat(filepath, iter->data->c_str());
		
		infile.open(filepath);
		if(infile.fail()) exit(1);
		string record_id,Move,Fname,Lname,Disease,Date,Age;
		LList * holdexits = new LList();


		while(infile>>record_id){		//import the data from the file
			bool insert = true;;
			LLNode* found = Records->Search(record_id);		
			infile >> Move;
			infile >> Fname;
			infile >> Lname;
			infile >> Disease;
			infile >> Age;

			if (stoi(Age)<=0 || stoi(Age)>120){
				cout << "Wrong age" << endl;
				insert = false;

			}else if(found!=NULL){					//list insert in increasing date order, so :
				if (Move=="ENTER"){				//the one found is chonologiacally before so same rid double Enter, error
					insert=false;
					//found->Print();
					// cout << "date is" << *(iter->data) << endl;
					//cout << "this record already exists" << endl;
				}

			}else{							//not found
				if (Move=="EXIT"){			//no enter beforehand so keep it in a list beacuse there might be an enter  on the same date further down the file
					insert=false;
					holdexits->Insert(record_id,Fname,Lname,Disease,*(dirnode->data),"--",*(iter->data),stoi(Age));

					//cout << " possible wrong chonologiacall exit" << endl;
				}
			} 

			//cout << "Insert record: Record_id "<< record_id <<"||"<<Fname << "||"  <<Lname<<  "||" << Disease << "||" << *(tempN->data) << "||" << *(iter->data)<< "||" << Move << "||" <<Age << endl;
			if(insert==true){
				if (Move=="ENTER"){
					Records->Insert(record_id,Fname,Lname,Disease,*(dirnode->data),*(iter->data),"--",stoi(Age));
					ChildHT->Insert(Disease,Records->getLLHead());
				}else if(Move=="EXIT"){
					found->setExit(*(iter->data));
				}
			}else{
				//cout << "ERROR" << endl;	
			}
		}
		LLNode * find;
		LLNode *i=holdexits->getLLHead();			//chekarw gia ta exits tou idiou arxeio hmeromhnias
		while(i!=NULL){
			// cout << "problem  is here obviously" << endl;
 			find=Records->Search( *(i->getid()) );		//brhke enter prin ara to bazw
			if(find!=NULL){
				// find->Print();
				find->setExit(*(i->getExit()) );
				find=NULL;
			}
			i=i->getNext();
		}
		delete holdexits;					
		iter = iter->Next;
		infile.close();
	}
}






void QueriesAnswer(LList* Records,Hashtable* HT,SSList * Countries,char * fifopathw,char * fifopathr,int &wfd,int &rfd,int bufferSize,int& SUCCESS,int& FAIL,char* input_dir){
	 string Request,disease,country,date1,date2;
 	int k;
 	int exit=0;
		// write()
//communication interface to parent
 	while(exit==0){
 
 		string* message = new string();
 		Read(fifopathr,rfd,bufferSize,message);
 		
 		// cout << *message << endl;
 		if (*message=="ERROR@#"){
 			FAIL++;
 		}else if(*message!="/exit@#"){
 			SUCCESS++;
 		}
//Decode the message	
		int count = 0; 		
 		int pos=0;
 		int length=message->length();
 		string ** vars = new string*[6];
 		for(int j=0;j<6;j++){
 			vars[j]=new string();
 		} 
 		for(int j=0;j<length;j++){
			if(message->at(j)=='@'){
				if(++count>6)	break;
				//cout << count-1 << endl;
				// if(message[0])
				vars[count-1]->assign(message->substr(pos,j-pos));
				//*vars[j]=message->substr(pos,j-pos);
				//cout << *vars[count-1] << endl;
				//break;
				pos = j+1;
			}
		}
		if(count>6) {
			//;
			cout << "Wrong input, try again" << endl;

		}
			if(*vars[0]=="/listCountries"){
			//success
				message->assign("K");
			
			}else if(*vars[0]=="/diseaseFrequency"){
				if (count==4){
					vars[4]->assign("");

				}else if(count!=5){
					cout << "Wrong input, try again" << endl;
					//continue;
				}
				char  num[20];
				//cout << HT->diseaseFrequency(vars[1],vars[4],vars[2],vars[3]) << endl;
				sprintf(num,"%d",HT->diseaseFrequency(vars[1],vars[4],vars[2],vars[3]));	// disease,country,date1,date2
				//cout << num << endl;
				message->assign("");
				message->append(num);
				// cout << *message << endl;

				// disease,country,date1,date2
			}else if(*vars[0]=="/topk-AgeRanges"){
				
				HT->TopKAgeRanges(stoi(*vars[1]),vars[3],vars[4],vars[5],vars[2],message);
				// TopKAgeRanges(int k,string* disease,string* date1,string* date2,string* country,string* &message)		//topk
				// cout << *message << endl;	

					//TopKDisease(diseasehashtable,k,country,date1,date2);					

			}else if(*vars[0]=="/searchPatientRecord"){
				// if (count!=2){
				// 	cout << "Wrong input,try again" << endl;
				// 	continue;
				// }
				//Print the information
				LLNode * node;
				if ((node = Records->Search(*vars[1]))!=NULL){
					node->Print();
					message->assign("Found");
				}

			}else if(*vars[0]=="/numPatientAdmissions"){	
				if (count==4){
					vars[4]->assign("");
				}else if(count!=5){
					cout << "Wrong input ,try again" << endl;
				}
				string EnorEx;
//Count for admissions
				EnorEx.assign("Entry");
				HT->numPatientAorD(vars[1],vars[2],vars[3],vars[4],message,Countries,&EnorEx); // disease,country,date1,date2

			}else if(*vars[0]=="/numPatientDischarges"){
				if (count==4){
					vars[4]->assign("");
				}else if(count!=5){
					cout << "Wrong input ,try again" << endl;
				}
				string EnorEx;
//Count for discharges
				EnorEx.assign("Exit");
				HT->numPatientAorD(vars[1],vars[2],vars[3],vars[4],message,Countries,&EnorEx); // disease,date1,date2,country,...

			}else if (*vars[0]=="/exit"){
//exit loop
				exit=1;
											//exit loop and user interface
			}else if (*vars[0]=="ERROR"){
				message->assign(*vars[0]);
			}
//flag $ gia na steilei to paidi ston gonea plhroforia oti termatizei wste na mhn to sumperilabei sto epomeno erwthma o goneas
 		if(wflag_int_q==1)					//apo thn stigmh pou einai sto read h sygkekrimenh diergasia kanw aparadoxh oti hdh ummetexei sto erwthma
			message->append("$");
		//cout << *message << endl;
	 	Write(fifopathw,wfd,bufferSize,message);
	 	for (int j=0;j<6;j++){
	 		//cout << *vars[j] << endl;
	 		delete vars[j];
	 	}
	 	delete[] vars;
	 	delete message;
 		if(wflag_int_q==1){					//apo thn stigmh pou einai sto read syscall h sygkekrimenh diergasia kanw aparadoxh oti hdh summetexei sto erwthma
 			break;							//opote prwta tha oloklhrwsei meta tha termatisei
 		}
 		if(flag_usr1==1){

 			LoadNewFile(HT,Records,Countries,input_dir);
 			
 		}
	}



}


void CreateLogfile(SSList* dirs,int SUCCESS,int FAIL){						//create the log file with the wanted information
	char pid[50];
	char logfile[50];
	int logfilefd;
	strcpy(logfile,"./logfiles/logfile.");
	sprintf(pid,"%d",getpid());
	strcat(logfile,pid);
	// cout << logfile << endl;
	if((logfilefd=open(logfile,O_WRONLY | O_CREAT | O_APPEND,0666))<0){
		cout << "ERROR opening logfile" << endl;
		exit(1);
	}
	string text="";
	SSLNode * tempN = dirs->SSLHead;
	while(tempN!=NULL){
		text.append(*(tempN->data));
		text.append("\n");
		tempN=tempN->Next;
	}
	text.append("TOTAL ");
	text.append(to_string(SUCCESS+FAIL));
	text.append("\n");
	text.append("SUCCESS ");
	text.append(to_string(SUCCESS));
	text.append("\n");
	text.append("FAIL ");
	text.append(to_string(FAIL));
	text.append("\n");
	int n=text.length();

	if(write(logfilefd,text.c_str(),n) !=n){
		exit(1);
		cout << "problem" << endl;
	}
}


void LoadNewFile(Hashtable* ChildHT,LList * Records,SSList *dirs,char * input_dir){
	char temp[100];
	string* message=new string("");
	
	SSLNode * tempN= dirs->SSLHead;
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
				if(flag_usr1==1 && Records->SearchDate(e->d_name)!=NULL){
					continue;	//if file found continue to next file
				}
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
		
		if (files->SSLHead!=NULL){
			message->assign(*tempN->data);
			message->append("\n");
		}

		SSLNode* idate=files->SSLHead;				
		while(idate!=NULL){
			message->append(*idate->data);
			message->append("\n");
			//cout << "ola kala" << endl;
			ChildHT->SummaryStatistics(idate->data,message);


			idate = idate->Next;

		}
		cout << *message << endl; //if reload its ok. just print summary statistics from worker
	//summary statistics ready send only if worker origianl and not reborn

		delete files;
		tempN= tempN->Next;
	}
	flag_usr1=0;
	delete message;
}
