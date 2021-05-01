DIR_INC = ./include ./include/easybmp
DIR_SRC = .
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.c)
SRCPP = $(wildcard $(DIR_SRC)/*.cpp)
OBJ = $(patsubst %.c,$(DIR_OBJ)/%.o,$(notdir $(SRC)))
OBJ += $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(notdir $(SRC)))

TARGET = image.out

BIN_TARGET = $(DIR_BIN)/$(TARGET)

CC = g++
AR = ar -rcs

CFLAGS = -g -Wall -I$(DIR_INC)
LFLAGS = -L./lib -leasybmp

$(BIN_TARGET):$(OBJ)
	$(CC) -o $@ $(OBJ) $(LFLAGS)

$(DIR_OBJ)/%.o:$(DIR_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_OBJ)/%.o:$(DIR_SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
    
clean:
	rm -rf $(OBJ)
	rm -rf $(DIR_BIN)/$(BIN_TARGET)