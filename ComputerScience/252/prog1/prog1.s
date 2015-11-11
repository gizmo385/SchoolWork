.data
	print:			.word 1
	multiply: 		.word 1
	subtraction: 	.word 1
	
	able: 			.half 16408
	golf:			.word -287
	kilo: 			.byte 42
	lima: 			.byte 92
	xray: 			.word 72
	zulu: 			.byte -17
	
    #Your code goes below this line
	newline:		.asciiz "\n"
	space:			.asciiz " "
	doubleSpace:	.asciiz "  "
	printText:		.asciiz "Printing the six values:"
	multiplyText:	.asciiz "\"Multiplying\" each value by 6:"
	subtractText:	.asciiz	"Subtracting all the values from zero: "
	
.text

main:
    # Function prologue -- even main has one
    addiu 			$sp, $sp, -24  			# allocate stack space -- default of 24 here
    sw    			$fp, 0($sp)    			# save caller's frame pointer
    sw    			$ra, 4($sp)    			# save return address
    addiu 			$fp, $sp, 20   			# setup main's frame pointer

	# The only thing we should need to do is check printing. The rest follows
	j	checkPrint							# Check if we need to print

checkPrint:
	# Check if printing is going to be performed
	la				$t0, print				# $t0 has the address of the print mode 		
	lw				$t1, 0($t0)				# $t1 now holds the print mode
	addi			$t2, $zero, 1			# $t2 now holds the value 1
	bne				$t1, $t2, checkMultiply	# If the print mode is not 1, move on to check multiplication
	
printNums:
	#Print the premable
	la				$a0, printText			# $t0 now has the address of printText
	addi			$v0, $zero, 4			# sycall value for print_string
	syscall
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall

	# Print the value in able
	la				$t0, able				# Point to the number
	lh				$a0, 0($t0)				# $a0 now holds the value of able
	addi			$v0, $zero, 1			# syscall value for print_int
	syscall
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall
	
	# Print the value in golf
	la				$t0, golf				# Point to the number
	lw				$a0, 0($t0)				# $a0 now has the value of golf
	addi			$v0, $zero, 1			# syscall value for print_int
	syscall
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall
	
	# Print the value in kilo
	la				$t0, kilo				# Point to the number
	lb				$a0, 0($t0)				# $a0 now holds the value of kilo
	addi			$v0, $zero, 1			# syscall value for print_int
	syscall
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall
	
	# Print the value in lima
	la				$t0, lima				# Point to the number
	lb				$a0, 0($t0)				# $a0 now holds the value of lima
	addi			$v0, $zero, 1			# syscall value for print_int
	syscall
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall
	
	
	# Print the value in xray
	la				$t0, xray				# Point to the number
	lw				$a0, 0($t0)				# $a0 now holds the value of xray
	addi			$v0, $zero, 1			# syscall value for print_int
	syscall
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall
	
	# Print the value in zulu
	la				$t0, zulu				# Point to the number
	lb				$a0, 0($t0)				# $a0 now holds the value of zulu
	addi			$v0, $zero, 1			# syscall value for print_int
	syscall
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall
	
checkMultiply:
	# Check if multiplication is going to be performed
	la				$t0, multiply			# $t0 has the address of the multiplication mode
	lw				$t1, 0($t0)				# $t1 now holds the multiplication mode
	addi			$t2, $zero, 1			# $t2 now holds the value 1
	bne				$t1, $t2, checkSubtract	# If the multiplication mode is not 1, move on to subtraction
	
