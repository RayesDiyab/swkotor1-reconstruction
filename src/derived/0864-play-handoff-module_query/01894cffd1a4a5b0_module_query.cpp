#include "d6878cc6be926234_module_query.hpp"
namespace recovered_module_query {
unsigned save_allowed_004b20e0(const String*module,Ports&p){void*memory=p.allocate(84);void*table=nullptr;if(memory){Ref ref;p.ref_literal(&ref,"modulesave");table=p.table_construct(memory,ref,0);}if(!table)return 1;if(!p.table_load(table)){p.table_delete(table,1);return 1;}String column;p.string_literal(&column,"IncludeInSave");unsigned value;int found=p.table_named_int(table,module,&column,&value);p.string_destroy(&column);p.table_delete(table,1);return found?(value!=0):1;}
unsigned query_004b51a0(const String*module,Ports&p){if(static_cast<int>(p.length(module))<1)return 1;String unusedA,unusedB;p.string_default(&unusedA);p.string_default(&unusedB);unsigned type=0xffff,result=0;
 auto exists=[&](unsigned t){Ref ref;p.ref_from_string(&ref,const_cast<String*>(module));return p.resource_exists(&ref,t,nullptr);};
 if(save_allowed_004b20e0(module,p)){p.party_mount();if(exists(0xbc1))type=0xbc1;else if(exists(0x809))type=0x809;if(type!=0xffff)result=1;p.party_unmount();}
 if(type==0xffff){String path;p.string_literal(&path,"NWMFILES:");p.directory(&path);p.string_destroy(&path);if(exists(0x80e)){type=0x80e;result=0;}p.string_literal(&path,"NWMFILES:");p.release(&path);p.string_destroy(&path);}
 if(type==0xffff){String path;p.string_literal(&path,"MODULES:");p.directory(&path);p.string_destroy(&path);if(exists(0x7db))type=0x7db;else if(exists(0xbba))type=0xbba;if(type!=0xffff)result=0;p.string_literal(&path,"MODULES:");p.release(&path);p.string_destroy(&path);}
 if(type==0xffff){String live;p.string_default(&live);unsigned i=1;while(i<p.live_count()){p.format_live(&live,i);if(p.length(p.resolve_alias(&live,0))){String suffix,combined;p.string_literal(&suffix,":MODULES");p.concat(&combined,&live,&suffix);p.assign_string(&live,&combined);p.string_destroy(&combined);p.string_destroy(&suffix);p.directory(&live);if(exists(0x7db)){type=0x7db;result=0;}else if(type==0xffff&&exists(0xbba)){type=0xbba;result=0;}p.release(&live);if(type!=0xffff)break;}++i;}p.string_destroy(&live);}
 p.reset_error();p.string_destroy(&unusedB);p.string_destroy(&unusedA);return type==0xffff?0:result;}
}
