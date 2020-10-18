#include<string>

using namespace std;

class SSLNode{

	public:
		string * data;
		SSLNode* Next;
		SSLNode(string);
		~SSLNode(void);
		void Print(void);

};


//template <class dataType> 
class SSList{
	
	public:
		SSLNode* SSLHead;
		SSList();
		~SSList();
		void Insert(string);
		//LList* Remove();
		SSLNode* Search(string);
		void Print();
		void Sort(int);
};


int DateCompareEarlier(string*,string*);

///////////////////////////////////////////////////////////////
class MaxHeapNode{
	private:
		int count;
		string* str;
		MaxHeapNode* left;
		MaxHeapNode* right;
	public:
		MaxHeapNode(string,int);
		~MaxHeapNode();
		MaxHeapNode* getleft();
		MaxHeapNode* getright();
		string* getstr();
		int getcount();
		void setstr(string*);
		void setcount(int);
		void Incount(void);
		void setleft(MaxHeapNode*);
		void setright(MaxHeapNode*);

};



class QNode{
	private:
		QNode * next;
		MaxHeapNode * data;
	public:
		QNode(MaxHeapNode*);
		~QNode();
		QNode * getnext();
		void setnext(QNode*);
		MaxHeapNode * getdata(void);
		void setdata(MaxHeapNode*);


};


class Queue {
	private:
		QNode * front;
		QNode * back;
	public:
		Queue();
		~Queue();
		void enQ(MaxHeapNode*);
		void deQ(void);
		int QEmpty(void);
		void Destroy();
		QNode * getfront();
		QNode * getback();
};











class MaxHeap{
	private:
		MaxHeapNode* Root;
		Queue * Q;
		string* key;
		int count;
	public:
		MaxHeap(string);
		~MaxHeap();
		void Insert(MaxHeapNode*);
		MaxHeapNode* getMax();
		string* getkey();
		MaxHeapNode* Search(MaxHeapNode*,string);
		void Destroy(MaxHeapNode*);
		void Heapify(MaxHeapNode*);
		void ShiftUP(MaxHeapNode*);
		void Preorder(MaxHeapNode*);
		void RemoveRoot();
		void RemoveRootHelper(MaxHeapNode*,MaxHeapNode*,Queue*);
};







/////////////////////////////////////////////////////////////////////






class LLNode{
	private:
		string* Record_id;
		string* Entry;
		string* Exit;
		string* Fname;
		string* Lname;
		string* Disease;
		string* Country;
		int Age;
		LLNode* Next;
	public:
		LLNode(string,string,string,string,string,string,string,int);
		~LLNode(void);
		void Print(void);
		string* getid(void);
		string* getFname(void);
		string* getLname(void);
		string* getDisease(void);
		string* getCountry(void);
		string* getEntry(void);
		string* getExit(void);
		//string* getDate(void);
		int getAge(void);
		void setAge(int);
		void setFname(string);
		void setLname(string);
		void setDisease(string);
		void setCountry(string);
		void setExit(string);
		void setEntry(string);
		LLNode* getNext(void);
		void setNext(LLNode*);
};


//template <class dataType> 
class LList{
	private:
		LLNode* LLHead;
	public:
		LList();
		~LList();
		void Insert(string,string,string,string,string,string,string,int);
		LLNode* Search(string);
		LLNode* SearchDate(string);
		LLNode* getLLHead();
		void Print();
};





class RBTreeNode{
	private:
		RBTreeNode* left;
		RBTreeNode* right;
		RBTreeNode* parent;
		int color;							//////0=RED   1=BLACK
		LLNode * Record;
		string* Date;
	public:
		RBTreeNode(LLNode*);
		~RBTreeNode();
		void setleft(RBTreeNode *);
		void setright(RBTreeNode *);
		void setColor(int);
		void setparent(RBTreeNode*);
		RBTreeNode * getleft(void);	
		RBTreeNode * getright(void);
		RBTreeNode * getparent(void);
		LLNode* getRecord(void);
		string* getDate(void);
		int getColor(void);
		void InorderPrinter();
		void PostorderPrinter();
		void PreorderPrinter();
		int DateCompareEarlier(RBTreeNode*,RBTreeNode*);
};



class RBTree{
	private:
		RBTreeNode * Root;
		int Nodecount;
	public:
		RBTree();
		~RBTree();
		RBTreeNode * getRoot(void);
		void setRoot(RBTreeNode*);
		void LRot(RBTreeNode*&);
		void RRot(RBTreeNode*&);
		void Inorder(RBTreeNode*);
		void Postorder(RBTreeNode*);
		void Preorder(RBTreeNode*);
		RBTreeNode* BSTInsert(RBTreeNode* , RBTreeNode*&);
		void FixRBInsert(RBTreeNode *&);
		void Insert(LLNode*);
		void Destroy(RBTreeNode*);
		int BetweenDatesCount(RBTreeNode*, string*, string*,string*,int);
		void SummaryStatistics(RBTreeNode* ,string*,int *&);
		void CountforTopk(RBTreeNode*,string*,string*,string*,int* &);



};








class BEntry{
	private:
		string* Key;
		RBTree* RBHead; 
	public:
		BEntry();
		~BEntry();
		void Insert(LLNode*,string);
		string* getkey();
		int BetweenDatesCount(string* ,string*,string*,int);
		void SummaryStatistics(string*,int *&);
		void CountforTopk(string*, string*, string*,int *&);

};






class Bucket{
	private:
		int B_Size;
		int count;
		BEntry** Content;
		Bucket * Next;
	public:
		Bucket(int);
		~Bucket(void);
		int Insert(LLNode*,string);
		Bucket * getNext();
		void setNext(Bucket*);
		void PrintBetweenDatesCount(string*,string*);
		int diseaseFrequency(string *,string *, string*, string*,string*);
		void TopK(MaxHeap*,string*,string*,int);
		void SummaryStatistics(string *,string*&);
		void TopKAgeRanges(int,string*,string*,string*,string*,string* &);		//topK function used for both topk applications


};










class Hashtable{
	private:
		int HTSize;
		Bucket** HTHead;
		int count;
		int B_Size;///// Isws kalutera edw
	public:
		Hashtable(int ,int);
		~Hashtable();
		int getHTSize(void);
		Bucket* getHTNode(void);
		int Hashfunction(string);
		void Insert(string,LLNode*);
		void getDisease();
		void PrintBetweenDatesCount(string*,string*);
		int diseaseFrequency(string*,string*,string*,string*);
		void TopK(int,string*,string*,string*,int);
		void SummaryStatistics(string *date,string*&);
		void numPatientAorD(string*,string*, string *, string*,string *&,SSList * ,string* );		//Acess the architecture-wise deeper structures to print the case count 
		void TopKAgeRanges(int,string*,string*,string*,string*,string*&);		//topk




};


/////////////////////////////////////////////////////////////