multiplyNums:
	#Print the premable
	la				$a0, multiplyText		# $t0 now has the address of multiplyText
	addi			$v0, $zero, 4			# sycall value for print_string
	syscall
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall
	
	# Multiply able by 6
	la				$s0, able				# $s0 now holds the address of able
	lh				$s1, 0($s0)				# $s1 now hows the value in able
	lh				$s2, 0($s0)				# $s2 now also holds the value of able
	add				$s3, $s2, $s1			# $s3 now holds the value of $s2 + $s1
	add				$s4, $s3, $s1			# $s4 now holds the value of $s3 + $s1
	add				$s5, $s4, $s1			# $s5 now holds the value of $s4 + $s1
	add				$s6, $s5, $s1			# $s6 now holds the value of $s5 + $s1
	add				$s7, $s6, $s1			# $s7 now holds the value of $s6 + $s1
	
	# Print able
	add				$a0, $zero, $s7			# $a0 now holds the value of $s7
	addi			$v0, $zero, 1			# syscall value for print_int
	syscall
	
	#Print space characters
	la				$a0, doubleSpace		# Loads the address of doubleSpace into $a0
	addi			$v0, $zero, 4			# syscall value for print_string
	syscall
	
	# Multiply golf by 6
	la				$s0, golf				# $s0 now holds the address of golf
	lw				$s1, 0($s0)				# $s1 now hows the value in golf
	lw				$s2, 0($s0)				# $s2 now also holds the value of golf
	add				$s3, $s2, $s1			# $s3 now holds the value of $s2 + $s1
	add				$s4, $s3, $s1			# $s4 now holds the value of $s3 + $s1
	add				$s5, $s4, $s1			# $s5 now holds the value of $s4 + $s1
	add				$s6, $s5, $s1			# $s6 now holds the value of $s5 + $s1
	add				$s7, $s6, $s1			# $s7 now holds the value of $s6 + $s1
	
	# Print golf
	addi			$a0, $s7, 0				# $a0 now holds the value of $s7
	addi			$v0, $zero, 1			# syscall value for print_int
	syscall
	
	#Print space characters
	la				$a0, doubleSpace		# Loads the address of doubleSpace into $a0
	addi			$v0, $zero, 4			# syscall value for print_string
	syscall
	
	# Multiply kilo by 6
	la				$s0, kilo				# $s0 now holds the address of kilo
	lb				$s1, 0($s0)				# $s1 now hows the value in kilo
	lb				$s2, 0($s0)				# $s2 now also holds the value of kilo
	add				$s3, $s2, $s1			# $s3 now holds the value of $s2 + $s1
	add				$s4, $s3, $s1			# $s4 now holds the value of $s3 + $s1
	add				$s5, $s4, $s1			# $s5 now holds the value of $s4 + $s1
	add				$s6, $s5, $s1			# $s6 now holds the value of $s5 + $s1
	add				$s7, $s6, $s1			# $s7 now holds the value of $s6 + $s1
	
	# Print kilo
	addi			$a0, $s7, 0				# $a0 now holds the value of $s7
	addi			$v0, $zero, 1			# syscall value for print_int
	syscall
	
	#Print space characters
	la				$a0, doubleSpace		# Loads the address of doubleSpace into $a0
	addi			$v0, $zero, 4			# syscall value for print_string
	syscall
	
	# Multiply lima by 6
	la				$s0, lima				# $s0 now holds the address of lima
	lb				$s1, 0($s0)				# $s1 now hows the value in lima
	lb				$s2, 0($s0)				# $s2 now also holds the value of lima
	add				$s3, $s2, $s1			# $s3 now holds the value of $s2 + $s1
	add				$s4, $s3, $s1			# $s4 now holds the value of $s3 + $s1
	add				$s5, $s4, $s1			# $s5 now holds the value of $s4 + $s1
	add				$s6, $s5, $s1			# $s6 now holds the value of $s5 + $s1
	add				$s7, $s6, $s1			# $s7 now holds the value of $s6 + $s1
	
	# Print lima
	addi			$a0, $s7, 0				# $a0 now holds the value of $s7
	addi			$v0, $zero, 1			# syscall value for print_int
	syscall
	
	#Print space characters
	la				$a0, doubleSpace		# Loads the address of doubleSpace into $a0
	addi			$v0, $zero, 4			# syscall value for print_string
	syscall
	
	# Multiply xray by 6
	la				$s0, xray				# $s0 now holds the address of xray
	lw				$s1, 0($s0)				# $s1 now hows the value in xray
	lw				$s2, 0($s0)				# $s2 now also holds the value of xray
	add				$s3, $s2, $s1			# $s3 now holds the value of $s2 + $s1
	add				$s4, $s3, $s1			# $s4 now holds the value of $s3 + $s1
	add				$s5, $s4, $s1			# $s5 now holds the value of $s4 + $s1
	add				$s6, $s5, $s1			# $s6 now holds the value of $s5 + $s1
	add				$s7, $s6, $s1			# $s7 now holds the value of $s6 + $s1
	
	# Print xray
	addi			$a0, $s7, 0				# $a0 now holds the value of $s7
	addi			$v0, $zero, 1			# syscall value for print_int
	syscall
	
	#Print space characters
	la				$a0, doubleSpace		# Loads the address of doubleSpace into $a0
	addi			$v0, $zero, 4			# syscall value for print_string
	syscall
	
	# Multiply zulu by 6
	la				$s0, zulu				# $s0 now holds the address of zulu
	lb				$s1, 0($s0)				# $s1 now hows the value in zulu
	lb				$s2, 0($s0)				# $s2 now also holds the value of zulu
	add				$s3, $s2, $s1			# $s3 now holds the value of $s2 + $s1
	add				$s4, $s3, $s1			# $s4 now holds the value of $s3 + $s1
	add				$s5, $s4, $s1			# $s5 now holds the value of $s4 + $s1
	add				$s6, $s5, $s1			# $s6 now holds the value of $s5 + $s1
	add				$s7, $s6, $s1			# $s7 now holds the value of $s6 + $s1
	
	# Print zulu
	addi			$a0, $s7, 0				# $a0 now holds the value of $s7
	addi			$v0, $zero, 1			# syscall value for print_int
	syscall
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall
	
