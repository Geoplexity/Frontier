BIN_DIR=bin
LIB_DIR=lib
OBJ_DIR=obj
SRC_DIR=src

CC=g++
AR=ar r


# GDS
MODULE = GDS
_DEPS  = defs.h List.hpp Random.hpp Vertex.hpp Edge.hpp Graph.hpp Cluster.hpp
_OBJ   = Vertex.o Edge.o Graph.o Cluster.o

INC_DIR = $(SRC_DIR)/$(MODULE)
DEPS    = $(patsubst %,$(INC_DIR)/%,$(_DEPS))
OBJ     = $(patsubst %,$(OBJ_DIR)/$(MODULE)_%,$(_OBJ))

CFLAGS=-I$(INC_DIR)
LIBS=

$(OBJ_DIR)/$(MODULE)_%.o: $(INC_DIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

$(LIB_DIR)/$(MODULE).a: $(OBJ)
	$(AR) $@ $^




# # binary
# EXEC=test
# INC_DIRS = $(SRC_DIR)/MFA

# CFLAGS=-I$(INC_DIR)
# _LIBS = MFA
# LIBS  = $(patsubst %,$(LIB_DIR)/%.a,$(_LIBS))

# exec:
# 	$(CC) -o $(BIN_DIR)/$(EXEC) $(SRC_DIR)/test/test.cpp $(CFLAGS) $(LIBS)





.PHONY: clean

clean:
	rm -f $(BIN_DIR)/* $(LIB_DIR)/* $(OBJ_DIR)/*
