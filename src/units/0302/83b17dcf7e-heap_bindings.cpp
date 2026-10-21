#include "af64fb3b38-growth.h"
#include "e97b3c65a6-allocator_live.h"
namespace menu_list_growth {
static const Bindings actual_heap={&live_operator_new,&live_free};
const Bindings& bindings=actual_heap;
}
