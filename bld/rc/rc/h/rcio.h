/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#ifndef RCIO_INCLUDED
#define RCIO_INCLUDED

#include <stdio.h>
#include "wresall.h"
#include "rctypes.h"
#include "semstr.h"
#include "sharedio.h"

typedef struct FullFontDirEntry {
    struct FullFontDirEntry *Next;
    struct FullFontDirEntry *Prev;
    FontDirEntry            Entry;
} FullFontDirEntry;

typedef struct FullFontDir {
    FullFontDirEntry        *Head;
    FullFontDirEntry        *Tail;
    uint_16                 NumOfFonts;
} FullFontDir;

typedef struct RcResFileID {
    char                    *filename;
    FullStringTable         *StringTable;
    FullStringTable         *ErrorTable;
    bool                    IsWatcomRes     : 1;
    bool                    IsOpen          : 1;
    uint_16                 NextCurOrIcon;
    WResDir                 dir;                    /* don't write this if !IsWatcomRes */
    WResFileID              fid;                    /* file I/O handle */
    FullFontDir             *FontDir;
} RcResFileID;

/**** Text file input ****/
typedef struct LogicalFileInfo {
    char                    *Filename;
    int                     LineNum;
    bool                    IsCOrHFile;
} LogicalFileInfo;

extern char         *RcMakeTmpInSameDir( const char *dirfile, char id, const char *ext );
extern bool         RcPass1IoInit( void );
extern void         RcPass1IoShutdown( void );
extern bool         RcPass2IoInit( void );
extern void         RcPass2IoShutdown( bool noerror );
extern void         RcIoTextInputInit( void );
extern bool         RcIoTextInputShutdown( void );
extern bool         RcIoPushTextInputFile( const char *filename );
extern bool         RcIoPopTextInputFile( void );
extern int          RcIoGetChar( void );
extern void         RcIoOverrideIsCOrHFlag( void );
extern void         RcIoSetIsCOrHFlag( void );
extern const LogicalFileInfo *RcIoGetLogicalFileInfo( void );
extern bool         RcIoIsCOrHFile( void );
extern void         RcIoSetLogicalFileInfo( int linenum, const char *filename );
extern WResFileID   RcIoOpenInput( const char *filename, bool text_mode );
extern int          RcFindResource( const char *name, char *fullpath );
extern const char   *RcGetEnv( const char *name );

#endif
