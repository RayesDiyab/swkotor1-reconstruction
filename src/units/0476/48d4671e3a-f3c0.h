#pragma once
#include "cd5676f4ba-f990.h"
namespace f3c0_ports {
using W=menu_resources::Word;using S=menu_resources::String;
// Opaque measured4-byte receiver, not Task34's24-byte TLK File class.
void* __fastcall Construct005e68d0(void*,void*,S*,W,S*);
W __fastcall Opened005e6a10(void*,void*);
void __fastcall Destroy005e69e0(void*,void*);
W __fastcall Read005e6960(void*,void*,void*,W,W);
W __fastcall Seek005e6aa0(void*,void*,W,W);
// Required genuine68-byte producer: real String+4, separate member+30h,
// final table73E1F8. This packet does not define that external class/lifetime.
void* __fastcall Archive0040ef90(void*,void*);
}
