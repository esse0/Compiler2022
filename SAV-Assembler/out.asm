.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "..\Debug\SAV_LIB.lib"
ExitProcess PROTO:DWORD 
.stack 4096

 outnum PROTO : DWORD

 outstr PROTO : DWORD

 outstrline PROTO : DWORD

 outnumline PROTO : DWORD

 inttostr PROTO : DWORD

 concat PROTO : DWORD, : DWORD

 copy PROTO : DWORD

 system_pause PROTO 

 random PROTO  : DWORD

 strLen PROTO  : DWORD

 inputInt PROTO 

 inputString PROTO 

 inputChar PROTO 

 power PROTO : DWORD, : DWORD

 getTimeNow PROTO 

 getDateNow PROTO 
.const
 null_division BYTE 'ERROR: DIVISION BY ZERO', 0
 overflow BYTE 'ERROR: VARIABLE OVERFLOW', 0 
	L1 BYTE 'Test cases', 0
	L2 BYTE ' ', 0
	L3 BYTE '---Data types---', 0
	L4 BYTE 'TEST char: ', 0
	L5 BYTE 'a', 0
	L6 BYTE 'TEST string: ', 0
	L7 BYTE 'string', 0
	L8 BYTE ' concat', 0
	L9 DWORD 1
	L10 BYTE 'Test BOOL: true', 0
	L11 BYTE 'TEST uint: ', 0
	L12 DWORD 10
	L13 BYTE '---Lib functions---', 0
	L14 BYTE 'TEST date function: ', 0
	L15 BYTE 'TEST time function: ', 0
	L16 BYTE 'TEST concat function: ', 0
	L17 BYTE 'TEST copy function: ', 0
	L18 BYTE 'TEST random function: ', 0
	L19 BYTE 'TEST pow function: ', 0
	L20 DWORD 5
	L21 DWORD 2
	L22 BYTE 'TEST strLen function: ', 0
	L23 BYTE 'TEST intToStr function: ', 0
	L24 BYTE 'TEST input string function: ', 0
	L25 BYTE 'Result: ', 0
	L26 BYTE 'TEST input char function: ', 0
	L27 BYTE '---Arithmetic operations---', 0
	L28 BYTE 'TEST 5 + 5 = ', 0
	L29 BYTE 'TEST 10 - 3 = ', 0
	L30 DWORD 3
	L31 BYTE 'TEST 10 * 5 = ', 0
	L32 BYTE 'TEST 10 / 5 = ', 0
	L33 BYTE 'TEST 10 mod 2 = ', 0
	L34 BYTE '---Shift operations---', 0
	L35 BYTE 'TEST 1 >> 1 = ', 0
	L36 DWORD 1
	L37 BYTE 'TEST 1 << 1 = ', 0
	L38 BYTE '---If, Else and logical operations---', 0
	L39 BYTE '1 == 1: True', 0
	L40 BYTE 'Test if,==: False', 0
	L41 DWORD 999
	L42 BYTE '10 != 999: True', 0
	L43 DWORD 9
	L44 BYTE '10 > 9: True', 0
	L45 BYTE '10 < 9: False', 0
	L46 BYTE '10 >= 10: True', 0
	L47 BYTE '9 <= 10: True', 0
	L48 BYTE '---Functions---', 0
	L49 BYTE 'Function result: ', 0
	L50 BYTE '---While---', 0
	L51 DWORD 0
	L52 BYTE 'Iteration N', 0
.data
	_plusres DWORD 0
	testChar DWORD ?
	testStr DWORD ?
	mainconct DWORD ?
	testBool DWORD 0
	testUint DWORD 0
	testDate DWORD ?
	testTime DWORD ?
	testConcat DWORD ?
	testCopy DWORD ?
	testRandom DWORD 0
	testPow DWORD 0
	testLen DWORD 0
	testIntToStr DWORD ?
	testInput DWORD ?
	testInput2 DWORD ?
	testPlus DWORD 0
	testMinus DWORD 0
	testUmn DWORD 0
	testDel DWORD 0
	testDel2 DWORD 0
	testSdvig DWORD 0
	testSdvig2 DWORD 0
	maini DWORD 0

.code

_plus PROC _plusa :  DWORD , _plusb :  DWORD 
	push _plusa
	push _plusb
	pop eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop _plusres

	mov eax, _plusres
	ret

SOMETHINGWRONG:
push offset null_division
call outstrline
call system_pause
push -1
call ExitProcess

