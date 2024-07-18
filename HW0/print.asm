.MODEL SMALL
.STACK 100H

.DATA
    msg DB 'rizzzzz$'

.CODE
MAIN PROC
    MOV AX, @DATA
    MOV DS, AX
    
    MOV AH, 09h ; Function to print string
    LEA DX, msg ; Load address of the message
    INT 21h ; Call DOS interrupt
    
    MOV AH, 4Ch ; Exit program
    INT 21h ; Call DOS interrupt
MAIN ENDP

END MAIN