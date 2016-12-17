objects =getCommonSeq/ComSeq.o\
  FuzzyMatchingLib/lib/\.libs/regerror.o\
  FuzzyMatchingLib/lib/\.libs/tre-compile.o\
  FuzzyMatchingLib/lib/\.libs/tre-match-parallel.o\
  FuzzyMatchingLib/lib/\.libs/tre-stack.o\
  FuzzyMatchingLib/lib/\.libs/regexec.o\
  FuzzyMatchingLib/lib/\.libs/tre-match-approx.o\
  FuzzyMatchingLib/lib/\.libs/tre-mem.o\
  FuzzyMatchingLib/lib/\.libs/regcomp.o\
  FuzzyMatchingLib/lib/\.libs/tre-ast.o\
  FuzzyMatchingLib/lib/\.libs/tre-match-backtrack.o\
  FuzzyMatchingLib/lib/\.libs/tre-parse.o
CC = g++ -g  -I getCommonSeq/    #-Wall
all:    prepareall
	$(CC) -o   motifTools    main.cpp  $(objects)
prepareall:    subsystem
subsystem:
	$(MAKE) -C getCommonSeq/
clean :  cleansub

	rm   motifTools
cleansub :
	rm     getCommonSeq/ComSeq.o 
