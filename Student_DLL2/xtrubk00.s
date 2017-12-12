;Student login:xtrubk00

[BITS 32]

	GLOBAL DllMain
	EXPORT DllMain

	
;xtrubk export
	GLOBAL full_movement
	EXPORT full_movement

	GLOBAL look_at
	EXPORT look_at

	GLOBAL	jump_movement
	EXPORT	jump_movement

	extern _GetStdHandle@4
	extern _WriteFile@20
	extern _printcube
	extern _chngStam
	extern _getBoostJ
	extern _getBoostS
	
section .data


CONST_PI:		dq		3.14159
CONST_180:		dq		180.0
CONST_90		dq		90.0
CONST_3			dq		3.0
CONST_2PI		dq		6.28318531
APROX_1			dq		1.27323954
APROX_2			dq		0.405284735
TMP				dq		0.0
ZERO			dq		0.0
ANGLE_Z			dq      30.0
CONST_M1		dq		-1.0
CONST_HALFG		dq		4.906375
CONST_EYE_HEIGHT dq		1.75
TMP_MEM			dd		0
TMP_MEM1		dd		0
;xtrubk const end

message:
    db      'Hello, World', 10
message_end:



section .text


DllMain:
	push ebp
	mov ebp,esp
	
		    ; hStdOut = GetstdHandle( STD_OUTPUT_HANDLE)
    push    -11
    call    _GetStdHandle@4
    mov     ebx, eax    

    ; WriteFile( hstdOut, message, length(message), &bytes, 0);
    push    0
    lea    eax, [ebp-4]
    push    eax
    push    (message_end - message)
    push    message
    push    ebx
    call    _WriteFile@20


	mov esp,ebp
	pop ebp
	mov eax,dword 1
	ret 12


jump_movement:
	push ebp
	mov ebp, esp
	sub esp, 4

	;ebp+8 pointer na double position.z ke zmene
	;ebp+12	pointer na double zPosition pro uchovani puvodni pozice
	;ebp+16 double jumpCoef...v0
	;ebp+24 pointer na double jumpTime...t
	;ebp+28 pointer na bool isJump
	;ebp+32 pointer na klavesy
	;ebp+36 vyska terenu

	mov ebx, [ebp+8]	;v ebx pointer na point position
	mov ecx, [ebp+12]	;v ecx pointer na double pozice zacatku skoku

	finit ;inicializace fpu

	;kontrola stisknuti mezerniku
	mov eax, [ebp+32]	;eax = keystates
	mov dl, [eax+44]	;hodnota u mezerniku
	mov dh, 0
	cmp dl, dh
	je no_space		;porovnani zda byl stisknut mezernik

	;kontrola zda neprovadime skok
	mov eax, [ebp+28]	;ukazatel na bool v eax
	mov dl, 1
	cmp dl, [eax]
	je no_space

	;nastaveni hodnot pro vypocet skoku
	mov [eax], dl		;nastaveni boolu na true
	mov eax, [ebp+24]	;resetovani casu
	fld qword [ZERO]	
	fstp qword [eax]	
	fld qword [ebx+16]	;ulozeni aktualniho z do pocatku skoku
	fstp qword [ecx]

	;ulozeni rychlosti aktualniho skoku
	push eax
	push ecx
	push edx
	call _getBoostJ
	mov [TMP_MEM], eax
	pop edx
	pop ecx
	pop eax

no_space:

	;provedeni skoku
	mov eax, [ebp+28]	;ukazatel na bool v eax
	mov dl, 1
	cmp dl, [eax]
	je jump_start

	;neskaceme

	fld qword [ebp+36] ;nacteni z na vysku terenu
	fstp qword [ebx+16]

	jmp jump_end

jump_start:
	;kontrola zda nejsme pod hranici
	fld qword [ZERO]
	fld qword [ebp+36] ;nacteni z na vysku terenu
	fld qword [CONST_M1]
	fmulp st1, st0
	fld qword [ebx+16]
	faddp st1, st0
	fcomi
	ja jump_else

	;nastaveni bool skoku na 0
	mov eax, [ebp+28]
	mov dl, 0
	mov [eax], dl
	jmp jump_end

