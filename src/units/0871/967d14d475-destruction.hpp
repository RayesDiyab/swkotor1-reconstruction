#pragma once
#include "76c2ae51fd-texture.hpp" // E1 epoch
namespace texture_destruction {
using U=texture::U;using I=texture::I;using Basic=texture::Basic;using Ref=texture::Ref;using List=texture::List;using FontInfo=texture::FontInfo;
// A bounded vtable-prefix carrier; the complete original controller type is held.
struct Member {void** volatile vtable;};
struct Bindings {
 void** basic_table;
 U (__fastcall* font_info_destroy)(FontInfo*,void*); // E1 edit D1: 00420610 raw EAX (lane-font declaration)
 void (__fastcall* memory_usage)(Basic*,void*,U*);
 void (__stdcall* gl_delete_textures)(I,const U*);
 U (__fastcall* free_image)(Basic*,void*);
 U (__fastcall* remove_ordered)(Basic*,void*);
 void (__fastcall* remove_cache)(List*,void*,U);
 U (__fastcall* base_destroy)(Ref*,void*);
 U volatile* accounting[5];
 U volatile* object_count;
 List* cache;
};
extern Bindings bindings;
extern "C" U __fastcall destroy_00422f40(Basic*,void*);
}
