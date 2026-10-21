// 00692540 CSWCMapPin::CSWCMapPin -- source-faithful reconstruction.
//
// Oracle body (read-only swkotor.exe 9C10E045...):
//   00692540  push -1 / push 007301F8 / fs:[0] SEH frame
//   00692555  push ecx (this local) / xor eax,eax
//   00692558  mov al,byte [0074663B]            ; immutable waypoint type = 0Ch
//   0069255D  push esi / mov esi,ecx / mov [esp+4],esi
//   00692564  push eax / call 0063E530          ; CSWCObject ctor(type), RET 4
//   0069256A  lea ecx,[esi+100h]
//   00692570  mov dword [esp+10h],0             ; EH state 0 before the member ctor
//   00692578  mov dword [esi],00754598          ; CSWCMapPin vtable
//   0069257E  call 005B3190                     ; CExoString default ctor at +100h
//   00692583  mov ecx,[esp+8]                   ; epilogue fs:[0] restore value
//   00692587  mov word [esi+48h],2711h
//   0069258D  mov dword [esi+0FCh],0
//   00692597  mov dword [esi+10Ch],0
//   006925A1  mov eax,esi / pop esi / mov fs:[0],ecx / add esp,10h / ret
//
// The only state-0 unwind action is ~CSWCObject (FuncInfo 00780634 ->
// unwind map 0078062C -> action 007301F0 -> jmp 0063E800).  The local
// BaseObjectScope below reproduces exactly that: it is armed only after the
// base constructor returns, and it is released after the last original write so
// the normal path never calls ~CSWCObject.
//
// The vtable store is deliberately an import of the canonical table symbol, not
// the numeric original address: the rebuilt image does not place .rdata at the
// original 00754598 and no selected provider currently defines the table.  This
// is reported as the one unresolved data edge; it is not replaced by a stub,
// a CSWCObject table (slot 0 would be ~CSWCObject instead of the map-pin
// scalar deleting destructor 00692610) or any other fallback.
#include "848078071a-map_pin_ctor.h"

namespace {

// Reproduces the original state-0 unwind action (007301F0 -> 0063E800).
// Construction order is intentional: the base ctor runs in this guard's
// constructor, so a throwing base ctor never arms the unwind action.
struct BaseObjectScope {
    void* self;

    explicit BaseObjectScope(void* object) : self(object) {
        (void)kotor_m15a_object_construct_0063e530(
            self, nullptr, kotor_map_pin::kWaypointObjectType_0074663B);
    }

    ~BaseObjectScope() noexcept(false) {
        if (self != nullptr) {
            kotor_m15e_object_destruct_0063e800(self, nullptr);
        }
    }

    void release() noexcept { self = nullptr; }
};

}  // namespace

extern "C" void* __fastcall recovered_client_map_pin_construct_00692540(
    void* self, void* /*unused_edx*/) noexcept(false) {
    unsigned char* const bytes = static_cast<unsigned char*>(self);

    BaseObjectScope base(self);

    // 00692578: mov dword [esi],00754598.  Canonical table symbol; see header.
    *reinterpret_cast<const void**>(bytes + kotor_map_pin::kVtableOffset) =
        static_cast<const void*>(recovered_client_map_pin_methods_00754598);

    // 0069257E: call 005B3190 with ECX=&this->name (+100h).
    (void)g11_exostring_default(bytes + kotor_map_pin::kNameOffset, nullptr);

    // 00692587 / 0069258D / 00692597: original write order and widths.
    *reinterpret_cast<std::uint16_t*>(bytes + kotor_map_pin::kAnimationWordOffset) =
        kotor_map_pin::kAnimationWord;
    *reinterpret_cast<std::uint32_t*>(bytes + kotor_map_pin::kFieldFcOffset) = 0;
    *reinterpret_cast<std::uint32_t*>(bytes + kotor_map_pin::kField10cOffset) = 0;

    // Normal completion: the original ctor does not run ~CSWCObject.
    base.release();
    return self;
}
