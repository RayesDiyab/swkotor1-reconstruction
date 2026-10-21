#include "5fb9bb198a-ini_remaining_join.h"
namespace base_ini_reconstruction {
void install_existing_ini_joins(Bindings& main,WriteBindings& write){
 static_assert(std::is_same<decltype(main.next_005e9b90),decltype(&next_005e9b90)>::value,"Exact genuine Next transport");
 static_assert(std::is_same<decltype(write.concat_005e5d10),decltype(&GuiConcat005e5d10)>::value,"Exact potentially throwing C++ Concat transport");
 static_assert(sizeof(Bindings)==112&&sizeof(WriteBindings)==12,"Sole table storage unchanged");
 main.next_005e9b90=&next_005e9b90;
 write.concat_005e5d10=&GuiConcat005e5d10;
}
}
