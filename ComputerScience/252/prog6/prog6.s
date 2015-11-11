# test with only one batter and only computes the batting average

.data

mainNumBatters:
   .word 1

mainBatter1:
   .word  27  # walks
   .word 101  # singles
   .word  22  # doubles
   .word   4  # triples
   .word  10  # home runs
   .word 423  # outs


mainNewline:
         .asciiz  "\n"
mainBatterNumber:
         .asciiz  "Batter number: "
mainBattingAverage:
         .asciiz  "Batting average: "
mainSluggingPercentage:
         .asciiz  "Slugging percentage: "
mainOnbasePercentage:
         .asciiz  "On-base percentage: "

.text

main:
         # Function prologue -- even main has one
         addiu      $sp, $sp, -24     # allocate stack space -- default of 24 here
         sw         $fp, 0($sp)       # save frame pointer of caller
         sw         $ra, 4($sp)       # save return address
         addiu      $fp, $sp, 20      # setup frame pointer of main

         # for (i = 0; i < mainNumBatters; i++)
         #    compute batting average
         #    compute slugging average

         la         $s0, mainNumBatters
         lw         $s7, 0($s0)       # $s7 = number of batters
         addi       $s6, $zero, 0     # $s6 = i = 0
         la         $s0, mainBatter1  # $s0 = addr of current batter's stats

mainLoopBegin:
         slt        $t0, $s6, $s7     # $t0 = i < number of batters
         beq        $t0, $zero, mainDone

         la         $a0, mainBatterNumber
         addi       $v0, $zero, 4
         syscall
         addi       $a0, $s6, 1
         addi       $v0, $zero, 1
         syscall
         la         $a0, mainNewline
         addi       $v0, $zero, 4
         syscall

         # Compute the batting average
         addi       $a0, $zero, 2      # $a0 = 1 = compute batting average
         #addi       $a0, $zero, 1      # $a0 = 1 = compute batting average
         lw         $a1,   0($s0)      # $a1 = walks
         lw         $a2,   4($s0)      # $a2 = singles
         lw         $a3,   8($s0)      # $a3 = doubles
         lw         $s1,  12($s0)      # $s1 = triples
         lw         $s2,  16($s0)      # $s2 = home runs
         lw         $s3,  20($s0)      # $s3 = outs

         sw         $s3,  -4($sp)      # put outs at top of average's stack
         sw         $s2,  -8($sp)      # put homeruns 2nd fm top of average's stack
         sw         $s1, -12($sp)      # put triples 3rd fm top of average's stack
         jal        average

         # Print the batting average
         mtc1       $v0, $f12          # get result fm $v0 before we print string
         la         $a0, mainBattingAverage
         addi       $v0, $zero, 4
         syscall
         addi       $v0, $zero, 2      # print the average
         syscall
         la         $a0, mainNewline
         addi       $v0, $zero, 4
         syscall

         addi       $s6, $s6, 1       # i++
         addi       $s0, $s0, 24      # $s0 = addr of next batter's stats
         j          mainLoopBegin

mainDone:
         # Epilogue for main -- restore stack & frame pointers and return
         lw         $ra, 4($sp)       # get return address from stack
         lw         $fp, 0($sp)       # restore frame pointer for caller
         addiu      $sp, $sp, 24      # restore frame pointer for caller
         jr         $ra               # return to caller

.data
printStatsOuts:
         .asciiz "Outs:      "
printStatsWalks:
         .asciiz "Walks:     "
printStatsSingles:
         .asciiz "Singles:   "
printStatsDoubles:
         .asciiz "Doubles:   "
printStatsTriples:
         .asciiz "Triples:   "
printStatsHomeruns:
         .asciiz "Home runs: "
printStatsNewline:
         .asciiz "\n"

