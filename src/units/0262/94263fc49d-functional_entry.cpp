#include "6a19be3575-functional_bootstrap.h"
#include <stdlib.h>

// The supported /MT WinMainCRTStartup owns cookie, native CRT, C++ initializers,
// compiler EH and process termination. There is no custom PE entry or second CRT.
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command, int show) {
    return kotor_functional::run({instance, previous, command, show, __argc, __argv});
}

