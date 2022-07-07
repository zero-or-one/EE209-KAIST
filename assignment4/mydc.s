# --------------------------------------------------------------------
# dc.s
#
# Desk Calculator (dc) (x86-64)
# 
# Student ID: 20180824
#
# S Bogom!!!!
# --------------------------------------------------------------------

    .equ   BUFFERSIZE, 32
    .equ   DIFF, 30
    .equ   EOF, -1
    .equ   SIZE, 8
    .equ   TRUE, 1
    .equ   FALSE, 0

# --------------------------------------------------------------------

.section ".rodata"

scanfFormat:
     .asciz "%s"

##print integer in top of stack
printfFormat:
	.asciz "%d\n"

##print error
StdErrorMsg:
    .asciz "dc: stack empty\n"

OverflowErrorMsg:
    .asciz  "dc: overflow happens\n"

DivisionErrorMsg:
    .asciz "dc: divide by zero\n"

# --------------------------------------------------------------------

    .section ".data"

zero:
	.word 0
# --------------------------------------------------------------------

    .section ".bss"

buffer:
    .skip  BUFFERSIZE

stackBase:
    .skip  4
# --------------------------------------------------------------------
    .section ".text"

    # -------------------------------------------------------------
    # int powerfunc(int base, int exponent)
    # Runs the power function.  Returns result.
    # -------------------------------------------------------------
    
    .globl	powerfunc
    .type   powerfunc, @function

    # base is stored in %rdi
    # exponent is stored in %rsi

powerfunc:

    ret            # return result

# -------------------------------------------------------------
# int main(void)
# Runs desk calculator program.  Returns 0.
# -------------------------------------------------------------

    .text
    .globl  main
    .type   main, @function

#------------------------------------------------------------------------------------------
##############
#     main   #
##############

main:

    pushq   %rbp
    movq    %rsp, %rbp

    # char buffer[BUFFERSIZE]
    subq    $BUFFERSIZE, %rsp

    movq    $0, %r12    #keep the size of stack here

    # while (1) {
    main_loop:
    
    # scanf("%s", buffer)
    jmp .input
    input_ret:

    # see if negative
	cmpb		$'_', -BUFFERSIZE(%rbp)
	je		    make_neg

	cmpb		$'0', -BUFFERSIZE(%rbp)
	je		    make_zero
    
    leaq    -BUFFERSIZE(%rbp), %rdi
    movb    $0, %al
    call    atoi
    cmpl    $FALSE, %eax
    je      operator
    
    subq    $8, %rsp

    xor     %rcx, %rcx
    lea     (%rax), %rcx
    
	pushq   %rcx
    inc     %r12

    # go to while loop
    jmp main_loop

#------------------------------------------------------------------------------------------
##############
#    loops   #
##############

.input: #d
    # %al must be set to 0 before scanf call
    leaq    scanfFormat, %rdi
    leaq    -BUFFERSIZE(%rbp), %rsi
    movb    $0, %al
    call    scanf

    # check if user input == EOF
    cmp	    $EOF, %eax
    je	    .quit

    # go to return of input
    jmp input_ret


.quit:	
    # return 0
    movq    $0, %rax
    addq    $BUFFERSIZE, %rsp
    movq    %rbp, %rsp
    popq    %rbp
    ret

#find operand
operator:
	#execute each case

	## if buffer[0]=='+'
	cmp		$'+', -BUFFERSIZE(%rbp)
	je		sum

	## if buffer[0]=='-'
	cmp		$'-', -BUFFERSIZE(%rbp)
	je		diff

	## if buffer[0]=='*'
	cmp		$'*', -BUFFERSIZE(%rbp)
	je		mul

	## if buffer[0]=='^'
	cmp		$'^', -BUFFERSIZE(%rbp)
	je		pow

	## if buffer[0]=='/'
	cmp		$'/', -BUFFERSIZE(%rbp)
	je		dev

	## if buffer[0]=='%'
	cmp		$'%', -BUFFERSIZE(%rbp)
	je		mod

	## if buffer[0]=='_'
	cmp		$'_' , -BUFFERSIZE(%rbp)
	je		make_neg

	## if buffer[0]=='q'
	cmp		$'q', -BUFFERSIZE(%rbp)
	je		.quit

	## if buffer[0]=='p'
	cmp		$'p', -BUFFERSIZE(%rbp)
	je		print_top

	## if buffer[0]=='f'
	cmp	$'f', -BUFFERSIZE(%rbp)
	je		print_all

	## if buffer[0]=='c'
	cmp		$'c', -BUFFERSIZE(%rbp)
	je		clear

	## if buffer[0]=='_'
	#cmp		$'_', -BUFFERSIZE(%rbp)
	#jge		make_neg

	jmp		main_loop


#-------------------------------------------------------------------------------------------
##############
#     ops    #
##############

print:  # to debug
    cmp	    -DIFF(%rbp), %rsp
	je	    std_error
	movq	$printfFormat, %rdi
    movq    -BUFFERSIZE(%rbp), %rsi
    movb    $0, %al
	call 	printf
	jmp 	main_loop