.text
printStats:
         # Function prologue
         addiu      $sp, $sp, -32     # allocate stack space
         sw         $a3, 20($sp)      # save $a0 thru $a3
         sw         $a2, 16($sp)
         sw         $a1, 12($sp)
         sw         $a0, 8($sp)
         sw         $ra, 4($sp)       # save return address
         sw         $fp, 0($sp)       # save frame pointer of caller
         addiu      $fp, $sp, 28      # setup frame pointer of average

         # printStats expects to find the following:
         # $a0 = walks
         # $a1 = singles
         # $a2 = doubles
         # $a3 = triples
         # 5th argument = homeruns
         # 6th argument = outs

         # print the outs
         la         $a0, printStatsOuts
         addi       $v0, $zero, 4
         syscall
         lw         $a0, 0($fp)       # the outs are at the top of our stack
         addi       $v0, $zero, 1
         syscall
         la         $a0, printStatsNewline
         addi       $v0, $zero, 4
         syscall

         # print the walks
         la         $a0, printStatsWalks
         addi       $v0, $zero, 4
         syscall
         lw         $a0, 8($sp)       # the walks were passed in $a0
         addi       $v0, $zero, 1
         syscall
         la         $a0, printStatsNewline
         addi       $v0, $zero, 4
         syscall

         # print the singles
         la         $a0, printStatsSingles
         addi       $v0, $zero, 4
         syscall
         addi       $a0, $a1, 0        # the singles were passed in $a1
         addi       $v0, $zero, 1
         syscall
         la         $a0, printStatsNewline
         addi       $v0, $zero, 4
         syscall

         # print the doubles
         la         $a0, printStatsDoubles
         addi       $v0, $zero, 4
         syscall
         addi       $a0, $a2, 0        # the doubles were passed in $a2
         addi       $v0, $zero, 1
         syscall
         la         $a0, printStatsNewline
         addi       $v0, $zero, 4
         syscall

         # print the triples
         la         $a0, printStatsTriples
         addi       $v0, $zero, 4
         syscall
         addi       $a0, $a3, 0        # the doubles were passed in $a3
         addi       $v0, $zero, 1
         syscall
         la         $a0, printStatsNewline
         addi       $v0, $zero, 4
         syscall

         # print the homeruns
         la         $a0, printStatsHomeruns
         addi       $v0, $zero, 4
         syscall
         lw         $a0, -4($fp)       # the homeruns are 4 bytes below the top of our stack
         addi       $v0, $zero, 1
         syscall
         la         $a0, printStatsNewline
         addi       $v0, $zero, 4
         syscall

printStatsDone:
         # Epilogue for printStats -- restore stack & frame pointers and return
         lw         $ra, 4($sp)       # get return address from stack
         lw         $fp, 0($sp)       # restore frame pointer for caller
         addiu      $sp, $sp, 32      # restore frame pointer for caller
         jr         $ra               # return to caller

# Your code goes below this line
average:
        # Computes the average for baseball stats
        # Function prologue
        addiu       $sp, $sp, -36           # allocate stack space
        sw          $a3, 20($sp)            # save $a0 thru $a3
        sw          $a2, 16($sp)
        sw          $a1, 12($sp)
        sw          $a0, 8($sp)
        sw          $ra, 4($sp)             # save return address
        sw          $fp, 0($sp)             # save frame pointer of caller
        addiu       $fp, $sp, 28            # setup frame pointer of average

        # Set up arguments for a call to printStats
        lw          $a0, 12($sp)            # a0 = walks
        lw          $a1, 16($sp)            # a1 = singles
        lw          $a2, 20($sp)            # a2 = doubles
        lw          $a3, 24($sp)            # a3 = triples

        # Put outs and homeruns into registers
        lw          $t4, 32($sp)
        lw          $t6, 28($sp)

        # Put 5th and 6th argument on the stack
        sw          $t4, -4($sp)            # Put outs at the top of printStats stack
        sw          $t6, -8($sp)            # Put homeruns 2nd from the top of printStats stack

        # Call print stats
        jal         printStats

        # Load values from stack
        lwc1        $f2, 12($sp)            # Number of walks
        lwc1        $f3, 16($sp)            # Number of single
        lwc1        $f4, 20($sp)            # Number of doubls
        lwc1        $f5, 24($sp)            # Number of triples
        lwc1        $f6, 28($sp)            # Number of homeruns
        lwc1        $f8, 32($sp)            # Number of outs

        # Convert integers to floats
        cvt.s.w     $f2, $f2
        cvt.s.w     $f3, $f3
        cvt.s.w     $f4, $f4
        cvt.s.w     $f5, $f5
        cvt.s.w     $f6, $f6
        cvt.s.w     $f8, $f8

        # Calculate number of hits
        add.s       $f7, $f3,$f4
        add.s       $f7, $f7,$f5
        add.s       $f7, $f7,$f6            # f7 now has number of hits

        # Determine what we're doing and jump to the appropriate label
        lw          $t0, 8($sp)             # Get $a0 from the stack

        # Test function mode
        addi        $t7, $zero, 1
        beq         $t0, $t7, averageBattingAverage

        addi        $t7, $zero, 2
        beq         $t0, $t7, averageSlugPercentage

        addi        $t7, $zero, 3
        beq         $t0, $t7, averageOnBase

