#include "05d2bcb3d0-contracts.h"
#include "15d4c2890e-canonical_views.h"
#include "c46300269d-allocator_live.h"
#include <cstddef>
#include <type_traits>
#include "33a7cc3451-gui_cells.h"
// Genuine OPENGL32 IAT slot owners: the same declaration-only C names that
// task17-iat/opengl_iat_aliases.cpp maps with /alternatename to __imp__glX@N
// (the loader-written import cells). Exact types, no casts, no backing storage.
extern "C" {
extern decltype(&::glDisable) kotor_gui_import_glDisable;
extern decltype(&::glBegin) kotor_gui_import_glBegin;
extern decltype(&::glColor4ubv) kotor_gui_import_glColor4ubv;
extern decltype(&::glTexCoord2f) kotor_gui_import_glTexCoord2f;
extern decltype(&::glVertex4f) kotor_gui_import_glVertex4f;
extern decltype(&::glEnd) kotor_gui_import_glEnd;
extern decltype(&::glEnable) kotor_gui_import_glEnable;
}
namespace gui_wrap_draw {
static_assert(sizeof(Bindings)==112,"actual x86 GUI descriptor");
static_assert(offsetof(Bindings,pulse_0078d3d4)==0,"actual field order pulse_0078d3d4");
static_assert(std::is_same<decltype(Bindings::pulse_0078d3d4),decltype(&kotor_gui_canonical_required::pulse_0078d3d4)>::value,"actual type pulse_0078d3d4");
static_assert(offsetof(Bindings,busy_007bb4d0)==4,"actual field order busy_007bb4d0");
static_assert(std::is_same<decltype(Bindings::busy_007bb4d0),decltype(&kotor_gui_canonical_required::busy_007bb4d0)>::value,"actual type busy_007bb4d0");
static_assert(offsetof(Bindings,disable_0078d45c)==8,"actual field order disable_0078d45c");
static_assert(std::is_same<decltype(Bindings::disable_0078d45c),decltype(&kotor_gui_canonical_required::disable_0078d45c)>::value,"actual type disable_0078d45c");
static_assert(offsetof(Bindings,reset_007a689c)==12,"actual field order reset_007a689c");
static_assert(std::is_same<decltype(Bindings::reset_007a689c),decltype(&kotor_gui_canonical_required::reset_007a689c)>::value,"actual type reset_007a689c");
static_assert(offsetof(Bindings,viewport_index_007b9460)==16,"actual field order viewport_index_007b9460");
static_assert(std::is_same<decltype(Bindings::viewport_index_007b9460),decltype(&kotor_gui_canonical_required::index_007b9460)>::value,"actual type viewport_index_007b9460");
static_assert(offsetof(Bindings,viewport_dimensions_007b946c)==20,"actual field order viewport_dimensions_007b946c");
static_assert(std::is_same<decltype(Bindings::viewport_dimensions_007b946c),decltype(&kotor_gui_canonical_required::dimensions_first_007b946c)>::value,"actual type viewport_dimensions_007b946c");
static_assert(offsetof(Bindings,hundred_00741838)==24,"actual field order hundred_00741838");
static_assert(std::is_same<decltype(Bindings::hundred_00741838),decltype(&kotor_gui_canonical_required::hundred_00741838)>::value,"actual type hundred_00741838");
static_assert(offsetof(Bindings,quarter_0073edf0)==28,"actual field order quarter_0073edf0");
static_assert(std::is_same<decltype(Bindings::quarter_0073edf0),decltype(&kotor_gui_canonical_required::quarter_0073edf0)>::value,"actual type quarter_0073edf0");
static_assert(offsetof(Bindings,negative_hundred_0074186c)==32,"actual field order negative_hundred_0074186c");
static_assert(std::is_same<decltype(Bindings::negative_hundred_0074186c),decltype(&kotor_gui_canonical_required::negative_hundred_0074186c)>::value,"actual type negative_hundred_0074186c");
static_assert(offsetof(Bindings,epsilon_00741870)==36,"actual field order epsilon_00741870");
static_assert(std::is_same<decltype(Bindings::epsilon_00741870),decltype(&kotor_gui_canonical_required::epsilon_00741870)>::value,"actual type epsilon_00741870");
static_assert(offsetof(Bindings,zero_0073d700)==40,"actual field order zero_0073d700");
static_assert(std::is_same<decltype(Bindings::zero_0073d700),decltype(&kotor_gui_canonical_required::zero_0073d700)>::value,"actual type zero_0073d700");
static_assert(offsetof(Bindings,one_0073d764)==44,"actual field order one_0073d764");
static_assert(std::is_same<decltype(Bindings::one_0073d764),decltype(&kotor_gui_canonical_required::one_0073d764)>::value,"actual type one_0073d764");
static_assert(offsetof(Bindings,channel_0073ede4)==48,"actual field order channel_0073ede4");
static_assert(std::is_same<decltype(Bindings::channel_0073ede4),decltype(&kotor_gui_canonical_required::channel_0073ede4)>::value,"actual type channel_0073ede4");
static_assert(offsetof(Bindings,quads_0073f34c)==52,"actual field order quads_0073f34c");
static_assert(std::is_same<decltype(Bindings::quads_0073f34c),decltype(&kotor_gui_canonical_required::quads_0073f34c)>::value,"actual type quads_0073f34c");
static_assert(offsetof(Bindings,ftol_006fae8c)==56,"actual field order ftol_006fae8c");
static_assert(std::is_same<decltype(Bindings::ftol_006fae8c),decltype(&::unresolved_ftol2_006fae8c)>::value,"actual type ftol_006fae8c");
static_assert(offsetof(Bindings,alpha_006fc99d)==60,"actual field order alpha_006fc99d");
static_assert(std::is_same<decltype(Bindings::alpha_006fc99d),decltype(&gui_viewport_list::modern_byte_isalpha)>::value,"actual type alpha_006fc99d");
static_assert(offsetof(Bindings,allocate_006fa7e6)==64,"actual field order allocate_006fa7e6");
static_assert(std::is_same<decltype(Bindings::allocate_006fa7e6),decltype(&::live_operator_new)>::value,"actual type allocate_006fa7e6");
static_assert(offsetof(Bindings,free_006fa390)==68,"actual field order free_006fa390");
static_assert(std::is_same<decltype(Bindings::free_006fa390),decltype(&::live_free)>::value,"actual type free_006fa390");
static_assert(offsetof(Bindings,array_allocate_005e03c0)==72,"actual field order array_allocate_005e03c0");
static_assert(std::is_same<decltype(Bindings::array_allocate_005e03c0),decltype(&menu_list_growth::allocate_005e03c0)>::value,"actual type array_allocate_005e03c0");
static_assert(offsetof(Bindings,array_add_00444fe0)==76,"actual field order array_add_00444fe0");
static_assert(std::is_same<decltype(Bindings::array_add_00444fe0),decltype(&gui_viewport_list::add_00444fe0)>::value,"actual type array_add_00444fe0");
static_assert(offsetof(Bindings,viewport_alpha_00458ef0)==80,"actual field order viewport_alpha_00458ef0");
static_assert(std::is_same<decltype(Bindings::viewport_alpha_00458ef0),decltype(&gui_viewport_list::product_00458ef0)>::value,"actual type viewport_alpha_00458ef0");
static_assert(offsetof(Bindings,disable_0073d22c)==84,"actual field order disable_0073d22c");
static_assert(std::is_same<decltype(Bindings::disable_0073d22c),decltype(&kotor_gui_canonical_required::disable_0073d22c)>::value,"actual type disable_0073d22c");
static_assert(offsetof(Bindings,begin_0073d2d0)==88,"actual field order begin_0073d2d0");
static_assert(std::is_same<decltype(Bindings::begin_0073d2d0),decltype(&kotor_gui_canonical_required::begin_0073d2d0)>::value,"actual type begin_0073d2d0");
static_assert(offsetof(Bindings,color_0073d2e8)==92,"actual field order color_0073d2e8");
static_assert(std::is_same<decltype(Bindings::color_0073d2e8),decltype(&kotor_gui_canonical_required::color_0073d2e8)>::value,"actual type color_0073d2e8");
static_assert(offsetof(Bindings,texcoord_0073d2cc)==96,"actual field order texcoord_0073d2cc");
static_assert(std::is_same<decltype(Bindings::texcoord_0073d2cc),decltype(&kotor_gui_canonical_required::texcoord_0073d2cc)>::value,"actual type texcoord_0073d2cc");
static_assert(offsetof(Bindings,vertex_0073d324)==100,"actual field order vertex_0073d324");
static_assert(std::is_same<decltype(Bindings::vertex_0073d324),decltype(&kotor_gui_canonical_required::vertex_0073d324)>::value,"actual type vertex_0073d324");
static_assert(offsetof(Bindings,end_0073d2c4)==104,"actual field order end_0073d2c4");
static_assert(std::is_same<decltype(Bindings::end_0073d2c4),decltype(&kotor_gui_canonical_required::end_0073d2c4)>::value,"actual type end_0073d2c4");
static_assert(offsetof(Bindings,enable_0073d390)==108,"actual field order enable_0073d390");
static_assert(std::is_same<decltype(Bindings::enable_0073d390),decltype(&kotor_gui_canonical_required::enable_0073d390)>::value,"actual type enable_0073d390");
// Owner-object conversions (field==alias type equalities above are unchanged).
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::pulse_0078d3d4),decltype(Bindings::pulse_0078d3d4)>::value,"owner type pulse_0078d3d4");
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::busy_007bb4d0),decltype(Bindings::busy_007bb4d0)>::value,"owner type busy_007bb4d0");
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::disable_0078d45c),decltype(Bindings::disable_0078d45c)>::value,"owner type disable_0078d45c");
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::reset_007a689c),decltype(Bindings::reset_007a689c)>::value,"owner type reset_007a689c");
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::viewport_index_007b9460),decltype(Bindings::viewport_index_007b9460)>::value,"owner type index_007b9460");
static_assert(std::is_convertible<decltype(reinterpret_cast<const unsigned char*>(kotor_renderer_cells::viewport_frame_words_007b9468)+4),decltype(Bindings::viewport_dimensions_007b946c)>::value,"owner type dimensions_first_007b946c");
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::hundred_00741838),decltype(Bindings::hundred_00741838)>::value,"owner type hundred_00741838");
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::quarter_0073edf0),decltype(Bindings::quarter_0073edf0)>::value,"owner type quarter_0073edf0");
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::negative_hundred_0074186c),decltype(Bindings::negative_hundred_0074186c)>::value,"owner type negative_hundred_0074186c");
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::epsilon_00741870),decltype(Bindings::epsilon_00741870)>::value,"owner type epsilon_00741870");
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::zero_0073d700),decltype(Bindings::zero_0073d700)>::value,"owner type zero_0073d700");
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::one_0073d764),decltype(Bindings::one_0073d764)>::value,"owner type one_0073d764");
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::channel_0073ede4),decltype(Bindings::channel_0073ede4)>::value,"owner type channel_0073ede4");
static_assert(std::is_convertible<decltype(&kotor_renderer_cells::quads_0073f34c),decltype(Bindings::quads_0073f34c)>::value,"owner type quads_0073f34c");
static_assert(std::is_convertible<decltype(&kotor_gui_import_glDisable),decltype(Bindings::disable_0073d22c)>::value,"owner type disable_0073d22c");
static_assert(std::is_convertible<decltype(&kotor_gui_import_glBegin),decltype(Bindings::begin_0073d2d0)>::value,"owner type begin_0073d2d0");
static_assert(std::is_convertible<decltype(&kotor_gui_import_glColor4ubv),decltype(Bindings::color_0073d2e8)>::value,"owner type color_0073d2e8");
static_assert(std::is_convertible<decltype(&kotor_gui_import_glTexCoord2f),decltype(Bindings::texcoord_0073d2cc)>::value,"owner type texcoord_0073d2cc");
static_assert(std::is_convertible<decltype(&kotor_gui_import_glVertex4f),decltype(Bindings::vertex_0073d324)>::value,"owner type vertex_0073d324");
static_assert(std::is_convertible<decltype(&kotor_gui_import_glEnd),decltype(Bindings::end_0073d2c4)>::value,"owner type end_0073d2c4");
static_assert(std::is_convertible<decltype(&kotor_gui_import_glEnable),decltype(Bindings::enable_0073d390)>::value,"owner type enable_0073d390");
namespace {
const Bindings descriptor={
 &kotor_renderer_cells::pulse_0078d3d4,
 &kotor_renderer_cells::busy_007bb4d0,
 &kotor_renderer_cells::disable_0078d45c,
 &kotor_renderer_cells::reset_007a689c,
 &kotor_renderer_cells::viewport_index_007b9460,
 reinterpret_cast<const unsigned char*>(kotor_renderer_cells::viewport_frame_words_007b9468)+4,
 &kotor_renderer_cells::hundred_00741838,
 &kotor_renderer_cells::quarter_0073edf0,
 &kotor_renderer_cells::negative_hundred_0074186c,
 &kotor_renderer_cells::epsilon_00741870,
 &kotor_renderer_cells::zero_0073d700,
 &kotor_renderer_cells::one_0073d764,
 &kotor_renderer_cells::channel_0073ede4,
 &kotor_renderer_cells::quads_0073f34c,
 &::unresolved_ftol2_006fae8c,
 &gui_viewport_list::modern_byte_isalpha,
 &::live_operator_new,
 &::live_free,
 &menu_list_growth::allocate_005e03c0,
 &gui_viewport_list::add_00444fe0,
 &gui_viewport_list::product_00458ef0,
 &kotor_gui_import_glDisable,
 &kotor_gui_import_glBegin,
 &kotor_gui_import_glColor4ubv,
 &kotor_gui_import_glTexCoord2f,
 &kotor_gui_import_glVertex4f,
 &kotor_gui_import_glEnd,
 &kotor_gui_import_glEnable
};
}
const Bindings& bindings=descriptor;
}
