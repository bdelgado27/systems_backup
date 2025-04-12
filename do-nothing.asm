# A test assembly program that does not much.

	.Code

#	li	t0,	13
#	addi	sp,	sp,	-4
#	sw	t0,	0(sp)
#	lw	a1,	0(sp)
#	li	a0,	0xca110001	# Set the EXIT syscall code
	call print
	ecall
	
	

