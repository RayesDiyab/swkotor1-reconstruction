// lane-m48-traps (main) as amended by lane-m15-f (M1.5: #71 lifted): CSWGuiMainMenu event handlers (D83 new-game/load/options category) and the
// SetInputClass in-game callees declared by gui-load/mainmenu.h. Evidence: lane-m48-traps/TRAPS.json.
//
// The four handlers keep their original guard prefix (identical in all four, 0067AFCB.. / 0067B1B8.. /
// 0067B268.. / 0067B308..): return when the panel state ([this+44h] & 600h) == 400h, when [this+140Ch] == 0
// (set to 1 by CSWGuiMainMenu::Draw 0067AC28; 0 from the ctor 0067CA21), or when the activating control's
// [control+4Ch] == 0. Past the guard each handler builds a whole game subsystem (character generation,
// save/load, title movies, options tree): D83 branch trap at the first post-guard instruction. The handlers
// run only when the user activates the corresponding main-menu button (registered by the ctor 0067C682..0067C81D
// for events 27h/2Dh), never on WinMain -> first menu frame -> idle.
#include "d26ddbc757-mainmenu.h"
#include "c272944ff0-kotor_unreachable_trap.h"
#include <cstdint>
#include <cstring>

// [lane-m15-f, Milestone 1.5] OnNewGamePicked 0067AFB0 (trap #71 0067AFFB) is LIFTED: its guard prefix and the
// post-guard body (0067AFFB..0067B184) are single-owner lane L15-A, which defines
// ?OnNewGamePicked@MainMenuHandlers@gui_load@@QAEXPAUControl@kotor_input_hover@@@Z. The shared guard helper
// (word_at / guard_returns) had no other user in this unit and is removed with it (W4/WX: unreferenced function).
// 0067B1A0 OnLoadSaveGame: trap #72 lifted in M50; single owner lane-m1-saveload (src/saveload.cpp).
// 0067B250 OnMoviesPicked: trap #73 lifted in M50; single owner lane-m1-movies (src/title_movies.cpp).
// 0067B2F0 OnOptionsPicked: trap #74 lifted in M50; single owner lane-m1-options (options/options_main.cpp).

extern "C" {
// 005F2BB0 CClientExoAppInternal::SetPlayerCreature (RET 4). Callers: CClientExoApp::SetPlayerCreature 005ED783
// (<- VM DestroyObject 00530087, HandleServerToPlayerCreatureUpdate_ControlObject 0064CB1B / _Appearance 00656AC5),
// MainLoop 00603242 (only when client GetPlayerCreature 005F1420 != 0), SetInputClass 00620191 (only when
// client+28Ch != 7F000000h and the server player creature exists). On the menu client+20h = client+28Ch =
// 7F000000h (ctor 005FC216/005FC219) and the server (AppManager+8) is 0.
// Lane W4: trap lifted; single owner lane-w4 src/frame/client_update.cpp (kotor_fastcall_005f2bb0).
// 005F5CF0 CClientExoAppInternal::AttachPlayerGameObjectToArea (RET). Callers: 005ED723 wrapper (<- VM DestroyObject
// 00530095, CreatureUpdate_ControlObject 0064CB8F / _Appearance 00656B07) and SetInputClass 0062019E (same gate as
// SetPlayerCreature above).
// Lane W4: trap lifted; single owner lane-w4 src/frame/client_update.cpp (kotor_fastcall_005f5cf0).
// 00671670 CSWMiniGame::SetCamera / 006714E0 CSWMiniGame::Go: traps removed (lane closeC); real bodies are
// single-owner srcw4 src/gui/claude_gui_char_minigame.cpp (kotor_fastcall_00671670 / kotor_fastcall_006714e0).
}
