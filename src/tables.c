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

/*
=============================================================================

ALPHALIGHT GENERATION

Find alphamap values that best match modulated lightmap values

This isn't used anymore, but I'm keeping it around...
=============================================================================
*/

uint16_t alphamap[32 * 32 * 32];

void Alphalight_Thread(int32_t i) {
    int32_t j;
    float r, g, b;
    float mr, mg, mb, ma;
    float distortion, bestdistortion;
    float v;

    r              = (i >> 10) * (1.0 / 16);
    g              = ((i >> 5) & 31) * (1.0 / 16);
    b              = (i & 31) * (1.0 / 16);

    bestdistortion = 999999;
    for (j = 0; j < 16 * 16 * 16 * 16; j++) {
        mr         = (j >> 12) * (1.0 / 16);
        mg         = ((j >> 8) & 15) * (1.0 / 16);
        mb         = ((j >> 4) & 15) * (1.0 / 16);
        ma         = (j & 15) * (1.0 / 16);

        v          = r * 0.5 - (mr * ma + 0.5 * (1.0 - ma));
        distortion = v * v;
        v          = g * 0.5 - (mg * ma + 0.5 * (1.0 - ma));
        distortion += v * v;
        v = b * 0.5 - (mb * ma + 0.5 * (1.0 - ma));
        distortion += v * v;

        distortion *= 1.0 + ma * 4;

        if (distortion < bestdistortion) {
            bestdistortion = distortion;
            alphamap[i]    = j;
        }
    }
}

void Cmd_Alphalight(void) {
    char savename[2080];

    GetToken(false);

    if (g_release) {
        ReleaseFile(token);
        return;
    }

    sprintf(savename, "%s%s", gamedir, token);
    printf("Building alphalight table...\n");

    RunThreadsOnIndividual(32 * 32 * 32, true, Alphalight_Thread);

    SaveFile(savename, (byte *)alphamap, sizeof(alphamap));
}
