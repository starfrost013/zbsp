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

#include "cmdlib.h"
#include "mathlib.h"
#include "bspfile.h"
#include "scriplib.h"

qboolean noskipfix = false; // qb: warn about SURF_SKIP contents rather than silently changing to zero

//=============================================================================

// qb: add qbsp types

int32_t nummodels;
dmodel_t *dmodels; //[MAX_MAP_MODELS_QBSP];

int32_t visdatasize;
byte *dvisdata; //[MAX_MAP_VISIBILITY_QBSP];
dvis_t *dvis;   // = (dvis_t *)dvisdata;

int32_t lightdatasize;
byte *dlightdata; //[MAX_MAP_LIGHTING_QBSP];

int32_t entdatasize;
char *dentdata; //[MAX_MAP_ENTSTRING_QBSP];

int32_t numleafs;
dleaf_t *dleafs;   //[MAX_MAP_LEAFS];
dleaf_tx *dleafsX; //[MAX_MAP_LEAFS_QBSP];

int32_t numplanes;
dplane_t *dplanes; //[MAX_MAP_PLANES_QBSP];

int32_t numvertexes;
dvertex_t *dvertexes; //[MAX_MAP_VERTS_QBSP];

int32_t numnodes;
dnode_t *dnodes;   //[MAX_MAP_NODES];
dnode_tx *dnodesX; //[MAX_MAP_NODES_QBSP];

int32_t numtexinfo;
texinfo_t *texinfo; //[MAX_MAP_TEXINFO_QBSP];

int32_t numfaces;
dface_t *dfaces;   //[MAX_MAP_FACES];
dface_tx *dfacesX; //[MAX_MAP_FACES_QBSP];

int32_t numedges;
dedge_t *dedges;   //[MAX_MAP_EDGES];
dedge_tx *dedgesX; //[MAX_MAP_EDGES_QBSP];

int32_t numleaffaces;
uint16_t *dleaffaces;  //[MAX_MAP_LEAFFACES];
uint32_t *dleaffacesX; //[MAX_MAP_LEAFFACES_QBSP];

int32_t numleafbrushes;
uint16_t *dleafbrushes;  //[MAX_MAP_LEAFBRUSHES];
uint32_t *dleafbrushesX; //[MAX_MAP_LEAFBRUSHES_QBSP];

int32_t numsurfedges;
int32_t *dsurfedges; //[MAX_MAP_SURFEDGES_QBSP];

int32_t numbrushes;
dbrush_t *dbrushes; //[MAX_MAP_BRUSHES_QBSP];

int32_t numbrushsides;
dbrushside_t *dbrushsides;   //[MAX_MAP_BRUSHSIDES];
dbrushside_tx *dbrushsidesX; //[MAX_MAP_BRUSHSIDES_QBSP];

int32_t numareas;
darea_t *dareas; //[MAX_MAP_AREAS];

int32_t numareaportals;
dareaportal_t *dareaportals; //[MAX_MAP_AREAPORTALS];

byte dpop[256];

