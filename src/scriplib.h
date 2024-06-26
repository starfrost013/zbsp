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

// scriplib.h
#pragma once

#include "cmdlib.h"

#define MAXTOKEN 512

extern char token[MAXTOKEN];
extern char *scriptbuffer, *script_p, *scriptend_p;
extern int32_t grabbed;
extern int32_t scriptline;
extern qboolean endofscript;

extern char brush_info[2000];
void MarkBrushBegin();

void LoadScriptFile(char *filename);
void ParseFromMemory(char *buffer, int32_t size);

qboolean GetToken(qboolean crossline);
void UnGetToken(void);
qboolean TokenAvailable(void);
