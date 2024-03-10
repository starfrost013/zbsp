

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\mario\Zombono\tools\zbsp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\mario\Zombono\tools\zbsp

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f obj\Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f obj\Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f obj\Makefile2 preinstall
.PHONY : preinstall/fast

#=============================================================================
# Target rules for targets named libq2tool

# Build rule for target.
libq2tool: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f obj\Makefile2 libq2tool
.PHONY : libq2tool

# fast build rule for target.
libq2tool/fast:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/build
.PHONY : libq2tool/fast

#=============================================================================
# Target rules for targets named q2tool

# fast build rule for target.
q2tool/fast:
	$(MAKE) $(MAKESILENT) -f obj\q2tool.dir\build.make obj/q2tool.dir/build
.PHONY : q2tool/fast

src/brushbsp.obj: src/brushbsp.c.obj
.PHONY : src/brushbsp.obj

# target to build an object file
src/brushbsp.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/brushbsp.c.obj
.PHONY : src/brushbsp.c.obj

src/brushbsp.i: src/brushbsp.c.i
.PHONY : src/brushbsp.i

# target to preprocess a source file
src/brushbsp.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/brushbsp.c.i
.PHONY : src/brushbsp.c.i

src/brushbsp.s: src/brushbsp.c.s
.PHONY : src/brushbsp.s

# target to generate assembly for a file
src/brushbsp.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/brushbsp.c.s
.PHONY : src/brushbsp.c.s

src/bsp.obj: src/bsp.c.obj
.PHONY : src/bsp.obj

# target to build an object file
src/bsp.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/bsp.c.obj
.PHONY : src/bsp.c.obj

src/bsp.i: src/bsp.c.i
.PHONY : src/bsp.i

# target to preprocess a source file
src/bsp.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/bsp.c.i
.PHONY : src/bsp.c.i

src/bsp.s: src/bsp.c.s
.PHONY : src/bsp.s

# target to generate assembly for a file
src/bsp.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/bsp.c.s
.PHONY : src/bsp.c.s

src/bspfile.obj: src/bspfile.c.obj
.PHONY : src/bspfile.obj

# target to build an object file
src/bspfile.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/bspfile.c.obj
.PHONY : src/bspfile.c.obj

src/bspfile.i: src/bspfile.c.i
.PHONY : src/bspfile.i

# target to preprocess a source file
src/bspfile.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/bspfile.c.i
.PHONY : src/bspfile.c.i

src/bspfile.s: src/bspfile.c.s
.PHONY : src/bspfile.s

# target to generate assembly for a file
src/bspfile.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/bspfile.c.s
.PHONY : src/bspfile.c.s

src/cmdlib.obj: src/cmdlib.c.obj
.PHONY : src/cmdlib.obj

# target to build an object file
src/cmdlib.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/cmdlib.c.obj
.PHONY : src/cmdlib.c.obj

src/cmdlib.i: src/cmdlib.c.i
.PHONY : src/cmdlib.i

# target to preprocess a source file
src/cmdlib.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/cmdlib.c.i
.PHONY : src/cmdlib.c.i

src/cmdlib.s: src/cmdlib.c.s
.PHONY : src/cmdlib.s

# target to generate assembly for a file
src/cmdlib.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/cmdlib.c.s
.PHONY : src/cmdlib.c.s

src/csg.obj: src/csg.c.obj
.PHONY : src/csg.obj

# target to build an object file
src/csg.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/csg.c.obj
.PHONY : src/csg.c.obj

src/csg.i: src/csg.c.i
.PHONY : src/csg.i

# target to preprocess a source file
src/csg.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/csg.c.i
.PHONY : src/csg.c.i

src/csg.s: src/csg.c.s
.PHONY : src/csg.s

# target to generate assembly for a file
src/csg.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/csg.c.s
.PHONY : src/csg.c.s

src/data.obj: src/data.c.obj
.PHONY : src/data.obj

# target to build an object file
src/data.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/data.c.obj
.PHONY : src/data.c.obj

src/data.i: src/data.c.i
.PHONY : src/data.i

# target to preprocess a source file
src/data.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/data.c.i
.PHONY : src/data.c.i