# Calculates the batting average
averageBattingAverage:
        # Calculate the at-bats
        add.s       $f9, $f7, $f8

        # Set the return value to zero by default
        mtc1        $zero, $f10
        cvt.s.w     $f10, $f10

        # Prevent division by zero
        cvt.w.s     $f9, $f9
        mfc1        $t7, $f9
        cvt.s.w     $f9, $f9
        beq         $t7, $zero, averageBattingAverageSkipDivision

        # Compute the batting average. hits / at-bats
        div.s       $f10, $f7, $f9

averageBattingAverageSkipDivision:
        # Move the result of the division into $v0 (standard return register)
        mfc1        $v0, $f10

        # Jumps to the end of the function
        j           averageDone

# Calculates the slug percentage
averageSlugPercentage:
        # Set up multiplication scalars
        addi        $t0, $zero, 2
        addi        $t1, $zero, 3
        addi        $t2, $zero, 4

        mtc1        $t0, $f11               # f11 = 2
        mtc1        $t1, $f12               # f12 = 3
        mtc1        $t2, $f13               # f13 = 4

        cvt.s.w     $f11, $f11
        cvt.s.w     $f12, $f12
        cvt.s.w     $f13, $f13

        mtc1        $zero, $f10             # Default return value
        mtc1        $zero, $f17             # f17 = 0

        cvt.s.w     $f10, $f10
        cvt.s.w     $f17, $f17

        # Multiply register values by scalars
        add.s       $f17, $f17, $f3         # f17 = singles

        mul.s       $f11, $f11, $f4         # f11 = doubles * 2
        add.s       $f17, $f17, $f11        # f17 = singles + (doubles * 2)

        mul.s       $f12, $f12, $f5         # f12 = (triples * 3)
        add.s       $f17, $f17, $f12        # f17 = singles + (doubles * 2) + (triples * 3)

        mul.s       $f13, $f13, $f6         # f13 = (homeruns * 4)
        add.s       $f17, $f17, $f13        # f17 = singles + (doubles * 2) + (triples * 3) + (homeruns * 4)

        # Calculate the at-bats
        add.s       $f9, $f7, $f8

        # Prevent division by zero
        mfc1        $t7, $f9
        beq         $t7, $zero, averageSlugPercentageSkipDivision

        # Perform division
        div.s       $f10, $f17, $f9

averageSlugPercentageSkipDivision:
        # Move the result of the division into $v0 (standard return register)
        mfc1        $v0, $f10

        # Jumps to the end of the function
        j           averageDone

# Calculates the on-base percentange
averageOnBase:
        # Calculate hits + walks
        add.s       $f11, $f7, $f2

        # Calculate atBats + walks
        add.s       $f12, $f7, $f8
        add.s       $f12, $f12, $f2

        # Put zero into $f10 by default
        mtc1        $zero, $f10
        cvt.s.w     $f10, $f10

        # Prevent division by zero
        mfc1        $t7, $f12
        beq         $t7, $zero, averageOnBaseSkipDivision

        # (hits + walks) / (atBats + walks)
        div.s       $f10, $f11, $f12

averageOnBaseSkipDivision:
        # Move the result of the division into $v0 (standard return register)
        mfc1        $v0, $f10

        # Jumps to the end of the function
        j           averageDone

averageDone:
        lw         $ra, 4($sp)                  # get return address from stack
        lw         $fp, 0($sp)                  # restore frame pointer for caller
        addiu      $sp, $sp, 36                 # restore frame pointer for caller
        jr         $ra                          # return to caller
