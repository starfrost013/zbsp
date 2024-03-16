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

#include "data.h"

//=================================================================

typedef struct
{
    int32_t numnormals;
    vec3_t normalsum;
} vertexnormals_t;

typedef struct
{
    vec3_t v;
    int32_t lightnormalindex;
} trivert_t;

typedef struct
{
    vec3_t mins, maxs;
    char name[16];
    trivert_t v[MAX_VERTS];
} frame_t;

//================================================================

frame_t g_frames[MAX_FRAMES];

dmdl_t model;

float scale_up;                      // set by $scale
vec3_t adjust;                       // set by $origin
int32_t g_fixedwidth, g_fixedheight; // set by $skinsize

//
// base frame info
//
vec3_t base_xyz[MAX_VERTS];
dstvert_t base_st[MAX_VERTS];
dtriangle_t triangles[MAX_TRIANGLES];

int32_t triangle_st[MAX_TRIANGLES][3][2];

// the command list holds counts, s/t values, and xyz indexes
// that are valid for every frame
int32_t commands[16384];
int32_t numcommands;
int32_t numglverts;
int32_t used[MAX_TRIANGLES];

char g_skins[MAX_MD2SKINS][64];

char cdarchive[2200];
char cdpartial[1100];
char cddir[2200];

char modelname[64]; // empty unless $modelname issued (players)

#define NUMVERTEXNORMALS 162

