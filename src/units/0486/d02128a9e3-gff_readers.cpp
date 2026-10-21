// Lane gui-load: faithful bodies for CResGFF field readers reached by
// CSWGuiMainMenu::LoadFromLayout 0067ACE0 -> CSWGuiPanel/CSWGuiControl loads.
// Every reader: (1) null struct/label -> failure, (2) GetFieldByLabel 00411630
// with ECX=this, (3) GetField(CResStruct*, index) 00410990, (4) exact type
// compare, (5) success out-param written BEFORE the value is produced.
#include "a7eeb7ef01-gff_readers.h"
#include <cstring>

namespace {
using namespace gui_load;
// Common prefix 00411A74..00411A86 (BYTE) / 00411C34.. (DWORD) / 00411CA4.. (INT) etc.
Field* lookup(Gff* self, Struct* s, const char* label) {
    const Word index = gff_label_00411630(self, nullptr, s, label);
    return self->GetField(s, index);
}
// Data-field producer 00410A20 (q05 donor). Returns address bits, writes size.
const unsigned char* data_of(Gff* self, Field* field, Word* size) {
    return reinterpret_cast<const unsigned char*>(self->GetDataField(field, size));
}
}

extern "C" {

// 00405EF0: if source null -> zero 4 dwords (00405F4E..00405F5D).
// else n=min(low byte of length,16) (00405EFC..00405F07, byte compare only);
// zero the (16-n) tail at self+n FIRST (00405F0C..00405F2C), then copy n bytes
// from source to self (00405F2E..00405F43). Returns self (00405F48).
gui_load::ResRef* __fastcall gui_load_resref_bytes_00405ef0(
    gui_load::ResRef* self, void*, const char* source, gui_load::Word length_word) {
    if (!source) {
        std::memset(self->bytes, 0, 16);
        return self;
    }
    Word n = length_word & 0xffu;
    if (n >= 16u) n = 16u;
    const Word pad = 16u - n;
    std::memset(self->bytes + n, 0, pad);
    // REP MOVSD (n>>2 dwords) then REP MOVSB (n&3 bytes), forward order.
    volatile Word* dword_to = reinterpret_cast<volatile Word*>(self->bytes);
    const volatile Word* dword_from = reinterpret_cast<const volatile Word*>(source);
    for (Word i = 0; i < (n >> 2); ++i) dword_to[i] = dword_from[i];
    volatile unsigned char* byte_to = self->bytes + (n & ~3u);
    const volatile unsigned char* byte_from =
        reinterpret_cast<const unsigned char*>(source) + (n & ~3u);
    for (Word i = 0; i < (n & 3u); ++i) byte_to[i] = byte_from[i];
    return self;
}

// 00405FB0: copy dwords +0,+4,+8,+C (00405FB5..CA) then byte NUL at out+10h (00405FCD).
void __fastcall gui_load_resref_copy_to_string_00405fb0(const gui_load::ResRef* self, void*, char* out) {
    const volatile Word* from = reinterpret_cast<const volatile Word*>(self->bytes);
    volatile Word* to = reinterpret_cast<volatile Word*>(out);
    for (int i = 0; i < 4; ++i) to[i] = from[i];
    out[16] = 0;
}

// 00411A10: out/in/label null -> 0 (00411A1B/23/2B). Field type 0x0E (00411A41)
// -> *out = field->data_or_offset (00411A46..4A), return 1; else 0.
gui_load::Word __fastcall gui_load_gff_struct_00411a10(
    gui_load::Gff* self, void*, gui_load::Struct* out, gui_load::Struct* in, const char* label) {
    if (!out || !in || !label) return 0;
    Field* field = lookup(self, in, label);
    if (!field || field->type != kStruct) return 0;
    out->index = field->data_or_offset;
    return 1;
}

// 00411A60: type 0 (00411A88). success=1 then AL=byte [field+8] (00411A92/98).
// Mismatch or null field -> success=0, return default (00411A9F..A8).
// Null struct/label -> success=0, return default (00411AB2..BD).
unsigned char __fastcall gui_load_gff_byte_00411a60(
    gui_load::Gff* self, void*, gui_load::Struct* s, const char* label,
    gui_load::Word* success, unsigned char fallback) {
    if (s && label) {
        Field* field = lookup(self, s, label);
        if (field && field->type == kByte) {
            *success = 1;
            return *reinterpret_cast<volatile unsigned char*>(&field->data_or_offset);
        }
    }
    *success = 0;
    return fallback;
}

// 00411C20: type 4 (00411C48); success=1 then EAX=[field+8] (00411C52/58).
gui_load::Word __fastcall gui_load_gff_dword_00411c20(
    gui_load::Gff* self, void*, gui_load::Struct* s, const char* label,
    gui_load::Word* success, gui_load::Word fallback) {
    if (s && label) {
        Field* field = lookup(self, s, label);
        if (field && field->type == kDword) {
            *success = 1;
            return *reinterpret_cast<volatile Word*>(&field->data_or_offset);
        }
    }
    *success = 0;
    return fallback;
}

// 00411C90: type 5 (00411CB8); success=1 then EAX=[field+8] (00411CC2/C8).
std::int32_t __fastcall gui_load_gff_int_00411c90(
    gui_load::Gff* self, void*, gui_load::Struct* s, const char* label,
    gui_load::Word* success, std::int32_t fallback) {
    if (s && label) {
        Field* field = lookup(self, s, label);
        if (field && field->type == kInt) {
            *success = 1;
            const Word bits = *reinterpret_cast<volatile Word*>(&field->data_or_offset);
            std::int32_t value;
            std::memcpy(&value, &bits, 4);
            return value;
        }
    }
    *success = 0;
    return fallback;
}

// 00411D00: type 8 (00411D28); success=1 (00411D31) then FLD of the raw field
// dword (00411D37..3E). Failure paths FLD the default (00411D4B/5E).
float __fastcall gui_load_gff_float_00411d00(
    gui_load::Gff* self, void*, gui_load::Struct* s, const char* label,
    gui_load::Word* success, float fallback) {
    if (s && label) {
        Field* field = lookup(self, s, label);
        if (field && field->type == kFloat) {
            *success = 1;
            const Word bits = *reinterpret_cast<volatile Word*>(&field->data_or_offset);
            float value;
            std::memcpy(&value, &bits, 4);
            return value;
        }
    }
    *success = 0;
    return fallback;
}

// 00411E10: type 0x0B (00411E38); GetDataField (00411E45) nonnull and size>=1
// (00411E52); len=first byte; size>=len+1 (00411E5D..67, unsigned) ->
// success=1 (00411E76) then CResRef(data+1, len) (00411E7C) into hidden out.
// Any failure: success=0 (00411E94) then dword-wise copy of *default (00411E9A..B0).
gui_load::ResRef* __fastcall gui_load_gff_resref_00411e10(
    gui_load::Gff* self, void*, gui_load::ResRef* out, gui_load::Struct* s,
    const char* label, gui_load::Word* success, const gui_load::ResRef* fallback) {
    if (s && label) {
        Field* field = lookup(self, s, label);
        if (field && field->type == kResRef) {
            Word size = 0;
            const unsigned char* data = data_of(self, field, &size);
            if (data && size >= 1u) {
                const unsigned char length = data[0];
                if (size >= static_cast<Word>(length) + 1u) {
                    *success = 1;
                    gui_load_resref_bytes_00405ef0(out, nullptr,
                        reinterpret_cast<const char*>(data + 1), length);
                    return out;
                }
            }
        }
    }
    *success = 0;
    const volatile Word* from = reinterpret_cast<const volatile Word*>(fallback);
    volatile Word* to = reinterpret_cast<volatile Word*>(out);
    for (int i = 0; i < 4; ++i) to[i] = from[i];
    return out;
}

// 00411EC0: type 0x0A (00411EFD); GetDataField (00411F0E) nonnull and size>=4
// (00411F1B); len=[data]; size >= len+4 (32-bit wrap, 00411F22..2F) ->
// success=1 (00411F52) then CExoString(data+4, len) 005E5B70 (00411F58).
// size < len+4 -> success=0 (00411F38) then copy-construct default 005E5B00.
// Other failures likewise (00411F74/96/B5 then 005E5B00).
gui_load::String* __fastcall gui_load_gff_string_00411ec0(
    gui_load::Gff* self, void*, gui_load::String* out, gui_load::Struct* s,
    const char* label, gui_load::Word* success, const gui_load::String* fallback) {
    if (s && label) {
        Field* field = lookup(self, s, label);
        if (field && field->type == kString) {
            Word size = 0;
            const unsigned char* data = data_of(self, field, &size);
            if (data && size >= 4u) {
                const Word length = *reinterpret_cast<const volatile Word*>(data);
                if (size >= length + 4u) {
                    *success = 1;
                    return resman_string_counted_005e5b70(out, nullptr,
                        reinterpret_cast<const char*>(data + 4), static_cast<int>(length));
                }
            }
        }
    }
    *success = 0;
    WindowCopy005e5b00(out, nullptr, fallback);
    return out;
}

// 004122A0: type 0x11 (004122D0); GetDataField (004122DD) nonnull and size>=0x0C
// (004122E6) -> success=1 (004122F5) then copy 3 dwords (004122FB..30B).
// Failures: success=0 then copy 3 dwords of *default (00412314.. / 00412349..).
gui_load::Vector* __fastcall gui_load_gff_vector_004122a0(
    gui_load::Gff* self, void*, gui_load::Vector* out, gui_load::Struct* s,
    const char* label, gui_load::Word* success, const gui_load::Vector* fallback) {
    const volatile Word* from = reinterpret_cast<const volatile Word*>(fallback);
    if (s && label) {
        Field* field = lookup(self, s, label);
        if (field && field->type == kVector) {
            Word size = 0;
            const unsigned char* data = data_of(self, field, &size);
            if (data && size >= 12u) {
                *success = 1;
                from = reinterpret_cast<const volatile Word*>(data);
                volatile Word* to = reinterpret_cast<volatile Word*>(out);
                for (int i = 0; i < 3; ++i) to[i] = from[i];
                return out;
            }
        }
    }
    *success = 0;
    volatile Word* to = reinterpret_cast<volatile Word*>(out);
    for (int i = 0; i < 3; ++i) to[i] = from[i];
    return out;
}

} // extern "C"
