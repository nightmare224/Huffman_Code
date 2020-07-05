//Author : B063040008 Hsuang-Ling Tai
//Date : Dec. 28,2018
//Exercise 8 : Huffman Code
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include<string>
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
class Huffman
{
private:
    int count[256];                 //record a frequence of char
    string Code[256];               //record a code of char
    unsigned int original_data_no;  //a bytes of orginal data
    unsigned int compress_data_no;  //a bytes of compress data
public:
    Huffman():original_data_no(0),compress_data_no(0)
    {
        for(int i=0;i<256;i++)
        {
            count[i]=0;             //initialize
            Code[i]="";
        }
    }
    //use for endcode
    void count_char_freq(char infile_name[]);           //count the frequence of every char in input file
    void encode(char []);                                //encode every char
    void generate_compress_file(char [],char []);         //generate compress file
    //use for decode
    int decode(char []);                                 //get code from compress file header
    void generate_uncompress_file(char [],char [],int);   //generate uncomprss file
};
int main(int argc,char *argv[])
{

    Huffman test;
    //./hw8-b063040008 huffman –c –i infile –o outfile
    if(argv[2][1]=='c')
    {
        test.encode(argv[4]);
        test.generate_compress_file(argv[4],argv[6]);
    }
    //./hw8-b063040008 huffman –u –i infile –o outfile
    else if(argv[2][1]=='u')
    {
        int pos=test.decode(argv[4]);
        test.generate_uncompress_file(argv[4],argv[6],pos);
    }
    else
        cout<<"Wrong command"<<endl;

    return 0;
}
void Huffman::encode(char infile_name[])
{
    count_char_freq(infile_name);                      //count the frequence of every char in input file
    
    List_Tree list(count);                             //construct a list
    list.grow_to_tree();                               //let list become a tree
    
    Node *current=list.getLast();                      //start at biggest node(Root)
    int index;
    while(current!=list.getFirst())                    //left tree is 0. right tree is 1
    {
        if(current->Left!=NULL)
        current->Left->code=current->code+"0";
        if(current->Right!=NULL)
        current->Right->code=current->code+"1";
        current=current->Prev;               //visit every node
        if(current->leaf)
        {
            current->Data<0?(index=current->Data+256):index=current->Data;
            Code[index]=current->code;                 //put char's code in array
        }
        delete list.getLast();                         //get the code, can delete node
        list.setLast(current);
    }
}
void Huffman::count_char_freq(char infile_name[])
{
    int index;
    char ch;
    ifstream infile(infile_name,ios_base::binary);
    if(!infile)                         //throw error when fail to open file
    {
        cout<<"Fail to open file"<<endl;
        exit(0);
    }
    while(1)
    {
        infile.read(&ch,1);             //read one char to ch
        index=(int)ch;
        if(!infile)                     //end of file
        break;
        if(index<0)                     //complement of negitive char
        index+=256;
        count[index]++;                 //count the frequence of 256 kinds of chars
        original_data_no++;             //recode a bytes of input data
    }
    
    infile.close();
}
void Huffman::generate_compress_file(char infile_name[],char outfile_name[])
{
    int pos,index;
    char ch;
    ofstream outfile(outfile_name,ios_base::binary);     //creat compressed file
    for(int i=0;i<256;i++)                               //print code table
    if(Code[i]!="")
    outfile<<(char)i<<"="<<Code[i]<<endl;
    outfile<<"uncompressed size : "<<setw(10)<<original_data_no<<endl;
    outfile<<"compressed size : ";
    pos=outfile.tellp();
    outfile<<setw(10)<<'0'<<endl;
    
    ifstream infile(infile_name,ios::binary);
    while(1)
    {
        infile.read(&ch,sizeof(char));
        index=(int)ch;
        if(!infile)
        break;
        if(index<0)
        index+=256;
        outfile<<Code[index];                           //find correspond Code and output to compressed file
    }
    
    compress_data_no=outfile.tellp();                            //record the bytes of data after compress
    outfile.seekp(pos);                                 //move to proper position
    outfile<<setw(10)<<(compress_data_no/8+compress_data_no%8);            //write the the bytes of data in file
    
    infile.close();
    outfile.close();
}
int Huffman::decode(char infile_name[])
{
    ifstream infile(infile_name,ios::binary);
    int pos;
    char ch;
    int index;
    string data="";
    while(1)
    {
        infile.read(&ch,1);
        getline(infile,data,'\n');
        if(data[0]!='=')
        break;
        if(ch<0)
        index=ch+256;
        else
        index=ch;
        Code[index]=data.substr(1,data.length()-1);
    }
    
    getline(infile,data,'\n');   //useless
    pos=infile.tellg();
    infile.close();
    
    return pos;
}
void Huffman::generate_uncompress_file(char infile_name[],char outfile_name[],int pos)
{
    List_Tree list;
    list.restruct_Tree(Code);
    
    ifstream infile(infile_name,ios::binary);              //compressed file
    ofstream outfile(outfile_name,ios::binary);            //recover to original file
    infile.seekg(pos);
    Node *current=list.getRoot();
    char ch;
    while(!infile.eof())
    {
        current=list.getRoot();               //search from root everytime
        while(current->leaf!=true)            //stop when meet leaf node. means find correspond char
        {
            infile.get(ch);
            if(ch=='1')                      //go right
            current=current->Right;
            else if(ch=='0')                 //go left
            current=current->Left;
        }
        if(infile)                  //output decoded char
        outfile<<(current->Data);
    }
    
    infile.close();
    outfile.close();
}
List_Tree::List_Tree(int count[])        //set normal leaf data into list
{
    int target=INT_MAX,i,index,small=0;
    bool Flag=true;
    First=new Node('!',0,false,NULL,NULL);  //head node, useless
    Last=First;
    //set other node
    while(Flag)                        //find less frequence char
    {
        target=INT_MAX;
        Flag=false;
        for(i=255;i>=0;i--)            //let dictionary small index char can be at front
        {
            if(count[i]!=0&&count[i]<=target)
            {
                index=i;
                target=count[i];
                Flag=true;
            }
        }
        if(!Flag)                     //means all array's data have been inserted into list
        break;
        count[index]=0;               //delete the data have been inserted into list
        if(index>127)
        index=index-256;
        target+=small;
        Last->Next=new Node(index,target,true,NULL,Last);
        Last=Last->Next;
    }
    Node *temp=First;
    First=First->Next;                //orginal first node is useless
    First->Prev=NULL;
    delete temp;
}
void List_Tree::grow_to_tree()
{
    Node *current=First,*parent;
    while(current->Next)
    {
        parent=new Node('!',(current->Freq)+(current->Next->Freq),false,NULL,NULL);
        //parent node didn't have data char, '!' is useless.  "false" represent it isn't leafnode
        Add(parent);                          //add parent node to sorted list
        parent->Left=current->Next;           //connect to left child(big)
        parent->Right=current;                //connect to right child(small)
        current=current->Next->Next;
    }
    Root=parent;
}
void List_Tree::Add(Node *node)                             //add non leaf data into list too
{
    Node *current=First;
    while(current!=NULL && current->Freq < node->Freq)     //find the proper place to insert
    current=current->Next;
    
    if(current==NULL)                   //this node have biggest frequence. It should be last node
    {
        Last->Next=node;                //link
        node->Prev=Last;
        Last=node;
    }
    else                                //normal case. Node would be insert at somewhere in middle
    {
        current->Prev->Next=node;       //link
        node->Prev=current->Prev;
        current->Prev=node;
        node->Next=current;
    }
}
void List_Tree::restruct_Tree(string Code[])
{
    Root=new Node;
    Node *current;
    int i,j;
    for(i=0;i<256;i++)
    {
        current=Root;
        if(Code[i]=="")                         //unused data
        continue;
        for(j=0;j<int(Code[i].size());j++)
        {
            if(Code[i][j]=='0')                 //the letter is 0, go left
            {
                if(current->Left==NULL)         //if left side is null constuct the new node
                current->Left=new Node;
                current=current->Left;          //normally go left
            }
            else if(Code[i][j]=='1')            //the letter is 1, go right
            {
                if(current->Right==NULL)        //if right side is null constuct the new node
                current->Right=new Node;
                current=current->Right;         //normally go left
            }
        }
        current->Data=(char)i;                  //put the Data in node
        current->leaf=true;                     //it is leaf node
    }
}