print_top:
    cmp	    $112, (%rsp)    # I could not make it smarter
	je	    std_error
    #popq    %rsi
    movq    (%rsp), %rsi
    movq	$printfFormat, %rdi
    xor     %rax, %rax
	call 	printf
	jmp 	main_loop  


sum:
	cmp     35(%rsp), %rbp  # found by inspection
	jge     std_error

    movl	16(%rsp), %edi
	movl	(%rsp), %esi

	addl     %esi, %edi
    movl    $0, %eax
    addl    %edi, %eax
    jo       overflow_error
    subq    $8, %rsp
	pushq   %rdi
    inc     %r12

	movl    (zero), %edx
	dec     %edx
	movl    %edx, (zero)
    #subq   $8, %rsp
    jmp		main_loop

diff:
	cmp     35(%rsp), %rbp  # found by inspection
	jge     std_error

    movl	16(%rsp), %edi
	movl	(%rsp), %esi

	sub     %esi, %edi
    movl    $0, %eax
    addl    %edi, %eax
    jo       overflow_error
    subq    $8, %rsp
	pushq   %rdi
    inc     %r12

	movl    (zero), %edx
	dec     %edx
	movl    %edx, (zero)
    #subq   $8, %rsp
    jmp		main_loop

mul:
	cmp     35(%rsp), %rbp  # found by inspection
	jge     std_error

    movl	16(%rsp), %edi
	movl	(%rsp), %esi

	imul    %esi, %edi
    movl    %edi, %eax
    movl    $0, %eax
    addl    %edi, %eax
    jo       overflow_error
    subq    $8, %rsp
	pushq   %rdi
    inc     %r12

	movl    (zero), %edx
	dec     %edx
	movl    %edx, (zero)
    #subq   $8, %rsp
    jmp		main_loop

dev:
	cmp     35(%rsp), %rbp  # found by inspection
	jge     std_error

    movl	16(%rsp), %eax
	movl	(%rsp), %esi
    cdq

    cmp     $0, %esi
    je  zero_div_error

	idivl   %esi
    jo  overflow_error
    subq    $8, %rsp
	pushq   %rax
    inc     %r12

	movl    (zero), %edx
	dec     %edx
	movl    %edx, (zero)
    #subq   $8, %rsp
    jmp		main_loop

mod:
	cmp     35(%rsp), %rbp  # found by inspection
	jge     std_error

    movl	16(%rsp), %eax
	movl    (%rsp), %esi
    cdq

    cmp     $0, %esi
    je      zero_div_error

	idivl    %esi
    jo  overflow_error
    subq     $8, %rsp
	pushq    %rdx
    inc     %r12

	movl    (zero), %edx
	dec     %edx
	movl    %edx, (zero)
    #subq   $8, %rsp
    jmp		main_loop

pow:
	cmp     35(%rsp), %rbp  # found by inspection
	jge     std_error

    movl	16(%rsp), %edi  # number
	movl    (%rsp), %esi    # power

	mov     $1, %eax
	xor     %ecx, %ecx

    jmp		pow_loop
    pow_end:
    subq     $8, %rsp
	pushq    %rdi  
    inc     %r12
    jmp		main_loop


pow_loop:
    cmp     %ecx,%esi
    jbe     main_loop

    inc     %ecx
    imul    %edi, %edi
    jmp     pow_end


#------------------------------------------------------------------------------------------
##############
#    extra   #
##############

clear:
    movq %rbp, %rsp
    subq    $BUFFERSIZE, %rsp
    jmp main_loop

make_neg:
	leaq    -BUFFERSIZE(%rbp), %rdi
	inc     %rdi
	call    atoi
    cmpl    $FALSE, %eax
    je      main_loop   
	subq    $8, %rsp
	neg     %rax
	pushq   %rax
    inc     %r12
    jmp     main_loop

make_zero:
	leaq    -BUFFERSIZE(%rbp), %rdi
	call    atoi 
	subq    $8, %rsp
	pushq   %rax
    inc     %r12
    jmp     main_loop

print_all:
	movq    %r12, %r9
	iter:
        leaq    (%rbx,%r9, 4), %rcx
        cmpq    $0, %rcx
		je      iter_end
        movl    (%rcx), %esi
        movb    $0, %al
        movq	$printfFormat, %rdi
		call    printf
		subq    $1, %r9
		jmp     iter
    iter_end:
        movq    %r10, %rsp
        jmp     main_loop


#------------------------------------------------------------------------------------------
##############
#   errors   #
##############
std_error:
    movq	$StdErrorMsg, %rdi
    movb    $0, %al
	call	printf
	jmp		main_loop

zero_div_error:
    movq	$DivisionErrorMsg, %rdi
    movb    $0, %al
    call	printf
    jmp		.quit

overflow_error:
    movq	$OverflowErrorMsg, %rdi
    movb    $0, %al
    call	printf
    jmp		.quit