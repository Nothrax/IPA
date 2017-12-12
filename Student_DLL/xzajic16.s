;Student login:

[BITS 32]

	GLOBAL DllMain
	EXPORT DllMain

;------------------------- xzajic16 -------------------------
	GLOBAL getType
	EXPORT getType

	GLOBAL getCount
	EXPORT getCount

	GLOBAL getActive
	EXPORT getActive

	GLOBAL getActiveCount
	EXPORT getActiveCount

	GLOBAL getStam
	EXPORT getStam

	GLOBAL getBoostS
	EXPORT getBoostS

	GLOBAL getBoostJ
	EXPORT getBoostJ

	GLOBAL getRange
	EXPORT getRange

	GLOBAL setActive
	EXPORT setActive
	
	GLOBAL chngStam
	EXPORT chngStam

	GLOBAL setUnpressed
	EXPORT setUnpressed

	GLOBAL removeItem
	EXPORT removeItem

	GLOBAL insertItem
	EXPORT insertItem

	GLOBAL setUnpressed
	EXPORT setUnpressed

	GLOBAL itemDetection
	EXPORT itemDetection
	;------------------------- xzajic16 -------------------------

	extern _GetStdHandle@4
	extern _WriteFile@20
	extern _printcube
	
section .data

;------------------------- xzajic16 -------------------------
inv_items	TIMES 10 dd 0
inv_count	TIMES 10 dd 0
inv_active	dd 0
max_cap		dd 255

pressed		dd 0

items_x		dq 170.0, 250.0, 400.0, 350.0
items_y		dq 170.0, 350.0, 150.0, 350.0
items_size	dq 5.0
in_range	dd 0

item_type	dd  1,  2,  3,  4
item_count	dd 10, 11, 12, 13

stamina_act dd 50
stamina_max	dd 500

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

;------------------------- xzajic16 -------------------------		
getType: ;-----------------------------------
	push ebp
	mov ebp, esp

	mov ecx, [ebp + 8]

	mov eax, [inv_items + ecx*4]


	mov esp, ebp
	pop ebp
ret

getCount: ;-----------------------------------
	push ebp
	mov ebp, esp

	mov ecx, [ebp + 8]

	mov eax, [inv_count + ecx*4]


	mov esp, ebp
	pop ebp
ret

getActive: ;-----------------------------------
	push ebp
	mov ebp, esp

	mov eax, [inv_active]

	mov esp, ebp
	pop ebp
ret

getActiveCount: ;-----------------------------------
	push ebp
	mov ebp, esp

	mov ecx, [inv_active]
	mov eax, [inv_count + ecx*4]


	mov esp, ebp
	pop ebp
ret

setActive: ;-----------------------------------
	push ebp
	mov ebp, esp

	sub esp, 4
	push ecx

	mov ecx, [ebp + 8]
	mov [inv_active], ecx

	pop ecx

	mov esp, ebp
	pop ebp
ret

getStam: ;-----------------------------------
	push ebp
	mov ebp, esp

	mov eax, [stamina_act]

	mov esp, ebp
	pop ebp
ret

getBoostS: ;-----------------------------------
push ebp
	mov ebp, esp
	
	sub esp, 8
	push ecx
	push edx
	xor edx, edx
	xor ecx, ecx

	mov eax, 500
	mov ecx, [stamina_act]

	sub eax, ecx
	mov ecx, 100
	div ecx

	add eax, 2

	pop edx
	pop ecx

	add esp, 8
	mov esp, ebp
	pop ebp
ret

getBoostJ: ;-----------------------------------
	push ebp
	mov ebp, esp

	sub esp, 8
	push ecx
	push edx
	xor edx, edx
	xor ecx, ecx

	mov eax, [stamina_act]
	mov ecx, 12
	div ecx

	add eax, 15

	pop edx
	pop ecx

	add esp, 8
	mov esp, ebp
	pop ebp
ret

getRange: ;-----------------------------------
	push ebp
	mov ebp, esp

	mov eax, [in_range]

	mov esp, ebp
	pop ebp
ret

