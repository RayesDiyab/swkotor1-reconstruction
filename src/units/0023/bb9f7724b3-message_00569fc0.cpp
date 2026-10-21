// Reconstruction of original swkotor.exe 00569FC0
//   CSWSMessage::SendServerToPlayerItemUpdate_DestroyItem
//   (base source item-notify path)
//
// Original range 00569FC0..0056A01D.  ABI: ECX = CSWSMessage; stack
// (CSWSPlayer* player, unsigned itemID); callee ret 8.  Begin(4,-1,1), write
// OBJECTID(itemID), finish; on finish failure return 0.  On success send
// major 5, minor 7, data/size to player->[+4] and return the send result.
//
// The original dereferences player->[+4] without a null check; that fault is
// preserved.  No trap/stub/guard.

#include <cstdint>

extern "C" {
void __fastcall kotor_fastcall_004d68a0(
        void*, void*, unsigned, unsigned, unsigned);
void __fastcall kotor_fastcall_00578570(void*, void*, unsigned);
unsigned __fastcall kotor_fastcall_004d6440(void*, void*, void**, unsigned*);
unsigned __fastcall kotor_fastcall_00567e60(
        void*, void*, void* recipient, unsigned, unsigned, void*, unsigned);
}

extern "C" unsigned __fastcall recovered_item_update_notify_00569fc0(
        void* self, void* /*edx unused*/, void* player, unsigned itemID) {
    kotor_fastcall_004d68a0(self, nullptr, 4, 0xffffffffu, 1); // 00569FC4..D1
    kotor_fastcall_00578570(self, nullptr, itemID);            // 00569FD1..D8

    void* data = nullptr;
    unsigned size = 0;
    if (!kotor_fastcall_004d6440(self, nullptr, &data, &size)) // 00569FDD..F0
        return 0;

    void* recipient = *reinterpret_cast<void**>(               // 0056A001..05
        static_cast<unsigned char*>(player) + 4);
    return kotor_fastcall_00567e60(                            // 0056A008..16
        self, nullptr, recipient, 5, 7, data, size);
}
