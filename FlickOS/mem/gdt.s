
global loadGdt
global setGdt

gdtr DW 0  ;for limit storage
     DD 0  ;for base storage

loadGdt:
    mov     eax, [esp+4]    
    lgdt    [eax]           
    ret                    

setGdt:
    mov eax, [esp+4]
    mov ebx, [eax]
    add ebx, 8
    mov [eax], ebx
    lgdt [gdtr]
    ret 