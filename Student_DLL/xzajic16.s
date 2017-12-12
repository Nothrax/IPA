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
inv_items		TIMES 10 dd 0
inv_count		TIMES 10 dd 0
inv_active		dd 0
max_cap			dd 255

pressed			dd 0

items_x			dq 170.0, 250.0, 400.0, 350.0
items_y			dq 170.0, 350.0, 150.0, 350.0
items_size		dq 5.0
in_range		dd 0

item_type	dd  1,  2,  3,  4
item_count	dd 10, 11, 12, 13

stamina_act		dd 50
stamina_count	dd 0
stamina_value	dd 0

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
getType: ; Vrat typ prvku v slotu, jehoz index je ebp+8 (prvni parametr)
	push ebp
	mov ebp, esp
	sub esp, 4
	push ecx

	mov ecx, [ebp + 8]
	mov eax, [inv_items + ecx*4]

	pop ecx
	add esp, 4
	mov esp, ebp
	pop ebp
ret

getCount: ; Vrat pocet prvku v slotu, jehoz index je ebp+8 (prvni parametr)
	push ebp
	mov ebp, esp
	sub esp, 4
	push ecx

	mov ecx, [ebp + 8]
	mov eax, [inv_count + ecx*4]

	pop ecx
	add esp, 4
	mov esp, ebp
	pop ebp
ret

getActive: ; Vrat aktivni slot
	push ebp
	mov ebp, esp

	mov eax, [inv_active]

	mov esp, ebp
	pop ebp
ret

getActiveCount: ; Vrat pocet prvku v aktivnim slotu
	push ebp
	mov ebp, esp
	sub esp, 4
	push ecx

	mov ecx, [inv_active]
	mov eax, [inv_count + ecx*4]

	pop ecx
	add esp, 4
	mov esp, ebp
	pop ebp
ret

setActive: ; Nastav aktivnim slotem, slot s indexem na ebp+8 (prvni parametr)
	push ebp
	mov ebp, esp
	sub esp, 4
	push ecx

	mov ecx, [ebp + 8]
	mov [inv_active], ecx

	pop ecx
	add esp, 4
	mov esp, ebp
	pop ebp
ret

getStam: ; Vrat aktualni staminu
	push ebp
	mov ebp, esp

	mov eax, [stamina_act]

	mov esp, ebp
	pop ebp
ret

getBoostS: ; Vraci aktualni speed boost - vypocitany z aktualni staminy 
		   ; -> cim vetsi stamina tim mensi hodnota boostu (rychlejsi pohyb)
	push ebp
	mov ebp, esp	
	sub esp, 8
	push ecx
	push edx
	xor edx, edx
	xor ecx, ecx

	;eax = 500 - [stamina_act]
	mov eax, 500
	mov ecx, [stamina_act]
	sub eax, ecx
	;eax =  eax / ecx
	mov ecx, 100
	div ecx

	;offset boostu - minimalni hodnota je 3
	add eax, 2

	pop edx
	pop ecx
	add esp, 8
	mov esp, ebp
	pop ebp
ret

getBoostJ: ; Vraci aktualni jump boost - vypocitany z aktualni staminy 
		   ; -> cim vetsi stamina tim vetsi hodnota boostu (vyzsi skok)
	push ebp
	mov ebp, esp
	sub esp, 8
	push ecx
	push edx
	xor edx, edx
	xor ecx, ecx

	;eax = [stamina_act] / ecx
	mov eax, [stamina_act]
	mov ecx, 12
	div ecx

	;offset boostu - minimalni hodnota je 15
	add eax, 15

	pop edx
	pop ecx
	add esp, 8
	mov esp, ebp
	pop ebp
ret

getRange: ; Vrati index itemu, ktery je v dosahu, pokud neni je nastaven na 0
	push ebp
	mov ebp, esp

	mov eax, [in_range]

	mov esp, ebp
	pop ebp
ret

chngStam: ;Zmen satminu o [ebp+8] (prvni parametr) hlida rozsah 1-500
	push ebp
	mov ebp, esp
	sub esp, 12
	push ecx
	push eax
	push ebx

	mov ecx, [ebp + 8]
	mov eax, [stamina_act]
	
	;pricitame nebo odecitame ?
	cmp ecx, 0
	jg positive

	;vyhodnoceni odcitani - hlida 1 (min)
