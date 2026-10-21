// claude_snapshot.cpp (lane worker snapshot): AurSnapshot 004211A0, the in-game screenshot writer, transcribed from the
// original swkotor.exe (read-only).
//   004211A0 AurSnapshot(char const* name, uchar include_gui)   cdecl, RET 0      frame_AurSnapshot_004211a0
// Sole caller: CClientExoAppInternal::MainLoop 00603E2B..00603E6E (rebuilt in 0413 ContinueSuffix, c46 0077ED8D), which
// calls it when client+150h != 0 && [007A2284] != 0 (.data 1, never written) with (client+158h, include_gui) where
// include_gui = ([client+4]->[+5Ch] & 1 (GUIsInScreenShot)) || [client+274h]->[+71h]; afterwards it clears +150h.
// client+150h is set by the screenshot input event 50h (00621555..006215EB: sprintf(client+158h, "KotOR%04i.tga",
// [client+154h]), probed with fopen "rb" and suffixed a..z while the file exists) and by PlayBackInputEvents 00622762.
// Event 50h is bound by CClientExoAppInternal::SetEventDescriptions 005EED06..005EED65 only when CClientOptions+58h
// (swkotor.ini [Game Options] EnableScreenShot, LoadOptions 0061F5ED..0061F67C; default 0, SetDefaultOptions 0061DB1E)
// is nonzero: CreateNewEvent(50h, 1, [0074D3C8]=0, [0074D478]=26h, 84h) + AddEvent(50h, 0/2/4); key code 26h is
// DIK_SYSRQ (CExoInputInternal ctor 005E1D47..005E1D4D: table[26h] = B7h), i.e. the Print Screen key.
// The file name is relative (no alias), so the TGA is written into the process working directory, as the original
// (the run launcher's cwd is the run root, never the install).
// Callees (existing reviewed providers, exact decorated names in the c46 map):
//   006FADB0 sprintf            _format_006fadb0                  0310-tpc-joins
//   0070ACAF _stricmp           _kotor_stricmp_0070acaf           0155-string-compare-existing
//   0073D244 glGetIntegerv      kotor_renderer_init::glGetIntegerv_0073d244 (windows_gl_imports -> opengl32)
//   0073D250 glReadPixels       kotor_renderer_init::glReadPixels_0073d250  (windows_gl_imports -> opengl32)
//   006FA7E6 operator new       _live_operator_new;  006FA390 free  _live_free
//   0045D990 ImageFlipY         _kotor_cdecl_0045d990             claude_closeB_save (reviewed REVIEW-CLOSURE)
//   0045F2A0 ImageWriteTGA      _kotor_cdecl_0045f2a0             claude_closeB_save (18-byte type-2 header, BGR swap)
//   00420F20 AurSaveGameSnapshot _kotor_cdecl_00420f20            claude_closeB_save
// .rdata constant [0073F370] = 1907h (GL_RGB; read only at 0042109B / 004212B1, never written).
// .bss cell rebuilt here: [007A47B4] snapshot counter (read/written only by 0042122A..0042123B).
// Quirks kept: the name is copied into a 64-byte frame buffer without a bound (the original overruns its frame; /GS
// fast-fails here); the width is rounded up to a multiple of 4 (signed remainder) and glReadPixels reads that rounded
// width; the no-GUI arm passes the ORIGINAL name pointer (NULL / "NULL" included, so the locally built "snap%d.tga"
// name and the 007A47B4 increment are unused there) and the rounded width as AurSaveGameSnapshot's size with
// square = pad = 0 (whose width <= height arm then uses the name pointer value as the output width, see closeB).
#include <cstdint>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
constexpr U k_gl_viewport = 0xba2;                                  // 0042124D push 0BA2h
constexpr U k_gl_rgb_0073f370 = 0x1907;                             // [0073F370]
constexpr U k_gl_unsigned_byte = 0x1401;                            // 004212A7 push 1401h
const char k_null_0073ee04[] = "NULL";                              // 0073EE04 (5 bytes compared incl. NUL)
const char k_snap_0073ee18[] = "snap%d.tga";                        // 0073EE18
const char k_tga_0073ee24[] = ".tga";                               // 0073EE24 (dword + NUL byte copied)
}

namespace kotor_renderer_init {
void __stdcall glGetIntegerv_0073d244(unsigned, int*);
void __stdcall glReadPixels_0073d250(int, int, int, int, unsigned, unsigned, void*);
}

