section .data
;COMPILAR: nasm -f elf -o calculadora.o calculadora.asm
;LIGAR: ld -m elf_i386 -o calc calculadora.o
    msg db 'Calculadora Assembly IA-32',0xa
    tamanho_msg equ $-msg
    
    menu_msg db 'Escolha uma Operacao',0xa
    tamanho_menu_msg equ $-menu_msg
    case1 db '1)Soma',0xa
    tamanho_case1 equ $-case1
    case2 db '2)Subtraca',0xa
    tamanho_case2 equ $-case2
    case3 db '3)Multiplicacao',0xa
    tamanho_case3 equ $-case3
    case4 db '4)Divisao',0xa
    tamanho_case4 equ $-case4
    case5 db '5)Potenciacao',0xa
    tamanho_case5 equ $-case5
    case6 db '6)Fatorial',0xa
    tamanho_case6 equ $-case6
    case7 db '7)Concatenacao de String',0xa
    tamanho_case7 equ $-case7
    case8 db '8)Repeticao de String',0xa
    tamanho_case8 equ $-case8
    case9 db '9)Sair',0xa
    tamanho_case9 equ $-case9

    endl db 0xa
    aux dd 0
    cin dd 0
    input1 dd 0
    input2 dd 0
    resposta dd 0
    negativo db '-'
    tamanho_aux dd 0


    msg_input1 db 'Digite o primeiro valor : ',0xa
    tamanho_msg_input1 equ $ - msg_input1
    msg_input2 db 'Digite o segundo valor : ',0xa
    tamanho_msg_input2 equ $ - msg_input2
    msg_resposta db 'R : '
    tamanho_msg_resposta equ $ - msg_resposta

section .bss
    maximo equ 20

section .text
    global _start
_start:
    mov edx, tamanho_msg
	mov ecx, msg
	mov ebx, 1
	mov eax, 4
	int 0x80
    
    loop:
    call menu
    call scanf

    mov ebx, dword[aux]
    sub ebx,0x30
    mov dword[cin], ebx

	call soma
    call subtracao
    
    cmp byte[cin],9
    jne loop

    exit:
        mov eax,1
        int 0x80
    menu: 
        mov ebx,1
        mov eax,4
        mov edx,tamanho_menu_msg
        mov ecx,menu_msg
        int 0x80

        mov edx, tamanho_case1
	    mov ecx, case1
	    mov ebx, 1
	    mov eax, 4
	    int 0x80

	    mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_case2
	    mov ecx, case2
	    int 0x80

	    mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_case3
	    mov ecx, case3
	    int 0x80

	    mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_case4
	    mov ecx, case4
	    int 0x80

        mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_case5
	    mov ecx, case5
	    int 0x80

        mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_case6
	    mov ecx, case6
	    int 0x80

        mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_case7
	    mov ecx, case7
	    int 0x80

        mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_case8
	    mov ecx, case8
	    int 0x80

        mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_case9
	    mov ecx, case9
	    int 0x80
	    
	    ret

    scanf:
	    mov ebx, 0
	    mov eax, 3
        mov edx, maximo
	    mov ecx, aux
	    int 0x80

	    ret


    soma:
        cmp byte[cin], 1
	    jne return

	    mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_msg_input1
	    mov ecx, msg_input1
	    int 0x80

	    call scanf
	    mov edx, aux
	    call str_2_int
	    call inverte
	    mov dword[input1], eax

	    mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_msg_input2
	    mov ecx, msg_input2
	    int 0x80

	    call scanf
	    mov edx, aux
	    call str_2_int
	    call inverte
	    mov dword[input2], eax

	    mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_msg_resposta
	    mov ecx, msg_resposta
	    int 0x80

	    mov eax, dword[input1]
	    add eax, dword[input2]
	    mov edi,resposta
	    call int_2_str

	    mov eax, 4
	    mov ebx, 1
	    mov edx, [tamanho_aux]
	    mov ecx, resposta
	    int 0x80

	    mov edx, 1
	    mov ecx, endl
	    mov ebx, 1
	    mov eax, 4
	    int 0x80

	    mov edx, 1
	    mov ecx, endl
	    mov ebx, 0
	    mov eax, 3
	    int 0x80

	    ret

    subtracao:
	    cmp byte[cin], 2
	    jne return

	    mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_msg_input1
	    mov ecx, msg_input1
	    int 0x80

	    call scanf
	    mov edx, aux
	    call str_2_int
	    call inverte
	    mov dword[input1], eax

	    mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_msg_input2
	    mov ecx, msg_input2
	    int 0x80

	    call scanf
	    mov edx, aux
	    call str_2_int
	    call inverte
	    mov dword[input2], eax

	    mov eax, 4
	    mov ebx, 1
	    mov edx, tamanho_msg_resposta
	    mov ecx, msg_resposta
	    int 0x80

	    mov eax, dword[input1]
	    sub eax, dword[input2]
	    mov edi,resposta
	    call int_2_str

	    mov eax, 4
	    mov ebx, 1
	    mov edx, [tamanho_aux]
	    mov ecx, resposta
	    int 0x80


	    mov edx, 1
	    mov ecx, endl
	    mov ebx, 1
	    mov eax, 4
	    int 0x80

	    mov edx, 1
	    mov ecx, endl
	    mov ebx, 0
	    mov eax, 3
	    int 0x80

	    ret


    
            
    inverte:
	    cmp ebx, '-'
	    jne return
	    NEG eax
	    ret

    int_2_str:
        xor ebx, ebx
	    cmp eax, 0
	    jge put
	    neg eax
	    push eax
	    mov edx, 1
	    mov ecx, negativo
	    mov ebx, 1
	    mov eax, 4
	    int 0x80
	    pop eax
	    xor ebx, ebx
	put:
	    xor edx, edx
	    mov ecx, 10
	    div ecx
	    add edx, 0x30
	    push edx
	    inc ebx
	    test eax, eax
	    jnz put
	    mov [tamanho_aux],ebx
	    out:
	    pop eax
	    stosb
	    dec ebx
	    cmp ebx, 0
	    jg out
	    mov eax, 0x0a
	    stosb
	    ret

    str_2_int:
        xor eax,eax
	    movzx ebx,byte[edx]
	    cmp ebx, '-'
	    je inverte_
	conv:
	    movzx ecx,byte[edx]
	    inc edx
	    cmp ecx,'0'
	    jb return
	    cmp ecx,'9'
	    ja return
	    sub ecx, 0x30
	    imul eax,10
	    add eax, ecx
	    jmp conv

	inverte_:
	    inc edx
	    jmp conv

    return:
        ret