src/data.s: src/data.c.s
.PHONY : src/data.s

# target to generate assembly for a file
src/data.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/data.c.s
.PHONY : src/data.c.s

src/faces.obj: src/faces.c.obj
.PHONY : src/faces.obj

# target to build an object file
src/faces.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/faces.c.obj
.PHONY : src/faces.c.obj

src/faces.i: src/faces.c.i
.PHONY : src/faces.i

# target to preprocess a source file
src/faces.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/faces.c.i
.PHONY : src/faces.c.i

src/faces.s: src/faces.c.s
.PHONY : src/faces.s

# target to generate assembly for a file
src/faces.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/faces.c.s
.PHONY : src/faces.c.s

src/flow.obj: src/flow.c.obj
.PHONY : src/flow.obj

# target to build an object file
src/flow.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/flow.c.obj
.PHONY : src/flow.c.obj

src/flow.i: src/flow.c.i
.PHONY : src/flow.i

# target to preprocess a source file
src/flow.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/flow.c.i
.PHONY : src/flow.c.i

src/flow.s: src/flow.c.s
.PHONY : src/flow.s

# target to generate assembly for a file
src/flow.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/flow.c.s
.PHONY : src/flow.c.s

src/images.obj: src/images.c.obj
.PHONY : src/images.obj

# target to build an object file
src/images.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/images.c.obj
.PHONY : src/images.c.obj

src/images.i: src/images.c.i
.PHONY : src/images.i

# target to preprocess a source file
src/images.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/images.c.i
.PHONY : src/images.c.i

src/images.s: src/images.c.s
.PHONY : src/images.s

# target to generate assembly for a file
src/images.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/images.c.s
.PHONY : src/images.c.s

src/l3dslib.obj: src/l3dslib.c.obj
.PHONY : src/l3dslib.obj

# target to build an object file
src/l3dslib.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/l3dslib.c.obj
.PHONY : src/l3dslib.c.obj

src/l3dslib.i: src/l3dslib.c.i
.PHONY : src/l3dslib.i

# target to preprocess a source file
src/l3dslib.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/l3dslib.c.i
.PHONY : src/l3dslib.c.i

src/l3dslib.s: src/l3dslib.c.s
.PHONY : src/l3dslib.s

# target to generate assembly for a file
src/l3dslib.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/l3dslib.c.s
.PHONY : src/l3dslib.c.s

src/lbmlib.obj: src/lbmlib.c.obj
.PHONY : src/lbmlib.obj

# target to build an object file
src/lbmlib.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/lbmlib.c.obj
.PHONY : src/lbmlib.c.obj

src/lbmlib.i: src/lbmlib.c.i
.PHONY : src/lbmlib.i

# target to preprocess a source file
src/lbmlib.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/lbmlib.c.i
.PHONY : src/lbmlib.c.i

src/lbmlib.s: src/lbmlib.c.s
.PHONY : src/lbmlib.s

# target to generate assembly for a file
src/lbmlib.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/lbmlib.c.s
.PHONY : src/lbmlib.c.s

src/leakfile.obj: src/leakfile.c.obj
.PHONY : src/leakfile.obj

# target to build an object file
src/leakfile.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/leakfile.c.obj
.PHONY : src/leakfile.c.obj

src/leakfile.i: src/leakfile.c.i
.PHONY : src/leakfile.i

# target to preprocess a source file
src/leakfile.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/leakfile.c.i
.PHONY : src/leakfile.c.i

src/leakfile.s: src/leakfile.c.s
.PHONY : src/leakfile.s

# target to generate assembly for a file
src/leakfile.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/leakfile.c.s
.PHONY : src/leakfile.c.s

src/lightmap.obj: src/lightmap.c.obj
.PHONY : src/lightmap.obj

# target to build an object file
src/lightmap.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/lightmap.c.obj
.PHONY : src/lightmap.c.obj

src/lightmap.i: src/lightmap.c.i
.PHONY : src/lightmap.i

# target to preprocess a source file
src/lightmap.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/lightmap.c.i
.PHONY : src/lightmap.c.i

src/lightmap.s: src/lightmap.c.s
.PHONY : src/lightmap.s

