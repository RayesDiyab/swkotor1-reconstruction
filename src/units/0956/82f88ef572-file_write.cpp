// lane-m48-traps / f-keys: CExoFileInternal::Write 005EBC60 (424 bytes), __thiscall RET 0Ch.
// Consumer: r4-lowers r4_log_joins.cpp (frame_LogWriteText_005e6a40 / frame_LogWriteString_005e6a70)
//   std::uint32_t __fastcall kotor_fastcall_005ebc60(void* file_internal, void*, const void* data, U32 size, U32 count)
// Object: file_internal::Internal (stream +0, name +4, mode +0Ch) = the observed CExoFileInternal 20-byte layout.
// D84 accepted CRT stdio equivalence: the original statically linked CRT stdio calls are served by the stage's
// single modern FILE boundary (file_internal::file_ops = UCRT fopen_s/fclose/fseek/ftell/ferror, the same family that
// opened Internal::stream in internal_construct_005eb840) plus UCRT std::fwrite for _fwrite 006FAB27 (FileOps has no
// write slot). Per-call records:
//   006FBA49 _ftell(stream)                -> file_ops->tell(stream)          (long position; same value domain)
//   006FAB27 _fwrite(data,size,count,fp)   -> std::fwrite(data,size,count,fp) (returns complete items)
//   test byte [FILE+0Ch],20h (_IOERR)      -> file_ops->error(stream) != 0    (UCRT ferror reports the error indicator)
//   006FACF0 fclose(stream)                -> file_ops->close(stream)
//   006FAD9D fopen(name,mode)              -> file_ops->open(name,mode)        (fopen_s; null on failure like fopen)
//   006FBB19 _fseek(stream,pos,SEEK_SET)   -> file_ops->seek(stream,pos,0)
//   IAT 0073D1E4 Sleep(500)                -> file_ops->delay(500)             (stage boundary = kernel32 Sleep)
// Retry log: CExoString 008328D8 and CExoString::Format 005E5680 / CStr 005E5670 through the published
// file_internal::engine bindings (same owner the CExoFileInternal::Read 005EBA40 provider uses).
#include "e507127f0f-internal.h"
#include <cstdio>

namespace {
// Original .rdata literals (format strings of 005EBC60).
const char write_retry_format_0074e538[] =
    "%sFAILED WRITE (%s) - ATTEMPT %d - Tried to write %d bytes at position %d, actually wrote %d bytes\n";
const char write_null_format_0074e4d8[] = "%sFile handle was null\n";
const char write_reopen_format_0074e4b8[] = "%sFile handle failed to reopen\n";
const char write_giveup_format_0074e494[] = "%sFile write failed.  Giving up\n";
const char write_recovered_format_0074e478[] = "%sFile write recovered.\n";
}

namespace file_internal {
extern "C" Word __fastcall kotor_fastcall_005ebc60(Internal* self, void*, const void* data, Word size, Word count) {
    if (!self->stream) return 0;                                              // 005EBC66..70 -> 005EBDFF (EAX 0)
    Word retries = 0;                                                         // 005EBC6C [esp+4] local0
    Word retried = 0;                                                         // 005EBC7A local2
    const long position = file_ops->tell(self->stream);                       // 005EBC79..7E _ftell, 005EBC8F local1
    Word bytes = static_cast<Word>(std::fwrite(data, size, count, self->stream)) * size;  // 005EBC93..A0 _fwrite, IMUL esi,ebx
    const Word requested = size * count;                                      // 005EBCA3 IMUL ebx,ebp
    if (bytes != requested) {                                                 // 005EBCA9..AB
        auto* log = engine->retry_log_008328d8;                               // 008328D8
        // Loop head 005EBCB1: the error indicator of the CURRENT [edi] stream is read every pass (original quirk:
        // after a failed reopen the stream word is null; the original then faults reading [0+0Ch]; UCRT ferror(nullptr)
        // raises the invalid-parameter handler -- both are process-fatal on that already-failing path).
        while (file_ops->error(self->stream) && retries < 10) {              // 005EBCB1..C2 (_IOERR, signed JGE 0Ah)
            retried = 1;                                                      // 005EBCD9
            engine->format_005e5680(log, write_retry_format_0074e538,
                                    engine->text_005e5670(&self->name, nullptr),   // 005EBCEA..EC (pushed last -> 1st %s)
                                    engine->text_005e5670(log, nullptr),           // 005EBCD4..E4
                                    retries, requested, position, bytes);          // 005EBCD0..D3 pushes esi,eax,ebx,ecx
            file_ops->delay(500);                                             // 005EBD04..09 Sleep(1F4h)
            if (self->stream) file_ops->close(self->stream);                  // 005EBD0F..16 fclose (word left stale)
            else engine->format_005e5680(log, write_null_format_0074e4d8, engine->text_005e5670(log, nullptr));  // 005EBD20..35
            self->stream = file_ops->open(engine->text_005e5670(&self->name, nullptr),
                                          engine->text_005e5670(&self->mode, nullptr));  // 005EBD3D..58 fopen(name, mode)
            if (self->stream) {                                               // 005EBD56..5A
                file_ops->seek(self->stream, position, 0);                    // 005EBD5C..64 _fseek(SEEK_SET)
                bytes = static_cast<Word>(std::fwrite(data, size, count, self->stream)) * size;  // 005EBD69..85
            } else {
                engine->format_005e5680(log, write_reopen_format_0074e4b8, engine->text_005e5670(log, nullptr));  // 005EBD8A..9F
            }
            ++retries;                                                        // 005EBDA7..AE
            if (bytes == requested) break;                                    // 005EBDAC..B2
        }
        if (bytes != requested) {                                             // 005EBDB8..BA
            engine->format_005e5680(log, write_giveup_format_0074e494, engine->text_005e5670(log, nullptr));  // 005EBDBC..EB
            return bytes;                                                     // 005EBDF3
        }
    }
    if (retried)                                                              // 005EBDCE..D4
        engine->format_005e5680(engine->retry_log_008328d8, write_recovered_format_0074e478,
                                engine->text_005e5670(engine->retry_log_008328d8, nullptr));  // 005EBDD6..EB
    return bytes;                                                             // 005EBDF3 (EAX = esi)
}
}