void InitBSPFile(void) {
    static qboolean init = false;
    if (!init) {
        init          = true;
        dmodels       = (dmodel_t *)malloc(sizeof(*dmodels) * MAX_MAP_MODELS_QBSP);
        dvisdata      = (byte *)malloc(sizeof(*dvisdata) * MAX_MAP_VISIBILITY_QBSP);
        dvis          = (dvis_t *)dvisdata;
        dlightdata    = (byte *)malloc(sizeof(*dlightdata) * MAX_MAP_LIGHTING_QBSP);
        dentdata      = (char *)malloc(sizeof(*dentdata) * MAX_MAP_ENTSTRING_QBSP);
        dleafs        = (dleaf_t *)malloc(sizeof(*dleafs) * MAX_MAP_LEAFS);
        dleafsX       = (dleaf_tx *)malloc(sizeof(*dleafsX) * MAX_MAP_LEAFS_QBSP);
        dplanes       = (dplane_t *)malloc(sizeof(*dplanes) * MAX_MAP_PLANES_QBSP);
        dvertexes     = (dvertex_t *)malloc(sizeof(*dvertexes) * MAX_MAP_VERTS_QBSP);
        dnodes        = (dnode_t *)malloc(sizeof(*dnodes) * MAX_MAP_NODES);
        dnodesX       = (dnode_tx *)malloc(sizeof(*dnodesX) * MAX_MAP_NODES_QBSP);
        texinfo       = (texinfo_t *)malloc(sizeof(*texinfo) * MAX_MAP_TEXINFO_QBSP);
        dfaces        = (dface_t *)malloc(sizeof(*dfaces) * MAX_MAP_FACES);
        dfacesX       = (dface_tx *)malloc(sizeof(*dfacesX) * MAX_MAP_FACES_QBSP);
        dedges        = (dedge_t *)malloc(sizeof(*dedges) * MAX_MAP_EDGES);
        dedgesX       = (dedge_tx *)malloc(sizeof(*dedgesX) * MAX_MAP_EDGES_QBSP);
        dleaffaces    = (uint16_t *)malloc(sizeof(*dleaffaces) * MAX_MAP_LEAFFACES);
        dleaffacesX   = (uint32_t *)malloc(sizeof(*dleaffacesX) * MAX_MAP_LEAFFACES_QBSP);
        dleafbrushes  = (uint16_t *)malloc(sizeof(*dleafbrushes) * MAX_MAP_LEAFBRUSHES);
        dleafbrushesX = (uint32_t *)malloc(sizeof(*dleafbrushesX) * MAX_MAP_LEAFBRUSHES_QBSP);
        dsurfedges    = (int32_t *)malloc(sizeof(*dsurfedges) * MAX_MAP_SURFEDGES_QBSP);
        dbrushes      = (dbrush_t *)malloc(sizeof(*dbrushes) * MAX_MAP_BRUSHES_QBSP);
        dbrushsides   = (dbrushside_t *)malloc(sizeof(*dbrushsides) * MAX_MAP_BRUSHSIDES);
        dbrushsidesX  = (dbrushside_tx *)malloc(sizeof(*dbrushsidesX) * MAX_MAP_BRUSHSIDES_QBSP);
        dareas        = (darea_t *)malloc(sizeof(*dareas) * MAX_MAP_AREAS);
        dareaportals  = (dareaportal_t *)malloc(sizeof(*dareaportals) * MAX_MAP_AREAPORTALS);
    }
}

/*
===============
CompressVis

===============
*/
int32_t CompressVis(byte *vis, byte *dest) {
    int32_t j;
    int32_t rep;
    int32_t visrow;
    byte *dest_p;

    dest_p = dest;
    //	visrow = (r_numvisleafs + 7)>>3;
    visrow = (dvis->numclusters + 7) >> 3;

    for (j = 0; j < visrow; j++) {
        *dest_p++ = vis[j];
        if (vis[j])
            continue;

        rep = 1;
        for (j++; j < visrow; j++)
            if (vis[j] || rep == 255)
                break;
            else
                rep++;
        *dest_p++ = rep;
        j--;
    }

    return dest_p - dest;
}

/*
===================
DecompressVis
===================
*/
void DecompressVis(byte *in, byte *decompressed) {
    int32_t c;
    byte *out;
    int32_t row;

    //	row = (r_numvisleafs+7)>>3;
    row = (dvis->numclusters + 7) >> 3;
    out = decompressed;

    do {
        if (*in) {
            *out++ = *in++;
            continue;
        }

        c = in[1];
        if (!c)
            Error("DecompressVis: 0 repeat");
        in += 2;
        while (c) {
            *out++ = 0;
            c--;
        }
    } while (out - decompressed < row);
}

//=============================================================================

