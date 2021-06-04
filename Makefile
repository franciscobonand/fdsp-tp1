all: numcheckseq numcheckdatapar numcheckfuncpar 

clean: 
	/bin/rm -f *.o numcheckseq

# vcoê deve criar as regras para gerar os dois outros programas.

numcheckseq: conditions.o numchecks.o timediff.o numcheckseq.o
	gcc -Wall conditions.o numchecks.o timediff.o numcheckseq.o -o numcheckseq

numcheckseq.o: numcheckseq.c conditions.h numchecks.h timediff.h
	gcc -Wall -c numcheckseq.c

conditions.o: conditions.c conditions.h
	gcc -Wall -c conditions.c

numchecks.o: numchecks.c numchecks.h
	gcc -Wall -c numchecks.c

timediff.o: timediff.c timediff.h
	gcc -Wall -c timediff.c
