DIR_INC = .
DIR_SRC = .
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.cpp)
OBJ = $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(notdir $(SRC)))

TARGET = libeasybmp.a

BIN_TARGET = $(DIR_BIN)/$(TARGET)

CC = g++
AR = ar -rcs
MKDIR = mkdir

CFLAGS = -g -Wall -I$(DIR_INC)

$(BIN_TARGET):$(OBJ) $(DIR_BIN)
	$(AR) $@ $(OBJ)

$(DIR_OBJ)/%.o:$(DIR_SRC)/%.cpp $(DIR_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_OBJ):
	$(MKDIR) $(DIR_OBJ)

$(DIR_BIN):
	$(MKDIR) $(DIR_BIN)

.PHONY:clean
    
clean:
	rm -rf $(OBJ)
	rm -rf $(DIR_BIN)/$(BIN_TARGET)