float avertexnormals[NUMVERTEXNORMALS][3] =
    {
        {-0.525731, 0.000000, 0.850651},
        {-0.442863, 0.238856, 0.864188},
        {-0.295242, 0.000000, 0.955423},
        {-0.309017, 0.500000, 0.809017},
        {-0.162460, 0.262866, 0.951056},
        {0.000000, 0.000000, 1.000000},
        {0.000000, 0.850651, 0.525731},
        {-0.147621, 0.716567, 0.681718},
        {0.147621, 0.716567, 0.681718},
        {0.000000, 0.525731, 0.850651},
        {0.309017, 0.500000, 0.809017},
        {0.525731, 0.000000, 0.850651},
        {0.295242, 0.000000, 0.955423},
        {0.442863, 0.238856, 0.864188},
        {0.162460, 0.262866, 0.951056},
        {-0.681718, 0.147621, 0.716567},
        {-0.809017, 0.309017, 0.500000},
        {-0.587785, 0.425325, 0.688191},
        {-0.850651, 0.525731, 0.000000},
        {-0.864188, 0.442863, 0.238856},
        {-0.716567, 0.681718, 0.147621},
        {-0.688191, 0.587785, 0.425325},
        {-0.500000, 0.809017, 0.309017},
        {-0.238856, 0.864188, 0.442863},
        {-0.425325, 0.688191, 0.587785},
        {-0.716567, 0.681718, -0.147621},
        {-0.500000, 0.809017, -0.309017},
        {-0.525731, 0.850651, 0.000000},
        {0.000000, 0.850651, -0.525731},
        {-0.238856, 0.864188, -0.442863},
        {0.000000, 0.955423, -0.295242},
        {-0.262866, 0.951056, -0.162460},
        {0.000000, 1.000000, 0.000000},
        {0.000000, 0.955423, 0.295242},
        {-0.262866, 0.951056, 0.162460},
        {0.238856, 0.864188, 0.442863},
        {0.262866, 0.951056, 0.162460},
        {0.500000, 0.809017, 0.309017},
        {0.238856, 0.864188, -0.442863},
        {0.262866, 0.951056, -0.162460},
        {0.500000, 0.809017, -0.309017},
        {0.850651, 0.525731, 0.000000},
        {0.716567, 0.681718, 0.147621},
        {0.716567, 0.681718, -0.147621},
        {0.525731, 0.850651, 0.000000},
        {0.425325, 0.688191, 0.587785},
        {0.864188, 0.442863, 0.238856},
        {0.688191, 0.587785, 0.425325},
        {0.809017, 0.309017, 0.500000},
        {0.681718, 0.147621, 0.716567},
        {0.587785, 0.425325, 0.688191},
        {0.955423, 0.295242, 0.000000},
        {1.000000, 0.000000, 0.000000},
        {0.951056, 0.162460, 0.262866},
        {0.850651, -0.525731, 0.000000},
        {0.955423, -0.295242, 0.000000},
        {0.864188, -0.442863, 0.238856},
        {0.951056, -0.162460, 0.262866},
        {0.809017, -0.309017, 0.500000},
        {0.681718, -0.147621, 0.716567},
        {0.850651, 0.000000, 0.525731},
        {0.864188, 0.442863, -0.238856},
        {0.809017, 0.309017, -0.500000},
        {0.951056, 0.162460, -0.262866},
        {0.525731, 0.000000, -0.850651},
        {0.681718, 0.147621, -0.716567},
        {0.681718, -0.147621, -0.716567},
        {0.850651, 0.000000, -0.525731},
        {0.809017, -0.309017, -0.500000},
        {0.864188, -0.442863, -0.238856},
        {0.951056, -0.162460, -0.262866},
        {0.147621, 0.716567, -0.681718},
        {0.309017, 0.500000, -0.809017},
        {0.425325, 0.688191, -0.587785},
        {0.442863, 0.238856, -0.864188},
        {0.587785, 0.425325, -0.688191},
        {0.688191, 0.587785, -0.425325},
        {-0.147621, 0.716567, -0.681718},
        {-0.309017, 0.500000, -0.809017},
        {0.000000, 0.525731, -0.850651},
        {-0.525731, 0.000000, -0.850651},
        {-0.442863, 0.238856, -0.864188},
        {-0.295242, 0.000000, -0.955423},
        {-0.162460, 0.262866, -0.951056},
        {0.000000, 0.000000, -1.000000},
        {0.295242, 0.000000, -0.955423},
        {0.162460, 0.262866, -0.951056},
        {-0.442863, -0.238856, -0.864188},
        {-0.309017, -0.500000, -0.809017},
        {-0.162460, -0.262866, -0.951056},
        {0.000000, -0.850651, -0.525731},
        {-0.147621, -0.716567, -0.681718},
        {0.147621, -0.716567, -0.681718},
        {0.000000, -0.525731, -0.850651},
        {0.309017, -0.500000, -0.809017},
        {0.442863, -0.238856, -0.864188},
        {0.162460, -0.262866, -0.951056},
        {0.238856, -0.864188, -0.442863},
        {0.500000, -0.809017, -0.309017},
        {0.425325, -0.688191, -0.587785},
        {0.716567, -0.681718, -0.147621},
        {0.688191, -0.587785, -0.425325},
        {0.587785, -0.425325, -0.688191},
        {0.000000, -0.955423, -0.295242},
        {0.000000, -1.000000, 0.000000},
        {0.262866, -0.951056, -0.162460},
        {0.000000, -0.850651, 0.525731},
        {0.000000, -0.955423, 0.295242},
        {0.238856, -0.864188, 0.442863},
        {0.262866, -0.951056, 0.162460},
        {0.500000, -0.809017, 0.309017},
        {0.716567, -0.681718, 0.147621},
        {0.525731, -0.850651, 0.000000},
        {-0.238856, -0.864188, -0.442863},
        {-0.500000, -0.809017, -0.309017},
        {-0.262866, -0.951056, -0.162460},
        {-0.850651, -0.525731, 0.000000},
        {-0.716567, -0.681718, -0.147621},
        {-0.716567, -0.681718, 0.147621},
        {-0.525731, -0.850651, 0.000000},
        {-0.500000, -0.809017, 0.309017},
        {-0.238856, -0.864188, 0.442863},
        {-0.262866, -0.951056, 0.162460},
        {-0.864188, -0.442863, 0.238856},
        {-0.809017, -0.309017, 0.500000},
        {-0.688191, -0.587785, 0.425325},
        {-0.681718, -0.147621, 0.716567},
        {-0.442863, -0.238856, 0.864188},
        {-0.587785, -0.425325, 0.688191},
        {-0.309017, -0.500000, 0.809017},
        {-0.147621, -0.716567, 0.681718},
        {-0.425325, -0.688191, 0.587785},
        {-0.162460, -0.262866, 0.951056},
        {0.442863, -0.238856, 0.864188},
        {0.162460, -0.262866, 0.951056},
        {0.309017, -0.500000, 0.809017},
        {0.147621, -0.716567, 0.681718},
        {0.000000, -0.525731, 0.850651},
        {0.425325, -0.688191, 0.587785},
        {0.587785, -0.425325, 0.688191},
        {0.688191, -0.587785, 0.425325},
        {-0.955423, 0.295242, 0.000000},
        {-0.951056, 0.162460, 0.262866},
        {-1.000000, 0.000000, 0.000000},
        {-0.850651, 0.000000, 0.525731},
        {-0.955423, -0.295242, 0.000000},
        {-0.951056, -0.162460, 0.262866},
        {-0.864188, 0.442863, -0.238856},
        {-0.951056, 0.162460, -0.262866},
        {-0.809017, 0.309017, -0.500000},
        {-0.864188, -0.442863, -0.238856},
        {-0.951056, -0.162460, -0.262866},
        {-0.809017, -0.309017, -0.500000},
        {-0.681718, 0.147621, -0.716567},
        {-0.681718, -0.147621, -0.716567},
        {-0.850651, 0.000000, -0.525731},
        {-0.688191, 0.587785, -0.425325},
        {-0.587785, 0.425325, -0.688191},
        {-0.425325, 0.688191, -0.587785},
        {-0.425325, -0.688191, -0.587785},
        {-0.587785, -0.425325, -0.688191},
        {-0.688191, -0.587785, -0.425325},
};