checkSubtract:
	# Check if subtractions is going to be performed
	la				$t0, subtraction		# $t0 has the address of the subtraction mode
	lw				$t1, 0($t0)				# $t1 now holds the subtraction mode
	addi			$t2, $zero, 1			# $t2 now holds the value 1
	bne				$t1, $t2, done			# If we are not performing subtract, finish
	
subtractNums:
	#Print the premable
	la				$a0, subtractText		# $t0 now has the address of subtractText
	addi			$v0, $zero, 4			# sycall value for print_string
	syscall
	
	# Load able
	la				$s0, able				# $s0 has the address of able
	lh				$s1, 0($s0)				# $s1 now holds the value of able
	
	# Load golf
	la				$s0, golf				# $s0 now has the address of golf
	lw				$s2, 0($s0)				# $s2 now holds the value of golf
	
	# Load kilo
	la				$s0, kilo				# $s0 now has the address of kilo
	lb				$s3, 0($s0)				# $s2 now holds the value of kilo
	
	# Load lima
	la				$s0, lima				# $s0 now has the address of lima
	lb				$s4, 0($s0)				# $s2 now holds the value of lima
	
	# Load xray
	la				$s0, xray				# $s0 now has the address of xray
	lw				$s5, 0($s0)				# $s2 now holds the value of xray
	
	# Load zulu
	la				$s0, zulu				# $s0 now has the address of zulu
	lb				$s6, 0($s0)				# $s2 now holds the value of zulu
	
	# Subtract values
	sub				$s7, $zero, $s1			# Subtract able from zero
	sub				$s7, $s7, $s2			# Subtract golf from that
	sub				$s7, $s7, $s3			# subtract kilo from that
	sub				$s7, $s7, $s4			# subtract lima from that
	sub				$s7, $s7, $s5			# subtract xray from that
	sub				$s7, $s7, $s6			# subtract zulu from that
	
	# Print answer
	add				$a0, $zero, $s7			# Copy the value in $s7 into $a0
	addi			$v0, $zero, 1			# sycall value for print_int
	syscall
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall	
	
	# Print newline character
	la				$a0, newline			# Point to the newline
	addi			$v0, $zero, 4			#sycall value for print_string
	syscall
	
	
done:    
	# Epilogue for main -- restore stack & frame pointers and return
    lw 				$ra, 4($sp)    			# get return address from stack
    lw 				$fp, 0($sp)    			# restore the caller's frame pointer
    jr 				$ra            			# return to caller's code
    addiu 			$sp, $sp, 24   			# restore the caller's stack pointer


