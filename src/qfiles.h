/*
===========================================================================
Copyright (C) 1997-2006 Id Software, Inc.
Copyright (C) 2024 starfrost

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
===========================================================================
*/

//
// qfiles.h: file formats
// This file must be identical in the engine and utils directories
//

/*
========================================================================

The .pak files are just a linear collapse of a directory tree

========================================================================
*/
#pragma once

#define IDPAKHEADER (('K' << 24) + ('C' << 16) + ('A' << 8) + 'P')

typedef struct
{
    char name[56];
    int32_t filepos, filelen;
} dpackfile_t;

typedef struct
{
    int32_t ident; // == IDPAKHEADER
    int32_t dirofs;
    int32_t dirlen;
} dpackheader_t;

#define MAX_FILES_IN_PACK 4096

/*
========================================================================

.MD2 triangle model file format

========================================================================
*/

#define IDALIASHEADER (('2' << 24) + ('P' << 16) + ('D' << 8) + 'I')
#define ALIAS_VERSION 8

#define MAX_TRIANGLES 4096
#define MAX_VERTS     2048
#define MAX_FRAMES    512
#define MAX_MD2SKINS  32
#define MAX_SKINNAME  64

typedef struct
{
    short s;
    short t;
} dstvert_t;

typedef struct
{
    short index_xyz[3];
    short index_st[3];
} dtriangle_t;

typedef struct
{
    byte v[3]; // scaled byte to fit in frame mins/maxs
    byte lightnormalindex;
} dtrivertx_t;

#define DTRIVERTX_V0   0
#define DTRIVERTX_V1   1
#define DTRIVERTX_V2   2
#define DTRIVERTX_LNI  3
#define DTRIVERTX_SIZE 4

typedef struct
{
    float scale[3];       // multiply byte verts by this
    float translate[3];   // then add this
    char name[16];        // frame name from grabbing
    dtrivertx_t verts[1]; // variable sized
} daliasframe_t;

// the glcmd format:
// a positive integer starts a tristrip command, followed by that many
// vertex structures.
// a negative integer starts a trifan command, followed by -x vertexes
// a zero indicates the end of the command list.
// a vertex consists of a floating point s, a floating point t,
// and an integer vertex index.

typedef struct
{
    int32_t ident;
    int32_t version;

    int32_t skinwidth;
    int32_t skinheight;
    int32_t framesize; // byte size of each frame

    int32_t num_skins;
    int32_t num_xyz;
    int32_t num_st; // greater than num_xyz for seams
    int32_t num_tris;
    int32_t num_glcmds; // dwords in strip/fan command list
    int32_t num_frames;

    int32_t ofs_skins;  // each skin is a MAX_SKINNAME string
    int32_t ofs_st;     // byte offset from start for stverts
    int32_t ofs_tris;   // offset for dtriangles
    int32_t ofs_frames; // offset for first frame
    int32_t ofs_glcmds;
    int32_t ofs_end; // end of file

} dmdl_t;

/*
========================================================================

.SP2 sprite file format

========================================================================
*/

#define IDSPRITEHEADER (('2' << 24) + ('S' << 16) + ('D' << 8) + 'I')
// little-endian "IDS2"
#define SPRITE_VERSION 2

typedef struct
{
    int32_t width, height;
    int32_t origin_x, origin_y; // raster coordinates inside pic
    char name[MAX_SKINNAME];    // name of pcx file
} dsprframe_t;

typedef struct {
    int32_t ident;
    int32_t version;
    int32_t numframes;
    dsprframe_t frames[1]; // variable sized
} dsprite_t;

/*
==============================================================================

  .BSP file format

==============================================================================
*/

// little-endian 'ZBSP'
#define ZBSP_HEADER               ('Z' | ('B' << 8) | ('S' << 16) | ('P' << 24))

#define ZBSP_VERSION              1

// upper design bounds
// leaffaces, leafbrushes, planes, and verts are still bounded by
// 
// qb: map bounds are +/- MAX
#define DEFAULT_MAP_SIZE         MAX_MAP_SIZE
#define MAX_MAP_SIZE             32768
#define MAX_BLOCK_SIZE           8192
#define MAX_POINTS_HASH          MAX_MAP_SIZE / 64

