#pragma once
namespace gui_viewport_list {
// Supported host CRT boundary, not a reconstructed-address implementation.
// Retained caller supplies an unsigned byte widened to int and tests != 0.
int __cdecl modern_byte_isalpha(int);
}