jump_else:
	;skaceme
	;position.z = z0 + (v0 * t) - (0.5*g*t^2);
	fld qword [CONST_HALFG]	;st0 = g
	mov eax, [ebp+24]	;nacteni t
	fld qword [eax]		;st0 = t, st1 = g
	fld qword [eax]		;st0 = t, st1 = t, st2 = g
	fmulp st1, st0		;st0 = t^2, st1 = g
	fmulp st1, st0		;st0 = t^2*g
	fld qword [CONST_M1]
	fmulp st1, st0
	fild dword [TMP_MEM];st0 = v0, st1 = -t^2*g
	;fld qword [ebp+16]	
	fld qword [eax]
	fmulp st1, st0		;st0 = v0*t, st1 = -t^2*g
	fld qword [ecx]		;nacteni z0 st0 = zo, st1 = v0*t, st2 = -t^2*g
	faddp st1, st0		;st0 = z0 + (v0 * t), st1 = -t^2*g
	fadd st0, st1		;st0 = vysledek
	fst qword [ebx+16]


jump_end:
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0


;****************************************************************************;
full_movement:
	push ebp
	mov ebp, esp
	sub esp, 4

	;ebp+8 pointer na keystates (32bit)
	;ebp+12 pointer na position (32bit)
	;ebp+16 angleZ (64bit)
	;ebp+24 angleY (64bit)

	mov eax, [ebp+8];eax = keystates
	mov ecx, [ebp+12];ecx = positionVoloue
	mov ebx, 2

	finit ;inicializace fpu
	
	push eax
	push ecx
	push edx
	call _getBoostS
	mov [TMP_MEM1], eax
	pop edx
	pop ecx
	pop eax

	mov dl, [eax+26]		;kontrola stisknuti klavesy w
	mov dh, 0
	cmp  dl, dh
	je no_w
	mov ebx, -1				;nastaveni priznaku pro staminu
	jmp forward_movement
no_w:						
	mov dl, [eax+22]		;kontrola stisknuti klavesy s
	cmp  dl, dh
	je no_s
	mov ebx, -1				;nastaveni priznaku pro staminu
	jmp backward_movement
no_s:
	mov dl, [eax+4]		;kontrola stisknuti klavesy a
	cmp  dl, dh
	je no_a
	mov ebx, -1				;nastaveni priznaku pro staminu
	jmp left_movement
no_a:
	mov dl, [eax+7]		;kontrola stisknuti klavesy d
	cmp  dl, dh
	je no_d
	mov ebx, -1				;nastaveni priznaku pro staminu
	jmp right_movement
no_d:

;nastavovani stamniny
	push ebx
	call _chngStam
	pop ebx
	
	add esp, 4
	mov esp, ebp
	pop ebp
ret 0

forward_movement:
	;vypocet x
	;positionVoulue.x += cos(angleZ*M_PI / 180) / SPEED_COEF;
	fld qword [ebp+16]
	fld qword [CONST_PI]
	fmulp st1, st0
	fld qword [CONST_180]
	fdivp st1, st0
	fld st0
	fcos
	fild dword [TMP_MEM1]	
	fdivp
	fld qword [ecx]
	faddp st1, st0
	fstp qword [ecx]

	;vypocet y
	;positionVoulue.y += sin(angleZ*M_PI / 180) / SPEED_COEF;
	fsin
	fild dword [TMP_MEM1]	
	fdivp
	fld qword [ecx+8]
	fadd
	fstp qword [ecx+8]
	
	jmp no_w

backward_movement:
	;vypocet x
	;positionVoulue.x -= cos(angleZ*M_PI / 180) / SPEED_COEF;
	fld qword [ebp+16]
	fld qword [CONST_PI]
	fmulp st1, st0
	fld qword [CONST_180]
	fdivp st1, st0
	fld st0
	fcos
	fild dword [TMP_MEM1]	
	fdivp
	fld qword [CONST_M1]
	fmulp
	fld qword [ecx]
	faddp
	fstp qword [ecx]

	;vypocet y
	;positionVoulue.y -= sin(angleZ*M_PI / 180) / SPEED_COEF;
	fsin
	fild dword [TMP_MEM1]	
	fdivp
	fld qword [CONST_M1]
	fmulp
	fld qword [ecx+8]
	fadd
	fstp qword [ecx+8]
	
	jmp no_s

