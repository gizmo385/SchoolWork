# Deeper recursion, three levels.  Balanced parens

.data

mainNumFormulas:
        .word 1
mainFormulas:
        .word mainFormula4
mainLengths:
        .word   33

mainFormula4:  .asciiz  "( 19 +(1+(bat/cat) * (xray-sam)))"

mainNewline:
            .asciiz "\n"
mainString:
            .asciiz " -- main\n"
mainAfterString:
            .asciiz "main: after call to parens:\n"
mainNotBalancedStr:
            .asciiz "main: parens reports the formula is Not Balanced\n"
mainBalancedStr:
            .asciiz "main: parens reports the formula is Balanced\n"
mainHyphenString:
            .asciiz "+---------+---------+---------+---------"
.text
main:
         # Function prologue -- even main has one
         addiu $sp, $sp, -24      # allocate stack space -- default of 24 here
         sw    $fp, 0($sp)        # save caller's frame pointer
         sw    $ra, 4($sp)        # save return address
         addiu $fp, $sp, 24       # setup main's frame pointer

         # for ( i = 0; i < mainNumFormulas; i++ )
         #    find length of string
         #    cleanFormula

         addi  $s0, $zero, 0      # $s0 = i = 0
         la    $t0, mainNumFormulas
         lw    $s1, 0($t0)        # $s1 = number of strings
         la    $s4, mainFormulas  # $s4 = addr mainFormulas[0]
mainLoopBegin:
         slt   $t0, $s0, $s1      # $t0 = i < mainNumFormulas
         beq   $t0, $zero, mainLoopEnd

         # print the hyphens
         la    $a0, mainHyphenString
         addi  $v0, $zero, 4
         syscall
         syscall
         la    $a0, mainNewline
         addi  $v0, $zero, 4
         syscall

         # print the string
         lw    $a0, 0($s4)        # $s4 = addr of start of current string
         addi  $v0, $zero, 4
         syscall

         la    $a0, mainString
         addi  $v0, $zero, 4
         syscall
         # print a blank line
         la    $a0, mainNewline
         addi  $v0, $zero, 4
         syscall

         lw    $a0, 0($s4)        # $a0 = addr of string start
         addi  $a1, $zero, 1      # $a1 = parens level, start at 1

         jal   parens

         addi  $t1, $v0, 0        # save return value in $t1

         # print the string
         la    $a0, mainNewline   # print a blank line
         addi  $v0, $zero, 4
         syscall
         la    $a0, mainAfterString
         addi  $v0, $zero, 4
         syscall
         lw    $a0, 0($s4)        # $a0 = addr of formula start
         addi  $v0, $zero, 4
         syscall
         la    $a0, mainNewline
         addi  $v0, $zero, 4
         syscall

         # Did parens return a -1 (not balanced)?
         addi  $t0, $zero, -1
         bne   $t1, $t0, mainPrintBalanced
         la    $a0, mainNotBalancedStr
         addi  $v0, $zero, 4
         syscall
         j     mainAfterBalance
mainPrintBalanced:
         la    $a0, mainBalancedStr
         addi  $v0, $zero, 4
         syscall

mainAfterBalance:
         # print a blank line
         la    $a0, mainNewline
         addi  $v0, $zero, 4
         syscall

         addi  $s0, $s0, 1        # i++
         addi  $s4, $s4, 4        # $s4 = addr of next string
         j     mainLoopBegin

mainLoopEnd:


mainDone:
         # Epilogue for main -- restore stack & frame pointers and return
         lw    $ra, 4($sp)        # get return address from stack
         lw    $fp, 0($sp)        # restore frame pointer of caller
         addiu $sp, $sp, 24       # restore stack pointer of caller
         jr    $ra                # return to caller

