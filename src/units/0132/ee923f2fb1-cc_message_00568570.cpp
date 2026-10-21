// Reconstruction of original swkotor.exe 00568570
//   CSWSMessage::SendServerToPlayerCCMessage
//
// Original range 00568570..00569B08 (5528 bytes), 27-entry switch table at
// 00569B08.  ABI: ECX = CSWSMessage*, stack (playerID, kind, data, flags),
// callee ret 0x10.  Dispatch is on low8(kind); every arm sends major 0x12 and
// a full 32-bit minor.  This source is the private lane successor for the
// selected W1 dispatch (NativePorts::call case 0x568570).
//
// The body deliberately preserves the original's re-reads of CSWCCMessageData
// getters, its kind 11 subtype order, its 32-bit Begin size wrap and its
// CExoString temporary lifetime (destroyed once per Str(i), with unwind
// cleanup).  No trap, stub, guard or placeholder is present.

#include <cstdint>

namespace {
using U = std::uint32_t;
using S = std::int32_t;
constexpr U kInvalidObjectId = 0x7f000000u;
}

// ---------------------------------------------------------------------------
// Selected real providers.  These are the exact decorated exports of the
// current selected candidate; all are thiscall-as-__fastcall (unused EDX,
// callee stack cleanup) except the 005E5C20 C++ member destructor.
// ---------------------------------------------------------------------------
extern "C" {
void __fastcall kotor_fastcall_004d62c0(void*, void*, U value, U bits);            // @16 WriteBYTE
void __fastcall kotor_fastcall_004d6300(void*, void*, U value, U bits);            // @16 WriteWORD
void __fastcall kotor_fastcall_004d6340(void*, void*, U value, U bits);            // @16 WriteDWORD
U    __fastcall kotor_fastcall_004d6440(void*, void*, void** out_data, U* out_size); // @16 GetWriteMessage
U    __fastcall recovered_message_get_integer_004d6480(void*, void*, U index);      // @12 GetInteger
float __fastcall feedback_get_float_004d64a0(void*, void*, U index);                // @12 GetFloat (ST0)
U    __fastcall recovered_message_get_object_004d64c0(void*, void*, U index);       // @12 GetObjectID
void __fastcall kotor_fastcall_004d68a0(void*, void*, U size, U field4c, U field48); // @20 CreateWriteMessage
void __fastcall kotor_fastcall_004d68e0(void*, void*, U value);                     // @12 WriteBOOL
void __fastcall kotor_fastcall_004d6920(void*, void*, U value_bits, U scale, U bits); // @20 WriteFLOAT
void __fastcall kotor_fastcall_004d6960(void*, void*, const void* str, U bits);     // @16 WriteCExoString
void* __fastcall recovered_event_string_004d7360(void*, void*, void* out, U index); // @16 GetString
U    __fastcall kotor_fastcall_00567e60(void*, void*, U recipient, U major, U minor,
                                        void* data, U size);                         // @28 SendServerToPlayerMessage
void __fastcall kotor_fastcall_00578570(void*, void*, U id);                        // @12 WriteOBJECTIDServer
void __fastcall kotor_fastcall_005785c0(void*, void*, void* loc, U gender);         // @16 WriteCExoLocStringServer
void* __fastcall recovered_item_lookup_004ae760(void*, void*, U id);                // @12 GetItemByID
}

namespace kotor_menu_10_19 {
// Exact layout/name of the selected 005E5C20 destructor export:
//   ?destroy_005e5c20@String@kotor_menu_10_19@@QAEXXZ
struct String {
    alignas(4) unsigned char bytes[8];
    void __thiscall destroy_005e5c20();
};
}
static_assert(sizeof(kotor_menu_10_19::String) == 8, "CExoString observed size");

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;   // [0x007A39FC] CAppManager*
}
extern "C" unsigned recovered_debug_cell_008327e4;   // [0x008327E4] kind 3 flag

