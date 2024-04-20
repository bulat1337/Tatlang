PATH_BKD_OBJ = ../../obj/backend_obj/
PATH_BKD_SRC = ./src/
BKD_SRC = $(wildcard $(PATH_BKD_SRC)*.cpp)
BKD_OBJ = $(patsubst $(PATH_BKD_SRC)%.cpp, $(PATH_BKD_OBJ)%.o, $(BKD_SRC))


PATH_ASM_OBJ = ../../CPU/obj/assembler_obj/
PATH_ASM_SRC = ../../CPU/CPU/Assembler/src/
ASM_SRC = $(wildcard $(PATH_ASM_SRC)*.cpp)
ASM_OBJ = $(patsubst $(PATH_ASM_SRC)%.cpp, $(PATH_ASM_OBJ)%.o, $(ASM_SRC))

PATH_GLOBAL_OBJ = ../../CPU/obj/global_obj/
PATH_GLOBAL_SRC = ../../CPU/Global/src/
GLOBAL_SRC = $(wildcard $(PATH_GLOBAL_SRC)*.cpp)
GLOBAL_OBJ = $(patsubst $(PATH_GLOBAL_SRC)%.cpp, $(PATH_GLOBAL_OBJ)%.o, $(GLOBAL_SRC))

PATH_STACK_OBJ = ../../CPU/obj/stack_obj/
PATH_STACK_SRC = ../../CPU/Stack/src/
STACK_SRC = $(wildcard $(PATH_STACK_SRC)*.cpp)
STACK_OBJ = $(patsubst $(PATH_STACK_SRC)%.cpp, $(PATH_STACK_OBJ)%.o, $(STACK_SRC))

PATH_PARSE_OBJ = ../../obj/file_parser_obj/
PATH_PARSE_SRC = ../../File_parser/src/
PARSE_SRC = $(wildcard $(PATH_PARSE_SRC)*.cpp)
PARSE_OBJ = $(patsubst $(PATH_PARSE_SRC)%.cpp, $(PATH_PARSE_OBJ)%.o, $(PARSE_SRC))

PATH_SPU_OBJ = ../../CPU/obj/SPU_obj/
PATH_SPU_SRC = ../../CPU/CPU/SPU/src/
SPU_SRC = $(wildcard $(PATH_SPU_SRC)*.cpp)
SPU_OBJ = $(patsubst $(PATH_SPU_SRC)%.cpp, $(PATH_SPU_OBJ)%.o, $(SPU_SRC))

PATH_DRIVERS_OBJ = ../../CPU/obj/drivers_obj/
PATH_DRIVERS_SRC = ../../CPU/Drivers/src/
DRIVERS_SRC = $(wildcard $(PATH_DRIVERS_SRC)*.cpp)
DRIVERS_OBJ = $(patsubst $(PATH_DRIVERS_SRC)%.cpp, $(PATH_DRIVERS_OBJ)%.o, $(DRIVERS_SRC))


PATH_LIB = ../../libs/

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

Include = -I./include/ -I../../CPU/CPU/Assembler/include/ -I../../CPU/CPU/SPU/include/ -I../../CPU/Drivers/include/ -I../../CPU/Stack/include/ -I../../B_tree/include/ -I../../Utils/include/


$(PATH_LIB)libbackend.a: $(BKD_OBJ)
	@ ar rvs $@ $(BKD_OBJ) $(ASM_OBJ) $(GLOBAL_OBJ) $(STACK_OBJ) $(PARSE_OBJ) $(SPU_OBJ) $(DRIVERS_OBJ)

$(PATH_BKD_OBJ)%.o: $(PATH_BKD_SRC)%.cpp
	@ $(CC) -c $< -o $@ $(FLAGS) $(Include)

clean:
	@rm $(PATH_BKD_OBJ)*.o
