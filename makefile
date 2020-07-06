trg = hw8-b063040008

all : $(trg)
$(trg) : huffman.o tree.o hw8-b063040008.o
	g++ -o $(trg) hw8-b063040008.o huffman.o tree.o
%.o:%.cpp
	g++ -c $<
clean:
	rm -f *.o $(trg)