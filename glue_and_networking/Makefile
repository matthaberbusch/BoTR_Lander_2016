all: test1 test2
test1: checksum.o rdt.o test.o test1.o irq_dummy.o
	gcc -g checksum.o rdt.o test.o test1.o irq_dummy.o -o test1
test2: checksum.o rdt.o test.o test2.o irq_dummy.o
	gcc -g checksum.o rdt.o test.o test2.o irq_dummy.o -o test2
checksum.o: packet.h checksum.h checksum.c
	gcc -g -c checksum.c
rdt.o: packet.h rdt.h checksum.h rdt.c
	gcc -g -c rdt.c
test.o: test.h test.c
	gcc -g -c test.c
test1.o: test.h rdt.h test1.c
	gcc -g -c test1.c
test2.o: test.h rdt.h test2.c
	gcc -g -c test2.c
irq_dummy.o: irq.h irq_dummy.c
	gcc -g -c irq_dummy.c
clean:
	rm checksum.o rdt.o test.o test1.o test2.o irq_dummy.o
