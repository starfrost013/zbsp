# Rewritten 3/15-3/16/2024 because CMake sucks

# Compiler defines
CC = gcc
#default to Linux, override later on Windows
CC_RUN = $(CC) $(CFLAGS_CURRENT) -o $@ -c $<

# Compiler flag defines

CFLAGS = -DGCC -DUSE_PTHREADS

LDFLAGS = -pthread

#todo: non x64 arch
OBJ_DIR = obj_gcc_x64
BIN_DIR = bin_gcc

# OS specific defines
ifeq ($(OS), Windows_NT)
 CFLAGS_DEBUG = $(CFLAGS) -D_DEBUG -DWIN32
 CFLAGS_RELEASE = $(CFLAGS) -O3 -DNDEBUG -DWIN32
 OUT_FILE = zbsp.exe
 DELETE_COMMAND = rmdir /s /q
 # setup compiler
else 
 CFLAGS_DEBUG = $(CFLAGS) -D_DEBUG
 CFLAGS_RELEASE = $(CFLAGS) -O3 -DNDEBUG
 OUT_FILE = zbsp
 DELETE_COMMAND = rm -r
endif

create_debug:
	@-mkdir $(BIN_DIR)
	@-mkdir $(OBJ_DIR)

	$(MAKE) $(BIN_DIR)/$(OUT_FILE) BUILDDIR="$(BIN_DIR)" CFLAGS="$(CFLAGS_DEBUG)"

create_release:
	@-mkdir $(BIN_DIR)
	@-mkdir $(OBJ_DIR)

# set vars here to allow per-cfg build dir
	$(MAKE) $(BIN_DIR)/$(OUT_FILE) BUILDDIR="$(BIN_DIR)" CFLAGS="$(CFLAGS_RELEASE)"

# CLEANUP 
clean:
	@-$(DELETE_COMMAND) $(BIN_DIR)
	@-$(DELETE_COMMAND) $(OBJ_DIR)

#main targets
debug: create_debug
release: create_release

# for LD
ZBSP_OBJECTS = \
	$(OBJ_DIR)/brushbsp.o \
	$(OBJ_DIR)/bsp.o \
	$(OBJ_DIR)/bspfile.o \
	$(OBJ_DIR)/cmdlib.o \
	$(OBJ_DIR)/csg.o \
	$(OBJ_DIR)/data.o \
	$(OBJ_DIR)/faces.o \
	$(OBJ_DIR)/flow.o \
	$(OBJ_DIR)/huffman.o \
	$(OBJ_DIR)/images.o \
	$(OBJ_DIR)/l3dslib.o \
	$(OBJ_DIR)/lbmlib.o \
	$(OBJ_DIR)/leakfile.o \
	$(OBJ_DIR)/lightmap.o \
	$(OBJ_DIR)/llwolib.o \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/map.o \
	$(OBJ_DIR)/mathlib.o \
	$(OBJ_DIR)/mdfour.o \
	$(OBJ_DIR)/models.o \
	$(OBJ_DIR)/patches.o \
	$(OBJ_DIR)/polylib.o \
	$(OBJ_DIR)/portals.o \
	$(OBJ_DIR)/prtfile.o \
	$(OBJ_DIR)/rad.o \
	$(OBJ_DIR)/scriplib.o \
	$(OBJ_DIR)/sprites.o \
	$(OBJ_DIR)/tables.o \
	$(OBJ_DIR)/textures.o \
	$(OBJ_DIR)/threads.o \
	$(OBJ_DIR)/trace.o \
	$(OBJ_DIR)/tree.o \
	$(OBJ_DIR)/trilib.o \
	$(OBJ_DIR)/vis.o \
	$(OBJ_DIR)/writebsp.o

$(BIN_DIR)/$(OUT_FILE) : $(ZBSP_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(ZBSP_OBJECTS) $(LDFLAGS)

$(OBJ_DIR)/brushbsp.o : src/brushbsp.c 
	$(CC_RUN)

$(OBJ_DIR)/bsp.o : src/bsp.c 
	$(CC_RUN)

$(OBJ_DIR)/bspfile.o : src/bspfile.c 
	$(CC_RUN)

$(OBJ_DIR)/cmdlib.o : src/cmdlib.c 
	$(CC_RUN)

$(OBJ_DIR)/csg.o : src/csg.c 
	$(CC_RUN)

$(OBJ_DIR)/data.o : src/data.c 
	$(CC_RUN)

$(OBJ_DIR)/faces.o : src/faces.c 
	$(CC_RUN)

$(OBJ_DIR)/flow.o : src/flow.c 
	$(CC_RUN)

$(OBJ_DIR)/huffman.o : src/huffman.c 
	$(CC_RUN)

$(OBJ_DIR)/images.o : src/images.c 
	$(CC_RUN)

$(OBJ_DIR)/l3dslib.o : src/l3dslib.c 
	$(CC_RUN)

$(OBJ_DIR)/lbmlib.o : src/lbmlib.c 
	$(CC_RUN)

$(OBJ_DIR)/leakfile.o : src/leakfile.c 
	$(CC_RUN)

$(OBJ_DIR)/lightmap.o : src/lightmap.c 
	$(CC_RUN)

$(OBJ_DIR)/llwolib.o : src/llwolib.c 
	$(CC_RUN)

$(OBJ_DIR)/main.o : src/main.c 
	$(CC_RUN)

$(OBJ_DIR)/map.o : src/map.c 
	$(CC_RUN)

$(OBJ_DIR)/mathlib.o : src/mathlib.c 
	$(CC_RUN)

$(OBJ_DIR)/mdfour.o : src/mdfour.c 
	$(CC_RUN)

$(OBJ_DIR)/models.o : src/models.c 
	$(CC_RUN)

$(OBJ_DIR)/patches.o : src/patches.c 
	$(CC_RUN)

$(OBJ_DIR)/polylib.o : src/polylib.c 
	$(CC_RUN)

$(OBJ_DIR)/portals.o : src/portals.c 
	$(CC_RUN)

$(OBJ_DIR)/prtfile.o : src/prtfile.c 
	$(CC_RUN)

$(OBJ_DIR)/rad.o : src/rad.c 
	$(CC_RUN)

$(OBJ_DIR)/scriplib.o : src/scriplib.c 
	$(CC_RUN)

$(OBJ_DIR)/sprites.o : src/sprites.c 
	$(CC_RUN)

$(OBJ_DIR)/tables.o : src/tables.c 
	$(CC_RUN)

$(OBJ_DIR)/textures.o : src/textures.c 
	$(CC_RUN)

$(OBJ_DIR)/threads.o : src/threads.c 
	$(CC_RUN)

$(OBJ_DIR)/trace.o : src/trace.c 
	$(CC_RUN)

$(OBJ_DIR)/tree.o : src/tree.c 
	$(CC_RUN)

$(OBJ_DIR)/trilib.o : src/trilib.c 
	$(CC_RUN)

$(OBJ_DIR)/vis.o : src/vis.c 
	$(CC_RUN)

$(OBJ_DIR)/writebsp.o : src/writebsp.c 
	$(CC_RUN)