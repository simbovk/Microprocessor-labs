.data


DATA_10 : .word 10 
REM : .word 0
COUNT_PP : .word 0
FLAG : .byte 0
NUM : .word 232
NUM_COPY : .word 2
.EQU NUMBER_2, 2

PP_NUMBERS : .space 10

.text

.GLOBAL main
.GLOBAL IS_PRIME	
.GLOBAL IS_PALINDROME
.GLOBAL REVERSE

main : 
    PUSH {LR}
    PUSH{R0}
    PUSH{R1}
    PUSH{R2}
    PUSH{R3}
    PUSH{R4}

    ITERATE_NUMBERS:
	
        LDR R0, =NUM
        LDR R1, [R0]

        BL IS_PRIME
        BL IS_PALINDROME

        LDR R0, =FLAG
        LDR R3, [R0]

        MOV R4, #0
        STR R4, [R0]


        MOV R4, R1
        ADD R4, R4, #1
        LDR R0, =NUM
        STR R4, [R0]

        CMP R3, #2
        BNE ITERATE_NUMBERS

        ADD R2, R2, #1
        LDR R0, =PP_NUMBERS
        ADD R0, R0, R2
        STR R1, [R0]
        
        CMP R2, #10
        BNE ITERATE_NUMBERS
    
        BX LR





IS_PRIME : 
    PUSH {LR}
    PUSH {R0}
    PUSH {R1}
    PUSH {R2}
    PUSH {R3}
    PUSH {R4}
    PUSH {R5}
    PUSH {R6}

    MOV R2, #2

    LOOP_IS_PRIME:
        LDR R0, =NUM
        LDR R1, [R0]
        CMP R2, R1
        BEQ IS_PRIME_TRUE
        
        @CALC MOD
        SDIV R3, R1, R2 @R3 = R1 / R2
        MUL R4, R3, R2  @R4 = R2 * R3
        SUB R3, R1, R4  @R3 = R1 mod R2
		ADD R2, R2, #1
		CMP R3, #0
        BNE LOOP_IS_PRIME
        
        @MOV R7, 0
        @STR R7, [FLAG]
		
		POP{R0-R6, LR}
        BX LR



    IS_PRIME_TRUE:  
        MOV R7, 1  
        LDR R0, =FLAG
        STR R7, [R0]

        POP{R0-R6, LR}
        BX LR



IS_PALINDROME:
    PUSH {LR}
    PUSH {R0}
    PUSH {R1}

    BL REVERSE  @PUT REVERSE OF NUM IN R7

    LDR R0, =NUM
    LDR R1, [R0]
    CMP R7, R1
    BEQ IS_PALINDROME_TRUE

    @LDR R0, =FLAG
    @LDR R1, [R0]
    @MOV R7, 0

    POP{R0-R1, LR}
	BX LR


    IS_PALINDROME_TRUE:
        MOV R7, 1
        LDR R0, =FLAG
        LDR R1, [R0]
        ADD R7, R7, R1
        STR R7, [R0] 
        
        POP{R0-R1, LR}
		BX LR
		
		
REVERSE:
    PUSH {LR}
    PUSH {R0}
    PUSH {R1}
    PUSH {R2}
    PUSH {R3}
    PUSH {R4}
    PUSH {R5}
    PUSH {R6}

    MOV R7, #0
    MOV R2, #10

    LDR R0, =NUM
    LDR R1, [R0]

    LOOP_REVERSE:
        @CALC MOD
        SDIV R3, R1, R2 @R3 = R1 / R2
        MUL R4, R3, R2  @R4 = R2 * R3
        SUB R5, R1, R4  @R5 = R1 mod R2

        MOV R1, R3      @R1 = R1 / 10

        MUL R7, R7, R2 @R7 = R7 * 10
        ADD R7, R7, R5  @R7 = R7 + R5


        CMP R1, #0
        BNE LOOP_REVERSE

        POP{R0-R6, LR}
        BX LR

		