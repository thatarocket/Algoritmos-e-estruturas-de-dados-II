all: conectividade.exe #para executar escrever make

conectividade.exe: conectividade.o
	gcc -o conectividade.exe conectividade.o 

conectividade.o: conectividade.c busca.o
	gcc -c conectividade.c

busca.o: busca.c busca.h
	gcc -c busca.c 

clean: 
	rm -f *.o *.exe