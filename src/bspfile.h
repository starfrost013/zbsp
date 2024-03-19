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
#pragma once

#include "qfiles.h"

// qb: consolidate some defines here from duplicate places

// if a brush just barely pokes onto the other side,
// let it slide by without chopping
#define PLANESIDE_EPSILON           0.1 // qb: maybe can be smaller, but this small leaves gaps //0.001

#define MAX_POINTS_ON_WINDING       64
#define MAX_POINTS_ON_FIXED_WINDING 12

// you can define on_epsilon in the makefile as tighter
#ifndef ON_EPSILON
#define ON_EPSILON 0.1
#endif

extern int32_t nummodels;
extern dmodel_t * dmodels;//[MAX_MAP_MODELS];

extern int32_t visdatasize;
extern byte * dvisdata;//[MAX_MAP_VISIBILITY];
extern dvis_t *dvis;

extern int32_t lightdatasize;
extern byte * dlightdata;//[MAX_MAP_LIGHTING];

extern int32_t entdatasize;
extern char * dentdata;//[MAX_MAP_ENTSTRING];

extern int32_t numleafs;
extern dleaf_t * dleafs;//[MAX_MAP_LEAFS];

extern int32_t numplanes;
extern dplane_t * dplanes;//[MAX_MAP_PLANES];

extern int32_t numvertexes;
extern dvertex_t * dvertexes;//[MAX_MAP_VERTS];

extern int32_t numnodes;
extern dnode_t * dnodes;//[MAX_MAP_NODES];

extern int32_t numtexinfo;
extern texinfo_t * texinfo;//[MAX_MAP_TEXINFO];

extern int32_t numfaces;
extern dface_t * dfaces;//[MAX_MAP_FACES];

extern int32_t numedges;
extern dedge_t * dedges;//[MAX_MAP_EDGES];

extern int32_t numleaffaces;
extern uint32_t * dleaffaces;//[MAX_MAP_LEAFFACES];

extern int32_t numleafbrushes;
extern uint32_t * dleafbrushes;//[MAX_MAP_LEAFBRUSHES];

extern int32_t numsurfedges;
extern int32_t * dsurfedges;//[MAX_MAP_SURFEDGES];

extern int32_t numareas;
extern darea_t * dareas;//[MAX_MAP_AREAS];

extern int32_t numareaportals;
extern dareaportal_t * dareaportals;//[MAX_MAP_AREAPORTALS];

extern int32_t numbrushes;
extern dbrush_t * dbrushes;//[MAX_MAP_BRUSHES];

extern int32_t numbrushsides;
extern dbrushside_t * dbrushsides;//[MAX_MAP_BRUSHSIDES];

extern byte dpop[256];

void DecompressVis(byte *in, byte *decompressed);
int32_t CompressVis(byte *vis, byte *dest);

void InitBSPFile(void);
void LoadBSPFile(char *filename);
void LoadBSPFileTexinfo(char *filename); // just for data2
void WriteBSPFile(char *filename);
void PrintBSPFileSizes(void);

//===============

typedef struct epair_s {
    struct epair_s *next;
    char *key;
    char *value;
} epair_t;

typedef struct
{
    vec3_t origin;
    int32_t firstbrush;
    int32_t numbrushes;
    epair_t *epairs;

    // only valid for func_areaportals
    int32_t areaportalnum;
    int32_t portalareas[2];
} entity_t;

extern int32_t num_entities;
extern entity_t entities[MAX_MAP_ENTITIES];

void ParseEntities(void);
void UnparseEntities(void);

void SetKeyValue(entity_t *ent, char *key, char *value);
char *ValueForKey(entity_t *ent, char *key);
// will return "" if not present

float FloatForKey(entity_t *ent, char *key);
void GetVectorForKey(entity_t *ent, char *key, vec3_t vec);

epair_t *ParseEpair(void);
void RemoveLastEpair(entity_t *ent);

void PrintEntity(entity_t *ent);
