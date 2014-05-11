BIN_DIR=../../bin
LIB_DIR=../../lib
OBJ_DIR=../../obj
SRC_DIR=..

CC=g++
AR=ar r


# GDS
MODULE = DRP
_DEPS  = Cluster_Print.hpp Group.hpp Cluster_Get.hpp Dist_Edge_Flow_Method.hpp \
	Dist_Methods.hpp Dense_Algo.hpp
_OBJ   = Group.o Cluster_Get.o Dist_Edge_Flow_Method.o \
	Dist_Methods.o Dense_Algo.o

INC_DIR = $(SRC_DIR)/$(MODULE)
DEPS    = $(patsubst %,$(INC_DIR)/%,$(_DEPS))
OBJ     = $(patsubst %,$(OBJ_DIR)/$(MODULE)_%,$(_OBJ))

CFLAGS=-I$(INC_DIR)
LIBS=

$(OBJ_DIR)/$(MODULE)_%.o: $(INC_DIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

$(LIB_DIR)/$(MODULE).a: $(OBJ)
	$(AR) $@ $^






.PHONY: clean

clean:
	rm -f $(BIN_DIR)/* $(LIB_DIR)/* $(OBJ_DIR)/*
