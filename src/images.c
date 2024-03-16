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

char mip_prefix[32]; // directory to dump the textures in

/*
==============
Cmd_Grab

$grab filename x y width height
==============
*/
void Cmd_Grab(void) {
    int32_t xl, yl, w, h, y;
    byte *cropped;
    char savename[2400];
    char dest[1200];

    GetToken(false);

    if (token[0] == '/' || token[0] == '\\')
        sprintf(savename, "%s%s.tga", gamedir, token + 1);
    else
        sprintf(savename, "%spics/%s.tga", gamedir, token);

    if (g_release) {
        if (token[0] == '/' || token[0] == '\\')
            sprintf(dest, "%s.tga", token + 1);
        else
            sprintf(dest, "pics/%s.tga", token);

        ReleaseFile(dest);
        return;
    }

    GetToken(false);
    xl = atoi(token);
    GetToken(false);
    yl = atoi(token);
    GetToken(false);
    w = atoi(token);
    GetToken(false);
    h = atoi(token);

    if (xl < 0 || yl < 0 || w < 0 || h < 0 || xl + w > byteimagewidth || yl + h > byteimageheight)
        Error("GrabPic: Bad size: %i, %i, %i, %i", xl, yl, w, h);

    // crop it to the proper size
    cropped = malloc(w * h);
    for (y = 0; y < h; y++) {
        memcpy(cropped + y * w, byteimage + (y + yl) * byteimagewidth + xl, w);
    }

    // save off the new image
    printf("***NOT IMPLEMENTED YET - FIX WHEN GL_ScreenShot FIXED **** saving %s\n", savename);
    CreatePath(savename);
    //WritePCXfile(savename, cropped, w, h, lbmpalette);
    
    free(cropped);
}

/*
==============
Cmd_Raw

$grab filename x y width height
==============
*/
void Cmd_Raw(void) {
    int32_t xl, yl, w, h, y;
    byte *cropped;
    char savename[2100];
    char dest[1200];

    GetToken(false);

    sprintf(savename, "%s%s.lmp", gamedir, token);

    if (g_release) {
        sprintf(dest, "%s.lmp", token);
        ReleaseFile(dest);
        return;
    }

    GetToken(false);
    xl = atoi(token);
    GetToken(false);
    yl = atoi(token);
    GetToken(false);
    w = atoi(token);
    GetToken(false);
    h = atoi(token);

    if (xl < 0 || yl < 0 || w < 0 || h < 0 || xl + w > byteimagewidth || yl + h > byteimageheight)
        Error("GrabPic: Bad size: %i, %i, %i, %i", xl, yl, w, h);

    // crop it to the proper size
    cropped = malloc(w * h);
    for (y = 0; y < h; y++) {
        memcpy(cropped + y * w, byteimage + (y + yl) * byteimagewidth + xl, w);
    }

    // save off the new image
    printf("saving %s\n", savename);
    CreatePath(savename);

    SaveFile(savename, cropped, w * h);

    free(cropped);
}

/*
===============
Cmd_Mipdir
===============
*/
void Cmd_TextureDir(void) {
    char filename[1100];

    GetToken(false);
    strcpy(mip_prefix, token);
    // create the directory if needed
    sprintf(filename, "%stextures", gamedir);
    Q_mkdir(filename);
    sprintf(filename, "%stextures/%s", gamedir, mip_prefix);
    Q_mkdir(filename);
}

