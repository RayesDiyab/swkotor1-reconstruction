// case95 / original 005EDBA0 and 005F1400.
//
// Original oracle (artifacts/inputs/swkotor.exe SHA-256
// 9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435):
//
//   005EDBA0  8B 49 04              mov  ecx,dword ptr [ecx+4]
//   005EDBA3  E9 58 38 00 00        jmp  005F1400
//   005EDBA8  90 x8                 (alignment padding to 005EDBB0)
//
//   005F1400  8B 89 A0 02 00 00     mov  ecx,dword ptr [ecx+000002A0h]
//   005F1406  85 C9                 test ecx,ecx
//   005F1408  74 05                 je   005F140F
//   005F140A  8B 01                 mov  eax,dword ptr [ecx]
//   005F140C  FF 60 2C              jmp  dword ptr [eax+2Ch]
//   005F140F  C3                    ret
//
// Both functions are __fastcall with the unused EDX home slot, so the
// decorated names are @kotor_fastcall_005edba0@8 / @kotor_fastcall_005f1400@8.
//
// Body adopted from the reviewed W4 donor
// docs/kotor/recovery/wip-lanes-2026-10-13/lane-w4/src/dialog/dialog_ingame.cpp
// lines 160-165 (SHA-256 4b6d781b8ad2cf4995506363a133b34dc38f136e68d669be37dca05c1f3b908c):
//
//   extern "C" void __fastcall kotor_fastcall_005f1400(void* internal, void*) {
//       void* drive = lp(internal, 0x2A0);
//       if (drive != nullptr) vcall<void>(drive, 0x2C);
//   }
//   extern "C" void __fastcall kotor_fastcall_005edba0(void* client, void*) {
//       kotor_fastcall_005f1400(lp(client, 4), nullptr);
//   }
//
// The donor's file-local lp/vcall helpers are expanded inline here so the
// object carries only the two original bodies (no local helper call frames).
// No guard, forced field, fake owner or substitute branch is introduced.  The
// null internal+0x2A0 branch returns; the non-null branch reads vtable+0x2C and
// enters it with the player-control object in ECX.

extern "C" void __fastcall kotor_fastcall_005f1400(void* internal, void* edx);

// 005EDBA0 CClientExoApp::ResetDriveAcceleration (16-byte original slot: 8
// bytes of code plus 8 alignment NOPs).  Loads the internal object from
// [client+4] and tail-enters 005F1400.  EDX is forwarded untouched: the
// original wrapper neither reads nor rewrites it.
extern "C" void __fastcall kotor_fastcall_005edba0(void* client, void* edx) {
    kotor_fastcall_005f1400(
        *reinterpret_cast<void* const*>(static_cast<const unsigned char*>(client) + 4), edx);
}

// 005F1400 CClientExoAppInternal::ResetDriveAcceleration.  Null control means
// return; otherwise virtual slot +0x2C is tail-dispatched with this == control.
// EDX is forwarded untouched, as in the original.
extern "C" void __fastcall kotor_fastcall_005f1400(void* internal, void* edx) {
    void* drive = *reinterpret_cast<void* const*>(static_cast<const unsigned char*>(internal) + 0x2A0);
    if (drive != nullptr) {
        void* vtable = *reinterpret_cast<void* const*>(drive);
        auto fn = *reinterpret_cast<void(__fastcall* const*)(void*, void*)>(
            static_cast<const unsigned char*>(vtable) + 0x2C);
        fn(drive, edx);
    }
}
