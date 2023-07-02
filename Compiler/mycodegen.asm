; Source: somecode.i
	.686P
	.model flat

_TEXT segment

_e$ = -4
_d$ = -5
_b$ = -9
_a$ = -13

_main_8f993366 proc

	push ebp
	mov ebp, esp
	sub esp, 13


	mov eax, -5
	mov DWORD PTR _a$[ebp], eax


	mov eax, DWORD PTR _a$[ebp]
	sub eax, 5
	neg eax
	mov DWORD PTR _b$[ebp], eax


	mov BYTE PTR _d$[ebp], 123


	inc DWORD PTR _a$[ebp]
	mov eax, DWORD PTR _a$[ebp]
	add eax, DWORD PTR _b$[ebp]
	mov ecx, 123
	sub ecx, BYTE PTR _d$[ebp]
	push ecx
	push eax
	call _testfunction_37dc0321	; testfunction
	add esp, 8
	mov DWORD PTR _e$[ebp], eax


	mov esp, ebp
	pop ebp
	ret

_main_8f993366 endp

_TEXT ends

_TEXT segment

_c$ = -4
_a$ = 8
_b$ = 12

_testfunction_37dc0321 proc

	push ebp
	mov ebp, esp
	sub esp, 4


	mov eax, DWORD PTR _a$[ebp]
	add eax, DWORD PTR _b$[ebp]
	mov DWORD PTR _c$[ebp], eax


	mov esp, ebp
	pop ebp
	ret

_testfunction_37dc0321 endp

_TEXT ends

END
