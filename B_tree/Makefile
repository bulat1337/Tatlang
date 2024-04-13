PATH_TREE_OBJ = ../obj/B_tree_obj/
PATH_TREE_SRC = ./src/
TREE_SRC = $(wildcard $(PATH_TREE_SRC)*.cpp)
TREE_OBJ = $(patsubst $(PATH_TREE_SRC)%.cpp, $(PATH_TREE_OBJ)%.o, $(TREE_SRC))

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

Include = -I./include/ -I../Utils/include/

$(PATH_LIB)libb_tree.a: $(TREE_OBJ)
	@ ar rvs $@ $(TREE_OBJ)

$(PATH_TREE_OBJ)%.o: $(PATH_TREE_SRC)%.cpp
	@ $(CC) -c $< -o $@ $(FLAGS) $(Include)

clean:
	@rm $(PATH_TREE_OBJ)*.o
