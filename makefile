a.out : Simu.o	SP_Heap.o		
	gcc Simu.o SP_Heap.o

Simu.o: Simu.c header.h	
	gcc -c Simu.c

SP_Heap.o : SP_Heap.c header.h	
	gcc -c SP_Heap.c

