//#ifndef huffman
//    #define huffman

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>


using namespace std;


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
//#endif