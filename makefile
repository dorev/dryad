# https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gCOMPILER_make.html
PROGRAM = dryad.exe
COMPILER = g++
OUTDIR = -o tmp/$@
INCLUDES = -Iextern/pugixml

$(PROGRAM):  main.o pugixml.o
	$(COMPILER) -o $(PROGRAM) $(addprefix tmp/, $^)

main.o : pugixml.o
	$(COMPILER) -g -c src/$*.cpp $(INCLUDES) $(OUTDIR)
	echo All done!

pugixml.o:
	$(COMPILER) -g -c extern/pugixml/$*.cpp $(INCLUDES) $(OUTDIR)

clean :
# remove directory recursive force
	-rm -d -r -f dryad *.o tmp/*.* 