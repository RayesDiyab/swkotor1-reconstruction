#include "ccf351fc413859fa_placeable_fields.hpp"
namespace kotor::server_mainloop {
Word placeable_fields_00585670(Memory&m,PlaceableFieldPorts&p,Word self,Word gff,Word root,Word templates,Word s){
 auto sign16=[](Word v){return (v&0x8000)?(v|0xffff0000u):(v&0xffff);};
 auto byte=[&](Word label,Word fallback=0){return p.byte_read(gff,root,label,s+0xc,fallback)&255;};
 auto dword=[&](Word label,Word fallback=0){return p.dword(gff,root,label,s+0xc,fallback);};
 auto scalar=[&](Word offset,Word label,bool narrow){Word v=byte(label);if(narrow)p.write_byte(self+offset,v);else m.write(self+offset,v);};
 auto ref=[&](Word label,Word target){p.ref_empty(s+0x24);Word v=p.ref_read(gff,s+0x4c,root,label,s+0xc,s+0x24);p.ref_assign(self+target,s+0x3c,v);};
 auto localized=[&](Word label,Word target){Word fallback=p.localized_construct(s+0x1c);Word v=p.localized_read(gff,s+0x24,root,label,s+0xc,fallback);p.localized_assign(self+target,v);p.localized_destroy(s+0x24);p.localized_destroy(s+0x1c);};
 p.string_zero(s+0x34);p.string_construct(s+0x24,0x73d71c);Word v=p.string_read(gff,s+0x1c,root,0x747a28,s+0xc,s+0x24);p.tag(self,v);p.string_destroy(s+0x1c);p.string_destroy(s+0x24);
 ref(0x747494,0x378);localized(0x7497e0,0x228);scalar(0x270,0x7497d0,false);m.write(self+0x23c,dword(0x7497c8));
 m.write(s+0x1c,byte(0x745368,m.read(self+0xf8)));if(!m.read(s+0xc))m.write(s+0x1c,byte(0x748f64,m.read(self+0xf8)));
 m.write(self+0x200,byte(0x7497c0,m.read(self+0x200)));scalar(0x218,0x7497b0,false);scalar(0x274,0x7497a4,true);
 p.string_construct(s+0x24,0x73d71c);v=p.string_read(gff,s+0x10,root,0x74979c,s+0xc,s+0x24);p.string_assign(self+0x264,v);p.string_destroy(s+0x10);p.string_destroy(s+0x24);
 scalar(0x280,0x74978c,false);scalar(0x284,0x74977c,false);scalar(0x27c,0x749770,true);scalar(0x276,0x749760,true);scalar(0x278,0x749754,false);
 v=byte(0x749748);p.write_byte(s+0x10,p.byte(self+0x290));Word fallback=m.read(s+0x10);m.write(self+0x288,v);p.write_byte(self+0x290,byte(0x74973c,fallback));scalar(0x328,0x748e28,false);
 v=byte(0x749734);if(!m.read(s+0xc))v=m.read(self+0x328)!=1;if(m.read(self+0x39c)==1)v=0;m.write(self+0x398,v);m.write(self+0xf8,v==1?1:m.read(s+0x1c));
 p.write_byte(self+0x230,dword(0x746efc));m.write(self+0xe0,sign16(p.short_read(gff,root,0x749730,s+0xc,0)));p.hitpoints(self,sign16(p.short_read(gff,root,0x749724,s+0xc,0)));
 const Word scalarRows[][3]={{0x258,0x749718,1},{0x314,0x749710,1},{0x315,0x749708,1},{0x316,0x749704,1},{0x330,0x7496f8,0},{0x260,0x7496f0,0},{0x324,0x7496e0,0},{0x26c,0x7496d4,0},{0x275,0x7496c8,1}};
 for(const auto&r:scalarRows)scalar(r[0],r[1],r[2]!=0);
 Word portrait=p.word_read(gff,root,0x74937c,s+0xc,65535);if((portrait&65535)>=65534){p.ref_empty(s+0x24);Word table=m.read(self);m.write(s+0x1c,table);v=p.ref_read(gff,s+0x4c,root,0x749370,s+0xc,s+0x24);p.portrait_ref(self,m.read(s+0x1c),v);}else p.portrait_id(self,portrait);
 ref(0x7496b8,0x240);p.write_byte(s+0x1c,p.byte(self+0x394));p.write_byte(self+0x394,byte(0x7494c8,m.read(s+0x1c)));scalar(0x334,0x7496a8,false);
 (void)byte(0x74969c,m.read(self+0x254));m.write(self+0x254,1);v=p.light(self);p.light_set(self,byte(0x749690,v));localized(0x7488c0,0x234);
 const Word scripts[][2]={{0x749684,0x294},{0x749678,0x29c},{0x744d0c,0x2a4},{0x74966c,0x2ac},{0x747a4c,0x2b4},{0x74965c,0x2bc},{0x749654,0x2c4},{0x749644,0x2cc},{0x74963c,0x2d4},{0x74962c,0x2dc},{0x749620,0x2ec},{0x749618,0x2f4},{0x747a3c,0x2fc},{0x74960c,0x304},{0x7495fc,0x30c},{0x7495ec,0x2e4}};
 for(const auto&r:scripts){p.ref_empty(s+0x24);v=p.ref_read(gff,s+0x4c,root,r[0],s+0xc,s+0x24);p.ref_string(v,s+0x34);p.string_assign(self+r[1],s+0x34);}
 if(!(self+0x2e4)||p.string_empty(self+0x2e4)||p.string_equal(self+0x2e4,0x73df6c)){
  p.string_zero(s+0x24);p.string_construct(s+0x1c,0x7495e0);Word row=p.byte(self+0x290);Word rules=m.read(0x7a3a28);Word tables=m.read(rules+0xb8);Word table=m.read(tables+0x8c);p.table_string(table,row,s+0x1c,s+0x24);p.string_destroy(s+0x1c);p.string_assign(self+0x2e4,s+0x24);p.string_destroy(s+0x24);
 }
 m.write(self+0x440,byte(0x7495d4,m.read(self+0x440)));m.write(self+0x44c,byte(0x7495c8,m.read(self+0x44c)));v=byte(0x7495c0);m.write(self+0x338,v);
 if(v)p.state(self,0x275b);else{v=p.integer(gff,root,0x746060,s+0xc,0);if(m.read(s+0xc))p.state(self,v);else{v=byte(0x7495b0);if(m.read(s+0xc)){const Word states[]={0x2710,0x275b,0x275c,0x2758,0x2759,0x275a};p.state(self,v<=5?states[v]:0x2710);}}}
 if(p.list(gff,s+0x5c,root,0x747210)==1){m.write(s+0x1c,0);if(p.count(gff,s+0x5c)){Word index=0;do{
  m.write(s+0x10,0);p.element(gff,s+0x18,s+0x5c,index);Word id=p.dword(gff,s+0x18,0x744c24,s+0xc,0x7f000000);Word storage=p.allocate(0x29c);m.write(s+0x24,storage);Word item=storage?p.item_construct(storage,id):0;m.write(s+0x10,item);
  if(item){bool ok;if(templates){p.ref_empty(s+0x3c);(void)p.ref_read(gff,s+0x4c,s+0x18,0x747200,s+0xc,s+0x3c);ok=m.read(s+0xc)&&p.item_template(m.read(s+0x10),s+0x4c);}else ok=p.item_load(item,gff,s+0x18)!=0;
   if(!ok){Word live=m.read(s+0x10);if(live)p.item_destroy(live);m.write(s+0x10,0);}else if(m.read(s+0x10))p.item_attach(self,s+0x10,0x7f000000,1);
  }
  index=m.read(s+0x1c)+1;m.write(s+0x1c,index);
 }while(index<p.count(gff,s+0x5c));}}
 Word id=m.read(self+4);Word manager=m.read(0x7a39fc);Word server=m.read(manager+8);Word module=p.module(server);p.register_tag(module,self+0x18,id);p.finish(self);p.string_destroy(s+0x34);return 1;
}
}
