PATH_R_PARSER_OBJ = ../obj/recursive_parser_obj/
PATH_R_PARSER_SRC = ./src/
R_PARSER_SRC = $(wildcard $(PATH_R_PARSER_SRC)*.cpp)
R_PARSER_OBJ = $(patsubst $(PATH_R_PARSER_SRC)%.cpp, $(PATH_R_PARSER_OBJ)%.o, $(R_PARSER_SRC))

PATH_LIB = ../libs/

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

Include = -I../B_tree/include/ -I./include/ -I../Utils/include/

$(PATH_LIB)librecursive_parser.a: $(R_PARSER_OBJ)
	@ ar rvs $@ $(R_PARSER_OBJ)

$(PATH_R_PARSER_OBJ)%.o: $(PATH_R_PARSER_SRC)%.cpp
	@ $(CC) -c $< -o $@ $(FLAGS) $(Include)

clean:
	@rm $(PATH_R_PARSER_OBJ)*.o
