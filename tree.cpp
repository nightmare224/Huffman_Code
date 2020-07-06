#include "tree.h"

using namespace std;

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