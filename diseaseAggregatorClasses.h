#include <iostream>


using namespace std;




class Child{

	public:
		int pid;
		char pathr[100];
		char pathw[100];
		int rfd;
		int wfd;
		char** directories;
		int numdir;
		int SIG_INT_Q;
		~Child(void);
		Child(int);
		void CreateRPath(int);
		void CreateWPath(int);
		void AssignDirectories(int,char ** ,int &, int &);
		int Find(char *);


};