// lane-m1-movies-play: CClientExoApp::PlayMovie / CClientExoAppInternal::PlayMovie (swkotor.exe GOG K1, base 0x400000;
// static disassembly only, nothing executed). Consumer: CSWGuiTitleMovies::OnMoviePicked 006DCF10 (006DCF5E).
//   005EDB30 CClientExoApp::PlayMovie(CExoString*, int*, int, int)          mov ecx,[ecx+4]; jmp 00602CF0
//   00602CF0 CClientExoAppInternal::PlayMovie(CExoString*, int*, int, int)  RET 10h
// The third and fourth stack arguments are not read by 00602CF0 (only [esp+8] at 00602D03 and [esp+10h] after the
// second push at 00602D20 are loaded); callers push 0 and 1 (006DCF49..4B).
// D1 form: original __thiscall entries are __fastcall(self, unused EDX, ...) with identical callee cleanup.
// Bound owners (declarations from their own headers): lane-config movie_list.h AddMovieToExoArrayList 005FBBF0,
// intro_queue.h PlayMoviesInExoArrayList 00602AF0.
#include "e3324ea9c3-intro_queue.h"     // client_intro::PlayMoviesInExoArrayList_00602af0, options_video::Client/String
#include "099e9b170e-movie_list.h"      // client_movies::AddMovieToExoArrayList_005fbbf0
#include "e9343a6dea-startup_string.h"  // kotor_reconstruction::StartupString (consumer-side CExoString spelling)
#include <cstdint>
#include <cstring>
#include <new>

namespace {
using options_video::Client;
using options_video::String;
std::uint32_t word_at(const void* p, std::size_t offset) {
    std::uint32_t v; std::memcpy(&v, static_cast<const unsigned char*>(p) + offset, 4); return v;
}
void* pointer_at(const void* p, std::size_t offset) {
    void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + offset, 4); return v;
}
static_assert(sizeof(String) == sizeof(kotor_reconstruction::StartupString), "8-byte CExoString views");
}

extern "C" {
std::int32_t __fastcall kotor_fastcall_00602cf0(Client* self, void* unused_edx, kotor_reconstruction::StartupString* name,
                                               std::int32_t* out, std::int32_t unused_3, std::int32_t unused_4) noexcept(false);
std::int32_t __fastcall kotor_fastcall_005edb30(void* client_app, void* unused_edx, kotor_reconstruction::StartupString* name,
                                               std::int32_t* out, std::int32_t unused_3, std::int32_t unused_4) noexcept(false);
}

// 00602CF0 CClientExoAppInternal::PlayMovie, RET 10h, EAX = 0 or PlayMoviesInExoArrayList's result.
std::int32_t __fastcall kotor_fastcall_00602cf0(Client* self, void* unused_edx, kotor_reconstruction::StartupString* name,
                                               std::int32_t* out, std::int32_t unused_3, std::int32_t unused_4) noexcept(false) {
    (void)unused_edx; (void)unused_3; (void)unused_4;
    if (word_at(self, 0x1F8) != 0) return 0;                                    // 00602CF3..00602D00 Disable Movies
    void* const list = static_cast<unsigned char*>(static_cast<void*>(self)) + 0x2D8;   // 00602D08 CExoArrayList<CExoString>
    String* const movie = std::launder(static_cast<String*>(static_cast<void*>(name)));
    if (client_movies::AddMovieToExoArrayList_005fbbf0(self, nullptr, movie, list) == 0) return 0;   // 00602D0E..00602D1D
    return client_intro::PlayMoviesInExoArrayList_00602af0(self, nullptr, list, out, 1, 0u, nullptr); // 00602D20..00602D35
                                                                                // push 0 (flags), 0 (forwarded), 1 (wait), out, list
}

// 005EDB30 CClientExoApp::PlayMovie: internal at [this+4], tail jump (same stack arguments, RET 10h in 00602CF0).
std::int32_t __fastcall kotor_fastcall_005edb30(void* client_app, void* unused_edx, kotor_reconstruction::StartupString* name,
                                               std::int32_t* out, std::int32_t unused_3, std::int32_t unused_4) noexcept(false) {
    (void)unused_edx;
    return kotor_fastcall_00602cf0(static_cast<Client*>(pointer_at(client_app, 4)), nullptr, name, out, unused_3, unused_4); // 005EDB30..33
}
