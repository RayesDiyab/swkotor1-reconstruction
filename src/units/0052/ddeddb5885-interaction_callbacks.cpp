// Five original clicked callbacks and real attack/skill senders; original payload zeros retained.
#include "8fbd11ef5e-w5_server_env.hpp"
#include <cstring>
using namespace w5;
extern "C" {
void* __fastcall kotor_fastcall_005ed6f0(void*, void*);
void __fastcall kotor_fastcall_004d68a0(void*, void*, U, U, U);
void __fastcall kotor_fastcall_004d68e0(void*, void*, U);
void __fastcall kotor_fastcall_004d6300(void*, void*, U, U);
void __fastcall kotor_fastcall_004d62c0(void*, void*, U, U);
void __fastcall kotor_fastcall_004d6920(void*, void*, U, U, U);
U __fastcall kotor_fastcall_004d6440(void*, void*, void**, U*);
U __fastcall recovered_client_send_00677410(void*, void*, U, U, void*, U);
I __fastcall show_tutorial_window_005edf40(void*, void*, U, U, U, U);
void __fastcall kotor_fastcall_0063d470(void*, void*);
void __fastcall kotor_fastcall_00610a10(void*, void*, U);
void __fastcall kotor_fastcall_005edf30(void*, void*);
// [00677A90,00677B03),115bytes,RET8/native@16,major6/minor2.
U __fastcall kotor_fastcall_00677a90(void* message, void*, U target, U mode) {
    kotor_fastcall_004d68a0(message, nullptr, 6, 0xffffffffu, 1);
    kotor_fastcall_004d68e0(message, nullptr, target);
    kotor_fastcall_004d6300(message, nullptr, mode, 16);
    void* bytes; U size;
    if (kotor_fastcall_004d6440(message, nullptr, &bytes, &size) == 0) return 0;
    return recovered_client_send_00677410(message, nullptr, 6, 2, bytes, size);
}
// [00677B10,00677BC5),181bytes,RET10/native@24,major6/minor7 (not4).
U __fastcall kotor_fastcall_00677b10(void* message, void*, U skill, U subtype, U target, const Vector* position) {
    kotor_fastcall_004d68a0(message, nullptr, 18, 0xffffffffu, 1);
    kotor_fastcall_004d62c0(message, nullptr, skill, 8);
    kotor_fastcall_004d62c0(message, nullptr, subtype, 8);
    kotor_fastcall_004d68e0(message, nullptr, target);
    U x, y, z;
    std::memcpy(&x, &position->x, 4);
    kotor_fastcall_004d6920(message, nullptr, x, 0x3f800000u, 32);
    std::memcpy(&y, &position->y, 4);
    kotor_fastcall_004d6920(message, nullptr, y, 0x3f800000u, 32);
    std::memcpy(&z, &position->z, 4);
    kotor_fastcall_004d6920(message, nullptr, z, 0x3f800000u, 32);
    void* bytes; U size;
    if (kotor_fastcall_004d6440(message, nullptr, &bytes, &size) == 0) return 0;
    return recovered_client_send_00677410(message, nullptr, 6, 7, bytes, size);
}
// [0060EEE0,0060EF02),34bytes,RET4/native@12; self is unused by original.
void __fastcall kotor_fastcall_0060eee0(void*, void*, U target) {
    void* message = kotor_fastcall_005ed6f0(client_app(), nullptr);
    if (message != nullptr) (void)kotor_fastcall_00677a90(message, nullptr, target, 0);
}
// [0060EE00,0060EE58),88bytes,RET0C/native@20; original zero-vector skill request.
void __fastcall kotor_fastcall_0060ee00(void*, void*, U skill, U subtype, U target) {
    if (target == 0x7f000000u) return;
    void* message = kotor_fastcall_005ed6f0(client_app(), nullptr);
    if (message != nullptr) {
        const Vector zero{0.0f, 0.0f, 0.0f};
        (void)kotor_fastcall_00677b10(message, nullptr, skill, subtype, target, &zero);
    }
}
// [005EDF90,005EDFA0),16bytes,RET4/native@12;[005EDFA0,005EDFAA),10bytes,RET0/native@8.
void __fastcall kotor_fastcall_005edf90(void* client, void*, U value) { at<U>(at<void*>(client, 4), 0x3e0) = value; }
U __fastcall kotor_fastcall_005edfa0(void* client, void*) { return at<U>(at<void*>(client, 4), 0x3e0); }
// [006826A0,006826DA),58bytes,RET8/native@16.
void __fastcall kotor_fastcall_006826a0(void* target, void*, U, void* actor) {
    if (actor != nullptr && show_tutorial_window_005edf40(client_app(), nullptr, 0x21, at<U>(actor, 4), at<U>(target, 4), 0) == 0)
        kotor_fastcall_0060eee0(actor, nullptr, at<U>(target, 4));
}
// [00683E90,00683EDE),78bytes,RET8/native@16.
void __fastcall kotor_fastcall_00683e90(void* target, void*, U, void* actor) {
    if (actor != nullptr && show_tutorial_window_005edf40(client_app(), nullptr, 0x21, at<U>(actor, 4), at<U>(target, 4), 1) == 0) {
        kotor_fastcall_0063d470(actor, nullptr);
        kotor_fastcall_0060eee0(actor, nullptr, at<U>(target, 4));
        kotor_fastcall_005edf30(client_app(), nullptr);
    }
}
// [00691900,0069194A),74bytes,RET8/native@16; action/actor unused, real message required.
void __fastcall kotor_fastcall_00691900(void* target, void*, U, void*) {
    void* message = kotor_fastcall_005ed6f0(client_app(), nullptr);
    if (message != nullptr) {
        const Vector zero{0.0f, 0.0f, 0.0f};
        (void)kotor_fastcall_00677b10(message, nullptr, 1, 0, at<U>(target, 4), &zero);
    }
}
// [00691950,00691964),20bytes,RET8/native@16; original actor method ignores self.
void __fastcall kotor_fastcall_00691950(void* target, void*, U, void* actor) {
    kotor_fastcall_0060ee00(actor, nullptr, 1, 0x65, at<U>(target, 4));
}
// [00616800,00616890),144bytes,RET8/native@16; original required actor dereference preserved.
void __fastcall kotor_fastcall_00616800(void* target, void*, U, void* actor) {
    if (kotor_fastcall_005edfa0(client_app(), nullptr) > 0 &&
        show_tutorial_window_005edf40(client_app(), nullptr, 0x23, at<U>(actor, 4), at<U>(target, 4), 0) != 0) return;
    if (show_tutorial_window_005edf40(client_app(), nullptr, 0x22, at<U>(actor, 4), at<U>(target, 4), 0) != 0) return;
    kotor_fastcall_00610a10(actor, nullptr, 1);
    kotor_fastcall_005edf90(client_app(), nullptr, 3000);
    const U id = at<U>(target, 4);
    (void)kotor_fastcall_00677a90(kotor_fastcall_005ed6f0(client_app(), nullptr), nullptr, id, 0);
}
}
