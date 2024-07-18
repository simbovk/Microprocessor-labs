include 'emu8086.inc'

org 100h
.data
a dw 5
fact dd ?
.code
mov ax,@data
mov ds,ax
mov ax,a
cmp ax,1  
mov dx,ax
dec dx
mov bx,dx
je exit
cmp ax,2
je exit
jne l2
l2:
mul bx
cmp bx,1
dec bx
je exit
jne l2
exit:    

GOTOXY 10, 5; where to print, col =10, row =5

;; print AX register ;;
CALL print_num
DEFINE_PRINT_NUM
DEFINE_PRINT_NUM_UNS ; required for print_num  
   
mov fact,ax  

Ret