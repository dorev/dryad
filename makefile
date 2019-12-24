# https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html
PROGRAM = dryad
CC = g++
TMPOUT = -o tmp/$@

$(PROGRAM):  main.o pugixml.o
	$(CC) -o $(PROGRAM) $(addprefix tmp/, $^)

main.o : pugixml.o
	$(CC) -g -c src/main.cpp -Iextern/pugixml $(TMPOUT)
	echo All done!

pugixml.o:
	$(CC) -g -c extern/pugixml/$*.cpp -Iextern/pugixml $(TMPOUT)

clean :
	-rm -d -r -f dryad tmp/*.*