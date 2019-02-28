BIN_DIR=bin
LIB_DIR=lib
OBJ_DIR=obj
SRC_DIR=src


# MODULES=

all: GDS DRP

GDS:
	$(MAKE) -C $(SRC_DIR)/GDS

DRP:
	$(MAKE) -C $(SRC_DIR)/DRP






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