/*
=============
SwapBSPFile

Byte swaps all data in a bsp file.
=============
*/
void SwapBSPFile(qboolean todisk) {
    int32_t i, j;
    dmodel_t *d;

    // models
    for (i = 0; i < nummodels; i++) {
        d            = &dmodels[i];

        d->firstface = LittleLong(d->firstface);
        d->numfaces  = LittleLong(d->numfaces);
        d->headnode  = LittleLong(d->headnode);

        for (j = 0; j < 3; j++) {
            d->mins[j]   = LittleFloat(d->mins[j]);
            d->maxs[j]   = LittleFloat(d->maxs[j]);
            d->origin[j] = LittleFloat(d->origin[j]);
        }
    }

    //
    // vertexes
    //
    for (i = 0; i < numvertexes; i++) {
        for (j = 0; j < 3; j++)
            dvertexes[i].point[j] = LittleFloat(dvertexes[i].point[j]);
    }

    //
    // planes
    //
    for (i = 0; i < numplanes; i++) {
        for (j = 0; j < 3; j++)
            dplanes[i].normal[j] = LittleFloat(dplanes[i].normal[j]);
        dplanes[i].dist = LittleFloat(dplanes[i].dist);
        dplanes[i].type = LittleLong(dplanes[i].type);
    }

    //
    // texinfos
    //
    for (i = 0; i < numtexinfo; i++) {
        texinfo[i].flags       = LittleLong(texinfo[i].flags);
        texinfo[i].value       = LittleLong(texinfo[i].value);
        texinfo[i].nexttexinfo = LittleLong(texinfo[i].nexttexinfo);
    }

    //
    // faces nodes leafs brushsides leafbrushes leaffaces
    //
    for (i = 0; i < numfaces; i++) {
        dfacesX[i].texinfo = LittleLong(dfacesX[i].texinfo);
        dfacesX[i].planenum = LittleLong(dfacesX[i].planenum);
        dfacesX[i].side = LittleLong(dfacesX[i].side);
        dfacesX[i].lightofs = LittleLong(dfacesX[i].lightofs);
        dfacesX[i].firstedge = LittleLong(dfacesX[i].firstedge);
        dfacesX[i].numedges = LittleLong(dfacesX[i].numedges);
    }

    for (i = 0; i < numnodes; i++) {
        dnodesX[i].planenum = LittleLong(dnodesX[i].planenum);
        for (j = 0; j < 3; j++) {
            dnodesX[i].mins[j] = LittleFloat(dnodesX[i].mins[j]);
            dnodesX[i].maxs[j] = LittleFloat(dnodesX[i].maxs[j]);
        }
        dnodesX[i].children[0] = LittleLong(dnodesX[i].children[0]);
        dnodesX[i].children[1] = LittleLong(dnodesX[i].children[1]);
        dnodesX[i].firstface = LittleLong(dnodesX[i].firstface);
        dnodesX[i].numfaces = LittleLong(dnodesX[i].numfaces);
    }

    for (i = 0; i < numleafs; i++) {
        dleafsX[i].contents = LittleLong(dleafsX[i].contents);
        dleafsX[i].cluster = LittleLong(dleafsX[i].cluster);
        dleafsX[i].area = LittleLong(dleafsX[i].area);
        for (j = 0; j < 3; j++) {
            dleafsX[i].mins[j] = LittleFloat(dleafsX[i].mins[j]);
            dleafsX[i].maxs[j] = LittleFloat(dleafsX[i].maxs[j]);
        }
        dleafsX[i].firstleafface = LittleLong(dleafsX[i].firstleafface);
        dleafsX[i].numleaffaces = LittleLong(dleafsX[i].numleaffaces);
        dleafsX[i].firstleafbrush = LittleLong(dleafsX[i].firstleafbrush);
        dleafsX[i].numleafbrushes = LittleLong(dleafsX[i].numleafbrushes);
    }

    for (i = 0; i < numbrushsides; i++) {
        dbrushsidesX[i].planenum = LittleLong(dbrushsidesX[i].planenum);
        dbrushsidesX[i].texinfo = LittleLong(dbrushsidesX[i].texinfo);
    }

    for (i = 0; i < numleafbrushes; i++)
        dleafbrushesX[i] = LittleLong(dleafbrushesX[i]);

    for (i = 0; i < numleaffaces; i++)
        dleaffacesX[i] = LittleLong(dleaffacesX[i]);

    //
    // surfedges
    //
    for (i = 0; i < numsurfedges; i++)
        dsurfedges[i] = LittleLong(dsurfedges[i]);

    //
    // edges
    //
    for (i = 0; i < numedges; i++) {
        dedgesX[i].v[0] = LittleLong(dedgesX[i].v[0]);
        dedgesX[i].v[1] = LittleLong(dedgesX[i].v[1]);
    }

    //
    // brushes
    //
    for (i = 0; i < numbrushes; i++) {
        dbrushes[i].firstside = LittleLong(dbrushes[i].firstside);
        dbrushes[i].numsides  = LittleLong(dbrushes[i].numsides);
        dbrushes[i].contents  = LittleLong(dbrushes[i].contents);
    }

    //
    // areas
    //
    for (i = 0; i < numareas; i++) {
        dareas[i].numareaportals  = LittleLong(dareas[i].numareaportals);
        dareas[i].firstareaportal = LittleLong(dareas[i].firstareaportal);
    }

    //
    // areasportals
    //
    for (i = 0; i < numareaportals; i++) {
        dareaportals[i].portalnum = LittleLong(dareaportals[i].portalnum);
        dareaportals[i].otherarea = LittleLong(dareaportals[i].otherarea);
    }

    //
    // visibility
    //
    if (todisk)
        j = dvis->numclusters;
    else
        j = LittleLong(dvis->numclusters);
    dvis->numclusters = LittleLong(dvis->numclusters);
    for (i = 0; i < j; i++) {
        dvis->bitofs[i][0] = LittleLong(dvis->bitofs[i][0]);
        dvis->bitofs[i][1] = LittleLong(dvis->bitofs[i][1]);
    }
}

