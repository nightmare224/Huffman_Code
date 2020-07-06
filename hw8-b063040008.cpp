//Author : B063040008 Hsuang-Ling Tai
//Date : Dec. 28,2018
//Exercise 8 : Huffman Code

#include "huffman.h"

using namespace std;

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




