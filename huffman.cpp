#include "huffman.h"
#include "tree.h"

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

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