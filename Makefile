PROJECT=simulation
SRC=${PROJECT}.cpp

ifeq (${OS},Windows_NT)
	EXE=${PROJECT}.exe
	CPPFLAGS=-g -I C:\msys64\mingw64\include
	LDFLAGS=-lopengl32 -lglfw3 -lglew32
else
	EXE=${PROJECT}
	CPPFLAGS=-g
	LDFLAGS=-lGL -lglfw -lGLEW
endif


${EXE}:${SRC} constants.hpp shaders.hpp camera.hpp
	g++ ${CPPFLAGS} $< -o $@ ${LDFLAGS}

clean:
	rm -f ${EXE}

run: ${EXE}
	./${EXE}