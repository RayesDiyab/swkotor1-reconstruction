// lane-m46-startup: 0041DB50 CTlkTable::SetCustomToken(id, CExoString const&), thiscall RET 8 (consumer
// gui-load mainmenu.h:51). Evidence: dis/0041db50.asm, 0041D910 (token ctor), 0059EA20 (token dtor).
// Algorithm cross-checked with the task-13 tlk-custom candidate (custom.cpp), but its function-pointer casts are
// not reused: the element constructor/destructor are typed functions of the M45 VectorConstruct_006fb55f slot types.
#include "70118177bd-mainmenu.h"
#include "4b90ecc798-startup_string.h"
#include "d08b97bcca-allocator_live.h"
#include <cstdint>
#include <cstring>
#include <new>

namespace menu_tables_m41 {   // identical declaration of menu-tables d1_bodies.h:12-26 (eh vector constructor iterator 006FB55F)
using Constructor=void*(__fastcall*)(void*,void*);
using Destructor=void(__fastcall*)(void*,void*);
void __stdcall VectorConstruct_006fb55f(void* destination,std::uint32_t size,std::uint32_t count,Constructor ctor,Destructor dtor);
}
namespace kotor_reconstruction {
// canonical-assignment-format owner: CExoString::operator=(CExoString const&) 005E5C50 (RET 4).
StartupString* __fastcall StartupAssign005e5c50(StartupString*, void*, StartupString*);
}

namespace {
using Word = std::uint32_t;
using String = kotor_reconstruction::StartupString;
Word word_at(const void* o, Word off) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(o) + off, 4); return v; }
void put_word(void* o, Word off, Word v) { std::memcpy(static_cast<unsigned char*>(o) + off, &v, 4); }
// CTlkTableToken (0Ch): +0 id, +4 CExoString text.
String* text_of(unsigned char* token) { return std::launder(reinterpret_cast<String*>(token + 4)); }
}

extern "C" {
// 0041D910 CTlkTableToken::CTlkTableToken: CExoString() at +4 (005B3190); EAX = this.
void* __fastcall tlk_token_construct_0041d910(void* token, void*) {
    ::new (static_cast<unsigned char*>(token) + 4) String();
    return token;
}
// 0059EA20 CTlkTableToken::~CTlkTableToken: add ecx,4; jmp 005E5C20.
void __fastcall tlk_token_destroy_0059ea20(void* token, void*) { text_of(static_cast<unsigned char*>(token))->~String(); }
}

// 0041DB50..0041DCE2.
//  id < 0 -> return. Binary search tokens [+48] (count [+4C], unsigned compare on +0): equal -> assign text, return.
//  Else: n = [+4C]+1; block = new(n*0Ch+4) (006FA7E6); [block]=n; next=block+4 constructed by 006FB55F(next,0Ch,n,
//  0041D910,0059EA20) (block null -> next=0, original unchecked); copy tokens [0,low) (id + 005E5C50 assign);
//  next[low] = {id, input}; copy [low,[+4C]) to i+1 ([+4C] reloaded); old array: 006FA852(old,0Ch,[old-4],0059EA20)
//  then free(old-4); [+48]=next; [+4C]=[+4C]+1.
void __fastcall kotor_fastcall_0041db50(void* tlk, void*, gui_load::Word token_id, gui_load::String* value) noexcept(false) {
    String* const input = reinterpret_cast<String*>(value);   // same 8-byte CExoString object (gui_load view)
    const std::int32_t id = static_cast<std::int32_t>(token_id);
    if (id < 0) return;                                                                          // 0041DB6E..0041DB72
    Word high = word_at(tlk, 0x4c), low = 0;                                                     // 0041DB78..0041DB81
    unsigned char* const old = reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(word_at(tlk, 0x48)));   // 0041DB87
    while (high > low) {                                                                         // 0041DB85 / 0041DBB0..0041DBB2
        const Word middle = (high + low) >> 1;                                                   // 0041DB90..0041DB93
        const Word key = word_at(old, middle * 12);                                              // 0041DB95..0041DB98
        if (key > static_cast<Word>(id)) high = middle;                                          // 0041DB9B..0041DB9F
        else if (key < static_cast<Word>(id)) low = middle + 1;                                  // 0041DBA3..0041DBAE
        else { kotor_reconstruction::StartupAssign005e5c50(text_of(old + middle * 12), nullptr, input); return; }   // 0041DC65..0041DC71
    }
    const Word count = word_at(tlk, 0x4c) + 1;                                                   // 0041DBB4..0041DBB7
    void* const block = live_operator_new(count * 12 + 4);                                       // 0041DBB8..0041DBC3
    unsigned char* next = nullptr;
    if (block) {                                                                                 // 0041DBD1..0041DBD7
        put_word(block, 0, count);                                                               // 0041DBEA
        next = static_cast<unsigned char*>(block) + 4;                                           // 0041DBE4
        menu_tables_m41::VectorConstruct_006fb55f(next, 12, count, &tlk_token_construct_0041d910, &tlk_token_destroy_0059ea20);   // 0041DBD9..0041DBEC
    }
    for (Word i = 0; i < low; ++i) {                                                             // 0041DBF1..0041DC34
        unsigned char* const from = reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(word_at(tlk, 0x48))) + i * 12;   // 0041DC10..0041DC18 ([+48] reloaded)
        put_word(next + i * 12, 0, word_at(from, 0));                                            // 0041DC1B
        kotor_reconstruction::StartupAssign005e5c50(text_of(next + i * 12), nullptr, text_of(from));   // 0041DC1E..0041DC23
    }
    put_word(next + low * 12, 0, static_cast<Word>(id));                                         // 0041DC36..0041DC49
    kotor_reconstruction::StartupAssign005e5c50(text_of(next + low * 12), nullptr, input);       // 0041DC45..0041DC4B
    for (Word i = low; i < word_at(tlk, 0x4c); ++i) {                                            // 0041DC50..0041DCA0 ([+4C] reloaded)
        unsigned char* const from = reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(word_at(tlk, 0x48))) + i * 12;   // 0041DC80..0041DC89
        put_word(next + (i + 1) * 12, 0, word_at(from, 0));                                      // 0041DC87..0041DC90
        kotor_reconstruction::StartupAssign005e5c50(text_of(next + (i + 1) * 12), nullptr, text_of(from));   // 0041DC8C..0041DC92
    }
    if (unsigned char* const a = reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(word_at(tlk, 0x48)))) {   // 0041DCA2..0041DCA7
        const Word n = word_at(a - 4, 0);
        for (Word k = n; k != 0; --k) tlk_token_destroy_0059ea20(a + (k - 1) * 12, nullptr);    // 0041DCAF..0041DCB8 (006FA852, last-to-first)
        live_free(a - 4);                                                                        // 0041DCBD..0041DCBE
    }
    put_word(tlk, 0x48, static_cast<Word>(reinterpret_cast<std::uintptr_t>(next)));              // 0041DCCA
    put_word(tlk, 0x4c, word_at(tlk, 0x4c) + 1);                                                 // 0041DCC6..0041DCCD
}