FILE *headerouthandle = NULL;

void Load3DSTriangleList(char *filename, triangle_t **pptri, int32_t *numtriangles);
void LoadLWOTriangleList(char *filename, triangle_t **pptri, int32_t *numtriangles);

//==============================================================

/*
===============
ClearModel
===============
*/
void ClearModel(void) {
    memset(&model, 0, sizeof(model));

    modelname[0] = 0;
    scale_up     = 1.0;
    VectorCopy(vec3_origin, adjust);
    g_fixedwidth = g_fixedheight = 0;
    g_skipmodel                  = false;
}

void H_printf(char *fmt, ...) {
    va_list argptr;
    char name[4000];

    if (!headerouthandle) {
        sprintf(name, "%s/tris.h", cddir);
        headerouthandle = SafeOpenWrite(name);
        fprintf(headerouthandle, "// %s\n\n", cddir);
        fprintf(headerouthandle, "// This file generated by data2 - Do NOT Modify\n\n");
    }

    va_start(argptr, fmt);
    vfprintf(headerouthandle, fmt, argptr);
    va_end(argptr);
}

/*
============
WriteModelFile
============
*/
void WriteModelFile(FILE *modelouthandle) {
    int32_t i;
    dmdl_t modeltemp;
    int32_t j, k;
    frame_t *in;
    daliasframe_t *out;
    byte buffer[MAX_VERTS * 4 + 128];
    float v;

    model.ident      = IDALIASHEADER;
    model.version    = ALIAS_VERSION;
    model.framesize  = (intptr_t) & ((daliasframe_t *)0)->verts[model.num_xyz];
    model.num_glcmds = numcommands;
    model.ofs_skins  = sizeof(dmdl_t);
    model.ofs_st     = model.ofs_skins + model.num_skins * MAX_SKINNAME;
    model.ofs_tris   = model.ofs_st + model.num_st * sizeof(dstvert_t);
    model.ofs_frames = model.ofs_tris + model.num_tris * sizeof(dtriangle_t);
    model.ofs_glcmds = model.ofs_frames + model.num_frames * model.framesize;
    model.ofs_end    = model.ofs_glcmds + model.num_glcmds * 4;

    //
    // write out the model header
    //
    for (i = 0; i < sizeof(dmdl_t) / 4; i++)
        ((int32_t *)&modeltemp)[i] = LittleLong(((int32_t *)&model)[i]);

    SafeWrite(modelouthandle, &modeltemp, sizeof(modeltemp));

    //
    // write out the skin names
    //
    SafeWrite(modelouthandle, g_skins, model.num_skins * MAX_SKINNAME);

    //
    // write out the texture coordinates
    //

    for (i = 0; i < model.num_st; i++) {
        base_st[i].s = LittleShort(base_st[i].s);
        base_st[i].t = LittleShort(base_st[i].t);
    }

    SafeWrite(modelouthandle, base_st, model.num_st * sizeof(base_st[0]));

    //
    // write out the triangles
    //
    for (i = 0; i < model.num_tris; i++) {
        int32_t j;
        dtriangle_t tri;

        for (j = 0; j < 3; j++) {
            tri.index_xyz[j] = LittleShort(triangles[i].index_xyz[j]);
            tri.index_st[j]  = LittleShort(triangles[i].index_st[j]);
        }

        SafeWrite(modelouthandle, &tri, sizeof(tri));
    }

    //
    // write out the frames
    //
    for (i = 0; i < model.num_frames; i++) {
        in  = &g_frames[i];
        out = (daliasframe_t *)buffer;

        strcpy(out->name, in->name);
        for (j = 0; j < 3; j++) {
            out->scale[j]     = (in->maxs[j] - in->mins[j]) / 255;
            out->translate[j] = in->mins[j];
        }

        for (j = 0; j < model.num_xyz; j++) {
            // all of these are byte values, so no need to deal with endianness
            out->verts[j].lightnormalindex = in->v[j].lightnormalindex;

            for (k = 0; k < 3; k++) {
                // scale to byte values & min/max check
                v = Q_rint((in->v[j].v[k] - out->translate[k]) / out->scale[k]);

                // clamp, so rounding doesn't wrap from 255.6 to 0
                if (v > 255.0)
                    v = 255.0;
                if (v < 0)
                    v = 0;
                out->verts[j].v[k] = v;
            }
        }

        for (j = 0; j < 3; j++) {
            out->scale[j]     = LittleFloat(out->scale[j]);
            out->translate[j] = LittleFloat(out->translate[j]);
        }

        SafeWrite(modelouthandle, out, model.framesize);
    }

    //
    // write out glcmds
    //
    SafeWrite(modelouthandle, commands, numcommands * 4);
}