# target to generate assembly for a file
src/lightmap.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/lightmap.c.s
.PHONY : src/lightmap.c.s

src/llwolib.obj: src/llwolib.c.obj
.PHONY : src/llwolib.obj

# target to build an object file
src/llwolib.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/llwolib.c.obj
.PHONY : src/llwolib.c.obj

src/llwolib.i: src/llwolib.c.i
.PHONY : src/llwolib.i

# target to preprocess a source file
src/llwolib.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/llwolib.c.i
.PHONY : src/llwolib.c.i

src/llwolib.s: src/llwolib.c.s
.PHONY : src/llwolib.s

# target to generate assembly for a file
src/llwolib.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/llwolib.c.s
.PHONY : src/llwolib.c.s

src/main.obj: src/main.c.obj
.PHONY : src/main.obj

# target to build an object file
src/main.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\q2tool.dir\build.make obj/q2tool.dir/src/main.c.obj
.PHONY : src/main.c.obj

src/main.i: src/main.c.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.c.i:
	$(MAKE) $(MAKESILENT) -f obj\q2tool.dir\build.make obj/q2tool.dir/src/main.c.i
.PHONY : src/main.c.i

src/main.s: src/main.c.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.c.s:
	$(MAKE) $(MAKESILENT) -f obj\q2tool.dir\build.make obj/q2tool.dir/src/main.c.s
.PHONY : src/main.c.s

src/map.obj: src/map.c.obj
.PHONY : src/map.obj

# target to build an object file
src/map.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/map.c.obj
.PHONY : src/map.c.obj

src/map.i: src/map.c.i
.PHONY : src/map.i

# target to preprocess a source file
src/map.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/map.c.i
.PHONY : src/map.c.i

src/map.s: src/map.c.s
.PHONY : src/map.s

# target to generate assembly for a file
src/map.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/map.c.s
.PHONY : src/map.c.s

src/mathlib.obj: src/mathlib.c.obj
.PHONY : src/mathlib.obj

# target to build an object file
src/mathlib.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/mathlib.c.obj
.PHONY : src/mathlib.c.obj

src/mathlib.i: src/mathlib.c.i
.PHONY : src/mathlib.i

# target to preprocess a source file
src/mathlib.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/mathlib.c.i
.PHONY : src/mathlib.c.i

src/mathlib.s: src/mathlib.c.s
.PHONY : src/mathlib.s

# target to generate assembly for a file
src/mathlib.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/mathlib.c.s
.PHONY : src/mathlib.c.s

src/mdfour.obj: src/mdfour.c.obj
.PHONY : src/mdfour.obj

# target to build an object file
src/mdfour.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/mdfour.c.obj
.PHONY : src/mdfour.c.obj

src/mdfour.i: src/mdfour.c.i
.PHONY : src/mdfour.i

# target to preprocess a source file
src/mdfour.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/mdfour.c.i
.PHONY : src/mdfour.c.i

src/mdfour.s: src/mdfour.c.s
.PHONY : src/mdfour.s

# target to generate assembly for a file
src/mdfour.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/mdfour.c.s
.PHONY : src/mdfour.c.s

src/models.obj: src/models.c.obj
.PHONY : src/models.obj

# target to build an object file
src/models.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/models.c.obj
.PHONY : src/models.c.obj

src/models.i: src/models.c.i
.PHONY : src/models.i

# target to preprocess a source file
src/models.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/models.c.i
.PHONY : src/models.c.i

src/models.s: src/models.c.s
.PHONY : src/models.s

# target to generate assembly for a file
src/models.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/models.c.s
.PHONY : src/models.c.s

src/patches.obj: src/patches.c.obj
.PHONY : src/patches.obj

# target to build an object file
src/patches.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/patches.c.obj
.PHONY : src/patches.c.obj

src/patches.i: src/patches.c.i
.PHONY : src/patches.i

# target to preprocess a source file
src/patches.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/patches.c.i
.PHONY : src/patches.c.i

src/patches.s: src/patches.c.s
.PHONY : src/patches.s

# target to generate assembly for a file
src/patches.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/patches.c.s
.PHONY : src/patches.c.s

src/polylib.obj: src/polylib.c.obj
.PHONY : src/polylib.obj

