# check if integer is even or odd
.data
msg: .asciiz "enter integer:"
even: .asciiz "\nNumber is even"
odd: .asciiz "\nNumber is odd"

.text
main:
    # Print prompt message
    li $v0, 4
    la $a0, msg
    syscall
    
    # Read integer
    li $v0, 5
    syscall
    
    # Check if even or odd (bitwise AND with 1)
    andi $t0, $v0, 1
    
    # Print result
    li $v0, 4
    beq $t0, $zero, even_label
    la $a0, odd
    j end_label
    
even_label:
    la $a0, even
    
end_label:
    syscall
    
    # Exit
    li $v0, 10
    syscall
