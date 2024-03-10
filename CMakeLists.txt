cmake_minimum_required(VERSION 3.11)
project(q2tools-220 LANGUAGES C)

set(CMAKE_STATIC_LIBRARY_PREFIX_C "")

add_library(q2tools-i INTERFACE)


if (WIN32)
    if (MSVC) # horrible visual studio hack
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE /ENTRY:main msvcrt.lib vcruntime.lib ucrt.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib")
    else()
        target_compile_options(q2tools-i INTERFACE -fno-common ${THREADING_OPTION} -DUSE_ZLIB -O3 -s -z stack-size=16777216)
        target_link_libraries(q2tools-i INTERFACE m ${THREADING_LIB})
        set(Q2T_USE_PTHREADS ON CACHE BOOL "compile with pthread support")

        if(Q2T_USE_PTHREADS)
            set(THREADING_OPTION -pthread -DUSE_PTHREADS)
	        set(THREADING_LIB pthread)
        else()
            set(THREADING_OPTION -DUSE_SETRLIMIT)
	        set(THREADING_LIB)
        endif()
    endif()
endif()

add_library(libq2tool STATIC
    src/bspfile.c
    src/cmdlib.c
    src/l3dslib.c
    src/lbmlib.c
    src/llwolib.c
    src/mathlib.c
    src/mdfour.c
    src/polylib.c
    src/scriplib.c
    src/threads.c
    src/trilib.c
    src/bsp.c
    src/brushbsp.c
    src/csg.c
    src/faces.c
    src/leakfile.c
    src/map.c
    src/portals.c
    src/prtfile.c
    src/textures.c
    src/tree.c
    src/writebsp.c

    src/vis.c
    src/flow.c

    src/rad.c
    src/lightmap.c
    src/patches.c
    src/trace.c

    src/data.c
    src/images.c
    src/models.c
    src/sprites.c
    src/tables.c
    src/video.c
)
target_include_directories(libq2tool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common)

add_executable(q2tool src/main.c)

if(WIN32 AND MSVC)
    target_link_libraries(q2tool q2tools-i libq2tool)
else()
    target_link_libraries(q2tool q2tools-i libq2tool)
endif()

install(TARGETS q2tool)

enable_testing()
add_test(NAME q2tool COMMAND q2tool -bsp)