# target to build an object file
src/polylib.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/polylib.c.obj
.PHONY : src/polylib.c.obj

src/polylib.i: src/polylib.c.i
.PHONY : src/polylib.i

# target to preprocess a source file
src/polylib.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/polylib.c.i
.PHONY : src/polylib.c.i

src/polylib.s: src/polylib.c.s
.PHONY : src/polylib.s

# target to generate assembly for a file
src/polylib.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/polylib.c.s
.PHONY : src/polylib.c.s

src/portals.obj: src/portals.c.obj
.PHONY : src/portals.obj

# target to build an object file
src/portals.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/portals.c.obj
.PHONY : src/portals.c.obj

src/portals.i: src/portals.c.i
.PHONY : src/portals.i

# target to preprocess a source file
src/portals.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/portals.c.i
.PHONY : src/portals.c.i

src/portals.s: src/portals.c.s
.PHONY : src/portals.s

# target to generate assembly for a file
src/portals.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/portals.c.s
.PHONY : src/portals.c.s

src/prtfile.obj: src/prtfile.c.obj
.PHONY : src/prtfile.obj

# target to build an object file
src/prtfile.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/prtfile.c.obj
.PHONY : src/prtfile.c.obj

src/prtfile.i: src/prtfile.c.i
.PHONY : src/prtfile.i

# target to preprocess a source file
src/prtfile.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/prtfile.c.i
.PHONY : src/prtfile.c.i

src/prtfile.s: src/prtfile.c.s
.PHONY : src/prtfile.s

# target to generate assembly for a file
src/prtfile.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/prtfile.c.s
.PHONY : src/prtfile.c.s

src/rad.obj: src/rad.c.obj
.PHONY : src/rad.obj

# target to build an object file
src/rad.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/rad.c.obj
.PHONY : src/rad.c.obj

src/rad.i: src/rad.c.i
.PHONY : src/rad.i

# target to preprocess a source file
src/rad.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/rad.c.i
.PHONY : src/rad.c.i

src/rad.s: src/rad.c.s
.PHONY : src/rad.s

# target to generate assembly for a file
src/rad.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/rad.c.s
.PHONY : src/rad.c.s

src/scriplib.obj: src/scriplib.c.obj
.PHONY : src/scriplib.obj

# target to build an object file
src/scriplib.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/scriplib.c.obj
.PHONY : src/scriplib.c.obj

src/scriplib.i: src/scriplib.c.i
.PHONY : src/scriplib.i

# target to preprocess a source file
src/scriplib.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/scriplib.c.i
.PHONY : src/scriplib.c.i

src/scriplib.s: src/scriplib.c.s
.PHONY : src/scriplib.s

# target to generate assembly for a file
src/scriplib.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/scriplib.c.s
.PHONY : src/scriplib.c.s

src/sprites.obj: src/sprites.c.obj
.PHONY : src/sprites.obj

# target to build an object file
src/sprites.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/sprites.c.obj
.PHONY : src/sprites.c.obj

src/sprites.i: src/sprites.c.i
.PHONY : src/sprites.i

# target to preprocess a source file
src/sprites.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/sprites.c.i
.PHONY : src/sprites.c.i

src/sprites.s: src/sprites.c.s
.PHONY : src/sprites.s

# target to generate assembly for a file
src/sprites.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/sprites.c.s
.PHONY : src/sprites.c.s

src/tables.obj: src/tables.c.obj
.PHONY : src/tables.obj

# target to build an object file
src/tables.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/tables.c.obj
.PHONY : src/tables.c.obj

src/tables.i: src/tables.c.i
.PHONY : src/tables.i

# target to preprocess a source file
src/tables.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/tables.c.i
.PHONY : src/tables.c.i

src/tables.s: src/tables.c.s
.PHONY : src/tables.s

# target to generate assembly for a file
src/tables.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/tables.c.s
.PHONY : src/tables.c.s

src/textures.obj: src/textures.c.obj
.PHONY : src/textures.obj

# target to build an object file
src/textures.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/textures.c.obj
.PHONY : src/textures.c.obj

src/textures.i: src/textures.c.i
.PHONY : src/textures.i