dheader_t *header;

int32_t CopyLump(int32_t lump, void *dest, int32_t size) {
    int32_t length, ofs;

    length = header->lumps[lump].filelen;
    ofs    = header->lumps[lump].fileofs;

    if (length % size)
        Error("LoadBSPFile: odd lump size  (length: %i  size: %i  remainder: %i)", length, size, length % size);

    memcpy(dest, (byte *)header + ofs, length);

    return length / size;
}

/*
=============
LoadBSPFile
=============
*/
void LoadBSPFile(char *filename) {
    int32_t i;

    InitBSPFile();

    //
    // load the file header
    //
    LoadFile(filename, (void **)&header);

    // swap the header
    for (i = 0; i < sizeof(dheader_t) / 4; i++)
        ((int32_t *)header)[i] = LittleLong(((int32_t *)header)[i]);


    switch (header->ident)
    {
    case ZBSPHEADER:
        break;
    default:
        Error("%s is not a ZBSP file.",
              filename);
    }
    if (header->version != ZBSPVERSION)
        Error("%s is version %i, not %i", filename, header->version, ZBSPVERSION);

    nummodels   = CopyLump(LUMP_MODELS, dmodels, sizeof(dmodel_t));
    numvertexes = CopyLump(LUMP_VERTEXES, dvertexes, sizeof(dvertex_t));
    numplanes   = CopyLump(LUMP_PLANES, dplanes, sizeof(dplane_t));

    numleafs = CopyLump(LUMP_LEAFS, dleafsX, sizeof(dleaf_tx));
    numnodes = CopyLump(LUMP_NODES, dnodesX, sizeof(dnode_tx));
    numtexinfo = CopyLump(LUMP_TEXINFO, texinfo, sizeof(texinfo_t));
    numfaces = CopyLump(LUMP_FACES, dfacesX, sizeof(dface_tx));
    numleaffaces = CopyLump(LUMP_LEAFFACES, dleaffacesX, sizeof(dleaffacesX[0]));
    numleafbrushes = CopyLump(LUMP_LEAFBRUSHES, dleafbrushesX, sizeof(dleafbrushesX[0]));

    numsurfedges = CopyLump(LUMP_SURFEDGES, dsurfedges, sizeof(dsurfedges[0]));

    numedges = CopyLump(LUMP_EDGES, dedgesX, sizeof(dedge_tx));

    numbrushes = CopyLump(LUMP_BRUSHES, dbrushes, sizeof(dbrush_t));

    numbrushsides = CopyLump(LUMP_BRUSHSIDES, dbrushsidesX, sizeof(dbrushside_tx));


    numareas       = CopyLump(LUMP_AREAS, dareas, sizeof(darea_t));
    numareaportals = CopyLump(LUMP_AREAPORTALS, dareaportals, sizeof(dareaportal_t));

    visdatasize    = CopyLump(LUMP_VISIBILITY, dvisdata, 1);
    lightdatasize  = CopyLump(LUMP_LIGHTING, dlightdata, 1);
    entdatasize    = CopyLump(LUMP_ENTITIES, dentdata, 1);

    CopyLump(LUMP_POP, dpop, 1);

    free(header); // everything has been copied out

    //
    // swap everything
    //
    SwapBSPFile(false);
}

