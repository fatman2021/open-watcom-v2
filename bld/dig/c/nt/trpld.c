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
* Description:  Win32 trap file loading.
*
****************************************************************************/


#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "trptypes.h"
#include "trpld.h"
#include "trpsys.h"
#include "tcerr.h"


static HANDLE           TrapFile = 0;
static trap_fini_func   *FiniFunc = NULL;

static TRAPENTRY_FUNC_PTR( InfoFunction );

void TrapTellHWND( HWND hwnd )
{
    TRAPENTRY_PTR_NAME( InfoFunction )( hwnd );
}

void KillTrap( void )
{
    ReqFunc = NULL;
    TRAPENTRY_PTR_NAME( InfoFunction ) = NULL;
    if( FiniFunc != NULL ) {
        FiniFunc();
        FiniFunc = NULL;
    }
    if( TrapFile != 0 ) {
        FreeLibrary( TrapFile );
        TrapFile = 0;
    }
}

char *LoadTrap( const char *parms, char *buff, trap_version *trap_ver )
{
    char                trpfile[256];
    const char          *ptr;
    char                *dst;
    bool                have_ext;
    char                chr;
    trap_init_func      *init_func;

    if( parms == NULL || *parms == '\0' )
        parms = "std";
    have_ext = false;
    dst = trpfile;
    for( ptr = parms; *ptr != '\0' && *ptr != TRAP_PARM_SEPARATOR; ++ptr ) {
        chr = *ptr;
        switch( chr ) {
        case ':':
        case '/':
        case '\\':
            have_ext = false;
            break;
        case '.':
#ifdef USE_FILENAME_VERSION
            *dst++ = ( USE_FILENAME_VERSION / 10 ) + '0';
            *dst++ = ( USE_FILENAME_VERSION % 10 ) + '0';
#endif
            have_ext = true;
            break;
        }
        *dst++ = chr;
    }
    if( !have_ext ) {
#ifdef USE_FILENAME_VERSION
        *dst++ = ( USE_FILENAME_VERSION / 10 ) + '0';
        *dst++ = ( USE_FILENAME_VERSION % 10 ) + '0';
#endif
        *dst++ = '.';
        *dst++ = 'd';
        *dst++ = 'l';
        *dst++ = 'l';
    }
    *dst = '\0';
    TrapFile = LoadLibrary( trpfile );
    if( TrapFile == NULL ) {
        sprintf( buff, TC_ERR_CANT_LOAD_TRAP, trpfile );
        return( buff );
    }
    init_func = (trap_init_func *)GetProcAddress( TrapFile, (LPSTR)1 );
    FiniFunc = (trap_fini_func *)GetProcAddress( TrapFile, (LPSTR)2 );
    ReqFunc = (trap_req_func *)GetProcAddress( TrapFile, (LPSTR)3 );
    TRAPENTRY_PTR_NAME( InfoFunction ) = TRAPENTRY_PTR_CAST( InfoFunction )GetProcAddress( TrapFile, (LPSTR)4 );
    strcpy( buff, TC_ERR_WRONG_TRAP_VERSION );
    if( init_func != NULL && FiniFunc != NULL && ReqFunc != NULL
      && TRAPENTRY_PTR_NAME( InfoFunction ) != NULL ) {
        parms = ptr;
        if( *parms != '\0' )
            ++parms;
        *trap_ver = init_func( parms, buff, trap_ver->remote );
        if( buff[0] == '\0' ) {
            if( TrapVersionOK( *trap_ver ) ) {
                TrapVer = *trap_ver;
                return( NULL );
            }
            strcpy( buff, TC_ERR_WRONG_TRAP_VERSION );
        }
    }
    KillTrap();
    return( buff );
}
