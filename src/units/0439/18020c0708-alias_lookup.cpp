#include "988aa59a9a-alias_path.h"
namespace alias_path {
String* AliasListView::GetAliasPath(String* query,std::int32_t count) {
 return internal->lookup(query,count);
}
String* InternalView::lookup(String* query,std::int32_t count) {
 if((query->*providers.equals_005e5290)(providers.empty_0073d71c)!=0)return &fallback;



 if(count==0)count=(query->*providers.length_005e5790)();

 NodeView* node=head;
 while(node) {



  const int result=(node->key.*providers.prefix_005e5900)(query,count);



  if(result==1)return &node->path;




  node=node->next;

 }



 return &fallback;

}
}
