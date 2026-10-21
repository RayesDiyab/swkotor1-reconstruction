// lane-prepare: ReInitAurora fullscreen backdrop worker.
// CreateThread at 00403885 (start=00401A30, parameter=the event created at
// 00403869); ReInitAurora signals the event at 00403C64 and joins the thread
// (00403C8A GetExitCodeThread loop, 00403CA5/00403CA8 CloseHandle).
// Reached only when aurora vslot 4 returns 1 and fullscreen==1 (00403850).
#include "2ff1299369-game_prepare_producers.h"
#include "df72b7ab87-window_shared_cells.h"   // instance_007a39c0, render_007a39d8
#include "68794e31bc-prepare_image_data.h"    // D13 generated .rdata strings

extern "C" {
// .data 007A3CEC (load image 0): "class registered" flag (00401A30 read, 00401AE3 write).
std::uint32_t kotor_cell_backdrop_class_007a3cec = 0;

// 00401590: xor eax,eax; ret 10h - the backdrop class window procedure returns
// 0 for every message (original body, not a placeholder).
LRESULT __stdcall kotor_stdcall_00401590(HWND, UINT, WPARAM, LPARAM) { return 0; }

// 00401A30 initCinematicBackdrop(LPVOID event) stdcall RET 4.
DWORD __stdcall kotor_stdcall_00401a30(void* event) {
    if (kotor_cell_backdrop_class_007a3cec == 0) {                        // 00401A30..00401A3D
        WNDCLASSA wc;                                                     // [esp+4] local (40 bytes)
        wc.style = 0;                                                     // 00401A4B
        wc.lpfnWndProc = &kotor_stdcall_00401590;                          // 00401A4F (00401590h)
        wc.cbClsExtra = 0;                                                // 00401A57
        wc.cbWndExtra = 0;                                                // 00401A5B
        wc.hInstance = kotor_menu_window_cells::instance_007a39c0;        // 00401A43/00401A5F
        wc.hIcon = LoadIconA(wc.hInstance, MAKEINTRESOURCEA(0x65));       // 00401A48..00401A6F
        wc.hCursor = LoadCursorA(nullptr, MAKEINTRESOURCEA(0x7F00));      // 00401A69..00401A79 (IDC_ARROW)
        wc.hbrBackground = nullptr;                                       // 00401A82
        wc.lpszMenuName = nullptr;                                        // 00401A86
        wc.lpszClassName = kotor_rdata_str_0073d634;                                    // 00401A8A (0073D634)
        if (RegisterClassA(&wc) == 0) {                                   // 00401A92..00401A9B (test ax,ax)
            // 00401A9D..00401AE0: FormatMessageA writes the LocalAlloc'd text
            // pointer into the thread-parameter stack slot, then MessageBoxA,
            // LocalFree, return 0.
            LPSTR text = nullptr;
            FormatMessageA(0x1300, nullptr, GetLastError(), 0x400, reinterpret_cast<LPSTR>(&text), 0, nullptr);
            MessageBoxA(nullptr, text, kotor_rdata_str_0073d62c, 0x40);                     // 00401AC0..00401AC9 (0073D62C)
            LocalFree(text);                                              // 00401AD4
            return 0;                                                     // 00401ADA
        }
        kotor_cell_backdrop_class_007a3cec = 1;                           // 00401AE3
    }
    WINDOWINFO info;                                                      // [esp+2Ch] local
    info.cbSize = 0x3C;                                                   // 00401AF9
    if (!GetWindowInfo(kotor_menu_window_cells::render_007a39d8, &info)) return 0; // 00401AED..00401B09
    // 00401B16..00401B37: both name arguments are 0073D610 in the original.
    HWND window = CreateWindowExA(8, kotor_rdata_str_0073d610, kotor_rdata_str_0073d610,
                                  0x90000000u, 0, 0, 0x640, 0x4B0, nullptr, nullptr,
                                  kotor_menu_window_cells::instance_007a39c0, nullptr);
    SetWindowPos(window, HWND_TOPMOST, 0, 0, 0x640, 0x4B0, 0x121);        // 00401B3D..00401B53
    HDC dc = GetDC(window);                                               // 00401B59/00401B5A
    PatBlt(dc, 0, 0, 0x640, 0x4B0, 0x42);                                 // 00401B60..00401B71 (BLACKNESS)
    ReleaseDC(window, dc);                                                // 00401B77..00401B79
    ShowWindow(window, 3);                                                // 00401B7F..00401B82
    while (WaitForSingleObject(static_cast<HANDLE>(event), 1) == 0x102) {} // 00401B88..00401B9C
    DestroyWindow(window);                                                // 00401B9E/00401B9F
    return 0;                                                             // 00401BA7
}
}
