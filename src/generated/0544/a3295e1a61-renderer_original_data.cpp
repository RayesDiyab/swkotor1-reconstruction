// Sole definer of the extracted original data objects (decision D13).
// Requires the LOCAL generated header on the include path (never committed):
//   python tools/extract_original_data.py spec/EXTRACT-SPEC.json <local-dir>
#include "eca27cee56-renderer_original_data.h"
#if !__has_include(<kotor_generated/renderer/kotor_original_renderer_data.generated.h>)
#error Run tools/extract_original_data.py against the pinned swkotor.exe first (D13).
#endif
#define KOTOR_ORIGINAL_CSTRING(name, length, literal) \
    const char name[length] = literal; \
    static_assert(sizeof(literal) == (length), #name " length differs from the extract manifest");
#define KOTOR_ORIGINAL_BYTES(name, length, ...) \
    const unsigned char name[length] = __VA_ARGS__;
namespace kotor_renderer_gl_cells {
#include <kotor_generated/renderer/kotor_original_renderer_data.generated.h>
}
