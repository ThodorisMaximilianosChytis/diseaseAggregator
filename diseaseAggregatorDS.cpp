#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include "diseaseAggregatorDS.h"
using namespace std;


int DateCompareEarlier(string* date1,string* date2){			//1 -> date1 is earlier || 2->date2 os earlier
		if(*date1=="--")					//-- is always larger
			return 2;
		else if(*date2=="--")
			return 1;
		//sum of dd-mm-yyyy , giving the correct weights
		unsigned int sum1= atoi(date1->substr(0,2).c_str())*1 + atoi(date1->substr(3.2).c_str())*31 + atoi(date1->substr(6.4).c_str())*31*12;		
		unsigned int sum2= atoi(date2->substr(0,2).c_str())*1 + atoi(date2->substr(3.2).c_str())*31 + atoi(date2->substr(6.4).c_str())*31*12;
	
		if (sum1<sum2)
			return 1;
		else if(sum2<sum1)
			return 2;
		else
			return 0; 

}





void SSLNode::Print(void){
	cout << *data << endl;
}



SSLNode::SSLNode(string _data){
	//cout << "Listnode constructor" << endl;
	data = new string(_data);
 
	Next = NULL;
};

SSLNode::~SSLNode(void){
	//cout << "deleting LLNode" << endl;
	delete data;
}


///////////////////////////////////////////////LList//////////////////////////////////////////////////////////////////////




void SSList::Print(void){
	SSLNode * temp = SSLHead;
	while (temp!=NULL){
		temp->Print();				//print listnode
		temp=temp->Next;
	}
}

void SSList::Insert(string _data){
	SSLNode*  newNode= new SSLNode(_data);	//listonde memory allocation
	newNode->Next = SSLHead;			//and list insertion
	SSLHead = newNode;

}

SSLNode * SSList::Search(string _data){					//search for a node with specific id . return that node or NULL
	SSLNode * temp = SSLHead;
	while(temp!=NULL){
		if (*(temp->data) == _data){
			return temp;
		}
		temp=temp->Next;
	}
	return NULL;
}






SSList::SSList(void):SSLHead(NULL){
	//cout << "calling LList constructor" << endl;
}

void SSList::Sort(int mode){
	if (SSLHead==NULL)
		return;

	SSLNode * iter = SSLHead;
	SSLNode * min = SSLHead;
	string * temp ;
	while (iter!=NULL){
		SSLNode * i =iter->Next;
		while(i!=NULL){
			
			if (DateCompareEarlier(i->data,iter->data)==mode){
				temp = i->data;
				i->data = iter->data;
				iter->data = temp;
			}
			i = i->Next;
		}
		iter = iter->Next;
	}
}