// Zombono BSP limits
#define MAX_MAP_AREAS           256
#define MAX_MAP_AREAPORTALS     1024
#define WARN_MAP_MODELS         32768
#define MAX_MAP_MODELS          131072
#define MAX_MAP_BRUSHES         1048576
#define WARN_MAP_ENTITIES       32768
#define MAX_MAP_ENTITIES        131072
#define MAX_MAP_ENTSTRING       13631488
#define MAX_MAP_TEXINFO         1048576
#define MAX_MAP_PLANES          1048576
#define MAX_MAP_NODES           1048576
#define MAX_MAP_LEAFS           1048576
#define MAX_MAP_VERTS           4194304
#define MAX_MAP_FACES           1048576
#define MAX_MAP_LEAFFACES       1048576
#define MAX_MAP_LEAFBRUSHES     1048576
#define MAX_MAP_EDGES           1048576
#define MAX_MAP_BRUSHSIDES      4194304
#define MAX_MAP_PORTALS         1048576
#define MAX_MAP_SURFEDGES       4194304
#define MAX_MAP_LIGHTING        54525952 // 0x3400000
#define MAX_MAP_VISIBILITY      0x8000000

// key / value pair sizes

#define MAX_KEY                  32
#define MAX_VALUE                1024

//=============================================================================

typedef struct
{
    int32_t fileofs, filelen;
} lump_t;

#define LUMP_ENTITIES    0
#define LUMP_PLANES      1
#define LUMP_VERTEXES    2
#define LUMP_VISIBILITY  3
#define LUMP_NODES       4
#define LUMP_TEXINFO     5
#define LUMP_FACES       6
#define LUMP_LIGHTING    7
#define LUMP_LEAFS       8
#define LUMP_LEAFFACES   9
#define LUMP_LEAFBRUSHES 10
#define LUMP_EDGES       11
#define LUMP_SURFEDGES   12
#define LUMP_MODELS      13
#define LUMP_BRUSHES     14
#define LUMP_BRUSHSIDES  15
#define LUMP_AREAS       16
#define LUMP_AREAPORTALS 17
#define HEADER_LUMPS     18

typedef struct
{
    int32_t ident;
    int32_t version;
    lump_t lumps[HEADER_LUMPS];
} dheader_t;

typedef struct
{
    float mins[3], maxs[3];
    float origin[3]; // for sounds or lights
    int32_t headnode;
    int32_t firstface, numfaces; // submodels just draw faces
                                 // without walking the bsp tree
} dmodel_t;

typedef struct
{
    float point[3];
} dvertex_t;

// 0-2 are axial planes
#define PLANE_X    0
#define PLANE_Y    1
#define PLANE_Z    2

// 3-5 are non-axial planes snapped to the nearest
#define PLANE_ANYX 3
#define PLANE_ANYY 4
#define PLANE_ANYZ 5

// planes (x&~1) and (x&~1)+1 are allways opposites

typedef struct
{
    float normal[3];
    float dist;
    int32_t type; // PLANE_X - PLANE_ANYZ ?remove? trivial to regenerate
} dplane_t;

// contents flags are seperate bits
// a given brush can contribute multiple content bits
// multiple brushes can be in a single leaf

// these definitions also need to be in q_shared.h!

// lower bits are stronger, and will eat weaker brushes completely
#define CONTENTS_SOLID        1 // an eye is never valid in a solid
#define CONTENTS_WINDOW       2 // translucent, but not watery
#define CONTENTS_AUX          4
#define CONTENTS_LAVA         8
#define CONTENTS_SLIME        16
#define CONTENTS_WATER        32
#define CONTENTS_MIST         64
#define LAST_VISIBLE_CONTENTS 64 // was 64

// remaining contents are non-visible, and don't eat brushes

#define CONTENTS_PROJECTILECLIP  (uint32_t)1 << 14 //qb: blocks projectile but not hitscan.  Requires game support
#define CONTENTS_AREAPORTAL   0x8000

#define CONTENTS_PLAYERCLIP   0x10000
#define CONTENTS_MONSTERCLIP  0x20000

// currents can be added to any other contents, and may be mixed
#define CONTENTS_CURRENT_0    0x40000
#define CONTENTS_CURRENT_90   0x80000
#define CONTENTS_CURRENT_180  0x100000
#define CONTENTS_CURRENT_270  0x200000
#define CONTENTS_CURRENT_UP   0x400000
#define CONTENTS_CURRENT_DOWN 0x800000

