# Q2: Find min, max, and their average (as float) - FIXED SIMPLIFIED VERSION

.data
array: .word 0:30           # Array for up to 30 integers
prompt: .asciiz "Enter integer (negative to stop): "
min_msg: .asciiz "\nMinimum: "
max_msg: .asciiz "\nMaximum: "
avg_msg: .asciiz "\nAverage: "

.text
main:
    la $s0, array           # $s0 = array base address
    li $s1, 0               # $s1 = element count
    
input_loop:
    # Print prompt
    li $v0, 4
    la $a0, prompt
    syscall
    
    # Read integer
    li $v0, 5
    syscall
    
    # Check if negative (stop)
    bltz $v0, find_minmax
    
    # Store in array
    sw $v0, 0($s0)
    addi $s0, $s0, 4        # Next position
    addi $s1, $s1, 1        # Count++
    
    # Check if reached 30 elements
    li $t0, 30
    blt $s1, $t0, input_loop
    
find_minmax:
    # Load first element as initial min and max
    la $s0, array           # Reset pointer to start
    lw $s2, 0($s0)          # $s2 = min
    lw $s3, 0($s0)          # $s3 = max
    
    li $t0, 1               # Start from index 1
    addi $s0, $s0, 4        # Move pointer to array[1] ← FIX!
    
minmax_loop:
    bge $t0, $s1, calculate_avg
    
    lw $t1, 0($s0)          # Load current element ← Now correct!
    
    # Update min if needed
    blt $t1, $s2, update_min
    # Update max if needed
    bgt $t1, $s3, update_max
    j next
    
update_min:
    move $s2, $t1
    bgt $t1, $s3, update_max
    j next
    
update_max:
    move $s3, $t1
    
next:
    addi $s0, $s0, 4        # Next element
    addi $t0, $t0, 1        # i++
    j minmax_loop

calculate_avg:
    # Print min
    li $v0, 4
    la $a0, min_msg
    syscall
    li $v0, 1
    move $a0, $s2
    syscall
    
    # Print max
    li $v0, 4
    la $a0, max_msg
    syscall
    li $v0, 1
    move $a0, $s3
    syscall
    
    # Calculate average: (min + max) / 2.0
    add $t0, $s2, $s3       # sum = min + max
    mtc1 $t0, $f12          # Move to float register
    cvt.s.w $f12, $f12      # Convert to float
    
    li.s $f2, 2.0           # Load 2.0 directly
    div.s $f12, $f12, $f2   # average = sum / 2.0
    
    # Print average
    li $v0, 4
    la $a0, avg_msg
    syscall
    li $v0, 2               # Print float syscall
    syscall
    
exit:
    li $v0, 10
    syscall