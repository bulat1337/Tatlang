SUBDIRS = ../File_parser/ ../Utils/ ../B_tree/ ../Recursive_parser/ ../Language/Frontend/  ../Language/Midend/ ../CPU/build/ ../Language/Backend/ ../Language_test/

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

build:
	mkdir -p ../obj
	mkdir -p ../obj/assembler_obj
	mkdir -p ../obj/drivers_obj
	mkdir -p ../obj/global_obj
	mkdir -p ../obj/SPU_obj
	mkdir -p ../obj/stack_obj
	mkdir -p ../obj/file_parser_obj
	mkdir -p ../obj/utils_obj
	mkdir -p ../obj/B_tree_obj
	mkdir -p ../obj/recursive_parser_obj
	mkdir -p ../obj/frontend_obj
	mkdir -p ../obj/midend_obj
	mkdir -p ../obj/backend_obj
	mkdir -p ../obj/language_test_obj
	mkdir -p ../CPU/obj/assembler_obj
	mkdir -p ../CPU/obj/drivers_obj
	mkdir -p ../CPU/obj/global_obj
	mkdir -p ../CPU/obj/SPU_obj
	mkdir -p ../CPU/obj/stack_obj
	mkdir -p ../CPU/libs/
	mkdir -p ../executables
	mkdir -p ../libs

clean: clean_junk clean_all
