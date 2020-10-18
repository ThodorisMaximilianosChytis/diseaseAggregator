#include "diseaseAggregatorDS.h"
#include "diseaseAggregatorClasses.h"



extern volatile sig_atomic_t aflag_int_q;
extern volatile sig_atomic_t sigchld_pid;


void sig_handler_parent(int signo);





void Write(char *,int &,int ,string* );

void Read(char * ,int &,int ,string* );

void SendError(Child**,int ,int ,int&);

// void ReConstructNewWorker();

void diseaseFrequency(Child **,string,string,string,string,string,int,int,int&);

void searchPatientRecord(Child **,string ,int,int,int&);

void listCountries(Child**,int,int,int&);

void UserQueries(Child**,int,int,char **,int&,int&);

void exit(Child**,int,int,int&);

void TopKAgeRanges(Child **,int,string,string,string,string,int,int,int&);


void ACreateLogfile(int,int,string *&);

void ReforkChild(Child**, int ,int ,char**);






// void diseaseFrequency(Hashtable *,string,string,string,string);

// void TopKCountries(Hashtable*,int,string,string,string);

// void TopKDisease(Hashtable*,int,string,string,string);



// void numCurrentPatients(Hashtable*,string);

// void exit(LList*,Hashtable*,Hashtable*);