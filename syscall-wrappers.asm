# ===================================================================================================================================
# syscall_wrappers.asm
#
# Small assmebly functions to set up and perform system calls.  These allow regular user-level code to perform system calls as
# though they were regular functions.
# ===================================================================================================================================



# ===================================================================================================================================
	.Code

	## exit()
	##   Arguments:
	##     a0:  The exit status of the terminating process.
exit:
	mv	a1,	a0			# Move the exit status into a1 (arg[1])
	lw	a0,	SYSCALL_EXIT_CODE	# Load the exit syscall code into a0 (arg[0])
	ecall
	ebreak					# If we reach here, something is terribly wrong.

	## run()
	##   Arguments:
	##     a0:  The ROM number to load into the new process
	##   Returns:
	##     a0:  0 = success, 1 = invalid ROM number, 2 = insufficient RAM, 3 = other error
run:
	mv	a1,	a0			# Move the ROM number into a1 (arg[1])
	lw	a0,	SYSCALL_RUN_CODE	# Load the run syscall code into a0 (arg[0])
ebreak	
ecall
	ret					# The return value of the syscall is the return value of this wrapper function.

	## print()
	##   Arguments:
	##     a0:  The null-terminated string (char array) to print to the console.
print:
	mv	a1,	a0			# Move the string pointer into a1 (arg[1])
	lw	a0,	SYSCALL_PRINT_CODE	# Load the print syscall code into a0 (arg[0])
	ecall
	ret

e_break:
        ebreak
        ret
# ===================================================================================================================================
	


# ===================================================================================================================================
	.Numeric

## The system call codes.
SYSCALL_EXIT_CODE:	0xca110001
SYSCALL_RUN_CODE:	0xca110002
SYSCALL_PRINT_CODE:	0xca110003
# ===================================================================================================================================