/*
===============
FinishModel
===============
*/
void FinishModel(void) {
    FILE *modelouthandle;
    int32_t i;
    char name[4000];

    if (!model.num_frames)
        return;

    //
    // copy to release directory tree if doing a release build
    //
    if (g_release) {
        if (modelname[0])
            sprintf(name, "%s", modelname);
        else
            sprintf(name, "%s/tris.md2", cdpartial);
        ReleaseFile(name);

        for (i = 0; i < model.num_skins; i++) {
            ReleaseFile(g_skins[i]);
        }
        model.num_frames = 0;
        return;
    }

    //
    // write the model output file
    //
    if (modelname[0])
        sprintf(name, "%s%s", gamedir, modelname);
    else
        sprintf(name, "%s/tris.md2", cddir);
    printf("saving to %s\n", name);
    CreatePath(name);
    modelouthandle = SafeOpenWrite(name);

    WriteModelFile(modelouthandle);

    printf("%3dx%3d skin\n", model.skinwidth, model.skinheight);
    printf("%4d vertexes\n", model.num_xyz);
    printf("%4d triangles\n", model.num_tris);
    printf("%4d frame\n", model.num_frames);
    printf("%4d glverts\n", numglverts);
    printf("%4d glcmd\n", model.num_glcmds);
    printf("%4d skins\n", model.num_skins);
    printf("file size: %d\n", (int32_t)ftell(modelouthandle));
    printf("---------------------\n");

    fclose(modelouthandle);

    // finish writing header file
    H_printf("\n");

    // scale_up is usefull to allow step distances to be adjusted
    H_printf("#define MODEL_SCALE\t\t%f\n", scale_up);

    fclose(headerouthandle);
    headerouthandle = NULL;
}

/*
=================================================================

ALIAS MODEL DISPLAY LIST GENERATION

=================================================================
*/

int32_t strip_xyz[128];
int32_t strip_st[128];
int32_t strip_tris[128];
int32_t stripcount;

/*
================
StripLength
================
*/
int32_t StripLength(int32_t starttri, int32_t startv) {
    int32_t m1, m2;
    int32_t st1, st2;
    int32_t j;
    dtriangle_t *last, *check;
    int32_t k;

    used[starttri] = 2;

    last           = &triangles[starttri];

    strip_xyz[0]   = last->index_xyz[(startv) % 3];
    strip_xyz[1]   = last->index_xyz[(startv + 1) % 3];
    strip_xyz[2]   = last->index_xyz[(startv + 2) % 3];
    strip_st[0]    = last->index_st[(startv) % 3];
    strip_st[1]    = last->index_st[(startv + 1) % 3];
    strip_st[2]    = last->index_st[(startv + 2) % 3];

    strip_tris[0]  = starttri;
    stripcount     = 1;

    m1             = last->index_xyz[(startv + 2) % 3];
    st1            = last->index_st[(startv + 2) % 3];
    m2             = last->index_xyz[(startv + 1) % 3];
    st2            = last->index_st[(startv + 1) % 3];

    // look for a matching triangle
nexttri:
    for (j = starttri + 1, check = &triangles[starttri + 1]; j < model.num_tris; j++, check++) {
        for (k = 0; k < 3; k++) {
            if (check->index_xyz[k] != m1)
                continue;
            if (check->index_st[k] != st1)
                continue;
            if (check->index_xyz[(k + 1) % 3] != m2)
                continue;
            if (check->index_st[(k + 1) % 3] != st2)
                continue;

            // this is the next part of the fan

            // if we can't use this triangle, this tristrip is done
            if (used[j])
                goto done;

            // the new edge
            if (stripcount & 1) {
                m2  = check->index_xyz[(k + 2) % 3];
                st2 = check->index_st[(k + 2) % 3];
            } else {
                m1  = check->index_xyz[(k + 2) % 3];
                st1 = check->index_st[(k + 2) % 3];
            }

            strip_xyz[stripcount + 2] = check->index_xyz[(k + 2) % 3];
            strip_st[stripcount + 2]  = check->index_st[(k + 2) % 3];
            strip_tris[stripcount]    = j;
            stripcount++;

            used[j] = 2;
            goto nexttri;
        }
    }
done:

    // clear the temp used flags
    for (j = starttri + 1; j < model.num_tris; j++)
        if (used[j] == 2)
            used[j] = 0;

    return stripcount;
}

