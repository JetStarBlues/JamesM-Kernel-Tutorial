;global isr0
;isr0:
;	cli                  ; Disable interrupts
;	push byte 0          ; Push a dummy error code (if ISR0 doesn't push it's own error code)
;	push byte 0          ; Push the interrupt number (0)
;	jmp isr_common_stub  ; Go to our common handler

%macro ISR_NOERRORCODE 1
	global isr%1
	isr%1:
		cli                  ; Disable interrupts
		push byte 0          ; Push a dummy error code (if ISRx doesn't push it's own error code)
		push byte %1         ; Push the interrupt number
		jmp isr_common_stub  ; Go to our common handler
%endmacro

%macro ISR_ERRORCODE 1
	global isr%1
	isr%1:
		cli                  ; Disable interrupts
		push byte %1         ; Push the interrupt number
		jmp isr_common_stub  ; Go to our common handler
%endmacro

; expand macro
ISR_NOERRORCODE 0
ISR_NOERRORCODE 1
ISR_NOERRORCODE 2
ISR_NOERRORCODE 3
ISR_NOERRORCODE 4
ISR_NOERRORCODE 5
ISR_NOERRORCODE 6
ISR_NOERRORCODE 7
ISR_ERRORCODE   8
ISR_NOERRORCODE 9
ISR_ERRORCODE   10
ISR_ERRORCODE   11
ISR_ERRORCODE   12
ISR_ERRORCODE   13
ISR_ERRORCODE   14
ISR_NOERRORCODE 15
ISR_NOERRORCODE 16
ISR_NOERRORCODE 17
ISR_NOERRORCODE 18
ISR_NOERRORCODE 19
ISR_NOERRORCODE 20
ISR_NOERRORCODE 21
ISR_NOERRORCODE 22
ISR_NOERRORCODE 23
ISR_NOERRORCODE 24
ISR_NOERRORCODE 25
ISR_NOERRORCODE 26
ISR_NOERRORCODE 27
ISR_NOERRORCODE 28
ISR_NOERRORCODE 29
ISR_NOERRORCODE 30
ISR_NOERRORCODE 31


extern isr_handler           ; in isr.c

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:

   pusha                     ; Save edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov  ax, ds               ; Lower 16-bits of eax = ds.
   push eax                  ; Save the data segment descriptor

   mov  ax, 0x10             ; Load the kernel data segment descriptor
   mov  ds, ax
   mov  es, ax
   mov  fs, ax
   mov  gs, ax

   call isr_handler

   pop  eax                  ; Restore the original data segment descriptor
   mov  ds, ax
   mov  es, ax
   mov  fs, ax
   mov  gs, ax

   popa                      ; Restore edi,esi,ebp...

   add  esp, 8               ; Cleans up the pushed error code and pushed ISR number

   sti                       ; Enable interrupts

   iret                      ; Return from interrupt, and restore processor state
                             ; Pops CS, SS, EFLAGS, ESP, EIP
