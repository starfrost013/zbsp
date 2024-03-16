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

// lbmlib.c

#include "cmdlib.h"
#include "imglib.h"

/*
============================================================================

                                                LBM STUFF

============================================================================
*/

typedef uint8_t UBYTE;
// conflicts with windows typedef short			WORD;
typedef uint16_t UWORD;
typedef uint32_t LONG;

typedef enum {
    ms_none,
    ms_mask,
    ms_transcolor,
    ms_lasso
} mask_t;

typedef enum {
    cm_none,
    cm_rle1
} compress_t;

typedef struct
{
    UWORD w, h;
    short x, y;
    UBYTE nPlanes;
    UBYTE masking;
    UBYTE compression;
    UBYTE pad1;
    UWORD transparentColor;
    UBYTE xAspect, yAspect;
    short pageWidth, pageHeight;
} bmhd_t;

extern bmhd_t bmhd; // will be in native byte order

#define FORMID ('F' + ('O' << 8) + ((int32_t)'R' << 16) + ((int32_t)'M' << 24))
#define ILBMID ('I' + ('L' << 8) + ((int32_t)'B' << 16) + ((int32_t)'M' << 24))
#define PBMID  ('P' + ('B' << 8) + ((int32_t)'M' << 16) + ((int32_t)' ' << 24))
#define BMHDID ('B' + ('M' << 8) + ((int32_t)'H' << 16) + ((int32_t)'D' << 24))
#define BODYID ('B' + ('O' << 8) + ((int32_t)'D' << 16) + ((int32_t)'Y' << 24))
#define CMAPID ('C' + ('M' << 8) + ((int32_t)'A' << 16) + ((int32_t)'P' << 24))

bmhd_t bmhd;

int32_t Align(int32_t l) {
    if (l & 1)
        return l + 1;
    return l;
}

/*
================
LBMRLEdecompress

Source must be evenly aligned!
================
*/
byte *LBMRLEDecompress(byte *source, byte *unpacked, int32_t bpwidth) {
    int32_t count;
    byte b, rept;

    count = 0;

    do {
        rept = *source++;

        if (rept > 0x80) {
            rept = (rept ^ 0xff) + 2;
            b    = *source++;
            memset(unpacked, b, rept);
            unpacked += rept;
        } else if (rept < 0x80) {
            rept++;
            memcpy(unpacked, source, rept);
            unpacked += rept;
            source += rept;
        } else
            rept = 0; // rept of 0x80 is NOP

        count += rept;

    } while (count < bpwidth);

    if (count > bpwidth)
        Error("Decompression exceeded width!\n");

    return source;
}

/*
============================================================================

TARGA IMAGE

============================================================================
*/

typedef struct _TargaHeader {
    uint8_t id_length, colormap_type, image_type;
    uint16_t colormap_index, colormap_length;
    uint8_t colormap_size;
    uint16_t x_origin, y_origin, width, height;
    uint8_t pixel_size, attributes;
} TargaHeader;

int32_t fgetLittleShort(FILE *f) {
    byte b1, b2;

    b1 = fgetc(f);
    b2 = fgetc(f);

    return (short)(b1 + b2 * 256);
}

int32_t fgetLittleLong(FILE *f) {
    byte b1, b2, b3, b4;

    b1 = fgetc(f);
    b2 = fgetc(f);
    b3 = fgetc(f);
    b4 = fgetc(f);

    return b1 + (b2 << 8) + (b3 << 16) + (b4 << 24);
}

