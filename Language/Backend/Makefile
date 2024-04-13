PATH_BKD_OBJ    = ../../obj/backend_obj/
PATH_BKD        = ./src/

BKD_SRC = $(wildcard $(PATH_BKD)*.cpp)
BKD_OBJ = $(patsubst $(PATH_BKD)%.cpp, $(PATH_BKD_OBJ)%.o, $(BKD_SRC))

BKD_TARGET = ../../executables/backend.out

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

Include = -I./include/ -I../../CPU/CPU/Assembler/include/ -I../../CPU/CPU/SPU/include/ -I../../CPU/Drivers/include/ -I../../CPU/Stack/include/

$(BKD_TARGET): $(BKD_OBJ)
	@ $(CC) $(LINK_FLAGS) $^ -o $@ -L../../libs -lSPU -lassembler -L/opt/homebrew/Cellar/sfml/2.6.1/lib/ -lsfml-graphics -lsfml-window -lsfml-system

$(PATH_BKD_OBJ)%.o: $(PATH_BKD)%.cpp
	@ $(CC) -c $< -o $@ $(FLAGS) $(Include)

clean:
	@rm $(BKD_TARGET) $(PATH_BKD_OBJ)*.o