/*
===========
FanLength
===========
*/
int32_t FanLength(int32_t starttri, int32_t startv) {
    int32_t m1, m2;
    int32_t st1, st2;
    int32_t j;
    dtriangle_t *last, *check;
    int32_t k;

    used[starttri] = 2;

    last           = &triangles[starttri];

    strip_xyz[0]   = last->index_xyz[(startv) % 3];
    strip_xyz[1]   = last->index_xyz[(startv + 1) % 3];
    strip_xyz[2]   = last->index_xyz[(startv + 2) % 3];
    strip_st[0]    = last->index_st[(startv) % 3];
    strip_st[1]    = last->index_st[(startv + 1) % 3];
    strip_st[2]    = last->index_st[(startv + 2) % 3];

    strip_tris[0]  = starttri;
    stripcount     = 1;

    m1             = last->index_xyz[(startv + 0) % 3];
    st1            = last->index_st[(startv + 0) % 3];
    m2             = last->index_xyz[(startv + 2) % 3];
    st2            = last->index_st[(startv + 2) % 3];

    // look for a matching triangle
nexttri:
    for (j = starttri + 1, check = &triangles[starttri + 1]; j < model.num_tris; j++, check++) {
        for (k = 0; k < 3; k++) {
            if (check->index_xyz[k] != m1)
                continue;
            if (check->index_st[k] != st1)
                continue;
            if (check->index_xyz[(k + 1) % 3] != m2)
                continue;
            if (check->index_st[(k + 1) % 3] != st2)
                continue;

            // this is the next part of the fan

            // if we can't use this triangle, this tristrip is done
            if (used[j])
                goto done;

            // the new edge
            m2                        = check->index_xyz[(k + 2) % 3];
            st2                       = check->index_st[(k + 2) % 3];

            strip_xyz[stripcount + 2] = m2;
            strip_st[stripcount + 2]  = st2;
            strip_tris[stripcount]    = j;
            stripcount++;

            used[j] = 2;
            goto nexttri;
        }
    }
done:

    // clear the temp used flags
    for (j = starttri + 1; j < model.num_tris; j++)
        if (used[j] == 2)
            used[j] = 0;

    return stripcount;
}

/*
================
BuildGlCmds

Generate a list of trifans or strips
for the model, which holds for all frames
================
*/
void BuildGlCmds(void) {
    int32_t i, j, k;
    int32_t startv;
    float s, t;
    int32_t len, bestlen, besttype = 0;
    int32_t best_xyz[1024];
    int32_t best_st[1024];
    int32_t best_tris[1024];
    int32_t type;

    //
    // build tristrips
    //
    numcommands = 0;
    numglverts  = 0;
    memset(used, 0, sizeof(used));
    for (i = 0; i < model.num_tris; i++) {
        // pick an unused triangle and start the trifan
        if (used[i])
            continue;

        bestlen = 0;
        for (type = 0; type < 2; type++)
        //	type = 1;
        {
            for (startv = 0; startv < 3; startv++) {
                if (type == 1)
                    len = StripLength(i, startv);
                else
                    len = FanLength(i, startv);
                if (len > bestlen) {
                    besttype = type;
                    bestlen  = len;
                    for (j = 0; j < bestlen + 2; j++) {
                        best_st[j]  = strip_st[j];
                        best_xyz[j] = strip_xyz[j];
                    }
                    for (j = 0; j < bestlen; j++)
                        best_tris[j] = strip_tris[j];
                }
            }
        }

        // mark the tris on the best strip/fan as used
        for (j = 0; j < bestlen; j++)
            used[best_tris[j]] = 1;

        if (besttype == 1)
            commands[numcommands++] = (bestlen + 2);
        else
            commands[numcommands++] = -(bestlen + 2);

        numglverts += bestlen + 2;

        for (j = 0; j < bestlen + 2; j++) {
            // emit a vertex into the reorder buffer
            k                                    = best_st[j];

            // emit s/t coords into the commands stream
            s                                    = base_st[k].s;
            t                                    = base_st[k].t;

            s                                    = (s + 0.5) / model.skinwidth;
            t                                    = (t + 0.5) / model.skinheight;

            *(float *)&commands[numcommands++]   = s;
            *(float *)&commands[numcommands++]   = t;
            *(int32_t *)&commands[numcommands++] = best_xyz[j];
        }
    }

    commands[numcommands++] = 0; // end of list marker
}

/*
===============================================================

BASE FRAME SETUP

===============================================================
*/