#define CONTENTS_ORIGIN       0x1000000 // removed before bsping an entity

#define CONTENTS_MONSTER      0x2000000 // should never be on a brush, only in game
#define CONTENTS_DEADMONSTER  0x4000000
#define CONTENTS_DETAIL       0x8000000  // brushes to be added after vis leafs
#define CONTENTS_TRANSLUCENT  0x10000000 // auto set if any surface has trans
#define CONTENTS_LADDER       0x20000000

//Surface flags
#define SURF_LIGHT            0x1 // value will hold the light strength

#define SURF_SLICK            0x2 // effects game physics

#define SURF_SKY              0x4  // don't draw, but add to skybox
#define SURF_WARP             0x8  // turbulent water warp
#define SURF_TRANS33          0x10 // 33% alpha blend
#define SURF_TRANS66          0x20 // 66% alpha blend
#define SURF_FLOWING          0x40 // scroll towards angle
#define SURF_NODRAW           0x80 // don't bother referencing the texture

#define SURF_HINT             0x0100 // make a bsp splitter
#define SURF_SKIP             0x0200 // ignore surface to make non-closed brushes

#define SURF_ALPHATEST        (uint32_t)1 << 25 //alpha test flag
#define SURF_N64_UV           (uint32_t)1 << 28 //N64 UV and surface flag hack
#define SURF_SCROLLX          (uint32_t)1 << 29 //slow x scroll
#define SURF_SCROLLY          (uint32_t)1 << 30 //slow y scroll
#define SURF_SCROLLFLIP       (uint32_t)1 << 31 //flip scroll directon


// qb: qbsp types - dnode_tx, dedge_t, dface_tx, dleaf_tx, dbrushside_tx

typedef struct
{
    int32_t planenum;
    int32_t children[2]; // negative numbers are -(leafs+1), not nodes
    float mins[3];       // for frustom culling
    float maxs[3];
    uint32_t firstface;
    uint32_t numfaces; // counting both sides
} dnode_t;            // qb: qbsp

#define TEXTURE_LENGTH      80

typedef struct texinfo_s {
    float vecs[2][4];    // [s/t][xyz offset]
    int32_t flags;       // miptex flags + overrides
    int32_t value;       // light emission, etc
    char texture[TEXTURE_LENGTH];    // texture name (textures/*.tga)
    int32_t nexttexinfo; // for animations, -1 = end of chain
} texinfo_t;

typedef struct
{
    uint32_t v[2]; // vertex numbers
} dedge_t;        // qb: qbsp

#define MAXLIGHTMAPS 4
typedef struct
{
    uint32_t planenum;
    int32_t side;

    int32_t firstedge; // we must support > 64k edges
    int32_t numedges;
    int32_t texinfo;

    // lighting info
    byte styles[MAXLIGHTMAPS];
    int32_t lightofs; // start of [numstyles*surfsize] samples
} dface_t;           // qb: qbsp

typedef struct
{
    int32_t contents; // OR of all brushes (not needed?)

    int32_t cluster;
    int32_t area;

    float mins[3]; // for frustum culling
    float maxs[3];

    uint32_t firstleafface;
    uint32_t numleaffaces;

    uint32_t firstleafbrush;
    uint32_t numleafbrushes;
} dleaf_t; // qb: qbsp

typedef struct
{
    uint32_t planenum; // facing out of the leaf
    int32_t texinfo;
} dbrushside_t; // qb: qbsp

typedef struct
{
    int32_t firstside;
    int32_t numsides;
    int32_t contents;
} dbrush_t;

#define ANGLE_UP   -1
#define ANGLE_DOWN -2

// the visibility lump consists of a header with a count, then
// byte offsets for the PVS and PHS of each cluster, then the raw
// compressed bit vectors
#define DVIS_PVS   0
#define DVIS_PHS   1
typedef struct
{
    int32_t numclusters;
    int32_t bitofs[8][2]; // bitofs[numclusters][2]
} dvis_t;

// each area has a list of portals that lead into other areas
// when portals are closed, other areas may not be visible or
// hearable even if the vis info says that it should be
typedef struct
{
    int32_t portalnum;
    int32_t otherarea;
} dareaportal_t;

typedef struct
{
    uint32_t numareaportals;
    uint32_t firstareaportal;
} darea_t;