extern "C" {
void* __cdecl live_operator_new(unsigned);                                                  // 006FA7E6
void __cdecl live_free(void*);                                                              // 006FA390
int __cdecl format_006fadb0(char*, const char*, ...);                                       // 006FADB0 sprintf
int __cdecl kotor_stricmp_0070acaf(const char*, const char*);                              // 0070ACAF _stricmp
void __cdecl kotor_cdecl_00420f20(const char* name, I size, U square, U pad);              // AurSaveGameSnapshot
void __cdecl kotor_cdecl_0045d990(unsigned char* data, I width, I height, I components);   // ImageFlipY
void __cdecl kotor_cdecl_0045f2a0(const char* file, I width, I height, I components, const unsigned char* data); // ImageWriteTGA

U kotor_snapshot_counter_007a47b4 = 0;                                                      // [007A47B4] (.bss)

void __cdecl frame_AurSnapshot_004211a0(void* name_arg, unsigned include_gui);
}

// ===================================================================================== 004211A0
extern "C" void __cdecl frame_AurSnapshot_004211a0(void* name_arg, unsigned include_gui) {
    const char* const name = static_cast<const char*>(name_arg);                     // 004211A4 ebx
    char file[0x40];                                                                 // [esp+20h] (up to the saved regs)
    bool generated = name == nullptr;                                                // 004211A8..004211AD
    if (!generated) {                                                                // 004211AF..004211BF repe cmpsb, 5 bytes
        U k = 0;
        while (k < 5 && name[k] == k_null_0073ee04[k]) ++k;
        generated = k == 5;
    }
    if (generated) {
        const U counter = kotor_snapshot_counter_007a47b4 + 1;                       // 0042122A..0042122F
        kotor_snapshot_counter_007a47b4 = counter;                                   // 0042123B
        (void)format_006fadb0(file, k_snap_0073ee18, static_cast<int>(counter));     // 00421230..00421245
    } else {
        for (U k = 0;; ++k) { file[k] = name[k]; if (name[k] == 0) break; }           // 004211C1..004211D8 (byte copy)
        I len = 0;
        while (file[len] != 0) ++len;                                                // 004211DA..004211E8
        bool append = true;
        if (len > 3)                                                                 // 004211EA..004211ED (signed)
            append = kotor_stricmp_0070acaf(file + len - 4, k_tga_0073ee24) != 0;    // 004211EF..00421203
        if (append) {                                                                // 00421205..00421225
            char* end = file;
            while (*end != 0) ++end;
            for (U k = 0; k < 5; ++k) end[k] = k_tga_0073ee24[k];                    // dword ".tga" + byte NUL
        }
    }
    int viewport[4];                                                                 // [esp+10h]
    kotor_renderer_init::glGetIntegerv_0073d244(k_gl_viewport, viewport);            // 00421248..00421252
    I width = viewport[2];                                                           // 00421258 ebp
    const I height = viewport[3];                                                    // 0042125C esi (also [esp+64h])
    const I rem = width % 4;                                                         // 00421260..00421271 (signed)
    if (rem != 0) width = width - rem + 4;                                           // 00421272..00421276
    if (static_cast<unsigned char>(include_gui) != 0) {                              // 00421279..0042127F
        const U bytes = static_cast<U>(height * width * 3);                          // 00421281..00421284
        unsigned char* const pixels = static_cast<unsigned char*>(live_operator_new(bytes)); // 00421287..0042128D
        for (U k = 0; k < bytes; ++k) pixels[k] = 0;                                 // 0042128F..004212AF rep stosd/stosb
        kotor_renderer_init::glReadPixels_0073d250(0, 0, width, height, k_gl_rgb_0073f370,
                                                   k_gl_unsigned_byte, pixels);      // 004212A4..004212BD
        kotor_cdecl_0045d990(pixels, width, height, 3);                              // 004212C3..004212C8
        kotor_cdecl_0045f2a0(file, width, height, 3, pixels);                        // 004212CD..004212D7
        live_free(pixels);                                                           // 004212DC..004212DD
        return;                                                                      // 004212E2..004212EC
    }
    kotor_cdecl_00420f20(name, width, 0, 0);                                         // 004212ED..004212F3
}                                                                                    // 004212F8..00421302
