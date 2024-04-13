SUBDIRS = ../Utils/ ../B_tree/ ../Recursive_parser/ ../Language/Frontend/ ../CPU/build/ ../Language/Backend/

FRD_SUBDIRS = ../Utils/ ../B_tree/ ../Recursive_parser/ ../Language/Frontend/
BKD_SUBDIRS = ../CPU/build/ ../Language/Backend/

TXT_JUNK = $(wildcard *.txt)
BIN_JUNK = $(wildcard *.bin)
EXE_JUNK = $(wildcard ../executables/*.out)
LIB_JUNK = $(wildcard ../libs/*.a)

all:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done

frd:
	@for dir in $(FRD_SUBDIRS); do $(MAKE) -C $$dir; done

bkd:
	@for dir in $(BKD_SUBDIRS); do $(MAKE) -C $$dir; done

clean_junk:
	@rm $(TXT_JUNK) $(BIN_JUNK) $(EXE_JUNK) $(LIB_JUNK)

clean_all:
	@for dir in $(SUBDIRS); do $(MAKE) clean -C $$dir; done

clean: clean_junk clean_all
