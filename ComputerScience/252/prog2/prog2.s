.data
 
forward:
         .byte  1
backward:
         .byte  1
 
numbers:
        .word  17
        .word -50
        .word   3
        .word -23
        .word -60
        .word  97
        .word -82
        .word  53
        .word  30
        .word -17
        .word 142
        .word  93
        .word  -1
        .word -298374
        .word   5
        .word   1
        .word -999
 
# Your code goes below this line
sentinel:
		.word -999

newline:
		.asciiz "\n"
		
forwardMessage:
		.asciiz "The integers in order:\n"
		
backwardMessage:
		.asciiz "The integers in backwards order:\n"

.text

main:
		# Function prologue -- even main has one
		addiu 			$sp, $sp, -24  			# allocate stack space -- default of 24 here
		sw    			$fp, 0($sp)    			# save caller's frame pointer
		sw    			$ra, 4($sp)    			# save return address
		addiu 			$fp, $sp, 20   			# setup main's frame pointer

checkPrintForward:
		#Check if we are printing forwards
		la				$t0, forward			# Load address of forward into $t0
		lb				$t0, 0($t0)				# Load value of forward into $t0
		addi			$t1, $zero, 1			# Put 1 into $t1
		bne				$t0, $t1, checkBWPrint	# If we aren't printing forwards, jump over the loop body
		
		#Print message
		la				$a0, forwardMessage		# Load address of forward message into $a0
		addi			$v0, $zero, 4			# Syscall print_string
		syscall
		
		#Load data
		la				$s0, numbers			# Load address of numbers into $s0
		addi			$s0, $s0, -4			# Start before the numbers because of how loop condition checks
		la				$s1, sentinel			# Load the address of the sentinel into $s1
		lw				$s1, 0($s1)				# Load the value of sentinel into $s1
		
	
beginForwardLoop:
		#Increment and check looping condition
		addi			$s0, $s0, 4				# Add 4 to the address
		lw				$a0, 0($s0)				# Load the value of $s0 into $a0
		beq				$a0, $s1, endFWLoop		# If we've reached the sentinel address, break out of the loop
		
		#Load value and print
		addi			$v0, $zero, 1			# Set the syscall value for print_int
		syscall									# Print the number
		
		#Load and print newline
		la				$a0, newline			#Load the address of newline into $a0
		addi			$v0, $zero, 4			#Set the syscall value for print_string
		syscall									#print
		
		#Jump back up to the top of the loop
		j				beginForwardLoop		# Jump back to the top of the loop
		
endFWLoop:
		#Print delimiting newline if necessary
		la				$t3, backward			# Load address for backward into $t3
		lb				$t3, 0($t3)				# Load the value of backward into $t3
		addi			$t4, $zero, 1			# Put 1 in the register $t4
		bne				$t3, $t4, done		# Skip loop and newline if we aren't printing newlines
		
		#Load and print newline
		la				$a0, newline			# Load the address of newline into $a0
		addi			$v0, $zero, 4			# Set the syscall value for print_string
		syscall	

checkBWPrint:
		#Check if we are printing backwards
		la				$t0, backward			# Load address of backward into $t0
		lb				$t0, 0($t0)				# Load value of backward into $t0
		addi			$t1, $zero, 1			# Put 1 into $t1
		bne				$t0, $t1, done			# If we aren't printing backwards, jump over the loop body
		
		#Print message
		la				$a0, backwardMessage	# Load address of backward message into $a0
		addi			$v0, $zero, 4			# Syscall print_string
		syscall

loadScanForwardData:
		#Load data
		la				$s0, numbers			# Load address of numbers into $s0
		addi			$s0, $s0, -4			# Start before the numbers because of how loop condition checks
		la				$s1, sentinel			# Load the address of the sentinel into $s1
		lw				$s1, 0($s1)				# Load the value of sentinel into $s1
		
scanForwardsLoop:
		#Increment and check looping condition
		addi			$s0, $s0, 4				# Add 4 to the address
		lw				$a0, 0($s0)				# Load the value of $s0 into $a0
		beq				$a0, $s1, loadBWData	# If we've reached the sentinel address, break out of the loop
		
		#Jump back up to the top of the loop
		j				scanForwardsLoop		# Jump back to the top of the loop
		
loadBWData:
		#Load data
		la				$s1, numbers			# Load the address of numbers
		addi			$s1, $s1, -4			# Shift the address in $s0
	
beginBWLoop:
		#Increment and check looping condition
		addi			$s0, $s0, -4			# Shift the address
		beq				$s0, $s1, done			# If we've reached the sentinel address, break out of the loop
		
		#Load value and print
		lw				$a0, 0($s0)				# Load the value at $s0 into $a0
		addi			$v0, $zero, 1			# Set the syscall value for print_int
		syscall									# Print the number
		
		#Load and print newline
		la				$a0, newline			# Load the address of newline into $a0
		addi			$v0, $zero, 4 			# Set the syscall value for print_string
		syscall									# Print
		
		#Jump back up to the top of the loop
		j				beginBWLoop				# Jump back to the top of the loop
	
done:    
		# Epilogue for main -- restore stack & frame pointers and return
		lw 				$ra, 4($sp)    			# get return address from stack
		lw 				$fp, 0($sp)    			# restore the caller's frame pointer
		jr 				$ra            			# return to caller's code
		addiu 			$sp, $sp, 24   			# restore the caller's stack pointer
