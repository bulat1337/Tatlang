PATH_FRD_OBJ = ../../obj/frontend_obj/
PATH_FRD_SRC = ./src/
FRD_SRC = $(wildcard $(PATH_FRD_SRC)*.cpp)
FRD_OBJ = $(patsubst $(PATH_FRD_SRC)%.cpp, $(PATH_FRD_OBJ)%.o, $(FRD_SRC))

PATH_B_TREE_OBJ = ../../obj/b_tree_obj/
PATH_B_TREE_SRC = ../../B_tree/src/
B_TREE_SRC = $(wildcard $(PATH_B_TREE_SRC)*.cpp)
B_TREE_OBJ = $(patsubst $(PATH_B_TREE_SRC)%.cpp, $(PATH_B_TREE_OBJ)%.o, $(B_TREE_SRC))

PATH_R_PARSE_OBJ = ../../obj/recursive_parser_obj/
PATH_R_PARSE_SRC = ../../Recursive_parser/src/
R_PARSE_SRC = $(wildcard $(PATH_R_PARSE_SRC)*.cpp)
R_PARSE_OBJ = $(patsubst $(PATH_R_PARSE_SRC)%.cpp, $(PATH_R_PARSE_OBJ)%.o, $(R_PARSE_SRC))

PATH_PARSE_OBJ = ../../obj/file_parser_obj/
PATH_PARSE_SRC = ../../File_parser/src/
PARSE_SRC = $(wildcard $(PATH_PARSE_SRC)*.cpp)
PARSE_OBJ = $(patsubst $(PATH_PARSE_SRC)%.cpp, $(PATH_PARSE_OBJ)%.o, $(PARSE_SRC))

PATH_UTILS_OBJ = ../../obj/utils_obj/
PATH_UTILS_SRC = ../../Utils/src/
UTILS_SRC = $(wildcard $(PATH_UTILS_SRC)*.cpp)
UTILS_OBJ = $(patsubst $(PATH_UTILS_SRC)%.cpp, $(PATH_UTILS_OBJ)%.o, $(UTILS_SRC))


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

Include =  -I./include/ -I../../B_tree/include/ -I../../Recursive_parser/include/ -I../../Utils/include/ -I../../File_parser/include/

$(PATH_LIB)libfrontend.a: $(FRD_OBJ)
	@ ar rvs $@ $(FRD_OBJ) $(B_TREE_OBJ) $(R_PARSE_OBJ) $(PARSE_OBJ) $(UTILS_OBJ)

$(PATH_FRD_OBJ)%.o: $(PATH_FRD_SRC)%.cpp
	@ $(CC) -c $< -o $@ $(FLAGS) $(Include)

clean:
	@rm $(PATH_FRD_OBJ)*.o