namespace {

// CExoString temporary: GetString fills the eight bytes, WriteCExoString copies
// them, and the guard destroys them once.  The guard is constructed *after*
// GetString returns, so an unwind out of the getter does not destroy
// uninitialized storage; the original's SEH state is also set after the getter
// and before WriteCExoString.
struct TempString {
    kotor_menu_10_19::String value;
    __forceinline void get(void* data, U index) {
        recovered_event_string_004d7360(data, nullptr, value.bytes, index);
    }
};
struct TempGuard {
    TempString* p;
    __forceinline ~TempGuard() { p->value.destroy_005e5c20(); }
};

__forceinline void begin(void* self, U size, U field4c, U field48) {
    kotor_fastcall_004d68a0(self, nullptr, size, field4c, field48);
}
__forceinline U get_int(void* data, U index) {
    return recovered_message_get_integer_004d6480(data, nullptr, index);
}
__forceinline U get_obj(void* data, U index) {
    return recovered_message_get_object_004d64c0(data, nullptr, index);
}
__forceinline void write_byte(void* self, U value, U bits) {
    kotor_fastcall_004d62c0(self, nullptr, value, bits);
}
__forceinline void write_word(void* self, U value, U bits) {
    kotor_fastcall_004d6300(self, nullptr, value, bits);
}
__forceinline void write_dword(void* self, U value, U bits) {
    kotor_fastcall_004d6340(self, nullptr, value, bits);
}
__forceinline void write_bool(void* self, U value) {
    kotor_fastcall_004d68e0(self, nullptr, value);
}
__forceinline void write_object(void* self, U id) {
    kotor_fastcall_00578570(self, nullptr, id);
}
__forceinline void write_float_bits(void* self, U value_bits) {
    kotor_fastcall_004d6920(self, nullptr, value_bits, 0x3f800000u, 0x20u);
}
__forceinline void write_string(void* self, kotor_menu_10_19::String& str) {
    kotor_fastcall_004d6960(self, nullptr, str.bytes, 0x20u);
}
__forceinline void write_locstring(void* self, void* loc) {
    kotor_fastcall_005785c0(self, nullptr, loc, 0u);
}
__forceinline void* item_by_id(U id) {
    void* manager = kotor_menu_window_cells::manager_007a39fc;      // [0x007A39FC]
    unsigned char* server = static_cast<unsigned char*>(manager) + 8; // manager->[+8]
    void* server_app = *reinterpret_cast<void**>(server);
    return recovered_item_lookup_004ae760(server_app, nullptr, id);
}

// Shared finish/send tail: finish writes the data pointer and total size; the
// send call always uses major 0x12 and the caller-supplied 32-bit minor.
__forceinline U finish_send(void* self, U playerID, U minor) {
    void* out_data = nullptr;
    U out_size = 0;
    if (!kotor_fastcall_004d6440(self, nullptr, &out_data, &out_size)) return 0;
    return kotor_fastcall_00567e60(self, nullptr, playerID, 0x12u, minor,
                                   out_data, out_size);
}

} // namespace

