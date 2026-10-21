// lane-m48-a: Aurora file-based image readers 0045DF10..0045F55E — the ORIGINAL COLD VALUES of the seven .data
// reader cells 0078D404..0078D41C (owned by lane-texture-epoch texture_cells.cpp, which takes their addresses).
// Evidence: swkotor.exe GOG K1 dumpbin /disasm (lane-m48-a/d/0045DF10.txt, d/0045F390.txt, d/0045DA80.txt).
// Reachability: the genuine installer 0070D9C0 (called from 005F8AFE) overwrites every one of these cells
// (0070D9DB..0070DA2B: 0078D404=0070C790, 0078D408=0070CC60, 0078D41C=0070C6E0, 0078D40C=0070D380,
// 0078D410=0070CDD0, 0078D414=0070D6B0, 0078D418=0070CE80) before any texture load; the only readers of the cells
// are 0041EB29/0041EB64/0041EB78/0041EB87/0041F9EE/0041F9FB. These bodies therefore run only if a cell is read before
// the installer (not on the menu path); they are reproduced in full, not stubbed.
// CRT family (D69 / lane-aurres aurres.cpp precedent): fopen 006FAD9D / fread 006FA9D4 / fclose 006FACF0 -> linked UCRT
// (/MT), FILE* never leaves this unit; sprintf 006FADB0 -> kotor_functional::modern_ini_sprintf (M40 binding of
// 006FADB0); operator new 006FA7E6 -> kotor_menu_10_19::dependencies::allocate_006fa7e6 (joined onto the live SBH
// allocator, the original allocator family). .rdata strings: 0073D5B0 "rb", 0073D5D0 "%s", 00741BC0 ".4pc",
// 00741BC8 "%s.dds", 00741BD0 "%s.tga", 00741BD8 "%s.plt" (bytes read from the pinned exe).
// Stack buffers keep the original sizes (0x100 name buffers; 0x20-byte PLT name buffer inside a 0x38-byte frame);
// over-long names overflow in the original too (no length check) — here /GS terminates instead (recorded deviation).
#include "a14ee97e1a-texture_cells.hpp"
#include "4282d8e4cc-ini_c_primitives.h" // kotor_functional::modern_ini_sprintf (006FADB0)
#include <cstdio>
#include <cstdint>
#include <cstring>

namespace kotor_menu_10_19 { namespace dependencies { void* allocate_006fa7e6(unsigned int); } } // 006FA7E6 (joined)
extern "C" std::int32_t __cdecl image_get_s3tc_size_0045e270(std::int32_t, std::int32_t, std::int32_t); // startup-misc owner

namespace {
using Signed = acquisition::Signed;
using Word = acquisition::Word;
constexpr const char kReadBinary_0073d5b0[] = "rb";
constexpr const char kPlain_0073d5d0[] = "%s";
constexpr const char kDds_00741bc8[] = "%s.dds";
constexpr const char kTga_00741bd0[] = "%s.tga";
constexpr const char kPlt_00741bd8[] = "%s.plt";
// 32-bit imul (wrapping) product, as the original computes sizes.
inline Word mul(Signed a, Signed b) { return static_cast<Word>(a) * static_cast<Word>(b); }
inline void* allocate(unsigned int size) { return kotor_menu_10_19::dependencies::allocate_006fa7e6(size); }
inline std::size_t read(void* buffer, std::size_t size, std::size_t count, std::FILE* file) { return std::fread(buffer, size, count, file); }

// Inline strcpy of the name into the 0x100-byte frame buffer, fopen; on failure append ".4pc" (00741BC0 dword +
// byte 00741BC4) at the terminator and retry (0045DF10..0045DF7F == 0045E010..0045E080).
std::FILE* open_name_or_4pc(char (&path)[0x100], const char* name) {
    char* out = path;                                                   // 0045DF14..0045DF2B byte copy incl. NUL
    for (const char* in = name;; ++in, ++out) { *out = *in; if (*in == '\0') break; }
    std::FILE* file = std::fopen(path, kReadBinary_0073d5b0);          // 0045DF2F..0045DF3E
    if (file != nullptr) return file;
    char* end = path;                                                   // 0045DF47..0045DF56 find terminator
    while (*end != '\0') ++end;
    end[0] = '.'; end[1] = '4'; end[2] = 'p'; end[3] = 'c'; end[4] = '\0'; // 0045DF58..0045DF70
    return std::fopen(path, kReadBinary_0073d5b0);                      // 0045DF64..0045DF78
}
// "%s" then fallback format (0045E1C4..0045E225 / 0045E370..0045E3BE / 0045E477..0045E4D8 / 0045F4C0..0045F50E).
std::FILE* open_formatted(char (&path)[0x100], const char* name, const char* fallback) {
    kotor_functional::modern_ini_sprintf(path, kPlain_0073d5d0, name);
    std::FILE* file = std::fopen(path, kReadBinary_0073d5b0);
    if (file != nullptr) return file;
    kotor_functional::modern_ini_sprintf(path, fallback, name);
    return std::fopen(path, kReadBinary_0073d5b0);
}
} // namespace

