#include "525a467aa1-adoption.h"
#include "722469ff1f-native_strings.h"
namespace kotor_reconstruction {
int StartupString::ComparePrefix005e5900(StartupString* input,std::int32_t count) {
 const char* right=input->data;



 if(!right)right=mapped_empty_008328a4;

 const char* left=data;



 if(!left)return 0;





 const std::uint32_t width=static_cast<std::uint32_t>(count);

 // Query bytes first, key bytes second, raw DWORD last, full result normalized.
 return alias_modern::bounded_compare(right,left,width)==0?1:0;
}
}