// ---------------------------------------------------------------------------
// 00568570 body.  All 27 table entries (25 unique original arm addresses; kinds
// 5/6 and 25/26 share code) plus the default arm are present.
// ---------------------------------------------------------------------------
extern "C" unsigned __fastcall recovered_cc_message_00568570(
        void* self, void* /*edx unused*/, U playerID, U kind, void* data,
        U /*flags never read*/) {
    if (data == nullptr) return 0;                         // 0056859D

    // EBX at dispatch.  Kinds 2/3/11/12 execute "or ebx,-1" and kind 15
    // "xor bl,bl"; those five arms therefore take the full kind reloaded from
    // the stack argument as the wire minor.  Every other arm uses EBX.
    U ebx = kind;

    switch (static_cast<unsigned char>(kind)) {
    // ---- kind 1: 005685CE -------------------------------------------------
    case 1:
        begin(self, 8, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        write_dword(self, get_int(data, 0), 32);
        return finish_send(self, playerID, ebx);

    // ---- kind 2: 00568637 -------------------------------------------------
    case 2:
        ebx = 0xffffffffu;                                  // 00568639 or ebx,-1
        begin(self, 6, ebx, 1);
        write_object(self, get_obj(data, 0));
        for (S i = 0; i < 15; ++i) {
            if (static_cast<S>(get_int(data, static_cast<U>(i))) > -1) {
                write_bool(self, 1);
                write_word(self, get_int(data, static_cast<U>(i)), 14);
            } else {
                write_bool(self, 0);
            }
        }
        return finish_send(self, playerID, kind);           // full-kind reload

    // ---- kind 3: 005686BD -------------------------------------------------
    case 3:
        ebx = 0xffffffffu;                                  // 005686BF or ebx,-1
        begin(self, 0xa, ebx, 1);
        write_object(self, get_obj(data, 0));
        write_object(self, get_obj(data, 1));
        for (S i = 0; i < 15; ++i) {
            if (static_cast<S>(get_int(data, static_cast<U>(i))) > -1) {
                write_bool(self, 1);
                write_word(self, get_int(data, static_cast<U>(i)), 14);
            } else {
                write_bool(self, 0);
            }
        }
        write_bool(self, recovered_debug_cell_008327e4);     // 00568726 [0x8327E4]
        if (recovered_debug_cell_008327e4 != 0) {            // 00568734 [0x8327E4]
            TempString tmp;
            tmp.get(data, 0);
            TempGuard guard{&tmp};
            write_string(self, tmp.value);
        }
        return finish_send(self, playerID, kind);           // full-kind reload

    // ---- kind 4: 00568931 -------------------------------------------------
    case 4:
        begin(self, 0xc, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        write_object(self, get_obj(data, 1));
        write_dword(self, get_int(data, 0), 32);
        return finish_send(self, playerID, ebx);

    // ---- kinds 5 and 6: 00568795 (header-only send) ------------------------
    case 5:
    case 6:
        return kotor_fastcall_00567e60(self, nullptr, playerID, 0x12u, ebx,
                                       nullptr, 0);

    // ---- kind 7: 0056889D -------------------------------------------------
    case 7:
        begin(self, 0xa, 0xffffffffu, 1);
        write_byte(self, get_int(data, 0), 8);
        write_byte(self, get_int(data, 1), 8);
        write_byte(self, get_int(data, 2), 5);
        write_byte(self, get_int(data, 3), 8);
        write_byte(self, get_int(data, 4), 8);
        write_byte(self, get_int(data, 5), 7);
        write_object(self, get_int(data, 6));
        return finish_send(self, playerID, ebx);

    // ---- kind 8: 00568976 -------------------------------------------------
    case 8:
        begin(self, 0x1c, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        write_object(self, get_obj(data, 2));
        write_dword(self, get_int(data, 0), 32);
        if (get_obj(data, 1) == kInvalidObjectId) {          // 005689B6..C6
            write_bool(self, 0);
        } else {
            write_bool(self, 1);
            void* item = item_by_id(get_obj(data, 1));       // 005689CF..E4
            if (item != nullptr) {
                write_locstring(self,
                    static_cast<unsigned char*>(item) + 0x280); // 005689ED..F7
            }
        }
        write_dword(self, get_int(data, 1), 32);
        write_dword(self, get_int(data, 2), 32);
        write_dword(self, get_int(data, 3), 32);
        return finish_send(self, playerID, ebx);

    // ---- kind 9: 00568A42 -------------------------------------------------
    case 9:
        begin(self, 0x20, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        write_object(self, get_obj(data, 1));
        write_dword(self, get_int(data, 0), 32);
        write_byte(self, get_int(data, 1), 5);
        write_byte(self, get_int(data, 2), 8);
        write_byte(self, get_int(data, 3), 7);
        write_bool(self, get_int(data, 4));
        write_byte(self, get_int(data, 5), 8);
        write_dword(self, get_int(data, 6), 32);
        return finish_send(self, playerID, ebx);

    // ---- kind 10: 005687AB ------------------------------------------------
    case 10:
        begin(self, 8, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        write_byte(self, get_int(data, 0), 8);
        return finish_send(self, playerID, ebx);

    // ---- kind 11: 00568BEE (subtype multiplexer) --------------------------
    case 11:
        ebx = 0xffffffffu;                                  // 00568BF0 or ebx,-1
        begin(self, 0x80, ebx, 1);
        write_word(self, get_int(data, 9), 16);
        if (get_obj(data, 0) != kInvalidObjectId &&
            get_int(data, 9) != 0x32 && get_int(data, 9) != 0x33 &&
            get_int(data, 9) != 0xa0 && get_int(data, 9) != 0xa1 &&
            get_int(data, 9) != 0xa2 && get_int(data, 9) != 0xa4 &&
            get_int(data, 9) != 0xa5 && get_int(data, 9) != 0xa3) {
            write_object(self, get_obj(data, 0));
        }
        if (get_int(data, 9) == 1 || get_int(data, 9) == 5 ||
            get_int(data, 9) == 0x47 || get_int(data, 9) == 0x6c ||
            get_int(data, 9) == 0x94 || get_int(data, 9) == 0x95 ||
            get_int(data, 9) == 0x97 || get_int(data, 9) == 0xe4 ||
            get_int(data, 9) == 0xb6 || get_int(data, 9) == 0xb7 ||
            get_int(data, 9) == 0xde) {
            write_dword(self, get_int(data, 0), 32);
        }
        if (get_int(data, 9) == 0x3e || get_int(data, 9) == 0x44 ||
            get_int(data, 9) == 0x3f || get_int(data, 9) == 0x42 ||
            get_int(data, 9) == 0x40 || get_int(data, 9) == 0x43 ||
            get_int(data, 9) == 0x45 || get_int(data, 9) == 0x46 ||
            get_int(data, 9) == 0x47 || get_int(data, 9) == 0x6c ||
            get_int(data, 9) == 0x8e) {
            write_dword(self, get_int(data, 1), 32);
        }
        if (get_int(data, 9) == 0x3e || get_int(data, 9) == 0x42 ||
            get_int(data, 9) == 0x43 || get_int(data, 9) == 0x46 ||
            get_int(data, 9) == 0x8e) {
            write_dword(self, get_int(data, 2), 32);
        }
        if (get_int(data, 9) == 0x8e) {                      // 00568E89
            union { float f; U u; } value;
            value.f = feedback_get_float_004d64a0(data, nullptr, 0); // 00568E99
            write_float_bits(self, value.u);                  // 00568EA4..AF
        }
        if (get_int(data, 9) == 0x32 || get_int(data, 9) == 0x33) {
            void* item = item_by_id(get_obj(data, 0));        // 00568ED0..E5
            if (item != nullptr) {
                write_bool(self, 1);
                write_locstring(self,
                    static_cast<unsigned char*>(item) + 0x280); // 00568EF0..F04
            }
        }
        if (get_int(data, 9) == 4 || get_int(data, 9) == 0x2f) {
            void* item = item_by_id(get_obj(data, 1));        // 00568F25..3A
            if (item != nullptr) {
                write_bool(self, 1);
                write_locstring(self,
                    static_cast<unsigned char*>(item) + 0x280); // 00568F45..59
            }
        }
        if (get_int(data, 9) == 0x5d &&
            get_obj(data, 0) != kInvalidObjectId) {
            write_dword(self, get_int(data, 0), 32);
        }
        if (get_int(data, 9) == 0x5e &&
            get_obj(data, 0) != kInvalidObjectId) {
            TempString tmp;
            tmp.get(data, 0);
            TempGuard guard{&tmp};
            write_string(self, tmp.value);
        }
        if (get_int(data, 9) == 0xa0) {
            write_object(self, get_obj(data, 1));
            {
                TempString tmp;
                tmp.get(data, 0);
                TempGuard guard{&tmp};
                write_string(self, tmp.value);
            }
            {
                TempString tmp;
                tmp.get(data, 1);
                TempGuard guard{&tmp};
                write_string(self, tmp.value);
            }
            write_dword(self, get_int(data, 0), 32);
        }
        if (get_int(data, 9) == 0xa1 || get_int(data, 9) == 0xa2 ||
            get_int(data, 9) == 0xa4 || get_int(data, 9) == 0xa5 ||
            get_int(data, 9) == 0xa3) {
            write_object(self, get_obj(data, 1));
            {
                TempString tmp;
                tmp.get(data, 1);
                TempGuard guard{&tmp};
                write_string(self, tmp.value);
            }
            write_dword(self, get_int(data, 0), 32);
            write_dword(self, get_int(data, 1), 32);
            if (get_int(data, 9) == 0xa3) {
                {
                    TempString tmp;
                    tmp.get(data, 2);
                    TempGuard guard{&tmp};
                    write_string(self, tmp.value);
                }
                write_dword(self, get_int(data, 2), 32);
                write_dword(self, get_int(data, 3), 32);
            }
        }
        if (get_int(data, 9) == 0xcc) {
            TempString tmp;
            tmp.get(data, 0);
            TempGuard guard{&tmp};
            write_string(self, tmp.value);
        }
        if (get_int(data, 9) == 0x8f) {
            write_dword(self, get_int(data, 0), 32);
        }
        return finish_send(self, playerID, kind);           // 005691E3 full kind

    // ---- kind 12: 00569438 ------------------------------------------------
    case 12:
        ebx = 0xffffffffu;                                  // 0056943A or ebx,-1
        begin(self, 0xe, ebx, 1);
        {
            TempString tmp;
            tmp.get(data, 0);
            TempGuard guard{&tmp};
            write_string(self, tmp.value);
        }
        write_dword(self, get_int(data, 0), 32);
        write_dword(self, get_int(data, 1), 32);
        write_dword(self, get_int(data, 2), 32);
        return finish_send(self, playerID, kind);           // full-kind reload

    // ---- kind 13: 00568AF7 ------------------------------------------------
    case 13:
        begin(self, 0xc, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        write_object(self, get_obj(data, 1));
        write_byte(self, get_int(data, 0), 8);
        write_byte(self, get_int(data, 1), 5);
        write_byte(self, get_int(data, 2), 4);
        write_bool(self, get_int(data, 3));
        if (static_cast<S>(get_int(data, 4)) > 0) {          // 00568B77 jle
            write_bool(self, 1);
            write_byte(self, get_int(data, 4), 7);
        } else {
            write_bool(self, 0);
        }
        return finish_send(self, playerID, ebx);

    // ---- kind 14: 00568BA7 ------------------------------------------------
    case 14:
        begin(self, 8, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        write_byte(self, get_int(data, 0), 5);
        write_byte(self, get_int(data, 1), 8);
        return finish_send(self, playerID, ebx);

    // ---- kind 15: 0056921E ------------------------------------------------
    case 15:
        begin(self, get_int(data, 0) * 2u + 5u, 0xffffffffu, 1); // 00569226..32
        write_object(self, get_obj(data, 0));
        write_byte(self, get_int(data, 0), 8);               // 0056924E
        ebx &= 0xffffff00u;                                  // 0056925F xor bl,bl
        {
            unsigned char bl = static_cast<unsigned char>(ebx);
            if (static_cast<unsigned char>(get_int(data, 0)) != 0) { // 00569261
                do {
                    write_word(self,
                        get_int(data, static_cast<U>(bl) + 1u), 9); // 00569270..81
                    ++bl;
                } while (bl < static_cast<unsigned char>(get_int(data, 0))); // 0056928C
            }
        }
        return finish_send(self, playerID, kind);           // full-kind reload

    // ---- kind 16: 005692B3 ------------------------------------------------
    case 16:
        begin(self, 0x20, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        write_object(self, get_obj(data, 1));
        write_bool(self, get_int(data, 0));
        for (U i = 1; i <= 10; ++i) write_word(self, get_int(data, i), 16);
        return finish_send(self, playerID, ebx);

    // ---- kind 17: 005687FD ------------------------------------------------
    case 17:
        if (get_int(data, 2) == 0) return 1;                 // 005687FD..0F
        begin(self, 0x14, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        write_object(self, get_obj(data, 1));
        write_byte(self, get_int(data, 0), 8);
        write_byte(self, get_int(data, 1), 8);
        write_byte(self, get_int(data, 2), 8);
        return finish_send(self, playerID, ebx);

    // ---- kind 18: 005694CB ------------------------------------------------
    case 18:
        begin(self, 0x2a, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        write_object(self, get_obj(data, 1));
        write_dword(self, get_int(data, 0), 32);
        write_word(self, get_int(data, 1), 16);
        for (U i = 2; i <= 8; ++i) write_dword(self, get_int(data, i), 32);
        write_bool(self, get_int(data, 9));
        write_bool(self, get_int(data, 10));
        write_bool(self, get_int(data, 11));
        return finish_send(self, playerID, ebx);

    // ---- kind 19: 005693CD ------------------------------------------------
    case 19:
        begin(self, 0xc, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        write_object(self, get_obj(data, 1));
        write_word(self, get_int(data, 0), 16);
        write_word(self, get_int(data, 1), 16);
        write_word(self, get_int(data, 2), 16);
        return finish_send(self, playerID, ebx);

    // ---- kind 20: 005695F4 ------------------------------------------------
    case 20:
        begin(self, 0x54, 0xffffffffu, 1);
        for (U i = 0; i <= 20; ++i) write_dword(self, get_int(data, i), 32);
        return finish_send(self, playerID, ebx);

    // ---- kind 21: 00569785 ------------------------------------------------
    case 21:
        begin(self, 0x20, 0xffffffffu, 1);
        for (U i = 0; i <= 6; ++i) write_dword(self, get_int(data, i), 32);
        return finish_send(self, playerID, ebx);

    // ---- kind 22: 0056980C ------------------------------------------------
    case 22:
        begin(self, 0x24, 0xffffffffu, 1);
        for (U i = 0; i <= 8; ++i) write_dword(self, get_int(data, i), 32);
        return finish_send(self, playerID, ebx);

    // ---- kind 23: 005698B9 ------------------------------------------------
    case 23:
        begin(self, 0x70, 0xffffffffu, 1);
        for (U i = 0; i <= 27; ++i) write_dword(self, get_int(data, i), 32);
        return finish_send(self, playerID, ebx);

    // ---- kind 24 and default: 00569ABE ------------------------------------
    case 24:
    default:
        // No Begin.  A fresh message (self+0xC == 0) fails finish and returns 0;
        // pre-existing state from an earlier message is sent unchanged.
        return finish_send(self, playerID, ebx);

    // ---- kinds 25 and 26: 005699D5 ----------------------------------------
    case 25:
    case 26:
        begin(self, 0x28, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        for (U i = 0; i <= 8; ++i) write_dword(self, get_int(data, i), 32);
        return finish_send(self, playerID, ebx);

    // ---- kind 27: 00569A8F ------------------------------------------------
    case 27:
        begin(self, 8, 0xffffffffu, 1);
        write_object(self, get_obj(data, 0));
        write_dword(self, get_int(data, 0), 32);
        return finish_send(self, playerID, ebx);
    }
}
