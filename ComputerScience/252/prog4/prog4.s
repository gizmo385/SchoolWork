.data

mainNumPhrases:
                        .word  4

mainPhrases:
                        .word  mainPhrase1
                        .word  mainPhrase2
                        .word  mainPhrase3
                        .word  mainPhrase4

mainPhrase1:
                        .asciiz "abcdefghijklmnfjdlafdkjalopqrstuvwxyz"
mainPhrase2:
                        .asciiz "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdeabcdeABCDEXYXYXYZz"
mainPhrase3:
                        .asciiz "1234567890-=`~!@#$%^&*()_+[]\{}|';:,./?><"
mainPhrase4:
                        .ascii  "I have sworn upon the altar of God eternal hostility "
                        .ascii  "against every form of tyranny over the mind of man. "
                        .asciiz "--  Thomas Jefferson."


# Your code goes below this line

newline:                .asciiz "\n"

lowercaseAlphabet:
                        .asciiz "a:   "
                        .asciiz "b:   "
                        .asciiz "c:   "
                        .asciiz "d:   "
                        .asciiz "e:   "
                        .asciiz "f:   "
                        .asciiz "g:   "
                        .asciiz "h:   "
                        .asciiz "i:   "
                        .asciiz "j:   "
                        .asciiz "k:   "
                        .asciiz "l:   "
                        .asciiz "m:   "
                        .asciiz "n:   "
                        .asciiz "o:   "
                        .asciiz "p:   "
                        .asciiz "q:   "
                        .asciiz "r:   "
                        .asciiz "s:   "
                        .asciiz "t:   "
                        .asciiz "u:   "
                        .asciiz "v:   "
                        .asciiz "w:   "
                        .asciiz "x:   "
                        .asciiz "y:   "
                        .asciiz "z:   "

# Create an array of 26 numbers for character count
characterCount:
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0
                        .word       0

# Debugging purposes only, should never be printed
sentinel:               .word       77

.text

main:
            # Function prologue -- even main has one
            addiu 			$sp, $sp, -24  			# allocate stack space -- default of 24 here
            sw    			$fp, 0($sp)    			# save caller's frame pointer
            sw    			$ra, 4($sp)    			# save return address
            addiu 			$fp, $sp, 20   			# setup main's frame pointer

            # Load important addresses and values
            la              $s2, mainPhrases        # $s2 = address of mainPhrases
            la              $s3, mainNumPhrases
            lw              $s3, 0($s3)             # $s3 = number of phrases
            la              $s4, characterCount     # $s4 = address of character count

            # Set up loop control variables
            addi            $s0, $zero, 0           # $s0 = outer loop control

            # The outer loop will loop over the phrases
outerLoopBegin:
            # Check if we've iterated over all of the words
            beq             $s0, $s3, done

            # Calculate location for next phrase
            add             $t0, $zero, $s0         # Copy the outer loop control
            sll             $t0, $t0, 2             # Multiply $t0 by 4
            add             $t1, $s2, $t0           # Get address of phrase

            # Load next phrase
            lw              $t2, 0($t1)             # $t2 has next phrase

            # Print the phrase
            add             $a0, $zero, $t2
            addi            $v0, $zero, 4
            syscall

            #Print newline
            la              $a0, newline
            addi            $v0, $zero, 4
            syscall

            addi            $s1, $zero, 0           # $s1 = inner loop control

            # The inner loop will iterate over the characters in the phrase
innerLoopBegin:
            # Calculate the location for the next character
            add             $s5, $t2, $s1
            lb              $s5, 0($s5)

            # Test if the character is the null character (0)
            beq             $s5, $zero, innerLoopEnd

            # Shift the values down so that A is 0 instead of 65
            addi            $s5, $s5, -65

            # Check if the value is still greater than 25
            slti            $s6, $s5, 26

            # If it isn't, jump over the next segment which shifts down by another 32
            bne             $s6, $zero, skipConversion

            # Shift down by another 32. a = 97. After first shift, a=32. Now, a=0. So (A|a) -> 0
            addi            $s5, $s5, -32

            #Is it less than zero? If it is, skip counting
            slti            $s6, $s5, 0
            bne             $s6, $zero, skipCounting

skipConversion:
            #Is it less than 26 (0-25)? Skip counting if it isn't.
            slti            $t9, $s5, 26
            beq             $t9, $zero, skipCounting

            #Is it less than zero? Skip counting if it is
            slti            $t9, $s5, 0
            bne             $t9, $zero, skipCounting

            # Load the associated count for this character
            sll             $s7, $s5, 2             # (ASCII char value * 4)
            add             $s7, $s4, $s7           # (Beginning of count array) + (ASCII char value * 5)
            lw              $s8, 0($s7)

            # Increment the count
            addi            $s8, $s8, 1

            # Store the count back into it's proper location
            sw              $s8, 0($s7)

skipCounting:
            # Jump back up to the loop
            addi            $s1, $s1, 1             # Increment inner loop control
            j               innerLoopBegin          # Jump back up to the top


innerLoopEnd:
            addi            $t7, $zero, 0           # Loop control for character print loop
            addi            $t9, $zero, 26
            la              $t6, lowercaseAlphabet  # Address of the lowercase alphabet

characterPrintLoopBegin:
            # Check for the end of the loop
            beq             $t7, $t9, characterPrintLoopEnd

            # Print "<character>:   "
            sll             $t8, $t7, 2             # Multiply $t7 by 4
            add             $t8, $t8, $t7           # Add $t7 to ($t7 * 4)
            add             $t8, $t8, $t7           # Add $t7 to ($t7 * 5)
            add             $a0, $t8, $t6           # Location of next letter
            addi            $v0, $zero, 4
            syscall

            # Print character count for letter
            add             $a0, $zero, $s4         # Base address of characterCount
            sll             $t8, $t7, 2             # Multiply loop control by 4
            add             $a0, $a0, $t8           # Add the loop control and base address
            lw              $a0, 0($a0)             # Load the value at the location
            addi            $v0, $zero, 1           # Print the integer
            syscall

            #Print newline
            la              $a0, newline
            addi            $v0, $zero, 4
            syscall

            addi            $t7, $t7, 1
            j               characterPrintLoopBegin

characterPrintLoopEnd:
            # Increment outer loop control and jump back up to the top
            addi            $s0, $s0, 1
            j               outerLoopBegin

done:
		    # Epilogue for main -- restore stack & frame pointers and return
		    lw 				$ra, 4($sp)    			# get return address from stack
		    lw 				$fp, 0($sp)    			# restore the caller's frame pointer
		    jr 				$ra            			# return to caller's code
            addiu 			$sp, $sp, 24   			# restore the caller's stack pointer



