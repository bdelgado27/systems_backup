/* =============================================================================================================================== */
/* Avoid multiple inclusion. */

#if !defined (_SYSCALL_WRAPPERS_H)
#define _SYSCALL_WRAPPERS_H
/* =============================================================================================================================== */



/* =============================================================================================================================== */
/* INCLUDES */

#include "types.h"
/* =============================================================================================================================== */



/* =============================================================================================================================== */
/* PROTOTYPES */

/**
 * End of a process.  Perform the `EXIT` system call, providing the exit status code.  This function *does not return*.
 *
 * \param status The exit status code; `0` implies success, non-`0` implies failure.
 */
void exit (word_t status);

/**
 * Start a new process.  Perform the `RUN` system call, using the program given in the ROM number as the executable.
 *
 * \param ROM_number The ROM number to load as an executable image in the new process.  If the system has _n_ ROMs, then this value
 *                   must be between _3_ and _n-1_ (inclusive).
 * \return the result code of the system call.  `0` implies success; `1` implies an invalid ROM number; `2` implies no available
 *         RAM; `3` is used for all other failures.
 */
int run (word_t ROM_number);

/**
 * Print a string.  Perform the `PRINT` system call, providing a pointer in the user-space of this process to a null-terminated
 * string to display on the console.
 *
 * \param msg A pointer to the null-terminated character array to be printed.
 */
void print (char* msg);

void e_break();
/* =============================================================================================================================== */



/* =============================================================================================================================== */
#endif /* _SYSCALL_WRAPPERS_H */
/* =============================================================================================================================== */
