AREA Palindrome, CODE, READONLY
	LDR R1,=string;
	LDR R7,=string
	MOV R3,#0X00
	MOV R4,#0x00
	
loop
	LDRB R2,[R1],#1;
	CMP R2,#0x00	
	BEQ loop2
	PUSH{R2}
	ADD R4,R4,#1
	
	B loop
	
loop2
	CMP R4,#0x00
	BEQ stop
	LDRB R5,[R7],#1
	POP{R6}
	CMP R5,R6
	MOVNE R0,#0x01
	MOVNE R0,#0x02
	BNE stop
	SUB R4,R4,#0x01
	b loop2
	
stop b stop
string dcb "madam",0
	END