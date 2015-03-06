extern  interrupt_handler
    %macro no_error_code_interrupt_handler 1
    global interrupt_handler_%1
    interrupt_handler_%1:
        push    dword 0                     ; push 0 as error code
        push    dword %1                    ; push the interrupt number
        jmp     common_interrupt_handler    ; jump to the common handler
    %endmacro

    %macro error_code_interrupt_handler 1
    global interrupt_handler_%1
    interrupt_handler_%1:
        push    dword %1                    ; push the interrupt number
        jmp     common_interrupt_handler    ; jump to the common handler
    %endmacro

    common_interrupt_handler:               ; the common parts of the generic interrupt handler
        ; save the registers
        push    eax
        push    ebx
        push    ecx
        push    edx
        push    esp
        push    ebp
        push    esi
        push    edi

        ;call the C function
        call    interrupt_handler

        ; restore the registers
        pop     edi
        pop     esi
        pop     ebp
        pop     esp
        pop     edx
        pop     ecx
        pop     ebx
        pop     eax

        ; restore the esp
        add     esp, 8

        ; return to the code that got interrupted
        iret

    no_error_code_interrupt_handler 0       ; create handler for interrupt 0
    no_error_code_interrupt_handler 1       ; create handler for interrupt 1
    no_error_code_interrupt_handler 2       ; create handler for interrupt 2
    no_error_code_interrupt_handler 3       ; create handler for interrupt 3
    no_error_code_interrupt_handler 4       ; create handler for interrupt 4
    no_error_code_interrupt_handler 5       ; create handler for interrupt 5
    no_error_code_interrupt_handler 6       ; create handler for interrupt 6
    no_error_code_interrupt_handler 7       ; create handler for interrupt 7
    error_code_interrupt_handler    8       ; create handler for interrupt 8
    no_error_code_interrupt_handler 9       ; create handler for interrupt 9
    error_code_interrupt_handler 10      ; create handler for interrupt 10
    error_code_interrupt_handler 11      ; create handler for interrupt 11
    error_code_interrupt_handler 12      ; create handler for interrupt 12
    error_code_interrupt_handler 13      ; create handler for interrupt 13
    error_code_interrupt_handler 14      ; create handler for interrupt 14
    no_error_code_interrupt_handler 15      ; create handler for interrupt 15
    
global  load_idt

    ; load_idt - Loads the interrupt descriptor table (IDT).
    ; stack: [esp + 4] the address of the first entry in the IDT
    ;        [esp    ] the return address
    load_idt:
        mov     eax, [esp+4]    ; load the address of the IDT into register eax
        lidt    [eax]             ; load the IDT
        ret                     ; return to the calling function