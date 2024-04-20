PATH_L_TEST_OBJ    = ../obj/language_test_obj/
PATH_L_TEST        = ./

L_TEST_SRC = $(wildcard $(PATH_L_TEST)*.cpp)
L_TEST_OBJ = $(patsubst $(PATH_L_TEST)%.cpp, $(PATH_L_TEST_OBJ)%.o, $(L_TEST_SRC))

L_TEST_TARGET = ../executables/language_test.out

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

Include = -I../Language/Frontend/include/ -I../B_tree/include/ -I../Recursive_parser/include/ -I../Utils/include/ -I../Language/Backend/include/ -I../Language/Midend/include/ -I../CPU/CPU/Assembler/include/ -I../CPU/CPU/SPU/include/ -I../CPU/Stack/include/ -I../CPU/Drivers/include/

$(L_TEST_TARGET): $(L_TEST_OBJ)
	@ $(CC) $(LINK_FLAGS) $^ -o $@ -L../libs -lfrontend -lmidend -lbackend -L/opt/homebrew/Cellar/sfml/2.6.1/lib/ -lsfml-graphics -lsfml-window -lsfml-system

$(PATH_L_TEST_OBJ)%.o: $(PATH_L_TEST)%.cpp
	@ $(CC) -c $< -o $@ $(FLAGS) $(Include)

clean:
	@rm $(L_TEST_TARGET) $(PATH_L_TEST_OBJ)*.o

