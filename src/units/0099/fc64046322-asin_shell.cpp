// Private implementation draft. Source/native/0740 recipe gates still pending.
// Assembly-only protocol: input/output ST0, original caller-owned qword input scratch.
#if !defined(_M_IX86)
#error "asin shell requires x86"
#endif
#include "1e09fc2084-pow_arithmetic.h" // proposed canonical0740 mutable20-byte endpoint range, existingCRT ASM protocols
extern "C" {
void __cdecl pointto_asin_compute_006fd028(); // sole shared0625 seven-instruction core, no double ABI
// No prior exact-name/address/alias owner in current sources/heldmap/twenty inherited donor tips.
char pointto_asin_name_007a27b0[5]={'a','s','i','n',0}; // original mutable name exposed to matherr callbacks
}
namespace {
// Original006FD00D..006FD0B9, sharing existing math computation and CRT protocol providers.
__declspec(naked) void __cdecl asin_internal(){
    __asm {
        push edx
        wait
        fnstcw word ptr [esp]
        je L_nonfinite
        cmp word ptr [esp],027fh
        je L_finite
        call task8_math_load_cw_00704215
    L_finite:
        cmp eax,03ff00000h
        jae L_endpoint_or_domain
        call pointto_asin_compute_006fd028
    L_normal_tail:
        cmp dword ptr [task8_crt_fast_math_enabled_00833bdc],0
        jne L_restore_fast
        mov edx,0eh
        lea ecx,pointto_asin_name_007a27b0
        // Original007042AB conditional postresult qword/error path.
        mov ax,word ptr [esp]
        cmp ax,027fh
        je L_pop_return
        and ax,20h
        je L_restore
        wait
        fnstsw ax
        and ax,20h
        je L_restore
        mov eax,8
        call task8_math_error_single_007041b7
        pop edx
        ret
    L_restore:
        fldcw word ptr [esp]
    L_pop_return:
        pop edx
        ret
    L_endpoint_or_domain:
        ja L_domain
        mov eax,dword ptr [esp+0ch]
        mov ecx,eax
        and eax,0fffffh
        or eax,dword ptr [esp+8]
        jne L_domain
        and ecx,080000000h
        fstp st(0)
        fld tbyte ptr [task8_pow_const_007a30e0+0ah]
        je L_normal_tail
        fchs
        jmp L_normal_tail
    L_nonfinite:
        test eax,0fffffh
        jne L_nan
        cmp dword ptr [esp+8],0
        jne L_nan
    L_domain:
        fstp st(0)
        fld tbyte ptr [task8_pow_const_007a30e0]
        mov eax,1
        jmp L_error_tail
    L_nan:
        // Original0070422C quiet-NaN classification/quieting, using existing0740 operand.
        test eax,080000h
        je L_signaling_nan
        mov eax,7
        jmp L_error_tail
    L_signaling_nan:
        fadd qword ptr [task8_pow_const_0075a960]
        mov eax,1
    L_error_tail:
        cmp dword ptr [task8_crt_fast_math_enabled_00833bdc],0
        jne L_restore_fast
        mov edx,0eh
        lea ecx,pointto_asin_name_007a27b0
        call task8_math_error_single_007041b7
        pop edx
        ret
    L_restore_fast:
        // Original0070429E, no unconditional result store.
        cmp word ptr [esp],027fh
        je L_fast_return
        fldcw word ptr [esp]
    L_fast_return:
        pop edx
        ret
    }
}
}
extern "C" __declspec(naked) void __cdecl pointto_fasin_006fcff0(){
    __asm {
        sub esp,0ch
        fst qword ptr [esp] // original nonpop store, including status/C1 effects
        // Original00704288 classifier inlined; preserve its final ZF and EAX payload.
        mov eax,dword ptr [esp+4]
        and eax,07ff00000h
        cmp eax,07ff00000h
        jne L_classified
        mov eax,dword ptr [esp+4]
    L_classified:
        call asin_internal
        add esp,0ch
        ret
    }
}
