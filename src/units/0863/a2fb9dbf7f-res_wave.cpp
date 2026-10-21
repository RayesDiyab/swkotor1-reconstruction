// lane-sound rev3: CResWave (sound wave resource) and its vtable 0074D398.
//   005DF1B0 CResWave::CResWave            RET 0, EAX=this   (@res_wave_construct_005df1b0@8)
//   005DF1E0 CResWave::~CResWave           vtable 0074D398, tail JMP CRes::~CRes 00409D40
//   005DF380 scalar deleting dtor          (slot 0)
//   slot 1 00406490 / slot 2 0063E7F0      shared CRes constant-word leaves (existing M46 bodies)
//   005DF1F0 OnResourceFreed               (slot 3)
//   005DF230 OnResourceServiced            (slot 4)
//   005DF360 GetData                       (slot 5)
// Image vtable 0074D398 = {005DF380, 00406490, 0063E7F0, 005DF1F0, 005DF230, 005DF360}
// (next table 0074D3B0 belongs to CResLYT 005DF3A0). Slots typed per D1/D21:
// ECX receiver -> __fastcall(self, unused EDX), stdcall no-arg leaves for 1/2.
#include <cstdint>
#include <cstring>

namespace menu_resources { struct Manager; }
extern "C" {
using Word = std::uint32_t;
void* __fastcall c2da_resource_base_ctor_00406460(void*, void*);               // CRes::CRes (M46)
void __fastcall c2da_resource_base_destroy_00409d40(void* self, void* unused_edx); // CRes::~CRes (M46)
Word __stdcall c2da_resource_constant_word_00406490();                         // shared slot-1 leaf (M46)
Word __stdcall c2da_resource_constant_word_0063e7f0();                         // shared slot-2 leaf (M46)
void __fastcall c2da_manager_00406540(menu_resources::Manager*, void*, void*);   // CExoResMan::FreeResourceData (M46)
menu_resources::Manager* __cdecl c2da_manager_007a39e8();                       // [007A39E8] (M46)
void __cdecl live_free(void*);                                                  // 006FA390
extern volatile std::uint32_t kotor_cell_stream_mode_007a2374;                  // lane-sound owner (image value 1)
__declspec(dllimport) std::int32_t __stdcall AIL_decompress_ASI(const void*, std::uint32_t, const char*, void**, std::uint32_t*, void*);
__declspec(dllimport) std::int32_t __stdcall AIL_decompress_ADPCM(const void*, void**, std::uint32_t*);
__declspec(dllimport) std::int32_t __stdcall AIL_WAV_info(const void*, void*);
__declspec(dllimport) void __stdcall AIL_mem_free_lock(void*);
void* __fastcall res_wave_construct_005df1b0(void* self, void* unused_edx);
void __fastcall res_wave_destroy_005df1e0(void* self, void* unused_edx);
void* __fastcall res_wave_deleting_destroy_005df380(void* self, void* unused_edx, Word flags);
Word __fastcall res_wave_freed_005df1f0(void* self, void* unused_edx);
Word __fastcall res_wave_serviced_005df230(void* self, void* unused_edx);
void* __fastcall res_wave_get_data_005df360(void* self, void* unused_edx);
}

namespace kotor_res_wave {
struct Methods0074d398 {
    void* (__fastcall* deleting_destroy_005df380)(void*, void*, Word);
    Word (__stdcall* constant_word_00406490)();
    Word (__stdcall* constant_word_0063e7f0)();
    Word (__fastcall* freed_005df1f0)(void*, void*);
    Word (__fastcall* serviced_005df230)(void*, void*);
    void* (__fastcall* get_data_005df360)(void*, void*);
};
static_assert(sizeof(Methods0074d398) == 24, "six x86 cells");
extern const Methods0074d398 method_table_0074d398;
const Methods0074d398 method_table_0074d398 = {
    &res_wave_deleting_destroy_005df380, &c2da_resource_constant_word_00406490, &c2da_resource_constant_word_0063e7f0,
    &res_wave_freed_005df1f0, &res_wave_serviced_005df230, &res_wave_get_data_005df360};
}

namespace {
unsigned char* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }
Word rd(void* p, unsigned off) { Word v; std::memcpy(&v, at(p, off), 4); return v; }
void wr(void* p, unsigned off, Word v) { std::memcpy(at(p, off), &v, 4); }
void* rp(void* p, unsigned off) { void* v; std::memcpy(&v, at(p, off), 4); return v; }
void wp(void* p, unsigned off, const void* v) { std::memcpy(at(p, off), &v, 4); }
const Word tag_bmu = 0x20554d42u, tag_v10 = 0x302e3156u, tag_riff = 0x46464952u; // "BMU ", "V1.0", "RIFF"
}

