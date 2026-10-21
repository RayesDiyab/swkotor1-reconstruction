#include "b16343a67a-warning_runtime.h"
#include "9c79b7f222-string_transforms.h"
#include "0ec67db7c4-native_strings.h"
#include "3e136bc34a-string_priority.h"
#include "2b0fe5a1e2-ini_file_primitives.h"

namespace base_ini_reconstruction {
using kotor_reconstruction::StartupString;
using kotor_reconstruction::StartupAssign005e5c50;
using kotor_reconstruction::StartupFormat005e5680;

void __fastcall warning_internal_005ec2b0(
    void*, void*, int line, const char* source, const char* message) {
    StartupString warning;
    if (!message)
        StartupFormat005e5680(&warning, warning_source_format_0074e660, source, line);
    else
        StartupFormat005e5680(&warning, warning_message_format_0074e5e0, source, line, message);

    void* stream = kotor_functional::modern_ini_open(warning_path_0074e5ac, warning_mode_0074e5a8);
    if (stream) {
        // Actual original printf-as-format behavior: no extra variadic words.
        kotor_functional::modern_ini_print(stream, WindowCStr005e5670(&warning, nullptr));
    }
    StartupString chunk;
    StartupString remainder;
    const char* basename = kotor_functional::modern_ini_strrchr(source, '\\');
    basename = basename ? basename + 1 : source;
    StartupFormat005e5680(&chunk, warning_prefix_0074e5c4, basename, line);
    (void)WindowCStr005e5670(&chunk, nullptr);
    if (message) {
        GuiAssign005e5140(&remainder, nullptr, message);
        std::int32_t length = remainder.BorrowedLength005e5790();
        while (length != 0) {
            if (length <= 50) {
                StartupAssign005e5c50(&chunk, nullptr, &remainder);
                GuiAssign005e5140(&remainder, nullptr, warning_empty_0073d71c);
            } else {
                {
                    alignas(StartupString) unsigned char storage[sizeof(StartupString)];
                    StartupString* temporary = left_005e5f70(&remainder, nullptr,
                        reinterpret_cast<StartupString*>(storage), 50);
                    StartupAssign005e5c50(&chunk, nullptr, temporary);
                    temporary->~StartupString();
                }
                std::int32_t cut = 50;
                const char* delimiter = kotor_functional::modern_ini_strrchr(
                    WindowCStr005e5670(&chunk, nullptr), ' ');
                if (delimiter)
                    cut = static_cast<std::int32_t>(delimiter - WindowCStr005e5670(&chunk, nullptr));
                delimiter = kotor_functional::modern_ini_strrchr(
                    WindowCStr005e5670(&chunk, nullptr), '\t');
                if (delimiter)
                    cut = static_cast<std::int32_t>(delimiter - WindowCStr005e5670(&chunk, nullptr));
                delimiter = kotor_functional::modern_ini_strrchr(
                    WindowCStr005e5670(&chunk, nullptr), '\r');
                if (delimiter)
                    cut = static_cast<std::int32_t>(delimiter - WindowCStr005e5670(&chunk, nullptr));
                {
                    alignas(StartupString) unsigned char storage[sizeof(StartupString)];
                    StartupString* temporary = left_005e5f70(&remainder, nullptr,
                        reinterpret_cast<StartupString*>(storage), cut);
                    StartupAssign005e5c50(&chunk, nullptr, temporary);
                    temporary->~StartupString();
                }
                {
                    alignas(StartupString) unsigned char storage[sizeof(StartupString)];
                    StartupString* temporary = right_005e6170(&remainder, nullptr,
                        reinterpret_cast<StartupString*>(storage), length - cut);
                    StartupAssign005e5c50(&remainder, nullptr, temporary);
                    temporary->~StartupString();
                }
            }
            (void)WindowCStr005e5670(&chunk, nullptr);
            length = remainder.BorrowedLength005e5790();
        }
    }
    if (stream) (void)kotor_functional::modern_ini_close(stream);
    // Actual three normal-path destructors: remainder, chunk, warning.
}
} // namespace base_ini_reconstruction