left_movement:
	;vypocet x
	;positionVoulue.x += cos((angleZ + 90)*M_PI / 180) / SPEED_COEF;
	fld qword [ebp+16]
	fld qword [CONST_90]
	faddp
	fld qword [CONST_PI]
	fmulp st1, st0
	fld qword [CONST_180]
	fdivp st1, st0
	fld st0
	fcos
	fild dword [TMP_MEM1]
	fdivp
	fld qword [ecx]
	faddp
	fstp qword [ecx]

	;vypocet y
	;positionVoulue.x += sin((angleZ + 90)*M_PI / 180) / SPEED_COEF;
	fsin
	fild dword [TMP_MEM1]
	fdivp
	fld qword [ecx+8]
	fadd
	fstp qword [ecx+8]
	
	jmp no_a

right_movement:
	;vypocet x
	;positionVoulue.x += cos((angleZ - 90)*M_PI / 180) / SPEED_COEF;
	fld qword [ebp+16]
	fld qword [CONST_90]
	fadd
	fld qword [CONST_PI]
	fmulp st1, st0
	fld qword [CONST_180]
	fdivp st1, st0
	fld st0
	fcos
	fild dword [TMP_MEM1]
	fdivp
	fld qword [CONST_M1]
	fmulp
	fld qword [ecx]
	faddp
	fstp qword [ecx]

	;vypocet y
	;positionVoulue.x += sin((angleZ - 90)*M_PI / 180) / SPEED_COEF;
	fsin
	fild dword [TMP_MEM1]
	fdivp
	fld qword [CONST_M1]
	fmulp
	fld qword [ecx+8]
	fadd
	fstp qword [ecx+8]
	
	jmp no_d

;****************************************************************************;
look_at:
	push ebp
	mov ebp, esp

	;+24 angleZ
	;+16 angleY
	;+12 position
	;+8 aimAt


	mov eax, [ebp+8] ;point aimAt
	mov ebx, [ebp+12] ;point position
	finit

	;cos(angleY*M_PI / 180)
	fld qword [ebp+16]
	fld qword [CONST_PI]
	fmulp st1, st0
	fld qword [CONST_180]
	fdivp st1, st0
	fcos			;st0 = cos(angleY*M_PI / 180)


	;aimAt.x
	;aimAt.x = position.x + cos(angleZ*M_PI / 180)*cos(angleY*M_PI / 180);
	fld qword [ebp+24]
	fld qword [CONST_PI]
	fmulp st1, st0
	fld qword [CONST_180]
	fdivp st1, st0
	fcos			;st0 = cos(angleZ*M_PI / 180), st1 = cos(angleY*M_PI / 180)
	fmul st0, st1
	fld qword [ebx] ;nahrani positon.x
	faddp st1, st0
	fstp qword [eax] ;ulozeni aimAt.x, mezivypocet zustane v st0

	;aimAt.y
	;aimAt.y = position.y + sin(angleZ*M_PI / 180)*cos(angleY*M_PI / 180);
	fld qword [ebp+24]
	fld qword [CONST_PI]
	fmulp st1, st0
	fld qword [CONST_180]
	fdivp st1, st0
	fsin			;st0 = cos(angleZ*M_PI / 180), st1 = cos(angleY*M_PI / 180)
	fmul st0, st1
	fld qword [ebx+8] ;nahrani positon.x
	faddp st1, st0
	fstp qword [eax+8] ;ulozeni aimAt.y

	;aimAt.z
	;aimAt.z = position.z + sin(angleY*M_PI / 180);
	fld qword [ebp+16]
	fld qword [CONST_PI]
	fmulp st1, st0
	fld qword [CONST_180]
	fdivp st1, st0
	fsin			;st0 = cos(angleZ*M_PI / 180)
	fld qword [ebx+16] ;nahrani positon.x
	faddp st1, st0
	fld qword [CONST_EYE_HEIGHT]	;aimAt.z += EYE_HEIGHT;
	faddp st1, st0
	fstp qword [eax+16] ;ulozeni aimAt.z

	;position.z += EYE_HEIGHT;
	fld qword [ebx+16]
	fld qword [CONST_EYE_HEIGHT]	;pricteni vysky
	faddp st1, st0
	fstp qword [ebx+16]

	
	mov esp, ebp
	pop ebp
	ret 0
;****************************************************************************;