// Included at EOF in the sole selected W1 native TU. No duplicated address body/storage.
namespace w2 { namespace g07 {
unsigned member_list_005be780(kotor::server_mainloop::Memory&, unsigned, unsigned);
} }
extern "C" void* __fastcall combat_reuse_faction_00513fc0(void* self, void*) {
    W1Memory memory;
    return ptr(kotor::server_mainloop::creature_faction_00513fc0(memory, word(self)));
}
extern "C" unsigned __fastcall combat_reuse_members_005be780(void* faction, void*, void** output) {
    // 005BE780: mov eax,[ecx]; mov edx,[esp+4]; mov [edx],eax; mov eax,[ecx+4]; ret 4. The original dereferences a
    // null faction unchecked (access violation at [0]); the W2 body performs the same reads in the same order.
    W1Memory memory;
    return w2::g07::member_list_005be780(memory, word(faction), word(output));
}
extern "C" void* __cdecl combat_reuse_message_004aedb0() {
    NativePorts ports;
    return ptr(w1::sws_message(ports));
}