/*
============
BuildST

Builds the triangle_st array for the base frame and
model.skinwidth / model.skinheight

  FIXME: allow this to be loaded from a file for
  arbitrary mappings
============
*/
void BuildST(triangle_t *ptri, int32_t numtri) {
    int32_t i, j;
    int32_t width, height, iwidth, iheight, swidth;
    float basex, basey;
    float s_scale, t_scale;
    float scale;
    vec3_t mins, maxs;
    float *pbasevert;
    vec3_t vtemp1, vtemp2, normal;

    //
    // find bounds of all the verts on the base frame
    //
    ClearBounds(mins, maxs);

    for (i = 0; i < numtri; i++)
        for (j = 0; j < 3; j++)
            AddPointToBounds(ptri[i].verts[j], mins, maxs);

    for (i = 0; i < 3; i++) {
        mins[i] = floor(mins[i]);
        maxs[i] = ceil(maxs[i]);
    }

    width  = maxs[0] - mins[0];
    height = maxs[2] - mins[2];

    if (!g_fixedwidth) {
        // old style
        scale = 8;
        if (width * scale >= 150)
            scale = 150.0 / width;
        if (height * scale >= 190)
            scale = 190.0 / height;

        s_scale = t_scale = scale;

        iwidth            = ceil(width * s_scale);
        iheight           = ceil(height * t_scale);

        iwidth += 4;
        iheight += 4;
    } else {
        // new style
        iwidth  = g_fixedwidth / 2;
        iheight = g_fixedheight;

        s_scale = (float)(iwidth - 4) / width;
        t_scale = (float)(iheight - 4) / height;
    }

    //
    // determine which side of each triangle to map the texture to
    //
    for (i = 0; i < numtri; i++) {
        VectorSubtract(ptri[i].verts[0], ptri[i].verts[1], vtemp1);
        VectorSubtract(ptri[i].verts[2], ptri[i].verts[1], vtemp2);
        CrossProduct(vtemp1, vtemp2, normal);

        if (normal[1] > 0) {
            basex = iwidth + 2;
        } else {
            basex = 2;
        }
        basey = 2;

        for (j = 0; j < 3; j++) {
            pbasevert            = ptri[i].verts[j];

            triangle_st[i][j][0] = Q_rint((pbasevert[0] - mins[0]) * s_scale + basex);
            triangle_st[i][j][1] = Q_rint((maxs[2] - pbasevert[2]) * t_scale + basey);
        }
    }

    // make the width a multiple of 4; some hardware requires this, and it ensures
    // dword alignment for each scan
    swidth           = iwidth * 2;
    model.skinwidth  = (swidth + 3) & ~3;
    model.skinheight = iheight;
}

// modified version that uses texture coordinates from the file
void BuildST2(triangle_t *ptri, int32_t numtri) {
    int32_t i, j;
    int32_t width = 512;

    for (i = 0; i < numtri; i++) {
        for (j = 0; j < 3; j++) {
            triangle_st[i][j][0] = fmin(width - 1, (int32_t)(width * ptri[i].u[j]));
            triangle_st[i][j][1] = fmin(width - 1, (int32_t)(width * ptri[i].v[j]));
        }
    }

    model.skinwidth  = width;
    model.skinheight = width;
}
/*
=================
Cmd_Base
=================
*/
void Cmd_Base(void) {
    triangle_t *ptri;
    int32_t i, j, k;
    int32_t time1;
    char file1[4000];

    GetToken(false);

    if (g_skipmodel || g_release || g_archive)
        return;

    printf("---------------------\n");
    sprintf(file1, "%s/%s.%s", cdarchive, token, trifileext);
    printf("%s\n", file1);

    ExpandPathAndArchive(file1);

    sprintf(file1, "%s/%s.%s", cddir, token, trifileext);
    printf("%s / %s . %s", cddir, token, trifileext);

    time1 = FileTime(file1);
    if (time1 == -1)
        Error("%s doesn't exist", file1);

    //
    // load the base triangles
    //
    if (do3ds) {
        Load3DSTriangleList(file1, &ptri, &model.num_tris);
        BuildST2(ptri, model.num_tris);
    } else {
        if (dolwo)
            LoadLWOTriangleList(file1, &ptri, &model.num_tris);
        else
            LoadTriangleList(file1, &ptri, &model.num_tris);
    }

    //
    // get the ST values
    //
    BuildST(ptri, model.num_tris);

    //
    // run through all the base triangles, storing each unique vertex in the
    // base vertex list and setting the indirect triangles to point to the base
    // vertices
    //
    for (i = 0; i < model.num_tris; i++) {
        for (j = 0; j < 3; j++) {
            // get the xyz index
            for (k = 0; k < model.num_xyz; k++)
                if (VectorCompare(ptri[i].verts[j], base_xyz[k]))
                    break; // this vertex is already in the base vertex list

            if (k == model.num_xyz) {
                // new index
                VectorCopy(ptri[i].verts[j], base_xyz[model.num_xyz]);
                model.num_xyz++;
            }

            triangles[i].index_xyz[j] = k;

            // get the st index
            for (k = 0; k < model.num_st; k++)
                if (triangle_st[i][j][0] == base_st[k].s && triangle_st[i][j][1] == base_st[k].t)
                    break; // this vertex is already in the base vertex list

            if (k == model.num_st) {
                // new index
                base_st[model.num_st].s = triangle_st[i][j][0];
                base_st[model.num_st].t = triangle_st[i][j][1];
                model.num_st++;
            }

            triangles[i].index_st[j] = k;
        }
    }

    // build triangle strips / fans
    BuildGlCmds();
}