/*
=============
LoadTGA
=============
*/
void LoadTGA(char *name, byte **pixels, int32_t *width, int32_t *height) {
    int32_t columns, rows, numPixels;
    byte *pixbuf;
    int32_t row, column;
    FILE *fin;
    byte *targa_rgba;
    TargaHeader targa_header;

    fin = fopen(name, "rb");
    if (!fin)
        Error("Couldn't read %s", name);

    targa_header.id_length       = fgetc(fin);
    targa_header.colormap_type   = fgetc(fin);
    targa_header.image_type      = fgetc(fin);

    targa_header.colormap_index  = fgetLittleShort(fin);
    targa_header.colormap_length = fgetLittleShort(fin);
    targa_header.colormap_size   = fgetc(fin);
    targa_header.x_origin        = fgetLittleShort(fin);
    targa_header.y_origin        = fgetLittleShort(fin);
    targa_header.width           = fgetLittleShort(fin);
    targa_header.height          = fgetLittleShort(fin);
    targa_header.pixel_size      = fgetc(fin);
    targa_header.attributes      = fgetc(fin);

    if (targa_header.image_type != 2 && targa_header.image_type != 10)
        Error("LoadTGA %s: Only type 2 and 10 targa RGB images supported\n", name);

    if (targa_header.colormap_type != 0 || (targa_header.pixel_size != 32 && targa_header.pixel_size != 24))
        Error("LoadTGA %s: Only 32 or 24 bit images supported (no colormaps)\n", name);

    columns   = targa_header.width;
    rows      = targa_header.height;
    numPixels = columns * rows;

    if (width)
        *width = columns;
    if (height)
        *height = rows;
    targa_rgba = malloc(numPixels * 4);
    *pixels    = targa_rgba;

    if (targa_header.id_length != 0)
        fseek(fin, targa_header.id_length, SEEK_CUR); // skip TARGA image comment

    if (targa_header.image_type == 2) { // Uncompressed, RGB images
        for (row = rows - 1; row >= 0; row--) {
            pixbuf = targa_rgba + row * columns * 4;
            for (column = 0; column < columns; column++) {
                uint8_t red, green, blue, alphabyte;
                switch (targa_header.pixel_size) {
                case 24:

                    blue      = getc(fin);
                    green     = getc(fin);
                    red       = getc(fin);
                    *pixbuf++ = red;
                    *pixbuf++ = green;
                    *pixbuf++ = blue;
                    *pixbuf++ = 255;
                    break;
                case 32:
                    blue      = getc(fin);
                    green     = getc(fin);
                    red       = getc(fin);
                    alphabyte = getc(fin);
                    *pixbuf++ = red;
                    *pixbuf++ = green;
                    *pixbuf++ = blue;
                    *pixbuf++ = alphabyte;
                    break;
                }
            }
        }
    } else if (targa_header.image_type == 10) { // Runlength encoded RGB images
                                                // qb: set defaults. from AAtools
        uint8_t red = 255, green = 255, blue = 255, alphabyte = 255, packetHeader, packetSize, j;
        for (row = rows - 1; row >= 0; row--) {
            pixbuf = targa_rgba + row * columns * 4;
            for (column = 0; column < columns;) {
                packetHeader = getc(fin);
                packetSize   = 1 + (packetHeader & 0x7f);
                if (packetHeader & 0x80) { // run-length packet
                    switch (targa_header.pixel_size) {
                    case 24:
                        blue      = getc(fin);
                        green     = getc(fin);
                        red       = getc(fin);
                        alphabyte = 255;
                        break;
                    case 32:
                        blue      = getc(fin);
                        green     = getc(fin);
                        red       = getc(fin);
                        alphabyte = getc(fin);
                        break;
                    }

                    for (j = 0; j < packetSize; j++) {
                        *pixbuf++ = red;
                        *pixbuf++ = green;
                        *pixbuf++ = blue;
                        *pixbuf++ = alphabyte;
                        column++;
                        if (column == columns) { // run spans across rows
                            column = 0;
                            if (row > 0)
                                row--;
                            else
                                goto breakOut;
                            pixbuf = targa_rgba + row * columns * 4;
                        }
                    }
                } else { // non run-length packet
                    for (j = 0; j < packetSize; j++) {
                        switch (targa_header.pixel_size) {
                        case 24:
                            blue      = getc(fin);
                            green     = getc(fin);
                            red       = getc(fin);
                            *pixbuf++ = red;
                            *pixbuf++ = green;
                            *pixbuf++ = blue;
                            *pixbuf++ = 255;
                            break;
                        case 32:
                            blue      = getc(fin);
                            green     = getc(fin);
                            red       = getc(fin);
                            alphabyte = getc(fin);
                            *pixbuf++ = red;
                            *pixbuf++ = green;
                            *pixbuf++ = blue;
                            *pixbuf++ = alphabyte;
                            break;
                        }
                        column++;
                        if (column == columns) { // pixel packet run spans across rows
                            column = 0;
                            if (row > 0)
                                row--;
                            else
                                goto breakOut;
                            pixbuf = targa_rgba + row * columns * 4;
                        }
                    }
                }
            }
        breakOut:;
        }
    }
    free(targa_rgba); // qb: memory leak
    fclose(fin);
}
