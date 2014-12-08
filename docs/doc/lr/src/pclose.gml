.func pclose
.synop begin
#include <stdio.h>
int pclose( FILE *fp );
.synop end
.desc begin
The
.id &func.
function closes the pipe associated with
.arg fp
and waits for the subprocess created by
.kw popen
to terminate.
.desc end
.return begin
The
.id &func.
function returns the termination status of the command
language interpreter.
If an error occured,
.id &func.
returns (-1) with
.kw errno
set appropriately.
.return end
.error begin
.begterm 12
:DTHD.Constant
:DDHD.Meaning
.term EINTR
The
.id &func.
function was interrupted by a signal while waiting for the
child process to terminate.
.term ECHILD
The
.id &func.
function was unable to obtain the termination status of the
child process.
.endterm
.error end
.see begin
.seelist &function. perror pipe popen
.see end
.seexmp _popen
.class POSIX 1003.1
.system