//===============================================================

char *FindFrameFile(char *frame) {
    int32_t time1;
    char file1[2400];
    static char retname[1024];
    char base[32];
    char suffix[32];
    char *s;

    if (strstr(frame, "."))
        return frame; // allready in dot format

    // split 'run1' into 'run' and '1'
    s = frame + strlen(frame) - 1;

    while (s != frame && *s >= '0' && *s <= '9')
        s--;

    strcpy(suffix, s + 1);
    strcpy(base, frame);
    base[s - frame + 1] = 0;

    // check for 'run1.tri'
    sprintf(file1, "%s/%s%s.%s", cddir, base, suffix, trifileext);
    time1 = FileTime(file1);
    if (time1 != -1) {
        sprintf(retname, "%s%s.%s", base, suffix, trifileext);
        return retname;
    }

    // check for 'run.1'
    sprintf(file1, "%s/%s.%s", cddir, base, suffix);
    time1 = FileTime(file1);
    if (time1 != -1) {
        sprintf(retname, "%s.%s", base, suffix);
        return retname;
    }

    Error("frame %s could not be found", frame);
    return NULL;
}

/*
===============
GrabFrame
===============
*/
void GrabFrame(char *frame) {
    triangle_t *ptri;
    int32_t i, j;
    trivert_t *ptrivert;
    int32_t num_tris;
    char file1[4000];
    frame_t *fr;
    vertexnormals_t vnorms[MAX_VERTS];
    int32_t index_xyz;
    char *framefile;

    // the frame 'run1' will be looked for as either
    // run.1 or run1.tri, so the new alias sequence save
    // feature an be used
    framefile = FindFrameFile(frame);

    sprintf(file1, "%s/%s", cdarchive, framefile);
    ExpandPathAndArchive(file1);

    sprintf(file1, "%s/%s", cddir, framefile);

    printf("grabbing %s\n", file1);

    if (model.num_frames >= MAX_FRAMES)
        Error("model.num_frames >= MAX_FRAMES");
    fr = &g_frames[model.num_frames];
    model.num_frames++;

    strcpy(fr->name, frame);

    //
    // load the frame
    //
    if (do3ds)
        Load3DSTriangleList(file1, &ptri, &num_tris);
    else
        LoadTriangleList(file1, &ptri, &num_tris);

    if (num_tris != model.num_tris)
        Error("%s: number of triangles doesn't match base frame\n", file1);

    //
    // allocate storage for the frame's vertices
    //
    ptrivert = fr->v;

    for (i = 0; i < model.num_xyz; i++) {
        vnorms[i].numnormals = 0;
        VectorClear(vnorms[i].normalsum);
    }
    ClearBounds(fr->mins, fr->maxs);

    //
    // store the frame's vertices in the same order as the base. This assumes the
    // triangles and vertices in this frame are in exactly the same order as in the
    // base
    //
    for (i = 0; i < num_tris; i++) {
        vec3_t vtemp1, vtemp2, normal;
        float ftemp;

        VectorSubtract(ptri[i].verts[0], ptri[i].verts[1], vtemp1);
        VectorSubtract(ptri[i].verts[2], ptri[i].verts[1], vtemp2);
        CrossProduct(vtemp1, vtemp2, normal);

        VectorNormalize(normal, normal);

        // rotate the normal so the model faces down the positive x axis
        ftemp     = normal[0];
        normal[0] = -normal[1];
        normal[1] = ftemp;

        for (j = 0; j < 3; j++) {
            index_xyz                = triangles[i].index_xyz[j];

            // rotate the vertices so the model faces down the positive x axis
            // also adjust the vertices to the desired origin
            ptrivert[index_xyz].v[0] = ((-ptri[i].verts[j][1]) * scale_up) +
                                       adjust[0];
            ptrivert[index_xyz].v[1] = (ptri[i].verts[j][0] * scale_up) +
                                       adjust[1];
            ptrivert[index_xyz].v[2] = (ptri[i].verts[j][2] * scale_up) +
                                       adjust[2];

            AddPointToBounds(ptrivert[index_xyz].v, fr->mins, fr->maxs);

            VectorAdd(vnorms[index_xyz].normalsum, normal, vnorms[index_xyz].normalsum);
            vnorms[index_xyz].numnormals++;
        }
    }

    //
    // calculate the vertex normals, match them to the template list, and store the
    // index of the best match
    //
    for (i = 0; i < model.num_xyz; i++) {
        int32_t j;
        vec3_t v;
        float maxdot;
        int32_t maxdotindex;
        int32_t c;

        c = vnorms[i].numnormals;
        if (!c)
            Error("Vertex with no triangles attached");

        VectorScale(vnorms[i].normalsum, 1.0 / c, v);
        VectorNormalize(v, v);

        maxdot      = -BOGUS_RANGE;
        maxdotindex = -1;

        for (j = 0; j < NUMVERTEXNORMALS; j++) {
            float dot;

            dot = DotProduct(v, avertexnormals[j]);
            if (dot > maxdot) {
                maxdot      = dot;
                maxdotindex = j;
            }
        }

        ptrivert[i].lightnormalindex = maxdotindex;
    }

    free(ptri);
}