# target to preprocess a source file
src/textures.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/textures.c.i
.PHONY : src/textures.c.i

src/textures.s: src/textures.c.s
.PHONY : src/textures.s

# target to generate assembly for a file
src/textures.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/textures.c.s
.PHONY : src/textures.c.s

src/threads.obj: src/threads.c.obj
.PHONY : src/threads.obj

# target to build an object file
src/threads.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/threads.c.obj
.PHONY : src/threads.c.obj

src/threads.i: src/threads.c.i
.PHONY : src/threads.i

# target to preprocess a source file
src/threads.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/threads.c.i
.PHONY : src/threads.c.i

src/threads.s: src/threads.c.s
.PHONY : src/threads.s

# target to generate assembly for a file
src/threads.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/threads.c.s
.PHONY : src/threads.c.s

src/trace.obj: src/trace.c.obj
.PHONY : src/trace.obj

# target to build an object file
src/trace.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/trace.c.obj
.PHONY : src/trace.c.obj

src/trace.i: src/trace.c.i
.PHONY : src/trace.i

# target to preprocess a source file
src/trace.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/trace.c.i
.PHONY : src/trace.c.i

src/trace.s: src/trace.c.s
.PHONY : src/trace.s

# target to generate assembly for a file
src/trace.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/trace.c.s
.PHONY : src/trace.c.s

src/tree.obj: src/tree.c.obj
.PHONY : src/tree.obj

# target to build an object file
src/tree.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/tree.c.obj
.PHONY : src/tree.c.obj

src/tree.i: src/tree.c.i
.PHONY : src/tree.i

# target to preprocess a source file
src/tree.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/tree.c.i
.PHONY : src/tree.c.i

src/tree.s: src/tree.c.s
.PHONY : src/tree.s

# target to generate assembly for a file
src/tree.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/tree.c.s
.PHONY : src/tree.c.s

src/trilib.obj: src/trilib.c.obj
.PHONY : src/trilib.obj

# target to build an object file
src/trilib.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/trilib.c.obj
.PHONY : src/trilib.c.obj

src/trilib.i: src/trilib.c.i
.PHONY : src/trilib.i

# target to preprocess a source file
src/trilib.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/trilib.c.i
.PHONY : src/trilib.c.i

src/trilib.s: src/trilib.c.s
.PHONY : src/trilib.s

# target to generate assembly for a file
src/trilib.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/trilib.c.s
.PHONY : src/trilib.c.s

src/video.obj: src/video.c.obj
.PHONY : src/video.obj

# target to build an object file
src/video.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/video.c.obj
.PHONY : src/video.c.obj

src/video.i: src/video.c.i
.PHONY : src/video.i

# target to preprocess a source file
src/video.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/video.c.i
.PHONY : src/video.c.i

src/video.s: src/video.c.s
.PHONY : src/video.s

# target to generate assembly for a file
src/video.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/video.c.s
.PHONY : src/video.c.s

src/vis.obj: src/vis.c.obj
.PHONY : src/vis.obj

# target to build an object file
src/vis.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/vis.c.obj
.PHONY : src/vis.c.obj

src/vis.i: src/vis.c.i
.PHONY : src/vis.i

# target to preprocess a source file
src/vis.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/vis.c.i
.PHONY : src/vis.c.i

src/vis.s: src/vis.c.s
.PHONY : src/vis.s

# target to generate assembly for a file
src/vis.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/vis.c.s
.PHONY : src/vis.c.s

src/writebsp.obj: src/writebsp.c.obj
.PHONY : src/writebsp.obj

# target to build an object file
src/writebsp.c.obj:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/writebsp.c.obj
.PHONY : src/writebsp.c.obj

src/writebsp.i: src/writebsp.c.i
.PHONY : src/writebsp.i

# target to preprocess a source file
src/writebsp.c.i:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/writebsp.c.i
.PHONY : src/writebsp.c.i

src/writebsp.s: src/writebsp.c.s
.PHONY : src/writebsp.s

# target to generate assembly for a file
src/writebsp.c.s:
	$(MAKE) $(MAKESILENT) -f obj\libq2tool.dir\build.make obj/libq2tool.dir/src/writebsp.c.s
.PHONY : src/writebsp.c.s

