#include "cc4aff633e-character_upload.hpp"
#include "ed6f7a65b7-startup_string.h"
#include "862e60d023-internal.h"

using kotor_reconstruction::StartupString;
namespace ini_comparison {
int __fastcall CompareNC005e58c0(StartupString*, void*, const char*);
}
StartupString* __fastcall GuiConcat005e5d10(StartupString*, void*, StartupString*, const StartupString*);
extern "C" {
void* __cdecl live_operator_new(unsigned);
void __cdecl live_free(void*);
void __fastcall c2da_ref_to_string_00405f70(const void*, void*, StartupString*);

void __fastcall frame_LogFileDtor_005e69e0(void*, void*);
unsigned __fastcall frame_LogFileOpened_005e6a10(void*, void*);
unsigned __fastcall frame_LogFileSize_005e6950(void*, void*);

void __fastcall kotor_fastcall_004d68a0(void*, void*, unsigned, unsigned, unsigned);
void __fastcall kotor_fastcall_004d6340(void*, void*, unsigned, unsigned);
void __fastcall kotor_fastcall_004d63e0(void*, void*, const void*, unsigned);
unsigned __fastcall kotor_fastcall_004d6440(void*, void*, void**, unsigned*);
unsigned __fastcall recovered_client_send_00677410(void*, void*, unsigned, unsigned, void*, unsigned);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
}

// 00677FA0..00678023, ECX message, RET 0Ch. DWORD count then exact BIC bytes.
extern "C" unsigned __fastcall recovered_client_character_payload_00677fa0(
    void* message, void*, const void* bytes, unsigned size, unsigned flag) {
    kotor_fastcall_004d68a0(message, nullptr, size + 4u, 0xffffffffu, 1u);
    kotor_fastcall_004d6340(message, nullptr, size, 32u);
    kotor_fastcall_004d63e0(message, nullptr, bytes, size);
    void* output;
    unsigned length;
    if (kotor_fastcall_004d6440(message, nullptr, &output, &length) == 0) return 0;
    return recovered_client_send_00677410(message, nullptr, 2u, flag == 0 ? 1u : 0x13u, output, length);
}

// 00678080..0067827B, ECX message, RET 8. File is a real four-byte CExoFile.
// Literal bytes transcribed from original: test, LOCALVAULT:, TEMP:, rb.
extern "C" unsigned __fastcall recovered_client_character_upload_00678080(
    void* message, void*, const void* reference16, unsigned flag) {
    StartupString path;                                      // 006780B1
    c2da_ref_to_string_00405f70(reference16, nullptr, &path);  // 006780C2
    {
        StartupString prefix(ini_comparison::CompareNC005e58c0(&path, nullptr, "test") != 0
            ? "LOCALVAULT:" : "TEMP:");
        // 005E5D10 constructs its output; reserve storage without a second ctor.
        alignas(4) unsigned char joined_storage[sizeof(StartupString)];
        auto* joined = reinterpret_cast<StartupString*>(joined_storage);
        StartupString* result = GuiConcat005e5d10(&prefix, nullptr, joined, &path);
        try { path.Assign005e5c50(result); }
        catch (...) { joined->~StartupString(); throw; }
        joined->~StartupString();                            // 00678147 before prefix destructor
    }
    void* file = live_operator_new(4u);                     // 00678160
    if (file != nullptr) {
        try {
            StartupString mode("rb");
            file = file_internal::exofile_construct_005e68d0(static_cast<tlk_file::File*>(file), nullptr, &path, 0x7dfu, &mode);
        } catch (...) { live_free(file); throw; }            // original constructor unwind allocation
    }
    // Original passes a failed (null) allocation straight to IsOpen 005E6A10, which faults on [ecx]; no trap substituted.
    if (frame_LogFileOpened_005e6a10(file, nullptr) == 0) {
        frame_LogFileDtor_005e69e0(file, nullptr);
        live_free(file);
        return 0;
    }
    const unsigned size = frame_LogFileSize_005e6950(file, nullptr);
    void* bytes = live_operator_new(size + 4u);              // original unsigned wrap retained
    // Original deliberately ignores short-read status and sends declared size.
    (void)file_internal::exofile_read_005e6960(static_cast<tlk_file::File*>(file), nullptr, bytes, 1u, size);
    frame_LogFileDtor_005e69e0(file, nullptr);
    live_free(file);                                         // file destroyed before payload/send
    const unsigned result = recovered_client_character_payload_00677fa0(message, nullptr, bytes, size, flag);
    live_free(bytes);                                        // original ordinary-return cleanup
    return result;                                          // path destroyed last
}