/*
===============
Cmd_Frame
===============
*/
void Cmd_Frame(void) {
    while (TokenAvailable()) {
        GetToken(false);
        if (g_skipmodel)
            continue;
        if (g_release || g_archive) {
            model.num_frames = 1; // don't skip the writeout
            continue;
        }

        H_printf("#define FRAME_%-16s\t%i\n", token, model.num_frames);

        GrabFrame(token);
    }
}

/*
===============
Cmd_Skin

Skins aren't actually stored in the file, only a reference
is saved out to the header file.
===============
*/
void Cmd_Skin(void) {
    byte *palette;
    byte *pixels;
    int32_t width, height;
    byte *cropped;
    int32_t y;
    char name[4000], savename[4000];

    GetToken(false);

    if (model.num_skins == MAX_MD2SKINS)
        Error("model.num_skins == MAX_MD2SKINS");

    if (g_skipmodel)
        return;

    sprintf(name, "%s/%s.tga", cdarchive, token);
    strcpy(name, ExpandPathAndArchive(name));

    if (TokenAvailable()) {
        GetToken(false);
        sprintf(g_skins[model.num_skins], "%s.tga", token);
        sprintf(savename, "%s%s.tga", gamedir, g_skins[model.num_skins]);
    } else {
        sprintf(savename, "%s/%s.tga", cddir, token);
        sprintf(g_skins[model.num_skins], "%s/%s.tga", cdpartial, token);
    }

    model.num_skins++;

    if (g_skipmodel || g_release || g_archive)
        return;

    /*
    * 
    * **** TEMP UNTIL GL_SCREENSHOT FIXED
    // load the image

    //*********************** Added for LWO support

    //*********************** [KDT]

    printf("loading %s\n", name);
    LoadTGA(name, &pixels, &width, &height);

    // crop it to the proper size
    cropped = malloc(model.skinwidth * model.skinheight);
    for (y = 0; y < model.skinheight; y++) {
        memcpy(cropped + y * model.skinwidth,
               pixels + y * width, model.skinwidth);
    }

    // save off the new image
    printf("saving %s\n", savename);
    CreatePath(savename);
    WritePCXfile(savename, cropped, model.skinwidth,
                 model.skinheight, palette);

    free(pixels);
    free(palette);
    free(cropped);
    */
}

/*
=================
Cmd_Origin
=================
*/
void Cmd_Origin(void) {
    // rotate points into frame of reference so model points down the
    // positive x axis
    GetToken(false);
    adjust[1] = -atof(token);

    GetToken(false);
    adjust[0] = atof(token);

    GetToken(false);
    adjust[2] = -atof(token);
}

/*
=================
Cmd_ScaleUp
=================
*/
void Cmd_ScaleUp(void) {
    GetToken(false);
    scale_up = atof(token);
    if (g_skipmodel || g_release || g_archive)
        return;

    printf("Scale up: %f\n", scale_up);
}

/*
=================
Cmd_Skinsize

Set a skin size other than the default
=================
*/
void Cmd_Skinsize(void) {
    GetToken(false);
    g_fixedwidth = atoi(token);
    GetToken(false);
    g_fixedheight = atoi(token);
}

/*
=================
Cmd_Modelname

Gives a different name/location for the file, instead of the cddir
=================
*/
void Cmd_Modelname(void) {
    GetToken(false);
    strcpy(modelname, token);
}

/*
===============
Cmd_Cd
===============
*/
void Cmd_Cd(void) {
    FinishModel();
    ClearModel();

    GetToken(false);

    // this is a silly mess...
    sprintf(cdpartial, "models/%s", token);
    sprintf(cdarchive, "%smodels/%s", gamedir + strlen(qdir), token);
    sprintf(cddir, "%s%s", gamedir, cdpartial);

    // if -only was specified and this cd doesn't match,
    // skip the model (you only need to match leading chars,
    // so you could regrab all monsters with -only monsters)
    if (!g_only[0])
        return;
    if (strncmp(token, g_only, strlen(g_only))) {
        g_skipmodel = true;
        printf("skipping %s\n", cdpartial);
    }
}
