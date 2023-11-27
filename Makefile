FILE_CODE_SRC = code
FILE_EXEC = exec
GTK_INCLUDE = `pkg-config --cflags gtk+-3.0`
GTK_LIBS = `pkg-config --libs gtk+-3.0`

main: file_executables
	gcc main.c -o main $(GTK_INCLUDE) $(GTK_LIBS)

file_executables: ${FILE_CODE_SRC}/*.c
	@for f in $(shell ls -1 ${FILE_CODE_SRC}/*.c | xargs -n 1 basename | sed 's/\.[a-z]*//g'); do gcc ${FILE_CODE_SRC}/$${f}.c -o ${FILE_EXEC}/$${f} $(GTK_INCLUDE) $(GTK_LIBS); done

clean:
	rm -f *.o main ${FILE_EXEC}/*