extern "C" {
// 005DF1B0 CResWave::CResWave, RET 0, EAX=this.
void* __fastcall res_wave_construct_005df1b0(void* self, void*) {
    (void)c2da_resource_base_ctor_00406460(self, nullptr);           // 005DF1B3
    wr(self, 0x30, 0); wr(self, 0x28, 0); wr(self, 0x2c, 0); wr(self, 0x34, 0); wr(self, 0x38, 0); // 005DF1B8-005DF1C6
    wp(self, 0, &kotor_res_wave::method_table_0074d398);             // 005DF1C9
    return self;                                                     // 005DF1CF
}
// 005DF1E0 CResWave::~CResWave, RET 0.
void __fastcall res_wave_destroy_005df1e0(void* self, void*) {
    wp(self, 0, &kotor_res_wave::method_table_0074d398);             // 005DF1E0
    c2da_resource_base_destroy_00409d40(self, nullptr);              // 005DF1E6 (tail JMP)
}
// 005DF380 scalar deleting dtor, RET 4, EAX=this.
void* __fastcall res_wave_deleting_destroy_005df380(void* self, void*, Word flags) {
    res_wave_destroy_005df1e0(self, nullptr);                        // 005DF383
    if (flags & 1) live_free(self);                                  // 005DF388-005DF390
    return self;                                                     // 005DF398
}
// 005DF1F0 CResWave::OnResourceFreed, RET 0, EAX=1.
Word __fastcall res_wave_freed_005df1f0(void* self, void*) {
    if (rd(self, 0x28) != 0) {                                       // 005DF1F3-005DF1FB
        AIL_mem_free_lock(rp(self, 0x10));                           // 005DF1FD-005DF201
        wr(self, 0x10, 0);                                           // 005DF207
    }
    wr(self, 0x30, 0); wr(self, 0x28, 0); wr(self, 0x2c, 0); wr(self, 0x34, 0); wr(self, 0x38, 0); // 005DF20A-005DF216
    return 1;                                                        // 005DF21A
}
// 005DF360 CResWave::GetData, RET 0.
void* __fastcall res_wave_get_data_005df360(void* self, void*) {
    if (rd(self, 0x28) == 0 && rd(self, 0x2c) == 1) return at(rp(self, 0x10), 0x1d6); // 005DF360-005DF375
    return rp(self, 0x10);                                           // 005DF376
}
// 005DF230 CResWave::OnResourceServiced, RET 0, EAX 0/1.
Word __fastcall res_wave_serviced_005df230(void* self, void*) {
    void* out = nullptr;                                             // [esp+8] (decompressed buffer)
    alignas(4) unsigned char info[0x24];                             // [esp+0Ch] AILSOUNDINFO
    std::memset(info, 0, sizeof info);                               // (D2-style: original stack bytes are written by the callee)
    unsigned char* data = static_cast<unsigned char*>(rp(self, 0x10)); // 005DF23D
    auto finish_decoded = [&]() -> Word {                            // 005DF28A-005DF2AF / 005DF332-005DF34F
        wr(self, 0x28, 1);
        c2da_manager_00406540(c2da_manager_007a39e8(), nullptr, self); // CExoResMan::FreeResourceData
        wp(self, 0x10, out);
        wr(self, 0x30, 1);
        return 1;
    };
    auto asi = [&]() -> Word {                                       // 005DF25A-005DF284
        const Word size = rd(self, 0x18);                            // 00644760 GetProperty0x18
        if (!AIL_decompress_ASI(data + 8, size - 8, ".mp3", &out, reinterpret_cast<std::uint32_t*>(at(self, 0x34)), nullptr)) return 0; // 005DF353
        return finish_decoded();
    };
    auto wav_checked = [&]() -> Word {                               // 005DF313-005DF34F (EDI=1)
        Word format; std::memcpy(&format, info, 4);
        if (format != 0x11) { wr(self, 0x30, 1); return 1; }         // 005DF313-005DF318 -> 005DF348
        if (!AIL_decompress_ADPCM(info, &out, reinterpret_cast<std::uint32_t*>(at(self, 0x34)))) return 0; // 005DF31A-005DF330
        return finish_decoded();
    };
    auto wav_plain = [&]() -> Word {                                 // 005DF2FE-005DF311
        if (AIL_WAV_info(data, info) != 1) return 0;
        return wav_checked();
    };
    auto riff_at_1d6 = [&]() -> Word {                               // 005DF2B8-005DF2E7
        unsigned char* inner = data + 0x1d6;
        Word tag; std::memcpy(&tag, inner, 4);
        if (tag != tag_riff) return 0;                               // 005DF2C3-005DF2C9 -> 005DF353
        wr(self, 0x2c, 1);                                           // 005DF2DA
        if (AIL_WAV_info(inner, info) != 1) { wr(self, 0x30, 1); return 1; } // 005DF2DD-005DF2E5 -> 005DF348
        return wav_checked();                                        // 005DF2E7
    };
    Word w0, w1; std::memcpy(&w0, data, 4); std::memcpy(&w1, data + 4, 4);
    if (kotor_cell_stream_mode_007a2374 == 0) {                      // 005DF230-005DF241 -> 005DF2E9
        if (w0 == tag_bmu && w1 == tag_v10) return asi();            // 005DF2E9-005DF2F8
        return wav_plain();                                          // 005DF2FE
    }
    if (w0 == tag_bmu) {                                             // 005DF247-005DF24F
        if (w1 == tag_v10) return asi();                             // 005DF251-005DF258
        return riff_at_1d6();                                        // 005DF258 -> 005DF2B8
    }
    if (w0 == tag_riff) return wav_plain();                          // 005DF2B0-005DF2B6
    return riff_at_1d6();                                            // 005DF2B8
}
}