chngStam: ;-----------------------------------
	push ebp
	mov ebp, esp

	sub esp, 8

	push ecx
	push eax

	mov ecx, [ebp + 8]
	mov eax, [stamina_act]
	
	cmp ecx, 0
	jg positive

negative:
	cmp eax, 1
	jle skip
	add eax, ecx
	jmp skip

positive:
	cmp eax, 500
	jge skip
	add eax, ecx

	cmp eax, 500
	jle skip
	mov eax, 500
	jmp skip

skip:
	mov [stamina_act], eax

	pop ecx
	pop eax

	mov esp, ebp
	pop ebp
ret

setUnpressed: ;-----------------------------------
	push ebp
	mov ebp, esp

	sub esp, 4

	push ecx

	mov ecx, 0
	mov [pressed], ecx

	pop ecx

	mov esp, ebp
	pop ebp
ret

removeItem: ;-----------------------------------
	push ebp
	mov ebp, esp

	sub esp, 12

	push eax
	push ecx
	push ebx

	mov ebx, [inv_active]
	mov ecx, [ebp + 8];count

	mov eax, [inv_count + ebx*4]
	sub eax, ecx
	mov [inv_count + ebx*4], eax

	cmp eax, 0
	jg filled
	mov eax, 0
	mov [inv_count + ebx*4], eax
	mov [inv_items + ebx*4], eax


filled:

	pop ebx
	pop ecx
	pop eax

	mov esp, ebp
	pop ebp
ret

insertItem: ;-----------------------------------
	push ebp
	mov ebp, esp

	sub esp, 16

	push eax
	push ecx
	push edx
	push ebx

	mov ecx, [ebp + 8] ;id
	mov eax, [pressed]
	mov ebx, 0

	cmp eax, 1
	jz end	
	mov eax, 0
	mov edx, -1 ; zarazka, pro hledání první 0, pokud neni jiny item vlozen		

;for i = 0; i < id.length; i++
loopInsert:	
	;if id[i].type == ecx break;
	cmp ecx, [inv_items + ebx*4]
	je end_loop

	;else if id[i].type == 0 edx = i;
	cmp eax, [inv_items + ebx*4]
	jne cont
	cmp edx, -1
	jne cont
	mov edx, ebx

cont:
	inc ebx
	cmp ebx, 10
	jl loopInsert	

	mov ebx, edx

end_loop:
	;count[i] += count
	mov eax, [inv_count + ebx*4]
	
	dec ecx
	mov edx, [item_count + ecx *4]
	inc ecx
	add eax, edx

	;if count[i] > max_cap count[i] = max_cap
	mov edx, [max_cap]
	cmp eax, edx
	jl add
	mov eax, edx

add:
	mov [inv_count + ebx*4], eax ;insetr items
	mov [inv_items + ebx*4], ecx ;set type if 0

	mov eax, 1
	mov [pressed], eax
end:

	pop ebx
	pop edx
	pop ecx
	pop eax

	mov esp, ebp
	pop ebp
ret

itemDetection: ;-----------------------------------
	push ebp
	mov ebp, esp

	sub esp, 12

	push ebx
	push ecx
	push eax

	mov ecx, [ebp+8] ;pos x + 0, y + 8
	mov ebx, 0

	xor eax, eax
	mov [in_range], eax

	finit
	fld qword [items_size]

loopDetection:

	;if |pos.x - item[i].x| < items_size
	fld qword [items_x + ebx*8]
	fld qword [ecx]
	fsubp
	fabs
	fcomip
	ja noColision

	;if |pos.y - item[i].y| < items_size
	fld qword [items_y + ebx*8]
	fld qword [ecx + 8]
	fsubp
	fabs
	fcomip
	ja noColision
	mov eax, [item_type + ebx*4]
	mov [in_range], eax
	jmp endDetection

noColision:
	inc ebx
	cmp ebx, 4
	jl loopDetection
	jmp endDetection

endDetection:
	fdecstp ;clear fld qword [items_size]

	pop eax
	pop ecx
	pop ebx
	
	mov esp, ebp
	pop ebp
ret
;------------------------- xzajic16 -------------------------
