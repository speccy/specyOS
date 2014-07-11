[BITS 32]                       ; All instructions should be 32-bit.

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide  kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
MBOOT_USE_GFX		equ 1<<2
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_USE_GFX
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[GLOBAL mboot]                  ; Make 'mboot' accessible from C.
[EXTERN code]                   ; Start of the '.text' section.
[EXTERN bss]                    ; Start of the .bss section.
[EXTERN end]                    ; End of the last loadable section.

SECTION .multiboot
ALIGN 4
mboot:
    dd  MBOOT_HEADER_MAGIC      
    dd  MBOOT_HEADER_FLAGS      
    dd  MBOOT_CHECKSUM          
    
    dd  mboot                   ; Location of this descriptor
    dd  code                    ; Start of kernel '.text' (code) section.
    dd  bss                     ; End of kernel '.data' section.
    dd  end                     ; End of kernel.
    dd  start                   ; Kernel entry point (initial EIP).
    
	dd 0x00000000 ; 0 = linear graphics
	dd 1280
	dd 800
	dd 32 ; Set me to 32 or else.

[GLOBAL start]                  ; Kernel entry point.
[EXTERN k_main]                   

start:
    ; Load multiboot information:
    push esp
    push ebx

    ; Execute the kernel:
    cli                         ; Disable interrupts.
    call k_main                  
    jmp $                       ;infinite loop
    
[GLOBAL gdt_flush]   

gdt_flush:
   mov eax, [esp+4]  ; Get the pointer to the GDT
   lgdt [eax]        ; Load the GDT pointer

   mov ax, 0x10      ; 0x10 is the offset in the GDT to data segment
   mov ds, ax        ; Load all data segment selectors
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax
   jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
   ret

[GLOBAL idt_flush]    

idt_flush:
   mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter.
   lidt [eax]        ; Load the IDT pointer.
   ret
   
   
%macro ISR_NOERR 1  ; define a macro, taking one parameter
  [GLOBAL isr%1]        ; %1 accesses the first parameter.
  isr%1:
    cli
    push 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERR 1
  [GLOBAL isr%1]
  isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro 

%macro IRQ 2
  [GLOBAL irq%1]
  irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR 8
ISR_NOERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NOERR 15
ISR_NOERR 16
ISR_NOERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31
ISR_NOERR 255

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

[EXTERN irq_handler]
irq_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	; Call the C kernel hardware interrupt handler
	mov eax, irq_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret
	

[EXTERN isr_handler]

isr_common_stub:
	pusha
	; push registers onto stack
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax		
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	; Call the C kernel fault handler
	mov eax, isr_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret
	
