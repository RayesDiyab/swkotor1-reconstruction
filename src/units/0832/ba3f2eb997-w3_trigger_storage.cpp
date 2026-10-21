#include "f4d9e1ca72-w3_trigger_storage.h"
#include "9b60e1d22a-w3_trigger_node.h"
#include <cstdlib>

extern "C" {
alignas(4) unsigned char w3_trigger_node_008304c0[0x50] = {};

void __cdecl w3_trigger_node_exit_0073be30() {
    aurora_trigger_destruct_0043e1a0(
        reinterpret_cast<aurora_model::MdlNode*>(w3_trigger_node_008304c0), nullptr);
}

void __cdecl w3_trigger_node_initialize_0073b0f0() {
    recovered_mdl_node_trigger_construct_0049d7b0(w3_trigger_node_008304c0, nullptr, "trigger");
    (void)std::atexit(w3_trigger_node_exit_0073be30);
}
}

// Original CRT initializer 0073B0F0 constructs the inline node, then registers
// 0073BE30. This entry uses the linked CRT's existing initializer mechanism.
#pragma section(".CRT$XCU", read)
extern "C" {
extern __declspec(allocate(".CRT$XCU")) void (__cdecl* const w3_trigger_node_startup_0073b0f0)() =
    w3_trigger_node_initialize_0073b0f0;
}
