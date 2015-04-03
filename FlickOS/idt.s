extern interrupt_handler
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
    error_code_interrupt_handler 10         ; create handler for interrupt 10
    error_code_interrupt_handler 11         ; create handler for interrupt 11
    error_code_interrupt_handler 12         ; create handler for interrupt 12
    error_code_interrupt_handler 13         ; create handler for interrupt 13
    error_code_interrupt_handler 14         ; create handler for interrupt 14
    no_error_code_interrupt_handler 15      ; create handler for interrupt 15
    no_error_code_interrupt_handler 16      ; create handler for interrupt 16
    error_code_interrupt_handler 17         ; create handler for interrupt 17
    no_error_code_interrupt_handler 18      ; create handler for interrupt 18
    no_error_code_interrupt_handler 19      ; create handler for interrupt 19
    no_error_code_interrupt_handler 20      ; create handler for interrupt 20
    no_error_code_interrupt_handler 21      ; create handler for interrupt 21
    no_error_code_interrupt_handler 22      ; create handler for interrupt 22
    no_error_code_interrupt_handler 23      ; create handler for interrupt 23
    no_error_code_interrupt_handler 24      ; create handler for interrupt 24
    no_error_code_interrupt_handler 25      ; create handler for interrupt 25
    no_error_code_interrupt_handler 26      ; create handler for interrupt 26
    no_error_code_interrupt_handler 27      ; create handler for interrupt 27
    no_error_code_interrupt_handler 28      ; create handler for interrupt 28
    no_error_code_interrupt_handler 29      ; create handler for interrupt 29
    no_error_code_interrupt_handler 30      ; create handler for interrupt 30
    no_error_code_interrupt_handler 31
    no_error_code_interrupt_handler 32
    no_error_code_interrupt_handler 33
    no_error_code_interrupt_handler 34
    no_error_code_interrupt_handler 35
    no_error_code_interrupt_handler 36
    no_error_code_interrupt_handler 37
    no_error_code_interrupt_handler 38
    no_error_code_interrupt_handler 39
    no_error_code_interrupt_handler 40
    no_error_code_interrupt_handler 41
    no_error_code_interrupt_handler 42
    no_error_code_interrupt_handler 43
    no_error_code_interrupt_handler 44
    no_error_code_interrupt_handler 45
    no_error_code_interrupt_handler 46
    no_error_code_interrupt_handler 47


global  load_idt

    ; load_idt - Loads the interrupt descriptor table (IDT).
    ; stack: [esp + 4] the address of the first entry in the IDT
    ;        [esp    ] the return address
    load_idt:
        mov     eax, [esp+4]    ; load the address of the IDT into register eax
        lidt    [eax]             ; load the IDT
        ret                     ; return to the calling function