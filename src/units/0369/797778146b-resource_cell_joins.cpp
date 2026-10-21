#include "9d0f111af7-resource_cell_joins.h"
#include "7b8c81fcff-allocator_live.h"
#include <type_traits>
namespace task10_resource_cells {
static_assert(sizeof(menu_normal_40_49::Providers)==160&&sizeof(menu_normal_retained_40_49::Providers)==116,"current tables");
static_assert(offsetof(menu_normal_40_49::Providers,resource_007a39e8)==128&&offsetof(menu_normal_retained_40_49::Providers,resource_007a39e8)==88,"current cell fields");
static_assert(offsetof(menu_normal_40_49::Providers,empty_0073d71c)==152&&offsetof(menu_normal_retained_40_49::Providers,empty_0073d71c)==108,"current literals");
static_assert(std::is_same<decltype(menu_normal_40_49::Providers::resource_007a39e8),ManagerCell>::value&&std::is_same<decltype(menu_normal_retained_40_49::Providers::resource_007a39e8),ManagerCell>::value,"one typed cell contract");
template<class T>static bool callbacks_owned(const T& t) noexcept {
 const auto c=task22_strings::callbacks();
 return t.string_construct_005b3190==c.construct&&t.string_destroy_005e5c20==c.destroy&&t.assign_005e5140==c.assign&&t.cstr_005e5670==c.cstr&&t.not_equal_005e5390==c.not_equal&&t.allocate_006fa7e6==&live_operator_new&&t.free_006fa390==&live_free;
}
static bool owners(const task22_strings::Attachment& s) noexcept {
 return s.active&&s.gui==&menu_normal_40_49::providers&&s.retained==&menu_normal_retained_40_49::providers&&callbacks_owned(*s.gui)&&callbacks_owned(*s.retained);
}
Result attach(Attachment& a,task22_strings::Attachment& s) noexcept {
 if(a.active)return Result::AlreadyAttached;
 if(!owners(s))return Result::InvalidOwners;
 const auto cell=canonical_cell();
 if((s.gui->resource_007a39e8&&s.gui->resource_007a39e8!=cell)||(s.retained->resource_007a39e8&&s.retained->resource_007a39e8!=cell)||s.gui->resource_007a39e8!=s.gui_resource_cell||s.retained->resource_007a39e8!=s.retained_resource_cell)return Result::Conflict;
 a.strings=&s;a.previous_gui=s.gui->resource_007a39e8;a.previous_retained=s.retained->resource_007a39e8;
 s.gui->resource_007a39e8=cell;s.retained->resource_007a39e8=cell;a.active=true;
 return Result::PreparedManagerCells;
}
bool detach(Attachment& a,bool drained) noexcept {
 if(!drained)return false;if(!a.active)return true;
 if(!a.strings||!owners(*a.strings))return false;
 const auto cell=canonical_cell();auto& s=*a.strings;
 if(s.gui->resource_007a39e8!=cell||s.retained->resource_007a39e8!=cell||s.gui_resource_cell!=a.previous_gui||s.retained_resource_cell!=a.previous_retained)return false;
 s.gui->resource_007a39e8=a.previous_gui;s.retained->resource_007a39e8=a.previous_retained;
 a.strings=nullptr;a.active=false;return true;
}
}
