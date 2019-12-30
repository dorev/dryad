# LEARN
# https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

# mute make print
#.SILENT:

PROGRAM = dryad
COMPILER = g++
OUTDIR = -o tmp/$@
INCLUDES = -Iextern -Iextern/gtest -Iextern/gtest/include

# MAIN PROGRAM ----------------------------------------------------------------
$(PROGRAM):  main.o pugixml.o
	@echo Linking...
	$(COMPILER) -o $@ $(addprefix tmp/, $^)
	@echo Build success!

main.o : pugixml.o
	@echo Compiling $*...
	$(COMPILER) -g -c src/$*.cpp $(INCLUDES) $(OUTDIR)

pugixml.o:
	@echo Compiling $*...
	$(COMPILER) -g -c extern/pugixml/$*.cpp $(INCLUDES) $(OUTDIR)

# TESTS -----------------------------------------------------------------------
GTESTLIBS = -L/usr/lib/x86_64-linux-gnu -pthread

test : tests.o
	@echo Linking tests...
	$(COMPILER) -o $(PROGRAM)$@ tmp/tests.o $(GTESTLIBS) 
	@echo Starting tests...
	./$(PROGRAM)$@

tests.o :
	@echo Compiling tests...
	$(COMPILER) -g -c tests/tests.cpp $(INCLUDES) $(OUTDIR) -I.

# CLEAN -----------------------------------------------------------------------
clean :
	rm -d -r -f $(PROGRAM) $(PROGRAM)test *.o tmp/*
	clear