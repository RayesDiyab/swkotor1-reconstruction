#include "1adde386db-resources.h"
namespace menu_resources {
Word Manager::exists_00408bc0(Ref* resource,Word type,Word* output){
 Word entry=0;
 const Word status=unresolved_key_entry_00407230(this,nullptr,resource,type,&entry,&type);
 if(status&&output){Word value;memcpy(&value,reinterpret_cast<const void*>(entry+0x1c),4);*output=value;}



 return status;

}
}