printFormula:
         # Function prologue
         addiu $sp, $sp, -24      # allocate stack space -- default of 24 here
         sw    $fp,  0($sp)       # save frame pointer of caller
         sw    $ra,  4($sp)       # save return address
         sw    $a0,  8($sp)       # save $a0 = addr of first char to print
         sw    $a1, 12($sp)       # save $a1 = how many chars to print
         addiu $fp, $sp, 20       # setup frame pointer of printFormula

         # for (i = $a0; i < $a0 + $a1; i++)
         #    print byte

         addi  $t0, $a0, 0        # i = $t0 = start of characters to print
         add   $t1, $a0, $a1      # $t1 = addr of last character to print

printFormulaLoopBegin:
         slt   $t2, $t0, $t1      # $t2 = i < $a0 + $a1
         beq   $t2, $zero, printFormulaLoopEnd

         # print the character
         lb    $a0, 0($t0)
         addi  $v0, $zero, 11
         syscall

         addi  $t0, $t0, 1        # i++
         j     printFormulaLoopBegin

printFormulaLoopEnd:

         # Epilogue for printFormula -- restore stack & frame pointers & return
         lw    $a1, 12($sp)       # restore $a1
         lw    $a0,  8($sp)       # restore $a0
         lw    $ra,  4($sp)       # get return address from stack
         lw    $fp,  0($sp)       # restore frame pointer of caller
         addiu $sp, $sp, 24       # restore stack pointer of caller
         jr    $ra                # return to caller

# Your code goes below this line
.data

notBalancedString:      .asciiz " Not Balanced!"

parensBalanced:         .asciiz " Balanced!"

parensNewline:          .asciiz "\n"

parensLevel:            .asciiz " -- parens level "

leftParen:              .word 40

rightParen:             .word 41

unbalancedFlag:         .word -1

.text
parens:
        # Function prologue
        addiu		    $sp, $sp, -24           # allocate stack space -- default of 24 here
        sw		        $fp,  0($sp)            # save frame pointer of caller
        sw		        $ra,  4($sp)            # save return address
        sw		        $a0,  8($sp)            # save $a0 = address of left parenthesis
        sw		        $a1, 12($sp)            # save $a1 = current recursion depth
        addiu		    $fp, $sp, 20            # setup frame pointer of parens

        # Get arguments
        add             $t0, $zero, $a0         # t0 = address of left parenthesis
        add             $t1, $zero, $a1         # t1 = current recursion depth
        addi            $t2, $zero, 0           # t2 = loop control variable
        la              $t5, leftParen
        lw              $t5, 0($t5)             # t5 = 40 = ASCII value of "("
        la              $t6, rightParen
        lw              $t6, 0($t6)             # t6 = 41 = ASCII value of ")"
        add             $t7, $zero, $zero       # t7 = characters processed

parensLoopBegin:
        # Increment counters
        addi            $t2, $t2, 1             # Increment loop control
        addi            $t7, $t7, 1             # Increment loop control

        # Get character
        add             $t4, $t0, $t2           # address of indexed character
        lb              $t4, 0($t4)             # indexed character (ASCII)

        # Branch conditions
        beq             $t4, $t5, pLeftParen    # if character = "(", branch
        beq             $t4, $t6, pRightParen   # if character = ")", branch
        beq             $t4, $zero, unbalanced  # if character = "\0", branch

        # Default case, jump to the top of the loop
        j               parensLoopBegin

pLeftParen:
        # Load $a1 off stock
        lw              $a1, 12($sp)

        # Set up the stack
        addiu           $sp, $sp, -32
        sw              $t0, 0($sp)
        sw              $t1, 4($sp)
        sw              $t2, 8($sp)
        sw              $t3, 12($sp)
        sw              $t4, 16($sp)
        sw              $t5, 20($sp)
        sw              $t6, 24($sp)
        sw              $t7, 28($sp)

        # Set up the arguments for parens
        add             $a0, $t0, $t2
        addi            $a1, $a1, 1

        # Recursively call parens
        jal             parens

        # Restore the stack
        lw              $t0, 0($sp)
        lw              $t1, 4($sp)
        lw              $t2, 8($sp)
        lw              $t3, 12($sp)
        lw              $t4, 16($sp)
        lw              $t5, 20($sp)
        lw              $t6, 24($sp)
        lw              $t7, 28($sp)
        addiu           $sp, $sp, 32

        # Check return value (v0)
        la              $t9, unbalancedFlag
        lw              $t9, 0($t9)
        beq             $v0, $t9, parensEnd

        # Increment loop control and characters processed
        add             $t2, $t2, $v0
        add             $t7, $t7, $v0

        j               parensLoopBegin

