TARGET=\
	build/cvpc

SRC=\
	src/main.c\
	src/scan.c\
	src/utils.c\
	src/targ.c \
	src/type.c \

.PHONY: all
all: $(SRC)
	$(CC) $(CFLAGS) -g $(SRC) -o $(TARGET)