#include <iostream>
// #include <fstream>
// #include <sstream>
#include <cstring>
#include "diseaseAggregatorClasses.h"

using namespace std;



Child::Child(int _pid):pid(_pid){
	CreateRPath(_pid);
	CreateWPath(_pid);
	SIG_INT_Q=0;
	numdir=0;
	directories=NULL;
}

Child::~Child(){	//directories for each child
	if (directories!=NULL){
		for (int j=0; j < this->numdir; j++){
			delete[] directories[j];
		}
		delete[] directories;
	}
}
//Create paths to fifo
void Child::CreateRPath(int _pid){
	
	strcpy(this->pathr,"./NamedPipes/fifo1.");
	char spid[20];
	sprintf(spid, "%d",_pid);
	strcat(this->pathr,spid);
	//cout << pathr << endl;
	
}

void Child::CreateWPath(int _pid){
	
	strcpy(this->pathw,"./NamedPipes/fifo2.");

	char spid[20];
	sprintf(spid, "%d",_pid);
	strcat(this->pathw,spid);
	
}

//Assign the correct directories to the children

void Child::AssignDirectories(int _nworkdir,char** _directories,int & _dirleft,int & j){
	this->numdir= _nworkdir;

	
	if (_dirleft>0){			//if mod still left incremet numbe rof dirs the child is responsible for
		this->numdir++;
		_dirleft--;
	}

	directories = new char*[numdir];
	
	for(int k=0; k < numdir ; k++){

		directories[k] = new char[50];

		strcpy(this->directories[k],_directories[j]);
		j++;			//this value comes from outside and to support the directories table
	}



}	

int Child::Find(char *country){
	for(int k=0; k<numdir ; k++){
		if (strcmp(country,directories[k])==0){
			return k;
		}
	}
	return -1;
}