SSList::~SSList(void){				//list constructor
	//cout << "deleting List" << endl; 
	SSLNode* temp;
	while (SSLHead!=NULL){
		temp=SSLHead;
		SSLHead=SSLHead->Next;
		delete temp;			//call listonode destructor and free listnode
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////
LLNode* LLNode::getNext(){
	return Next;
}

string* LLNode::getid(){

	return Record_id;
}


string * LLNode::getDisease(){
	return Disease;
}



string* LLNode::getEntry(){
	return Entry;
}

string* LLNode::getExit(){
	return Exit;
}

string* LLNode::getCountry(){
	return Country;
}

int LLNode::getAge(){
	return Age;
}

void LLNode::setExit(string _Exit){
	*Exit=_Exit;
}

void LLNode::Print(void){
	cout << endl;
	cout << "------------------" << endl;
	cout << *Record_id << endl;
	cout << *Fname << endl;
	cout << *Lname << endl;
	cout << *Disease << endl;
	cout << *Country << endl;
	cout << *Entry << endl;
	cout << *Exit << endl;
	cout << Age << endl;
	cout << "------------------" << endl; 
}

void LLNode::setNext(LLNode* _Node){
	Next=_Node;
}

LLNode::LLNode(string _record_id,string _Fname, string _Lname, string _Disease, string _Country, string _Entry, string _Exit, int _Age):Age(_Age){
	//cout << "Listnode constructor" << endl;
	Record_id = new string(_record_id);
	Fname = new string(_Fname);
	Lname = new string(_Lname);
	Disease = new string(_Disease);						//Listonode Constructor and memory allocation for record strings
	Country = new string(_Country);
	Entry = new string(_Entry);
	Exit = new string(_Exit);
	Next = NULL;
};

LLNode::~LLNode(void){
	//cout << "deleting LLNode" << endl;
	delete Record_id;
	delete Fname;
	delete Lname;							//Listonode destructor and memory free for record strings
	delete Disease;
	delete Country;
	delete Entry;
	delete Exit;
}


///////////////////////////////////////////////LList//////////////////////////////////////////////////////////////////////



LLNode* LList::getLLHead(){
	return LLHead;
}


void LList::Print(void){
	LLNode * temp = LLHead;
	while (temp!=NULL){
		temp->Print();				//print listnode
		temp=temp->getNext();
	}
}

void LList::Insert(string _record_id,string _Fname, string _Lname, string _Disease, string _Country, string _Entry, string _Exit,int _Age){
	LLNode*  newNode= new LLNode(_record_id,_Fname,_Lname,_Disease,_Country,_Entry,_Exit,_Age);	//listonde memory allocation
	newNode->setNext(LLHead);			//and list insertion
	LLHead = newNode;

}

LLNode * LList::Search(string id){					//search for a node with specific id . return that node or NULL
	LLNode * temp = LLHead;
	while(temp!=NULL){
		if (*(temp->getid())==id){
			return temp;
		}
		temp=temp->getNext();
	}
	return NULL;
}


LLNode * LList::SearchDate(string date){					//search for a node with specific id . return that node or NULL
	LLNode * temp = LLHead;
	while(temp!=NULL){
		if (*(temp->getEntry())==date || *(temp->getExit())==date ){
			return temp;
		}
		temp=temp->getNext();
	}
	return NULL;
}


LList::LList(void):LLHead(NULL){
	//cout << "calling LList constructor" << endl;
}


LList::~LList(void){				//list constructor
	//cout << "deleting List" << endl; 
	LLNode* temp;
	while (LLHead!=NULL){
		temp=LLHead;
		LLHead=LLHead->getNext();
		delete temp;			//call listonode destructor and free listnode
	}

}
/////////////////////////////////////////////////////////////RBTreeNode/////////////////////////////////////////////////////////


RBTreeNode::RBTreeNode(LLNode* _Record):left(NULL),right(NULL),parent(NULL),color(0),Date(NULL),Record(_Record){
	Date=_Record->getEntry();		//use date in record
	//cout << "TreeNode constructed and Initilized succesfully" << endl;					//Treenode constructor
}


RBTreeNode::~RBTreeNode(){
	//cout << "treenode destroyed" << endl;					//treenode destructor
}


void RBTreeNode::setleft(RBTreeNode * _left){
	left = _left; 
}


void RBTreeNode::setright(RBTreeNode * _right){
	right = _right; 

}


void RBTreeNode::setparent(RBTreeNode* _parent){
	parent = _parent; 

}


void RBTreeNode::setColor(int _color){				//0==red 1==black
	color = _color;
}


RBTreeNode * RBTreeNode::getright(void){
	return right;
}


RBTreeNode * RBTreeNode::getleft(void){

	return left;
}


RBTreeNode * RBTreeNode::getparent(void){
	return parent;
}


int RBTreeNode::getColor(void){
	return color;
}


string* RBTreeNode::getDate(){
	return Date;
}


LLNode* RBTreeNode::getRecord(){			//the record is in the list I just return the pointer to it
	return Record;
}



//////////////////////////////////////////////////////////RBTree/////////////////////////////////////////////////////////////////////





RBTree::RBTree():Root(NULL),Nodecount(0){			//Rbtree constructor //Nodecount=number n of nodes

	//cout << "tree has been created" << endl;
}

RBTree::~RBTree(){					//Rbtree destructor
	Destroy(Root);					//use helper function to destroy(delete) the tree
	//cout << "tree hass been destoyed" << endl;
}


RBTreeNode * RBTree::getRoot(void){
	return Root;
}


void RBTree::setRoot(RBTreeNode* _Root){
	Root=_Root;
}


//Different traversals to print the tree data


void RBTree::Inorder(RBTreeNode* Root){			
	if (Root==NULL) return;
	Inorder(Root->getleft());
	cout << *Root->getDate() << endl;
	Inorder(Root->getright());
}


void RBTree::Postorder(RBTreeNode* Root){
	if (Root==NULL) return;
	Postorder(Root->getleft());
	Postorder(Root->getright());
	cout << *Root->getDate() << endl;
}


void RBTree::Preorder(RBTreeNode* Root){
	if (Root==NULL) return;
	cout << *Root->getDate() << endl;
	Preorder(Root->getleft());
	Preorder(Root->getright());

}

	



RBTreeNode * RBTree::BSTInsert(RBTreeNode *root, RBTreeNode *&newNode){			//Recursive Binary search Tree insert 
	if (root==NULL)
		return newNode;
	if (DateCompareEarlier(newNode->getDate(),root->getDate())==1){			//go left to insert if the date of new node is smaller than the date of the saved one
		root->setleft(BSTInsert(root->getleft(), newNode));
		root->getleft()->setparent(root);
	}else if (DateCompareEarlier(newNode->getDate(),root->getDate())==2 || DateCompareEarlier(newNode->getDate(),root->getDate())==0){   //go right to insert if the date of the new node
		root->setright(BSTInsert(root->getright(),newNode));																			//is larger or the same as the date of the saved one
		root->getright()->setparent(root);
	}
	return root;
}



void RBTree::LRot( RBTreeNode * &STRootNode){		//STRootNode = subtreerootnode	


	///Perform a left rotation on the BSTree to preserve BST attributes

	RBTreeNode * tbSTRootNode = STRootNode->getright();			


	STRootNode->setright(tbSTRootNode->getleft());		//the left child of the to be root node is now right child of the one that was root

	if(STRootNode->getright() != NULL)					
		STRootNode->getright()->setparent(STRootNode);

	tbSTRootNode->setparent(STRootNode->getparent());	//the parent of root is now parent of to be root

	if (STRootNode->getparent() == NULL)				
		Root = tbSTRootNode;					//root was RBTree Root so set RBTree Root to the to be root
	else if(STRootNode == STRootNode-> getparent()->getleft())		//else attach correctly to parent root
		STRootNode->getparent()->setleft(tbSTRootNode);
	else
		STRootNode->getparent()->setright(tbSTRootNode);

	tbSTRootNode->setleft(STRootNode);	//connect the nodes to complete the rotation
	STRootNode->setparent(tbSTRootNode);


}



void RBTree::RRot( RBTreeNode *   &STRootNode){

	//Perform a right rotation ont the BSTree to preserve BST attributes 

	RBTreeNode * tbSTRootNode = STRootNode->getleft();	


	STRootNode->setleft(tbSTRootNode->getright());			////the right child of the to be root node is now left child of the one that was root

	if(STRootNode->getleft() != NULL)
		STRootNode->getleft()->setparent(STRootNode);

	tbSTRootNode->setparent(STRootNode->getparent());		//the parent of root is now parent of to be root

	if (STRootNode->getparent() == NULL)
		Root = tbSTRootNode;								//root was RBTree Root so set RBTree Root to the to be root
	else if(STRootNode == STRootNode-> getparent()->getleft())
		STRootNode->getparent()->setleft(tbSTRootNode);		//else attach correctly to parent root
	else
		STRootNode->getparent()->setright(tbSTRootNode);

	tbSTRootNode->setright(STRootNode);			//connect the nodes to complete the rotation
	STRootNode->setparent(tbSTRootNode);


}



void RBTree::FixRBInsert(RBTreeNode *&newNode){		//RED=0 BLACK=1			//A function to detect and fix the RBTree violations that occured after BSTreee insertion
    RBTreeNode *parent = NULL;
    RBTreeNode *grandparent = NULL;

    while (newNode != Root && newNode->getColor() == 0 && newNode->getparent()->getColor() == 0) {
        parent = newNode->getparent();
        grandparent = parent->getparent();
        
        if (parent == grandparent->getleft()) {				//1)parent's uncle is also red -> so recolor
            RBTreeNode *uncle = grandparent->getright();
            if (uncle!=NULL && uncle->getColor()== 0) {
                uncle->setColor(1);
                parent->setColor(1);
                grandparent->setColor(0);
                newNode = grandparent;
            } else {				
                
                if (newNode == parent->getright()) {			//i)parent is right of parent -> LRot
                
                    LRot(parent);
                    newNode = parent;
                    parent = newNode->getparent();
                }
                RRot(grandparent);				//ii)parent is left of parent ->RRot
                int temp=parent->getColor();
                parent->setColor(grandparent->getColor());
               	grandparent->setColor(temp);
                newNode = parent;
            }
        } else {							//2)parent's parent is right of parent's grandparent

            RBTreeNode *uncle = grandparent->getleft();
            if(uncle!=NULL && uncle->getColor() == 0) {		//i)parent's uncle is also red -> recolor
                uncle->setColor(1);
                parent->setColor(1);
                grandparent->setColor(0);
                newNode = grandparent;
            }else {									//ii) parent is left of parent's parent -> RRot
                if (newNode == parent->getleft()) {
                    RRot(parent);
                    newNode = parent;
                    parent = newNode->getparent();
                }
                LRot(grandparent);				//iii)parent is right of parent's parent -> LRot
                int temp = parent->getColor();
                parent->setColor(grandparent->getColor());
                grandparent->setColor(temp);
                newNode = parent;
            }
        }
    }
    Root->setColor(1);
}



void RBTree::Insert(LLNode* record){			//Insert into RBTRee
	RBTreeNode * nRBTNode =  new RBTreeNode(record);		//treenode allocation
	Root = BSTInsert(Root,nRBTNode);			//BSTinsert
	Nodecount++;			
	//Preorder(Root);
	FixRBInsert(nRBTNode);		//Fix the Insertion to preserve RBTree attributes
	//Preorder(Root);
	//Inorder(Root);
}


void RBTree::Destroy(RBTreeNode * Node){		//Postorder Recursive Destroy the RBTree 

	if(Node != NULL){
		Destroy(Node->getleft());
		Destroy(Node->getright());
		delete Node;
	}

}

//Recursive count for statistcs for topk query

void RBTree::CountforTopk(RBTreeNode* _Root,string* date1,string* date2,string* searchfor,int* & agestats){
	
	if (_Root==NULL){
		return;
	}
	int Age = _Root->getRecord()->getAge();
	// cout << *_Root->getRecord()->getCountry() << endl;
	if(*searchfor==*_Root->getRecord()->getCountry() && DateCompareEarlier(_Root->getDate(),date1)!=1 && DateCompareEarlier(_Root->getDate(),date2)!=2){
			if (Age>60){
				agestats[3]++;
			}else if(Age>40){
				agestats[2]++;

			}else if(Age>20){
				agestats[1]++;

			}else{
				agestats[0]++;

			}

	}
	if (_Root->getleft()!=NULL && DateCompareEarlier(_Root->getDate(),date1)!=1){ //
		CountforTopk(_Root->getleft(),date1,date2,searchfor,agestats);			//go left.recusrion babyyy
	}
	if (_Root->getright()!=NULL && DateCompareEarlier(_Root->getDate(),date2)!=2){ //
		CountforTopk(_Root->getright(),date1,date2,searchfor,agestats);			//go left.recusrion babyyy
	}
	return;
}




//Count for summary statistics recursively

void RBTree::SummaryStatistics(RBTreeNode* _Root,string* date,int* & agestats){
	
	if (_Root==NULL){
		return;
	}

	int Age = _Root->getRecord()->getAge();
	if(*date==*_Root->getRecord()->getEntry()){
			if (Age>60){
				agestats[3]++;
			}else if(Age>40){
				agestats[2]++;
			}else if(Age>20){
				agestats[1]++;
			}else{
				agestats[0]++;
			}

	}
	if (_Root->getleft()!=NULL && DateCompareEarlier(date,_Root->getRecord()->getEntry())!=2){ //
		SummaryStatistics(_Root->getleft(),date,agestats);			//go left.recusrion babyyy
	}
	if (_Root->getright()!=NULL && DateCompareEarlier(date,_Root->getRecord()->getEntry())!=1){ //
		SummaryStatistics(_Root->getright(),date,agestats);			//go right.recusrion babyyy
	}
	return;
}

int RBTree::BetweenDatesCount(RBTreeNode* _Root, string* date1,string* date2,string * searchfor,int type){

	int count=0;			
	if(_Root==NULL)			//Base condition
		return 0;



	//type 3-4 for discharges	
		if (type==4 &&  *searchfor==*_Root->getRecord()->getCountry()){
			if(DateCompareEarlier(_Root->getRecord()->getExit(),date1)!=1 && DateCompareEarlier(_Root->getRecord()->getExit(),date2)!=2){
				count++;		//only count those nodes where the nodes's exit date e[date1,date2]
			}
		}
		if (type==3){		

			if(DateCompareEarlier(_Root->getRecord()->getExit(),date1)!=1 && DateCompareEarlier(_Root->getRecord()->getExit(),date2)!=2){
				count ++;
					//////only count those nodes where the nodes's exit date e[date1,date2]
			}
		}
		if (type==2 && *searchfor==*_Root->getRecord()->getCountry()){	
			if(DateCompareEarlier(_Root->getDate(),date1)!=1 && DateCompareEarlier(_Root->getDate(),date2)!=2){
				
				count++;		//only count those nodes where the nodes's entry date e[date1,date2]
			}
		}
		if (type==1){		
		
			
			if(DateCompareEarlier(_Root->getDate(),date1)!=1 && DateCompareEarlier(_Root->getDate(),date2)!=2){
				count ++;
					//////only count those nodes where the nodes's entry date e[date1,date2]
			}
		}	

	if(_Root->getleft()!=NULL ){
			if(DateCompareEarlier(_Root->getDate(),date1)!=1 || type==3 ||type==4){
				count+=BetweenDatesCount(_Root->getleft(),date1,date2,searchfor,type);			//go left.recusrion babyyy
			
			}	
	}			//_Root->getRecord()->Print();												///which counts people still being nursed

	if(_Root->getright()!=NULL){
		//go right. recursion babyy
			if(DateCompareEarlier(_Root->getDate(),date2)!=2 || type==3 || type==4){
				count+=BetweenDatesCount(_Root->getright(),date1,date2,searchfor,type);			//go left.recusrion babyyy
			} 
	}
	//cout << "Counting inside the tree " << count << endl;
	return count;

}







////////////////////////////////////////////////BEntry///////////////////////////////////////////////////////////////





void BEntry::Insert(LLNode* record,string key){		//Insert into the bucket Entry

		if (RBHead==NULL){
			Key = new string(key);
			RBHead = new RBTree();
		}
		RBHead->Insert(record);

}


string* BEntry::getkey(void){	
	return Key;
}



BEntry::BEntry():Key(NULL),RBHead(NULL){
	//cout << "BucketEntry Constructor" << endl; 
}

void BEntry::SummaryStatistics(string* date,int * &agestats){
	RBHead->SummaryStatistics(RBHead->getRoot(),date,agestats);
}

int BEntry::BetweenDatesCount(string* date1,string* date2,string* searchfor,int type){		//call the counter function with the demanded type


	return RBHead->BetweenDatesCount(RBHead->getRoot(),date1,date2,searchfor,type);
}

void BEntry::CountforTopk(string* date1, string* date2, string* country,int *&agestats){
	RBHead->CountforTopk(RBHead->getRoot(),date1,date2,country,agestats);
}




BEntry::~BEntry(){
	//cout << "BucketEntry Destructor" << endl;
	if (Key!=NULL) delete Key;
	if (RBHead!=NULL) delete RBHead;
}




//////////////////////////////////////////////////////////////////Bucket//////////////////////////////////////////////////

Bucket * Bucket::getNext(void){
	return Next;
}


void Bucket::setNext(Bucket * _Next){
	Next = _Next;
}


Bucket::Bucket(int Bsize):B_Size(Bsize), count(0),Next(NULL){		//Bucket constructor
	//cout << "bucket constructorand Bsize is"  << B_Size << endl;
	Content = new BEntry*[B_Size];		//array of pointers to bucketentries
	for (int i=0;i<=B_Size-1;i++){
		Content[i]= new BEntry();		//fill the array with BEntrypointers
	}
}


Bucket::~Bucket(void){		//Bucket entry constructor
	//cout << "bucket destructor" << endl;
	for (int i=0 ; i<=B_Size-1 ; i++ ){
			delete Content[i];			//delte Bentries
	}
	delete[] Content;
}


int Bucket::Insert(LLNode * record,string key){			//Insert into the bucket
 	int i;


		for (i=0 ; i<=B_Size-1 ; i++){
			if (Content[i]->getkey()==NULL || *(Content[i]->getkey())==key){
				Content[i]->Insert(record,key);
				return 1;
			}	
		}
		return 0;
		
 }




void Bucket::SummaryStatistics(string * date,string*& message){
	int i;
	char num[12];
	int * agestats = new int[4];
	int k;
	//cout << *date << endl;
	for(k=0;k<=B_Size-1;k++){
		if (Content[k]->getkey()!=NULL){
			for(i=0;i<4;i++){
				agestats[i]=0;
			}
			//cout << *Content[k]->getkey() << endl;
			Content[k]->SummaryStatistics(date,agestats);
			if (agestats[0]+agestats[1]+agestats[2]+agestats[3]>0){
				string pattern[4]={"Age range 0-20 years: ","Age range 21-40 years: ","Age range 41-60 years: ","Age range 61+ years: "};
				message->append(*Content[k]->getkey());
				message->append("\n");
				for(int i=0;i<4;i++){
					// char* num;
					message->append(pattern[i]);
					sprintf(num, "%d",agestats[i]);
					message->append(num);
					message->append(" cases\n");
				}
				message->append("\n");

			}

			// 
		}
	}
	delete[] agestats;
		//cout << k << endl;

}


int  Bucket::diseaseFrequency(string * virusName,string * country, string* date1, string* date2,string * EnorEx){
//initialization for different betweendate couns calls	
	int sWOcountry,sWcountry;
	if(*EnorEx=="Entry"){
		sWOcountry=1;
		sWcountry=2;
	}else if(*EnorEx=="Exit"){
		sWcountry=4;
		sWOcountry=3;
	}
	//cout << *EnorEx << endl;
	int count=-1;
	for(int i=0;i<=B_Size-1;i++){
		if (Content[i]->getkey()!=NULL){
			if (*(Content[i]->getkey()) == *virusName){
				if (*country==""){
					// cout << "exw mpei edw pou shmainei oti xwris xwra metraw gia " << *virusName << endl;
					count= Content[i]->BetweenDatesCount(date1,date2,country,sWOcountry);
				}
				else{
					// cout << "exw mpei edw pou shmainei oti me xwra " << *country <<" metraw gia " << *virusName << endl;

					count= Content[i]->BetweenDatesCount(date1,date2,country,sWcountry);
				}	
				break;
			}
		}
	}
	if (count==-1){
		// cout << *virusName << " not found frequency is 0" << endl;
		return 0;
	}
	return count;
}


void Bucket::TopKAgeRanges(int k,string* disease,string* date1,string* date2,string* country,string* & message){		//topK function used for both topk applications
	int * agestats = new int[4];




	int sum=-1;
	for(int i=0;i<4;i++){
		agestats[i]=0;
	}
	for (int i=0;i<=B_Size-1;i++){
		if (Content[i]->getkey()!=NULL){
//Count age statistics
			if (*(Content[i]->getkey()) == *disease){
				sum=0;
				Content[i]->CountforTopk(date1,date2,country,agestats);
				sum=agestats[0]+agestats[1]+agestats[2]+agestats[3];
				if (sum>0){
					break;
				}
			}	
		}
	}
	if(sum==-1){
		cout << *disease << "not found" << endl;
	}
	else{
		float AgestatsPercent[4];
		int AgestatsPercentIndex[4];
		string pattern[4] = { "0-20: ","21-40: " ,"41-60: ","60+: "  };
		if(sum>0){
//create percentage array			
			for(int i=0;i<4;i++){ //mexi k ama thelw all ok
				AgestatsPercent[i] =100* (float)agestats[i]/sum ;
				AgestatsPercentIndex[i] = i;
			}
//sort array and swap indeces accordigly
			float tempf;
			int tempi; 
			int maxpos;
			string temps;
			for(int i=0;i<4;i++){
				maxpos=i;
				for(int j=i+1;j<4;j++){					
					if (AgestatsPercent[maxpos]<AgestatsPercent[j]){
						maxpos=j;
					}
				}
				
				tempi=AgestatsPercentIndex[maxpos];
				AgestatsPercentIndex[maxpos]=AgestatsPercentIndex[i];
				AgestatsPercentIndex[i]=tempi;

				tempf=AgestatsPercent[maxpos];
				AgestatsPercent[maxpos]=AgestatsPercent[i];
				AgestatsPercent[i]=tempf;

			}
		}
	
		char perc[20];
//create message using the indeces table		
		for(int i=0;i<k;i++){
			
			if(sum==0){
				message->append(pattern[i]);
				message->append("0");
			}else{
				// cout << AgestatsPercentIndex[i] << endl;
				message->append(pattern[AgestatsPercentIndex[i]]);
				sprintf(perc,"%.2f",AgestatsPercent[i]);
				message->append(perc);
			}	
			message->append("%\n");
		}

	}

	delete[] agestats;
}





//////////////////////////////////////////////////////Hashtable///////////////////////////////////////////////////////////// 
 
void Hashtable::SummaryStatistics(string *date,string*& message){
	int i;
	for (i=0;i<=HTSize-1;i++){
		Bucket * temp = HTHead[i];
		while(temp!=NULL){
			temp->SummaryStatistics(date,message);
			temp = temp->getNext();
		}

	}
}

void Hashtable::numPatientAorD(string* disease,string* date1, string * date2, string* country,string *& message,SSList * Countries,string* EnorEx){		//Acess the architecture-wise deeper structures to print the case count 
	

	char count[10];
	int index;
	index = Hashfunction(*disease);
	Bucket * temp = HTHead [index];
	message->assign("");
 	while(temp!=NULL){
 		if (*country!=""){
			//temp->diseaseFrequency(virusName,country,date1,date2,EnorEx);
			message->append(*country);
				message->append(" ");
 				sprintf(count,"%d",temp->diseaseFrequency(disease,country,date1,date2,EnorEx));
 				message->append(count);
 				message->append("\n");
 		}else{
			SSLNode *  iter;
 			iter = Countries->SSLHead;
			while (iter!= NULL){
				message->append(*(iter->data));
				message->append(" ");
 				sprintf(count,"%d",temp->diseaseFrequency(disease,iter->data,date1,date2,EnorEx));
 				message->append(count);
 				message->append("\n");
				iter=iter->Next;
 			}
 		}
		temp = temp->getNext();
	}	 	
// cout << *message << endl;

}





int Hashtable::diseaseFrequency(string * virusName,string* country,string* date1,string* date2){ 	//Acess  structures to print frequency of a specific virus between dates [ for a country]
	//cout <<*virusName<<*country<<*date1<<*date2 << endl;
	int index;
	int count=0;
	string EnorEx="Entry";
	index = Hashfunction(*virusName);
	Bucket * temp = HTHead [index];
	while (temp!=NULL){
		count=temp->diseaseFrequency(virusName,country,date1,date2,&EnorEx);
		temp = temp->getNext();
		if (count!=0){
			break;
		}
	}
	return count;

}
void Hashtable::TopKAgeRanges(int k,string* disease,string* date1,string* date2,string* country,string* &message){		//topk
	

	message->assign("");	
	int index = Hashfunction(*disease);
	Bucket * temp = HTHead [index];				
	while (temp!=NULL){
		temp->TopKAgeRanges(k,disease,date1,date2,country,message);	
		temp = temp->getNext();
		if(*message!=""){
			break;
		}
	}
}




int Hashtable::Hashfunction(string key){				//hashfunction to get index of the hashtable for key
	int a=HTSize+1;
	int hash=0;
	for(int i=0 ; i<=key.size(); i++){
		hash = (a*hash + (unsigned int) key[i])% HTSize;		//each character of string works into a creating a sum thas is moded(%)
	}
	int hashcode = (hash % HTSize); 
	return hashcode;
}


void Hashtable::Insert(string key,LLNode * record){		//Insert a record pointer into hash......treenode

	int index = Hashfunction(key);

	if(HTHead[index]==NULL)
		HTHead[index] = new Bucket(B_Size);
	//cout <<" key is " << key << " hashcode is " <<index << endl;

	Bucket* temp = HTHead[index];
	while(temp->Insert(record,key)==0){		//only insert if key not found in some bucket
		if (temp->getNext()==NULL){
			temp->setNext(new Bucket(B_Size));
		}
		temp=temp->getNext();
	}
	count++;
}


Hashtable::Hashtable(int _HTSize, int BSize):HTSize(_HTSize),B_Size(BSize){
	//cout << "construct hashtable" << endl;
	
	HTHead = new Bucket*[HTSize];
	for (int i=0;i<=HTSize-1;i++){
		HTHead[i]=NULL;			//inistialize with null. Only gets an address a record is to be inserted 
	}
}


Hashtable::~Hashtable(){			//Hashtable destructor
	
	//cout << "destroy hashtable" << endl;
	
	for (int i=0;i<=HTSize-1;i++){
		Bucket * temp;
		while (HTHead[i]!=NULL){
			temp=HTHead[i];
			HTHead[i]=HTHead[i]->getNext();
			delete temp;
		}
	}
	delete[] HTHead;
}


