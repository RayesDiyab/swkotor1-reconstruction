// 006925B0 CSWCMapPin::~CSWCMapPin and 00692610 scalar deleting destructor --
// source-faithful reconstruction.
//
// Oracle body (read-only swkotor.exe 9C10E045...):
//
//   006925B0  push -1 / push 007301F8 / fs:[0] SEH frame
//   006925C5  push ecx (this local) / push esi
//   006925C7  mov esi,ecx / mov [esp+4],esi
//   006925CD  mov dword [esi],00754598          ; canonical map-pin table
//   006925D3  lea ecx,[esi+100h]
//   006925D9  mov dword [esp+10h],0             ; EH state 0
//   006925E1  call 005E5C20                     ; CExoString dtor at +100h
//   006925E6  mov ecx,esi
//   006925E8  mov dword [esp+10h],-1            ; state -1 before the base dtor
//   006925F0  call 0063E800                     ; ~CSWCObject
//   006925F5  epilogue / RET 0
//
//   00692610  push esi / mov esi,ecx
//   00692613  call 006925B0                     ; destructor exactly once
//   00692618  testb $1,[esp+8]                  ; low delete-flag bit only
//   0069261D  je 00692628
//   0069261F  push esi / call 006FA390 / add esp,4
//   00692628  mov eax,esi / pop esi / RET 4
//
// EH exactness: the state-0 unwind action is 007301F0
//   mov ecx,[ebp-10h]; jmp 0063E800
// and the constructor/destructor share handler 007301F8 / FuncInfo 00780634.
// The guard below is armed only after the table store and before the CExoString
// destructor; it is released before the explicit base destructor call.  A throw
// from the CExoString destructor therefore destroys the base exactly once.  A
// throw from the base destructor happens after release, so it is not destroyed a
// second time.  There is no try/catch that would swallow either exception.
//
// The scalar deleting destructor has no EH frame of its own: if the destructor
// throws, the free is not reached and the exception propagates unchanged.
//
// Reused selected providers: 005E5C20 -> @frame_LogStringDtor_005e5c20@8 at
// 0064AF00 [0459...]; 0063E800 -> @kotor_m15e_object_destruct_0063e800@8 at
// 00545DE0 [0831.obj]; 006FA390 -> _live_free at 005FBA60 [0004-live-policy.obj].
#include "cd5f8e5e6a-map_pin_dtor.h"

namespace {

// Reproduces the original state-0 unwind action (007301F0 -> 0063E800).
// `armed` is a separate flag, not a null-this sentinel: the original unwind
// action is unconditional once state 0 is live.
struct BaseObjectUnwindScope {
    void* self;
    bool armed;

    explicit BaseObjectUnwindScope(void* object) noexcept : self(object), armed(true) {}

    ~BaseObjectUnwindScope() noexcept(false) {
        if (armed) {
            kotor_m15e_object_destruct_0063e800(self, nullptr);
        }
    }

    void release() noexcept { armed = false; }

    BaseObjectUnwindScope(const BaseObjectUnwindScope&) = delete;
    BaseObjectUnwindScope& operator=(const BaseObjectUnwindScope&) = delete;
};

}  // namespace

extern "C" void __fastcall recovered_client_map_pin_destruct_006925b0(
    void* self, void* /*unused_edx*/) noexcept(false) {
    unsigned char* const bytes = static_cast<unsigned char*>(self);

    // 006925CD: mov dword [esi],00754598.  Canonical table symbol; no numeric
    // original address, no base-table alias and no per-object copy.
    *reinterpret_cast<const void**>(bytes + kotor_map_pin::kVtableOffset) =
        static_cast<const void*>(recovered_client_map_pin_methods_00754598);

    // State 0 is live before the member destructor call.
    BaseObjectUnwindScope base(self);

    // 006925E1: call 005E5C20 with ECX=&this->name (+100h).
    frame_LogStringDtor_005e5c20(bytes + kotor_map_pin::kNameOffset, nullptr);

    // 006925E8: state -1 before the explicit base destructor.
    base.release();
    kotor_m15e_object_destruct_0063e800(self, nullptr);

    // 00692604: RET 0.
}

extern "C" void* __fastcall recovered_client_map_pin_scalar_delete_00692610(
    void* self, void* /*unused_edx*/, std::uint32_t delete_flag) noexcept(false) {
    // 00692613: call 006925B0 exactly once.
    recovered_client_map_pin_destruct_006925b0(self, nullptr);

    // 00692618: testb $1,[esp+8].  Only the low bit of the one stack dword
    // controls the free; flags 0/2 do not free, flags 1/3 do.
    if ((delete_flag & 1u) != 0u) {
        // 0069261F: push esi; call 006FA390; add esp,4.
        live_free(self);
    }

    // 00692628: mov eax,esi; pop esi; RET 4.
    return self;
}
