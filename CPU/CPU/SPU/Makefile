PATH_SPU_OBJ = ../../obj/SPU_obj/
PATH_SPU_SRC = ./src/
SPU_SRC = $(wildcard $(PATH_SPU_SRC)*.cpp)
SPU_OBJ = $(patsubst $(PATH_SPU_SRC)%.cpp, $(PATH_SPU_OBJ)%.o, $(SPU_SRC))

PATH_GLOBAL_OBJ = ../../obj/global_obj/
PATH_GLOBAL_SRC = ../../Global/src/
GLOBAL_SRC = $(wildcard $(PATH_GLOBAL_SRC)*.cpp)
GLOBAL_OBJ = $(patsubst $(PATH_GLOBAL_SRC)%.cpp, $(PATH_GLOBAL_OBJ)%.o, $(GLOBAL_SRC))

PATH_STACK_OBJ = ../../obj/stack_obj/
PATH_STACK_SRC = ../../Stack/src/
STACK_SRC = $(wildcard $(PATH_STACK_SRC)*.cpp)
STACK_OBJ = $(patsubst $(PATH_STACK_SRC)%.cpp, $(PATH_STACK_OBJ)%.o, $(STACK_SRC))

PATH_PARSE_OBJ = ../../obj/file_parser_obj/
PATH_PARSE_SRC = ../../File_parser/src/
PARSE_SRC = $(wildcard $(PATH_PARSE_SRC)*.cpp)
PARSE_OBJ = $(patsubst $(PATH_PARSE_SRC)%.cpp, $(PATH_PARSE_OBJ)%.o, $(PARSE_SRC))

PATH_DRIVERS_OBJ = ../../obj/drivers_obj/
PATH_DRIVERS_SRC = ../../Drivers/src/
DRIVERS_SRC = $(wildcard $(PATH_DRIVERS_SRC)*.cpp)
DRIVERS_OBJ = $(patsubst $(PATH_DRIVERS_SRC)%.cpp, $(PATH_DRIVERS_OBJ)%.o, $(DRIVERS_SRC))

PATH_LIB = ../../../libs/

CC = g++

FLAGS = -D _DEBUG -ggdb3 \
    -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat        \
    -Wmissing-declarations -Wcast-qual -Wchar-subscripts  \
    -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security \
    -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual \
    -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
    -Wstrict-overflow=2 \
    -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef \
    -Wunreachable-code -Wunused -Wvariadic-macros \
    -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs \
    -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow \
    -fno-omit-frame-pointer -Wlarger-than=8192 \
    -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

LINK_FLAGS = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

Include = -I./include/ -I../../Global/include/ -I../../Stack/include/ -I../../File_parser/include/ -I../../Drivers/include/ -I/opt/homebrew/Cellar/sfml/2.6.1/include/

$(PATH_LIB)libSPU.a: $(SPU_OBJ)
	@ ar rvs $@ $(SPU_OBJ) $(GLOBAL_OBJ) $(STACK_OBJ) $(PARSE_OBJ) $(DRIVERS_OBJ)

$(PATH_SPU_OBJ)%.o: $(PATH_SPU_SRC)%.cpp
	@ $(CC) -c $< -o $@ $(FLAGS) $(Include)

clean:
	@rm $(PATH_SPU_OBJ)*.o