negative:
	add eax, ecx

	cmp eax, 1
	jge skip
	mov eax, 1
	jmp skip

	;vyhodnoceni pricitani - hlida 500 (max)
positive:
	add eax, ecx

	cmp eax, 500
	jle skip
	mov eax, 500
	jmp skip

skip:
	mov [stamina_act], eax

	pop ebx
	pop ecx
	pop eax
	sub esp, 12
	mov esp, ebp
	pop ebp
ret

setUnpressed: ; Reset flagu, ktery hlida nabeznou hranu stisku klavesy -pro single vyhodnoceni
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

removeItem: ; z aktivniho slotu odecte dany pocet itemu (strelba)
	push ebp
	mov ebp, esp
	sub esp, 12
	push eax
	push ecx
	push ebx

	;nacteni indexu aktivniho slotu, a poctu k odecteni
	mov ebx, [inv_active]
	mov ecx, [ebp + 8];count

	;nacte pocet itemu v aktivnim slotu a odecte pozadovany pocet
	mov eax, [inv_count + ebx*4]
	sub eax, ecx
	mov [inv_count + ebx*4], eax

	;pokud je slot prazdny nastav type na 0 - neobsahuje item
	;pokud count mensi nez 0, resetni na 0
	cmp eax, 0
	jg not_empty
	mov eax, 0
	mov [inv_count + ebx*4], eax
	mov [inv_items + ebx*4], eax


not_empty:

	pop ebx
	pop ecx
	pop eax
	add esp, 12
	mov esp, ebp
	pop ebp
ret

insertItem: ; pokud je mozne vlozi item do slotu (popr pricte pocet)
			; pokud je naplnena kapacita slotu, nevytavri dalsi slot se stejnym prvke
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

	;kontrola nabezne hrany
	cmp eax, 1
	je end	
	mov eax, 0
	mov edx, -1 ; zarazka, pro hledání první 0, pokud neni jiny item vlozen		

;for i = 0; i < id.length; i++
loopInsert:
	;if id[i].type == ecx break;
	;stejny typ itemu
	cmp ecx, [inv_items + ebx*4]
	je end_loop

	;else if id[i].type == 0 edx = i 
	;-> pouze pokud nebyl jiz nalezen
	cmp eax, [inv_items + ebx*4]
	jne loopInsert_next
	cmp edx, -1
	jne loopInsert_next
	mov edx, ebx

loopInsert_next:
	inc ebx
	;cykly dokud nejsi na koci pole
	cmp ebx, 10
	jl loopInsert	

	mov ebx, edx
	;pokud je rovno -1 neni volne misto
	cmp ebx, -1
	je end	

end_loop:
	;count[i] += count
	;nacti pocet itemu v slotu kam pridavas
	mov eax, [inv_count + ebx*4]
	
	;odecti protoze typ 1 je na indexu 0, presun a pote opet zvetsi at sedi typ
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
	mov [inv_count + ebx*4], eax ;uloz pocet
	mov [inv_items + ebx*4], ecx ;uloy typ

	;nastav flag vlozeni - pokud se klavesa nepusti jiz nevkladej znovu
	mov eax, 1
	mov [pressed], eax
end:

	pop ebx
	pop edx
	pop ecx
	pop eax
	add esp, 16
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

	;vypocet |pos.y - item[i].y|
	;vypocet vzdalenosti od daneho itemu
	fld qword [items_y + ebx*8]
	fld qword [ecx + 8]
	fsubp
	fabs

	;kontrola kolize |pos.y - item[i].y| < items_size
	fcomip
	ja noColision

	;nastav do priznaku kolize typ itemu
	mov eax, [item_type + ebx*4]
	mov [in_range], eax
	jmp endDetection

noColision:
	;kolize nenastala pokracuj, dokud nedojdes na konec pole itemu
	inc ebx
	cmp ebx, 4
	jl loopDetection

endDetection:
	fdecstp ;clear fld qword [items_size]

	pop eax
	pop ecx
	pop ebx
	add esp, 12
	mov esp, ebp
	pop ebp
ret
;------------------------- xzajic16 -------------------------
