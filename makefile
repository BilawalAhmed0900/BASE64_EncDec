CC = gcc
CFLAGS = -Wall -Werror -Ofast
MACRO_NAME = LIBRARY_EXPORT

SRC_DIR = ./src
EXMPL_DIR = ./examples
OBJ_DIR = ./objects
BIN_DIR = ./binary

SRC_OBJ = $(OBJ_DIR)/base64_encdec.o
EXMPL_OBJ = $(OBJ_DIR)/example.o

SRC_SBIN = $(BIN_DIR)/base64_encdec.$(SBIN_EXT)
EXMPL_BIN = $(BIN_DIR)/example

ifeq ($(OS),Windows_NT)
	SBIN_EXT = dll
	RM = del
	SUBST_TO =\\
	SUBST_FROM =/
	BINEXT = .exe
	FPIC =
	MOVE = move
    SBIN = $(SRC_SBIN)
else
	SBIN_EXT = so
	RM = rm
	SUBST_TO =/
	SUBST_FROM =\\
	BINEXT =
	FPIC = -fPIC
	MOVE = mv
    SBIN =
endif

all: $(SRC_SBIN) $(EXMPL_BIN)
main: $(SRC_SBIN)
example: $(EXMPL_BIN)
.PHONY: clean 

$(SRC_SBIN): $(SRC_OBJ)
	$(CC) -shared $(CFLAGS) -D $(MACRO_NAME) -o $@ $^

$(SRC_OBJ): $(SRC_DIR)/base64_encdec.c
	$(CC) $(CFLAGS) $(FPIC) -D $(MACRO_NAME) -o $@ -c $^

$(EXMPL_BIN): $(EXMPL_OBJ) $(SRC_OBJ) $(SBIN)
	$(CC) $(CFLAGS) -o $@ $^ 
	

$(EXMPL_OBJ): $(EXMPL_DIR)/example_main.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean: 
	$(RM) $(subst $(SUBST_FROM),$(SUBST_TO),$(SRC_SBIN))
	$(RM) $(subst $(SUBST_FROM),$(SUBST_TO),$(EXMPL_BIN))$(BINEXT)
	$(RM) $(subst $(SUBST_FROM),$(SUBST_TO),$(SRC_OBJ))
	$(RM) $(subst $(SUBST_FROM),$(SUBST_TO),$(EXMPL_OBJ))
