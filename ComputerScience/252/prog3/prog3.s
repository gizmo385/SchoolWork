.data
numbers:
         .word   9
         .word -17
         .word  14
         .word   9
         .word -23
         .word   8
         .word -23
         .word   9
         .word  14
         .word -17
         .word   9
numNumbers:
         .word  11
findNums:
         .word  19
         .word   8
         .word   9
         .word   0
numFindNums:
         .word   4

# Your code goes below this line
newline:
        .asciiz "\n"

foundItem:
        .asciiz "Found "

atLocation:
        .asciiz " at location "


notFoundAndNewline:
        .asciiz " not found\n"

.text
main:
      	# Function prologue -- even main has one
        addiu           $sp, $sp, -24           # allocate stack space -- default of 24 here
        sw              $fp, 0($sp)             # save caller's frame pointer
        sw              $ra, 4($sp)             # save return address
        addiu           $fp, $sp, 20            # setup main's frame pointer

        #Intialize the loop data
        addi            $s0, $zero, 0           # Outer loop control variable
        addi            $s1, $zero, 0           # Inner loop control variable

        la              $t2, numFindNums        # Address of numFindNums
        lw              $t2, 0($t2)             # Value of numFindNums

        la              $t3, numNumbers         # Address of numNumbers
        lw              $t3, 0($t3)             # Value of numNumbers

        la              $t0, numbers            # Address of numbers
        la              $t1, findNums           # Address of find nums


outerLoopBegin:
        # The outer loop will loop over every item in numbers

        # Check if we need to exit the outer loop
        beq             $s0, $t2, outerLoopEnd  # Exit outer loop if the index is equal to the number of elements

        # Calculate the index for the next element in numbers
        add             $s3, $zero, $s0         # Put $s0 (outer loop control) into $s3
        add             $s3, $s3, $s3           # $s3 = 2 * $s0
        add             $s3, $s3, $s3           # $s3 = 4 * $s0
        add             $s3, $s3, $t1           # Gets current address: $s3 = (4 * $s0) + $t1

        # Load value at the calculated index
        lw              $t4, 0($s3)

        # Set a variable to mark how many times the item has been found
        addi            $s7, $zero, 0           # The item has been found 0 times at the start of the loop

        # Create/reset inner loop control
        addi            $s1, $zero, 0           # Inner loop control variable

innerLoopBegin:
        # The inner loop will check the number loaded in the outer loop against the numbers in findNums
        beq             $s1, $t3, innerLoopEnd  # Exit inner loop if index is equal to the number of elements

        # Calculate i for findNums[i]
        add             $s4, $zero, $s1         # Put $s1 (inner loop control) into $s4
        add             $s4, $s4, $s4           # $s4 = 2 * $s1
        add             $s4, $s4, $s4           # $s4 = 4 * $s1
        add             $s4, $s4, $t0           # Gets current address: $s5 = (4 * $s1) + $t0

        # Load the value in find nums
        lw              $t5, 0($s4)             # Load value in findNums into $t5

        # Check to see if we've found an instance of the value in numbers
        bne             $t4, $t5, incrementInnerLoop

        # Print out that we found the value
        la              $a0, foundItem          # "Found "
        addi            $v0, $zero, 4
        syscall

        add             $a0, $zero, $t4         # item
        addi            $v0, $zero, 1
        syscall

        la              $a0, atLocation         # " at location "
        addi            $v0, $zero, 4
        syscall

        add             $a0, $zero, $s1         # Index of item
        addi            $v0, $zero, 1
        syscall

        la              $a0, newline            # "\n"
        addi            $v0, $zero, 4
        syscall

        # Increment the amount of times the item was found
        addi            $s7, $s7, 1

incrementInnerLoop:
        addi            $s1, $s1, 1             # Add 1 to the inner loop control
        j               innerLoopBegin          # Jump back up to inner loop


innerLoopEnd:
        # Check if we're printing out that we didn't find the value
        bne             $s7, $zero, incrementAndJump

        add             $a0, $zero, $t4         # item
        addi            $v0, $zero, 1
        syscall

        la              $a0, notFoundAndNewline # " not found\n"
        addi            $v0, $zero, 4
        syscall

incrementAndJump:
        addi            $s0, $s0, 1             # Increment outer loop control
        j               outerLoopBegin          # Jump back up to outer loop


outerLoopEnd:
        # Basically just a fallthrough point

done:
      	# Epilogue for main -- restore stack & frame pointers and return
        lw              $ra, 4($sp)             # get return address from stack
        lw              $fp, 0($sp)             # restore the caller's frame pointer
        jr              $ra                     # return to caller's code
        addiu           $sp, $sp, 24            # restore the caller's stack pointer