extern "C" {
// 0045DA80 ImageTGASwapBGRtoRGB(pixels, width, height, components), cdecl, no result (EAX unspecified).
void __cdecl image_tga_swap_bgr_to_rgb_0045da80(char* pixels, Signed width, Signed height, Signed components) {
    if (components < 3) return;                                         // 0045DA81..0045DA88 jl
    const Signed total = static_cast<Signed>(mul(width, height) * static_cast<Word>(components)); // 0045DA8A..0045DA93 (imul, wraps)
    if (total <= 0) return;                                             // 0045DA96..0045DA98 jle
    for (Signed index = 0; index < total; index += components) {        // 0045DAA0..0045DAC1 (edx = eax-2-pixels)
        const char red = pixels[index];                                 // 0045DAB0 dl = [eax-2]
        pixels[index] = pixels[index + 2];                              // 0045DAB3/0045DAB7
        pixels[index + 2] = red;                                        // 0045DAB5
    }
}

// 0045DF10 ImageRead16bit(name, width*, height*, float*, word*) -> pixel buffer (2 bytes per pixel) or 0.
char* __cdecl image_read_16bit_0045df10(const char* name, Signed* width, Signed* height, float* scale, Signed* extra) {
    char path[0x100];                                                   // 0045DF1B sub esp,100h
    std::FILE* const file = open_name_or_4pc(path, name);
    if (file == nullptr) return nullptr;                                // 0045DF81..0045DF89 (EAX = esi = 0)
    read(width, 4, 1, file);                                            // 0045DF94..0045DF99
    read(height, 4, 1, file);                                           // 0045DFA5..0045DFAB
    read(scale, 4, 1, file);                                            // 0045DFB7..0045DFBD
    read(extra, 4, 1, file);                                            // 0045DFC9..0045DFCF
    char* const pixels = static_cast<char*>(allocate(mul(*width, *height) << 1));   // 0045DFD4..0045DFDF (imul; shl 1)
    read(pixels, 1, mul(*width, *height) << 1, file);           // 0045DFE4..0045DFF2 (re-read; no null test)
    std::fclose(file);                                                  // 0045DFF7..0045DFF8
    return pixels;                                                      // 0045E000 mov eax,ebp
}

// 0045E010 ImageRead16bitHeader(name, width*, height*, float*, word*) -> 1, or 0 when no file opens.
Word __cdecl image_read_16bit_header_0045e010(const char* name, Signed* width, Signed* height, float* scale, Signed* extra) {
    char path[0x100];
    std::FILE* const file = open_name_or_4pc(path, name);              // 0045E014..0045E07F
    if (file == nullptr) return 0;                                      // 0045E082
    read(width, 4, 1, file);                                            // 0045E08C..0045E099
    read(height, 4, 1, file);                                           // 0045E09E..0045E0AB
    read(scale, 4, 1, file);                                            // 0045E0B0..0045E0BD
    read(extra, 4, 1, file);                                            // 0045E0C2..0045E0CF
    std::fclose(file);                                                  // 0045E0D7..0045E0D8
    return 1;                                                           // 0045E0E0
}

// 0045E0F0 ImageReadS3TCHeader(FILE*, width*, height*, components*, size*, float*) -> 0 (null file) / 1.
Word __cdecl image_read_s3tc_header_file_0045e0f0(std::FILE* file, Signed* width, Signed* height, Signed* components,
                                                  Signed* size, float* value) {
    if (file == nullptr) return 0;                                      // 0045E0F5..0045E0FC
    *size = 0;                                                          // 0045E115 [edi]
    *width = 0;                                                         // 0045E11B [eax]
    *height = 0;                                                        // 0045E123 [ebx]
    *components = 0;                                                    // 0045E12B [ebp]
    *value = -1.0f;                                                     // 0045E133 mov [ecx],0BF800000h
    read(width, 4, 1, file);                                            // 0045E121..0045E139
    read(height, 4, 1, file);                                           // 0045E13E..0045E144
    read(components, 4, 1, file);                                       // 0045E149..0045E14F
    read(size, 4, 1, file);                                             // 0045E154..0045E15A
    read(value, 4, 1, file);                                            // 0045E15F..0045E16C
    return 1;                                                           // 0045E177
}

// 0045E180 ImageReadS3TCHeader(name, width*, height*, components*, size*, float*) -> 1 / 0.
Word __cdecl image_read_s3tc_header_0045e180(const char* name, Signed* width, Signed* height, Signed* components,
                                             Signed* size, float* value) {
    char path[0x100];
    if (name == nullptr || *name == '\0') return 0;                     // 0045E191..0045E19C -> 0045E260
    *size = 0;                                                          // 0045E1BE [ebx]
    *width = 0;                                                         // 0045E1C5 [ebp]
    *height = 0;                                                        // 0045E1D5 [eax]
    *components = 0;                                                    // 0045E1DC [ecx]
    std::FILE* const file = open_formatted(path, name, kDds_00741bc8);  // 0045E1C4..0045E225
    if (file == nullptr) return 0;                                      // 0045E225 je 0045E260
    image_read_s3tc_header_file_0045e0f0(file, width, height, components, size, value); // 0045E227..0045E242 (EAX dropped)
    std::fclose(file);                                                  // 0045E247..0045E248
    return 1;                                                           // 0045E253
}

// 0045E2E0 ImageReadTGAHeader(FILE*, width*, height*, bytes_per_pixel*): 18-byte TGA header into a 20-byte frame.
// EAX on return = bits>>3 (the last stored value); callers 0045E3DB/0045E4E5 discard it.
Word __cdecl image_read_tga_header_0045e2e0(std::FILE* file, Word* width, Word* height, Word* bytes) {
    unsigned char header[0x14];                                         // 0045E2E4 sub esp,14h
    read(header, 0x12, 1, file);                                        // 0045E2E7..0045E2F1
    *width = static_cast<Word>(header[0x0C] | (header[0x0D] << 8));     // 0045E2F6 movzx word [buf+0C]; 0045E304
    *height = static_cast<Word>(header[0x0E] | (header[0x0F] << 8));    // 0045E2FF movzx word [buf+0E]; 0045E30F
    const Word bits = header[0x10];                                     // 0045E306 movzx byte [buf+10]
    *bytes = bits >> 3;                                                 // 0045E315..0045E318
    return bits >> 3;
}

// 0045E320 ImageReadTGAImage(FILE*, width, height, bytes): new[w*h*b] then fread (no null test).
char* __cdecl image_read_tga_image_0045e320(std::FILE* file, Word width, Word height, Word bytes) {
    const Word size = width * height * bytes;                           // 0045E321..0045E32A
    char* const pixels = static_cast<char*>(allocate(size));           // 0045E330..0045E331
    read(pixels, 1, size, file);                                        // 0045E338..0045E341
    return pixels;                                                      // 0045E349
}

// 0045E350 ImageReadTGA(name, width*, height*, bytes*) -> RGB(A) pixels or 0.
char* __cdecl image_read_tga_body_0045e350(const char* name, Word* width, Word* height, Word* bytes) {
    char path[0x100];                                                   // 0045E350 sub esp,104h (+4: pixel local)
    if (name == nullptr || *name == '\0') return nullptr;               // 0045E35F..0045E36A -> 0045E41E
    std::FILE* const file = open_formatted(path, name, kTga_00741bd0);  // 0045E370..0045E3BE
    if (file == nullptr) return nullptr;
    image_read_tga_header_0045e2e0(file, width, height, bytes);         // 0045E3C0..0045E3DB
    char* const pixels = image_read_tga_image_0045e320(file, *width, *height, *bytes); // 0045E3E0..0045E3EB
    std::fclose(file);                                                  // 0045E3F0..0045E3F5
    image_tga_swap_bgr_to_rgb_0045da80(pixels, static_cast<Signed>(*width), static_cast<Signed>(*height),
                                       static_cast<Signed>(*bytes));    // 0045E3FA..0045E409 (values re-read)
    return pixels;                                                      // 0045E414
}

// 0045E430: jmp 0045E350 (the cell 0078D404 cold value is this thunk).
char* __cdecl image_read_tga_0045e430(const char* name, Word* width, Word* height, Word* bytes) {
    return image_read_tga_body_0045e350(name, width, height, bytes);    // 0045E430 jmp 0045E350
}

// 0045E440 ImageReadHeader(name, width*, height*, bytes*) -> 1 / 0 (TGA header only).
Word __cdecl image_read_header_0045e440(const char* name, Signed* width, Signed* height, Signed* bytes) {
    char path[0x100];
    if (name == nullptr || *name == '\0') return 0;                     // 0045E451..0045E45C -> 0045E503
    *width = 0;                                                         // 0045E478 [ebx]
    *height = 0;                                                        // 0045E487 [ebp]
    *bytes = 0;                                                         // 0045E48F [eax]
    std::FILE* const file = open_formatted(path, name, kTga_00741bd0);  // 0045E477..0045E4D8
    if (file == nullptr) return 0;
    image_read_tga_header_0045e2e0(file, reinterpret_cast<Word*>(width), reinterpret_cast<Word*>(height),
                                   reinterpret_cast<Word*>(bytes));     // 0045E4DA..0045E4E5 (same 4-byte cells)
    std::fclose(file);                                                  // 0045E4EA..0045E4EB
    return 1;                                                           // 0045E4F6
}

// 0045E510 ImageReadPLT(name, width*, height*, unused*) -> 2-byte-per-pixel layer buffer or 0. The third out
// parameter is never written; on allocation failure the stream is NOT closed (0045E57D..0045E582), as original.
char* __cdecl image_read_plt_0045e510(const char* name, Signed* width, Signed* height, Signed* /*never written*/) {
    struct Frame { unsigned char header[0x18]; char path[0x20]; } frame; // 0045E514 sub esp,38h ([esp+4]..[esp+3B])
    kotor_functional::modern_ini_sprintf(frame.path, kPlt_00741bd8, name); // 0045E518..0045E523
    std::FILE* const file = std::fopen(frame.path, kReadBinary_0073d5b0); // 0045E528..0045E532
    if (file == nullptr) return nullptr;                                // 0045E53C..0045E544 (EAX = 0)
    read(frame.header, 0x18, 1, file);                                  // 0045E546..0045E550
    Signed w, h;
    std::memcpy(&w, frame.header + 0x10, 4);                            // 0045E555 ecx = [hdr+10h]
    std::memcpy(&h, frame.header + 0x14, 4);                            // 0045E55D eax = [hdr+14h]
    *width = w;                                                         // 0045E561
    *height = h;                                                        // 0045E567
    char* const pixels = static_cast<char*>(allocate(mul(h, w) << 1)); // 0045E569..0045E56F
    if (pixels == nullptr) return nullptr;                              // 0045E579..0045E582 (file left open)
    read(pixels, 1, mul(h, w) << 1, file);                            // 0045E583..0045E593 (frame words re-read)
    std::fclose(file);                                                  // 0045E598..0045E599
    return pixels;                                                      // 0045E5A1
}

// 0045F390 ImageReadS3TC(FILE*, width*, height*, components*, size*, float*) -> DXT mip chain buffer or 0.
char* __cdecl image_read_s3tc_file_0045f390(std::FILE* file, Signed* width, Signed* height, Signed* components,
                                            Signed* size, float* value) {
    if (file == nullptr) return nullptr;                                // 0045F398..0045F3A2
    read(width, 4, 1, file);                                            // 0045F3AA..0045F3B0
    read(height, 4, 1, file);                                           // 0045F3B5..0045F3BF
    read(components, 4, 1, file);                                       // 0045F3C4..0045F3CE
    read(size, 4, 1, file);                                             // 0045F3D3..0045F3DD
    read(value, 4, 1, file);                                            // 0045F3E2..0045F3EF
    const Signed block = (*components == 4) ? 16 : 8;                   // 0045F3FC..0045F410 (ecx*8+8)
    char* const chain = static_cast<char*>(allocate(static_cast<unsigned int>(
        image_get_s3tc_size_0045e270(*width, *height, *components))));  // 0045F404..0045F41A
    read(chain, 1, static_cast<std::size_t>(static_cast<unsigned int>(*size)), file); // 0045F41F..0045F42B (no null test)
    char* level = chain;                                                // ecx = buffer
    Signed level_bytes = *size;                                         // 0045F434 ebp = [size]
    Signed w = *width, h = *height;                                     // 0045F437 ebx / 0045F439 edi
    for (;;) {
        w >>= 1; h >>= 1;                                               // 0045F43E/0045F440 sar
        if (w == 0) { if (h == 0) break; w = 1; }                       // 0045F442..0045F44A
        if (h == 0) h = 1;                                              // 0045F44F..0045F453
        level += level_bytes;                                           // 0045F45C add ecx,ebp
        level_bytes = static_cast<Signed>(mul((h + 3) / 4, (w + 3) / 4) * static_cast<Word>(block)); // 0045F458..0045F477 (signed /4 via cdq/and 3/sar 2; imul)
        read(level, 1, static_cast<std::size_t>(static_cast<unsigned int>(level_bytes)), file); // 0045F47C..0045F485
    }
    return chain;                                                       // 0045F493 eax = saved buffer
}

// 0045F4A0 ImageReadS3TC(name, width*, height*, components*, size*, float*) -> chain or 0.
char* __cdecl image_read_s3tc_0045f4a0(const char* name, Signed* width, Signed* height, Signed* components,
                                       Signed* size, float* value) {
    char path[0x100];
    if (name == nullptr || *name == '\0') return nullptr;               // 0045F4AF..0045F4BA -> 0045F554
    std::FILE* const file = open_formatted(path, name, kDds_00741bc8);  // 0045F4C0..0045F50E
    if (file == nullptr) return nullptr;
    char* const chain = image_read_s3tc_file_0045f390(file, width, height, components, size, value); // 0045F510..0045F539
    std::fclose(file);                                                  // 0045F53E..0045F541
    return chain;                                                       // 0045F549
}
} // extern "C"
