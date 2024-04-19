SUBDIRS = ../File_parser/ ../Utils/ ../B_tree/ ../Recursive_parser/ ../Language/Frontend/ ../CPU/build/ ../Language/Backend/ ../Language_test/

TXT_JUNK = $(wildcard *.txt)
PNG_JUNK = $(wildcard *.png)
BIN_JUNK = $(wildcard *.bin)
DOT_JUNK = $(wildcard *.dot)
EXE_JUNK = $(wildcard ../executables/*.out)
LIB_JUNK = $(wildcard ../libs/*.a)

all:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done

clean_junk:
	@rm $(TXT_JUNK) $(BIN_JUNK) $(EXE_JUNK) $(LIB_JUNK) $(PNG_JUNK) $(DOT_JUNK)

clean_all:
	@for dir in $(SUBDIRS); do $(MAKE) clean -C $$dir; done

clean: clean_junk clean_all