/*
=============
LoadBSPFileTexinfo

Only loads the texinfo lump to scan for textures
=============
*/
void LoadBSPFileTexinfo(char *filename) {
    int32_t i;
    FILE *f;
    int32_t length, ofs;

    header = malloc(sizeof(dheader_t));

    f      = fopen(filename, "rb");
    if (!fread(header, sizeof(dheader_t), 1, f))
        Error("Texinfo header read error");

    // swap the header
    for (i = 0; i < sizeof(dheader_t) / 4; i++)
        ((int32_t *)header)[i] = LittleLong(((int32_t *)header)[i]);

    if (header->ident != ZBSPHEADER)
        Error("%s is not a ZBSP file", filename);
    if (header->version != ZBSPVERSION)
        Error("%s is version %i, not %i", filename, header->version, ZBSPVERSION);

    length = header->lumps[LUMP_TEXINFO].filelen;
    ofs    = header->lumps[LUMP_TEXINFO].fileofs;

    fseek(f, ofs, SEEK_SET);
    if (!fread(texinfo, length, 1, f))
        Error("Texinfo lump read error");
    fclose(f);

    numtexinfo = length / sizeof(texinfo_t);

    free(header); // everything has been copied out

    SwapBSPFile(false);
}

//============================================================================

FILE *wadfile;
dheader_t outheader;

void AddLump(int32_t lumpnum, void *data, int32_t len) {
    lump_t *lump;

    lump          = &header->lumps[lumpnum];

    lump->fileofs = LittleLong(ftell(wadfile));
    lump->filelen = LittleLong(len);
    SafeWrite(wadfile, data, (len + 3) & ~3);
}

/*
=============
WriteBSPFile

Swaps the bsp file in place, so it should not be referenced again
=============
*/
void WriteBSPFile(char *filename) {

    header = &outheader;
    memset(header, 0, sizeof(dheader_t));

    SwapBSPFile(true);

    header->ident = LittleLong(ZBSPHEADER);

    header->version = LittleLong(ZBSPVERSION);

    wadfile         = SafeOpenWrite(filename);
    SafeWrite(wadfile, header, sizeof(dheader_t)); // overwritten later

    AddLump(LUMP_PLANES, dplanes, numplanes * sizeof(dplane_t));

    AddLump(LUMP_LEAFS, dleafsX, numleafs * sizeof(dleaf_tx));

    AddLump(LUMP_VERTEXES, dvertexes, numvertexes * sizeof(dvertex_t));

    AddLump(LUMP_NODES, dnodesX, numnodes * sizeof(dnode_tx));

    AddLump(LUMP_TEXINFO, texinfo, numtexinfo * sizeof(texinfo_t));

    AddLump(LUMP_FACES, dfacesX, numfaces * sizeof(dface_tx));

    AddLump(LUMP_BRUSHES, dbrushes, numbrushes * sizeof(dbrush_t));

    AddLump(LUMP_BRUSHSIDES, dbrushsidesX, numbrushsides * sizeof(dbrushside_tx));
    AddLump(LUMP_LEAFFACES, dleaffacesX, numleaffaces * sizeof(dleaffacesX[0]));
    AddLump(LUMP_LEAFBRUSHES, dleafbrushesX, numleafbrushes * sizeof(dleafbrushesX[0]));

    AddLump(LUMP_SURFEDGES, dsurfedges, numsurfedges * sizeof(dsurfedges[0]));

    AddLump(LUMP_EDGES, dedgesX, numedges * sizeof(dedge_tx));

    AddLump(LUMP_MODELS, dmodels, nummodels * sizeof(dmodel_t));
    AddLump(LUMP_AREAS, dareas, numareas * sizeof(darea_t));
    AddLump(LUMP_AREAPORTALS, dareaportals, numareaportals * sizeof(dareaportal_t));

    AddLump(LUMP_LIGHTING, dlightdata, lightdatasize);
    AddLump(LUMP_VISIBILITY, dvisdata, visdatasize);
    AddLump(LUMP_ENTITIES, dentdata, entdatasize);
    AddLump(LUMP_POP, dpop, sizeof(dpop));

    fseek(wadfile, 0, SEEK_SET);
    SafeWrite(wadfile, header, sizeof(dheader_t));
    fclose(wadfile);
}

