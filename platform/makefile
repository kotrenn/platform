NAME = platform
src := $(wildcard *.cpp)
obj := $(src:%.cpp=%.o)
#FLAGS = -O0 -g -Wall -Wextra -std=c++0x
FLAGS = -O3 -std=c++0x
#LINKS = -lm -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lopengl32 -lglu32
LINKS = -lm -lSDL -lSDL_image -lSDL_mixer

all: levels ${NAME}.exe

levels: levels/pipe.txt levels/shapes.txt levels/terrain.txt

${NAME}.exe: $(obj)
	g++ -o ${NAME} $(obj) ${FLAGS} ${LINKS}

$(obj): %.o: %.cpp
	g++ ${FLAGS} -c -o $@ $<

levels/pipe.txt: gen_pipe.py
	python gen_pipe.py

levels/shapes.txt: gen_shapes.py
	python gen_shapes.py

levels/terrain.txt: gen_terrain.py
	python gen_terrain.py

doxygen:
	rm -rf html
	rm -rf latex
	doxygen Doxyfile

check:
	cppcheck --enable=all --inconclusive --std=posix --quiet .