pRightParen:
        # Set up the stack
        addiu           $sp, $sp, -32
        sw              $t0, 0($sp)
        sw              $t1, 4($sp)
        sw              $t2, 8($sp)
        sw              $t3, 12($sp)
        sw              $t4, 16($sp)
        sw              $t5, 20($sp)
        sw              $t6, 24($sp)
        sw              $t7, 28($sp)

        # Set up arguments for printFormula
        addi            $a1, $t2, 1         # a1 = characters processed
        #add            $a1, $zero, $t7     # a1 = characters processed
        add             $a0, $t0, $zero

        # Call printFormula
        jal             printFormula

        # Restore the stack
        lw              $t0, 0($sp)
        lw              $t1, 4($sp)
        lw              $t2, 8($sp)
        lw              $t3, 12($sp)
        lw              $t4, 16($sp)
        lw              $t5, 20($sp)
        lw              $t6, 24($sp)
        lw              $t7, 28($sp)
        addiu           $sp, $sp, 32

        # Grow stack for printing
        addiu           $sp, $sp, -8
        sw              $a0, 0($sp)
        sw              $a1, 4($sp)

        # Print out recursive level information
        la              $a0, parensLevel
        addi            $v0, $zero, 4
        syscall

        add             $a0, $t1, $zero
        addi            $v0, $zero, 1
        syscall

        # Is it level 1?
        addi            $t9, $zero, 1
        bne             $t1, $t9, printNewline

        la              $a0, parensBalanced
        addi            $v0, $zero, 4
        syscall

printNewline:
        la              $a0, parensNewline
        addi            $v0, $zero, 4
        syscall

        # Shrink stack after printing
        addiu           $sp, $sp, 8
        lw              $a0, 0($sp)
        lw              $a1, 4($sp)

        # Put the characters returned into $v0 and return
        add             $v0, $zero, $t7
        j               parensEnd

unbalanced:
        # Set up the stack
        addiu           $sp, $sp, -32
        sw              $t0, 0($sp)
        sw              $t1, 4($sp)
        sw              $t2, 8($sp)
        sw              $t3, 12($sp)
        sw              $t4, 16($sp)
        sw              $t5, 20($sp)
        sw              $t6, 24($sp)
        sw              $t7, 28($sp)

        # Set up arguments for printFormula
        addi            $a1, $t2, 0         # a1 = characters processed
        add             $a0, $t0, $zero

        # Call printFormula
        jal             printFormula

        # Restore the stack
        lw              $t0, 0($sp)
        lw              $t1, 4($sp)
        lw              $t2, 8($sp)
        lw              $t3, 12($sp)
        lw              $t4, 16($sp)
        lw              $t5, 20($sp)
        lw              $t6, 24($sp)
        lw              $t7, 28($sp)
        addiu           $sp, $sp, 32

        # Print out recursive level information
        la              $a0, parensLevel
        addi            $v0, $zero, 4
        syscall

        add             $a0, $t1, $zero
        addi            $v0, $zero, 1
        syscall

        la              $a0, notBalancedString
        addi            $v0, $zero, 4
        syscall

        la              $a0, parensNewline
        addi            $v0, $zero, 4
        syscall

        # Set the flag
        addi            $v0, $zero, -1

parensEnd:
        # Epilogue for parens -- restore stack & frame pointers & return
        lw			    $a1, 12($sp)        # restore $a1
        lw			    $a0,  8($sp)        # restore $a0
        lw			    $ra,  4($sp)        # get return address from stack
        lw			    $fp,  0($sp)        # restore frame pointer of caller
        addiu		    $sp, $sp, 24        # restore stack pointer of caller
        jr			    $ra                 # return to caller
