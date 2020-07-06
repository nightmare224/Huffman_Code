//#ifndef LISTTREE
//    #define LISTTREE


#include <iomanip>
#include <string>

using namespace std;

struct Node
{
    char Data;      //the char
    int Freq;       //the freqence of Data in input file
    bool leaf;      //record it is leaf or not
    Node *Next;     //for when it is list
    Node *Prev;
    Node *Left;     //for when it is tree
    Node *Right;
    string code;    //record the code temparily when encode
    Node(char ch,int freq,bool tf,Node *next,Node *prev):Data(ch),Freq(freq),leaf(tf),Next(next),Prev(prev),code(""){}
    Node():Data('!'),Freq(0),leaf(false),Next(NULL),Prev(NULL),Left(NULL),Right(NULL),code(""){}
};
class List_Tree
{
    private:
        Node *First;
        Node *Last;
        Node *Root;
    public:
        List_Tree():First(NULL),Last(NULL),Root(NULL){};
        //use for encode
        List_Tree(int count[256]);          //put a data into list, and sorted according to freq
        void Add(Node *);                   //add new node to list
        void grow_to_tree();                //the added node would be a parent
        //use for decode
        void restruct_Tree(string Code[]);  //restruct tree by Code[256]
        
        Node* getFirst(){return First;}
        Node* getLast(){return Last;}
        Node* getRoot(){return Root;}
        void setLast(Node *current){Last=current;}
};

//#endif