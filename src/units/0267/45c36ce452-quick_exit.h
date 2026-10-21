#pragma once
#include "ed9d03e5b0-selected_termination.h"

namespace kotor_quick_exit_dependencies {
//006FB13F is a general cdecl three-int provider. return_to_caller!=0 may return;
//the zero branch must follow its real termination route and never return normally.
//No implementation or direct host-CRT/ExitProcess substitution is supplied here.
void __cdecl internal_exit_006fb13f(int code,int quick,int return_to_caller);
}