# Help Target
help:
	@echo The following are some of the valid targets for this Makefile:
	@echo ... all (the default if no target is provided)
	@echo ... clean
	@echo ... depend
	@echo ... edit_cache
	@echo ... install
	@echo ... install/local
	@echo ... install/strip
	@echo ... list_install_components
	@echo ... rebuild_cache
	@echo ... test
	@echo ... libq2tool
	@echo ... q2tool
	@echo ... src/brushbsp.obj
	@echo ... src/brushbsp.i
	@echo ... src/brushbsp.s
	@echo ... src/bsp.obj
	@echo ... src/bsp.i
	@echo ... src/bsp.s
	@echo ... src/bspfile.obj
	@echo ... src/bspfile.i
	@echo ... src/bspfile.s
	@echo ... src/cmdlib.obj
	@echo ... src/cmdlib.i
	@echo ... src/cmdlib.s
	@echo ... src/csg.obj
	@echo ... src/csg.i
	@echo ... src/csg.s
	@echo ... src/data.obj
	@echo ... src/data.i
	@echo ... src/data.s
	@echo ... src/faces.obj
	@echo ... src/faces.i
	@echo ... src/faces.s
	@echo ... src/flow.obj
	@echo ... src/flow.i
	@echo ... src/flow.s
	@echo ... src/images.obj
	@echo ... src/images.i
	@echo ... src/images.s
	@echo ... src/l3dslib.obj
	@echo ... src/l3dslib.i
	@echo ... src/l3dslib.s
	@echo ... src/lbmlib.obj
	@echo ... src/lbmlib.i
	@echo ... src/lbmlib.s
	@echo ... src/leakfile.obj
	@echo ... src/leakfile.i
	@echo ... src/leakfile.s
	@echo ... src/lightmap.obj
	@echo ... src/lightmap.i
	@echo ... src/lightmap.s
	@echo ... src/llwolib.obj
	@echo ... src/llwolib.i
	@echo ... src/llwolib.s
	@echo ... src/main.obj
	@echo ... src/main.i
	@echo ... src/main.s
	@echo ... src/map.obj
	@echo ... src/map.i
	@echo ... src/map.s
	@echo ... src/mathlib.obj
	@echo ... src/mathlib.i
	@echo ... src/mathlib.s
	@echo ... src/mdfour.obj
	@echo ... src/mdfour.i
	@echo ... src/mdfour.s
	@echo ... src/models.obj
	@echo ... src/models.i
	@echo ... src/models.s
	@echo ... src/patches.obj
	@echo ... src/patches.i
	@echo ... src/patches.s
	@echo ... src/polylib.obj
	@echo ... src/polylib.i
	@echo ... src/polylib.s
	@echo ... src/portals.obj
	@echo ... src/portals.i
	@echo ... src/portals.s
	@echo ... src/prtfile.obj
	@echo ... src/prtfile.i
	@echo ... src/prtfile.s
	@echo ... src/rad.obj
	@echo ... src/rad.i
	@echo ... src/rad.s
	@echo ... src/scriplib.obj
	@echo ... src/scriplib.i
	@echo ... src/scriplib.s
	@echo ... src/sprites.obj
	@echo ... src/sprites.i
	@echo ... src/sprites.s
	@echo ... src/tables.obj
	@echo ... src/tables.i
	@echo ... src/tables.s
	@echo ... src/textures.obj
	@echo ... src/textures.i
	@echo ... src/textures.s
	@echo ... src/threads.obj
	@echo ... src/threads.i
	@echo ... src/threads.s
	@echo ... src/trace.obj
	@echo ... src/trace.i
	@echo ... src/trace.s
	@echo ... src/tree.obj
	@echo ... src/tree.i
	@echo ... src/tree.s
	@echo ... src/trilib.obj
	@echo ... src/trilib.i
	@echo ... src/trilib.s
	@echo ... src/video.obj
	@echo ... src/video.i
	@echo ... src/video.s
	@echo ... src/vis.obj
	@echo ... src/vis.i
	@echo ... src/vis.s
	@echo ... src/writebsp.obj
	@echo ... src/writebsp.i
	@echo ... src/writebsp.s
.PHONY : help