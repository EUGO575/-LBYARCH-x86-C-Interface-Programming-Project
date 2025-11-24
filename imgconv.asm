SECTION .data
    align 4
recip_255: dd 0.00392156862745098039215686274  ; 1.0f / 255.0f

SECTION .text
    global _imgCvtGrayInttoFloat

; cdecl, 32-bit:
; [ebp+8]  -> in (unsigned char*)
; [ebp+12] -> out (float*)
; [ebp+16] -> width (unsigned int)
; [ebp+20] -> height (unsigned int)
_imgCvtGrayInttoFloat:
    push    ebp
    mov     ebp, esp
    push    esi
    push    edi
    push    ebx

    mov     esi, [ebp + 8]      ; in pointer
    mov     edi, [ebp + 12]     ; out pointer
    mov     eax, [ebp + 16]     ; width
    mov     ebx, [ebp + 20]     ; height

    imul    eax, ebx            ; eax = count = width * height
    test    eax, eax
    jz      .done               ; nothing to do

    xor     ecx, ecx            ; index = 0

    ; load reciprocal into xmm1
    movss   xmm1, dword [recip_255]

.loop:
    cmp     ecx, eax
    jge     .done_loop

    ; load byte in -> integer register edx (zero-extended)
    movzx   edx, byte [esi + ecx]   ; edx = (unsigned)in[ecx]

    ; convert integer (edx) -> float in xmm0
    cvtsi2ss xmm0, edx              ; xmm0 = float(edx)

    ; multiply by reciprocal 1/255
    mulss   xmm0, xmm1

    ; store float at out[ecx] (each float is 4 bytes)
    movss   [edi + ecx*4], xmm0

    inc     ecx
    jmp     .loop

.done_loop:
    ; restore and return
    pop     ebx
    pop     edi
    pop     esi
    mov     esp, ebp
    pop     ebp
    ret

.done:
    ; restore and return
    pop     ebx
    pop     edi
    pop     esi
    mov     esp, ebp
    pop     ebp
    ret
