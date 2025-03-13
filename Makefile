TARGET=\
       ./build/cvpc

SRC=\
	./src/main.c\
	./src/scan.c\
	./src/utils.c\
	./src/targ.c \
	./src/type.c \
	./src/scope.c \
	./src/token.c

.PHONY: all
all: $(SRC)
	mkdir -p build/
	$(CC) $(CFLAGS) -g $(SRC) -o $(TARGET)
