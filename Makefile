DIR_INC = -I./include -I./include/easybmp -I./include/jpeglib
DIR_SRC = .
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.c)
SRCPP = $(wildcard $(DIR_SRC)/*.cpp)
OBJ = $(patsubst %.c,$(DIR_OBJ)/%.o,$(notdir $(SRC)))
OBJ += $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(notdir $(SRCPP)))

TARGET = image.out

BIN_TARGET = $(TARGET)

CC = g++
AR = ar -rcs
MKDIR = mkdir

CFLAGS = -g -Wall $(DIR_INC)
LFLAGS = -L./lib -leasybmp -ljpeg

$(BIN_TARGET):$(OBJ)
	$(CC) -o $@ $(OBJ) $(LFLAGS)

$(DIR_OBJ)/%.o:$(DIR_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_OBJ)/%.o:$(DIR_SRC)/%.cpp $(DIR_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_OBJ):
	$(MKDIR) -p $@

.PHONY:clean
    
clean:
	rm -rf $(OBJ)
	rm -rf $(BIN_TARGET)
