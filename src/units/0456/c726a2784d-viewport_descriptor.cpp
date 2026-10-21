#include "cb2403cfb0-providers.h"
#include "15d4c2890e-canonical_views.h"
#include "33a7cc3451-gui_cells.h"
#include <type_traits>
#include <cstddef>
namespace gui_viewport_list {
static_assert(sizeof(ViewportBindings)==12,"actual x86 viewport descriptor");
static_assert(offsetof(ViewportBindings,index_007b9460)==0 && offsetof(ViewportBindings,alpha_007b9248)==4 && offsetof(ViewportBindings,one_0073d764)==8,"actual naked consumer offsets");
// Constant initialization from the sole owner objects (no .CRT$XCU). Each owner
// is exactly the object the matching kotor_gui_canonical_required alias binds.
static_assert(std::is_same<decltype(ViewportBindings::index_007b9460),decltype(&kotor_gui_canonical_required::index_007b9460)>::value && std::is_convertible<decltype(&kotor_renderer_cells::viewport_index_007b9460),decltype(ViewportBindings::index_007b9460)>::value,"index owner");
static_assert(std::is_same<decltype(ViewportBindings::alpha_007b9248),decltype(&kotor_gui_canonical_required::alpha_first_007b9248)>::value && std::is_convertible<decltype(&kotor_renderer_cells::viewport_alphas_007b9248[0]),decltype(ViewportBindings::alpha_007b9248)>::value,"alpha owner");
static_assert(std::is_same<decltype(ViewportBindings::one_0073d764),decltype(&kotor_gui_canonical_required::one_0073d764)>::value && std::is_same<decltype(&kotor_renderer_cells::one_0073d764),decltype(ViewportBindings::one_0073d764)>::value,"one owner");
namespace {
const ViewportBindings descriptor={
 &kotor_renderer_cells::viewport_index_007b9460,
 &kotor_renderer_cells::viewport_alphas_007b9248[0],
 &kotor_renderer_cells::one_0073d764
};
}
const ViewportBindings& viewport=descriptor;
}