EXIT_OVERFLOW:
push offset overflow
call outstrline
call system_pause
push -2
call ExitProcess
_plus ENDP
main PROC

	push offset L1
	call outstrline

	push offset L2
	call outstrline

	push offset L3
	call outstrline

	push offset L4
	call outstr
	push offset L5
	pop testChar


	push testChar
	call outstrline

	push offset L6
	call outstr
	push offset L7
	pop testStr


	push testStr
	call outstrline
	push offset L8
	pop mainconct

	push L9
	pop testBool

	mov eax, testBool
	mov ebx, L9
	.IF eax  ==  ebx

	push offset L10
	call outstrline

	.ENDIF

	push offset L11
	call outstr
	push L12
	pop testUint


	push testUint
	call outnumline

	push offset L2
	call outstrline

	push offset L13
	call outstrline

	push offset L14
	call outstr
	push offset getDateNow
	pop edx
		call getDateNow
	push eax
	pop testDate


	push testDate
	call outstrline

	push offset L15
	call outstr
	push offset getTimeNow
	pop edx
		call getTimeNow
	push eax
	pop testTime


	push testTime
	call outstrline

	push offset L16
	call outstr
	push offset concat
	push testStr
	push mainconct
	pop edx
	pop edx
	pop edx
	push mainconct
	push testStr
		call concat
	push eax
	pop testConcat


	push testConcat
	call outstrline

	push offset L17
	call outstr
	push offset copy
	push testConcat
	pop edx
	pop edx
	push testConcat
		call copy
	push eax
	pop testCopy


	push testCopy
	call outstrline

	push offset L18
	call outstr
	push random
	push L12
	pop edx
	pop edx
	push L12
		call random
	push eax
	pop testRandom


	push testRandom
	call outnumline

	push offset L19
	call outstr
	push power
	push L20
	push L21
	pop edx
	pop edx
	pop edx
	push L21
	push L20
		call power
	push eax
	pop testPow


	push testPow
	call outnumline

	push offset L22
	call outstr
	push strLen
	push testStr
	pop edx
	pop edx
	push testStr
		call strLen
	push eax
	pop testLen


	push testLen
	call outnumline

	push offset L23
	call outstr
	push offset intToStr
	push testUint
	pop edx
	pop edx
	push testUint
		call intToStr
	push eax
	pop testIntToStr


	push testIntToStr
	call outstrline

	push offset L24
	call outstrline
	push offset inputString
	pop edx
		call inputString
	push eax
	pop testInput


	push offset L25
	call outstr

	push testInput
	call outstrline

	push offset L26
	call outstrline
	push offset inputChar
	pop edx
		call inputChar
	push eax
	pop testInput2


	push offset L25
	call outstr

	push testInput2
	call outstrline

	push offset L2
	call outstrline

	push offset L27
	call outstrline

	push offset L28
	call outstr
	push L20
	push L20
	pop eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop testPlus


	push testPlus
	call outnumline

	push offset L29
	call outstr
	push testPlus
	push L30
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop testMinus


	push testMinus
	call outnumline

	push offset L31
	call outstr
	push testPlus
	push L20
	pop eax
	pop ebx
	imul ebx
	jo EXIT_OVERFLOW
	push eax
	pop testUmn


	push testUmn
	call outnumline

	push offset L32
	call outstr
	push testPlus
	push L20
	pop ebx
	pop eax
	cmp ebx,0
	je SOMETHINGWRONG
	cdq
	idiv ebx
	push eax
	pop testDel


	push testDel
	call outnumline

	push offset L33
	call outstr
	push testPlus
	push L21
	pop ebx
	pop eax
	cmp ebx,0
	je SOMETHINGWRONG
	cdq
	idiv ebx
	push edx
	pop testDel2


	push testDel2
	call outnumline

	push offset L2
	call outstrline

	push offset L34
	call outstrline

	push offset L35
	call outstr
	push L36
	pop testSdvig

	mov eax, testSdvig
	mov ebx, L21
	mov ecx, 0
	.WHILE ecx < ebx
		sar eax, 1
		inc ecx
	.ENDW
	mov testSdvig, eax

	push testSdvig
	call outnumline

	push offset L37
	call outstr
	push L36
	pop testSdvig2

	mov eax, testSdvig2
	mov ebx, L36
	mov ecx, 0
	.WHILE ecx < ebx
		sal eax, 1
		inc ecx
	.ENDW
	mov testSdvig2, eax

	push testSdvig2
	call outnumline

	push offset L38
	call outstrline
	mov eax, L36
	mov ebx, L36
	.IF eax  ==  ebx

	push offset L39
	call outstrline
	.ELSE

	push offset L40
	call outstrline

	.ENDIF
	mov eax, testPlus
	mov ebx, L41
	.IF eax  !=  ebx

	push offset L42
	call outstrline

	.ENDIF
	mov eax, L12
	mov ebx, L43
	.IF eax  >  ebx

	push offset L44
	call outstrline

	.ENDIF
	mov eax, L12
	mov ebx, L43
	.IF eax  < ebx
	.ELSE

	push offset L45
	call outstrline

	.ENDIF
	mov eax, L12
	mov ebx, L12
	.IF eax  >=  ebx

	push offset L46
	call outstrline

	.ENDIF
	mov eax, L43
	mov ebx, L12
	.IF eax  <=  ebx

	push offset L47
	call outstrline

	.ENDIF

	push offset L2
	call outstrline

	push offset L48
	call outstrline
	push _plus
	push L20
	push L12
	pop edx
	pop edx
	pop edx
	push L12
	push L20
		call _plus
	push eax
	pop testUint


	push offset L49
	call outstr

	push testUint
	call outnumline

	push offset L2
	call outstrline

	push offset L50
	call outstrline
	push L51
	pop maini

while1:
	mov eax, maini
	mov ebx, L20

	.IF eax  >=  ebx
		jmp endwhile1
	.ENDIF

	push maini
	push L36
	pop eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop maini


	push offset L52
	call outstr

	push maini
	call outnumline
	jmp while1

endwhile1:
call system_pause
push 0
call ExitProcess
SOMETHINGWRONG:
push offset null_division
call outstrline
call system_pause
push -1
call ExitProcess
EXIT_OVERFLOW:
push offset overflow
call outstrline
call system_pause
push -2
call ExitProcess
main ENDP
end main