//============================================================================

/*
=============
PrintBSPFileSizes

Dumps info about current file
=============
*/
void PrintBSPFileSizes(void) {
    if (!num_entities)
        ParseEntities();
    printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FILE STATS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("models:      %7i        size: %7i\n", nummodels, (int32_t)(nummodels * sizeof(dmodel_t)));
    printf("brushes:     %7i        size: %7i\n", numbrushes, (int32_t)(numbrushes * sizeof(dbrush_t)));

    printf("brushsides:  %7i        size: %7i\n", numbrushsides, (int32_t)(numbrushsides * sizeof(dbrushside_tx)));

    printf("planes:      %7i        size: %7i\n", numplanes, (int32_t)(numplanes * sizeof(dplane_t)));
    printf("texinfo:     %7i        size: %7i\n", numtexinfo, (int32_t)(numtexinfo * sizeof(texinfo_t)));
    printf("entdata:     %7i        size: %7i\n", num_entities, entdatasize);

    printf("vertices:    %7i        size: %7i\n", numvertexes, (int32_t)(numvertexes * sizeof(dvertex_t)));

    printf("nodes:       %7i        size: %7i\n", numnodes, (int32_t)(numnodes * sizeof(dnode_tx)));
    printf("faces:       %7i        size: %7i\n", numfaces, (int32_t)(numfaces * sizeof(dface_tx)));
    printf("leafs:       %7i        size: %7i\n", numleafs, (int32_t)(numleafs * sizeof(dleaf_tx)));
    printf("leaffaces:   %7i        size: %7i\n", numleaffaces, (int32_t)(numleaffaces * sizeof(dleaffacesX[0])));
    printf("leafbrushes: %7i        size: %7i\n", numleafbrushes, (int32_t)(numleafbrushes * sizeof(dleafbrushesX[0])));
    printf("edges:       %7i        size: %7i\n", numedges, (int32_t)(numedges * sizeof(dedge_tx)));

    printf("surfedges:   %7i        size: %7i\n", numsurfedges, (int32_t)(numsurfedges * sizeof(dsurfedges[0])));
    printf("                  lightdata size: %7i\n", lightdatasize);
    printf("                    visdata size: %7i\n", visdatasize);
}

//============================================

int32_t num_entities;
entity_t entities[MAX_MAP_ENTITIES_QBSP];

void StripTrailing(char *e) {
    char *s;

    s = e + strlen(e) - 1;
    while (s >= e && *s <= 32) {
        *s = 0;
        s--;
    }
}

/*
=================
ParseEpair
=================
*/
epair_t *ParseEpair(void) {
    epair_t *e;

    e = malloc(sizeof(epair_t));
    memset(e, 0, sizeof(epair_t));

    if (strlen(token) >= MAX_KEY - 1)
        Error("ParseEpar: token too long");
    e->key = copystring(token);
    GetToken(false);
    if (strlen(token) >= MAX_VALUE - 1)
        Error("ParseEpar: token too long");
    e->value = copystring(token);

    // strip trailing spaces
    StripTrailing(e->key);
    StripTrailing(e->value);

    return e;
}

