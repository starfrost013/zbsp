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

#include "qbsp.h"

// huffman.c : Huffman encoding

typedef struct hnode_s {
    int32_t count;
    qboolean used;
    int32_t children[2];
} hnode_t;

int32_t numhnodes;
hnode_t hnodes[512];
unsigned charbits[256];
int32_t charbitscount[256];

int32_t SmallestNode(void) {
    int32_t i;
    int32_t best, bestnode;

    best = 99999999;
    bestnode = -1;
    for (i = 0; i < numhnodes; i++) {
        if (hnodes[i].used)
            continue;
        if (!hnodes[i].count)
            continue;
        if (hnodes[i].count < best) {
            best = hnodes[i].count;
            bestnode = i;
        }
    }

    if (bestnode == -1)
        return -1;

    hnodes[bestnode].used = true;
    return bestnode;
}

void BuildChars(int32_t nodenum, unsigned bits, int32_t bitcount) {
    hnode_t* node;

    if (nodenum < 256) {
        if (bitcount > 32)
            Error("bitcount > 32");
        charbits[nodenum] = bits;
        charbitscount[nodenum] = bitcount;
        return;
    }

    node = &hnodes[nodenum];
    bits <<= 1;
    BuildChars(node->children[0], bits, bitcount + 1);
    bits |= 1;
    BuildChars(node->children[1], bits, bitcount + 1);
}

/*
==================
Huffman
==================
*/
cblock_t Huffman(cblock_t in) {
    int32_t i;
    hnode_t* node;
    int32_t outbits, c;
    unsigned bits;
    byte* out_p;
    cblock_t out;
    int32_t max;

    // count
    memset(hnodes, 0, sizeof(hnodes));
    for (i = 0; i < in.count; i++)
        hnodes[in.data[i]].count++;

    // normalize counts
    max = 0;
    for (i = 0; i < 256; i++) {
        if (hnodes[i].count > max) {
            max = hnodes[i].count;
        }
    }
    if (max == 0)
        Error("Huffman: max == 0");

    for (i = 0; i < 256; i++) {
        hnodes[i].count = (hnodes[i].count * 255 + max - 1) / max;
    }

    // build the nodes
    numhnodes = 256;
    while (numhnodes != 511) {
        node = &hnodes[numhnodes];

        // pick two lowest counts
        node->children[0] = SmallestNode();
        if (node->children[0] == -1)
            break; // no more

        node->children[1] = SmallestNode();
        if (node->children[1] == -1) {
            if (node->children[0] != numhnodes - 1)
                Error("Bad smallestnode");
            break;
        }
        node->count = hnodes[node->children[0]].count +
            hnodes[node->children[1]].count;
        numhnodes++;
    }

    BuildChars(numhnodes - 1, 0, 0);

    out_p = out.data = malloc(in.count * 2 + 1024);
    memset(out_p, 0, in.count * 2 + 1024);

    // write count
    *out_p++ = in.count & 255;
    *out_p++ = (in.count >> 8) & 255;
    *out_p++ = (in.count >> 16) & 255;
    *out_p++ = (in.count >> 24) & 255;

    // save out the 256 normalized counts so the tree can be recreated
    for (i = 0; i < 256; i++)
        *out_p++ = hnodes[i].count;

    // write bits
    outbits = 0;
    for (i = 0; i < in.count; i++) {
        c = charbitscount[in.data[i]];
        bits = charbits[in.data[i]];
        while (c) {
            c--;
            if (bits & (1 << c))
                out_p[outbits >> 3] |= 1 << (outbits & 7);
            outbits++;
        }
    }

    out_p += (outbits + 7) >> 3;

    out.count = out_p - out.data;

    return out;
}