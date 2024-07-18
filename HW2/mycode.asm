.Model MEDIUM
.STACK 64
.DATA

DATA_IN DW 181 
COPY_DATA_IN DW 181
DATA_10 DW 10   
REM DW 0  
COUNT_PP DW 10  
FLAG DB 0 
NUM DW 2  
NUM_COPY DW 2 
NUMBER_2 EQU 2    

PP_NUMBERS 10 DUP(?)
         

.CODE

MAIN PROC FAR
     MOV AX, @DATA
     MOV DS, AX   
     
MAIN_LOOP:
     
     ;MOV AX, NUM    
     CALL IS_PRIME      
     MOV SI, OFFSET FLAG
     MOV [SI], AX   
     MOV SI, OFFSET NUM  
     MOV AX, NUM_COPY
     MOV [SI], AX
     CALL IS_PALINDROME 
     MOV SI, OFFSET FLAG    
     MOV BX, [SI]
     ADD BX, AX
     MOV [SI], BX  
     MOV AX, [SI] 
     
     CMP AX, NUMBER_2
     JE INC_COUNT_PP 
CONTINUE_LABEL:   
     
     MOV SI, OFFSET NUM_COPY
     MOV CX, [SI]
     INC CX
     MOV [SI], CX    
     
     MOV SI, OFFSET NUM
     MOV [SI], CX
          
     CMP COUNT_PP, 0 
     JNE MAIN_LOOP
     
     
     
     
     
     MOV AH, 4CH  
          
     INT 21H
MAIN ENDP   


IS_PRIME PROC     
      
     MOV CX, 02H
       
LOOP_CHECK:  
     MOV AX, NUM   
     CMP CX, AX
     JE RETURN_TRUE
     SUB DX, DX
     DIV CX  
     INC CX
     CMP DX, 0H   
     JNE LOOP_CHECK   
     MOV AX, 0H
     RET 
         
 
     
IS_PRIME ENDP  




IS_PALINDROME PROC  
     CALL REVERSE
     MOV AX, NUM_COPY
     CMP AX, BX  
     JE RETURN_TRUE
     MOV AX, 0           
     RET             

IS_PALINDROME ENDP 

REVERSE PROC
    MOV AX, NUM   
    SUB BX, BX
   
LOOP_REVERSE:   
    SUB DX, DX
    DIV DATA_10   
    MOV SI, OFFSET NUM
    MOV [SI], AX  
    MOV AX, BX 
    MOV DI, OFFSET REM
    MOV [DI], DX
    MUL DATA_10
    MOV BX, AX 
    MOV DX, [DI]
    ADD BX, DX 
    MOV AX, [SI]
    CMP AX, 0  
    JNE LOOP_REVERSE
    RET
     

REVERSE ENDP 

RETURN_TRUE:  
    MOV AX, 01H
    RET  
    
INC_COUNT_PP:

    
    MOV SI, OFFSET NUM_COPY
    MOV DI, OFFSET PP_NUMBERS
    MOV AX, [SI]    
    
    MOV CX, DATA_10
    SUB CX, COUNT_PP 
    ADD DI, CX
    
    MOV [DI], AX  
    
    
    MOV SI, OFFSET COUNT_PP  
    MOV CX, [SI]
    DEC CX
    MOV [SI], CX 
    
    
    
    MOV AX, 0
    CMP AX,0
    JE CONTINUE_LABEL

     END MAIN