/*
================
ParseEntity
================
*/
qboolean ParseEntity(void) {
    epair_t *e;
    entity_t *mapent;

    if (!GetToken(true))
        return false;

    if (strcmp(token, "{"))
        Error("ParseEntity: { not found");

    if (num_entities == MAX_MAP_ENTITIES_QBSP)
        Error("num_entities == MAX_MAP_ENTITIES_QBSP  (%i)", MAX_MAP_ENTITIES_QBSP);

    mapent = &entities[num_entities];
    num_entities++;

    do {
        if (!GetToken(true))
            Error("ParseEntity: EOF without closing brace");
        if (!strcmp(token, "}"))
            break;
        e              = ParseEpair();
        e->next        = mapent->epairs;
        mapent->epairs = e;
    } while (1);

    return true;
}

/*
================
ParseEntities

Parses the dentdata string into entities
================
*/
void ParseEntities(void) {
    num_entities = 0;
    ParseFromMemory(dentdata, entdatasize);

    while (ParseEntity()) {
    }
}

/*
================
UnparseEntities

Generates the dentdata string from all the entities
================
*/
void UnparseEntities(void) {
    char *buf, *end;
    epair_t *ep;
    char line[2060];
    int32_t i;
    char key[1024], value[1024];

    buf  = dentdata;
    end  = buf;
    *end = 0;

    for (i = 0; i < num_entities; i++) {
        ep = entities[i].epairs;
        if (!ep)
            continue; // ent got removed

        strcat(end, "{\n");
        end += 2;

        for (ep = entities[i].epairs; ep; ep = ep->next) {
            strcpy(key, ep->key);
            StripTrailing(key);
            strcpy(value, ep->value);
            StripTrailing(value);

            sprintf(line, "\"%s\" \"%s\"\n", key, value);
            strcat(end, line);
            end += strlen(line);
        }
        strcat(end, "}\n");
        end += 2;

        if (end > buf + MAX_MAP_ENTSTRING_QBSP)
            Error("QBSP Entity text too long");
    }
    entdatasize = end - buf + 1;
}

void PrintEntity(entity_t *ent) {
    epair_t *ep;

    printf("------- entity %p -------\n", ent);
    for (ep = ent->epairs; ep; ep = ep->next) {
        printf("%s = %s\n", ep->key, ep->value);
    }
}

void SetKeyValue(entity_t *ent, char *key, char *value) {
    epair_t *ep;

    for (ep = ent->epairs; ep; ep = ep->next)
        if (!strcmp(ep->key, key)) {
            free(ep->value);
            ep->value = copystring(value);
            return;
        }
    ep = malloc(sizeof(*ep));
    if (ep) // qb: gcc -fanalyzer: could be NULL
    {
        ep->next    = ent->epairs;
        ent->epairs = ep;
        ep->key     = copystring(key);
        ep->value   = copystring(value);
    }
}

char *ValueForKey(entity_t *ent, char *key) {
    epair_t *ep;

    for (ep = ent->epairs; ep; ep = ep->next)
        if (!strcmp(ep->key, key))
            return ep->value;
    return "";
}

vec_t FloatForKey(entity_t *ent, char *key) {
    char *k;

    k = ValueForKey(ent, key);
    return atof(k);
}

void GetVectorForKey(entity_t *ent, char *key, vec3_t vec) {
    char *k;
    double v1, v2, v3;

    k  = ValueForKey(ent, key);
    // scanf into doubles, then assign, so it is vec_t size independent
    v1 = v2 = v3 = 0;
    sscanf(k, "%lf %lf %lf", &v1, &v2, &v3);
    vec[0] = v1;
    vec[1] = v2;
    vec[2] = v3;
}

void RemoveLastEpair(entity_t *ent) {
    epair_t *e = ent->epairs->next;

    if (ent->epairs->key)
        free(ent->epairs->key);
    if (ent->epairs->value)
        free(ent->epairs->value);

    free(ent->epairs);
    ent->epairs = e;
}
