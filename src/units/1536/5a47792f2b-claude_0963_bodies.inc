// 0963-gates (Claude): faithful bodies for the former required(0x...) gates of this unit.
// Included at the end of constructor_native_item_ai_update.cpp inside the anonymous namespace.
// Every statement is annotated with the original swkotor.exe instruction range.
// Original-address constants are bound to their proven .rdata values (read from swkotor.exe):
//   [0073D71C] = "" (CResRef literal)        [0073ECD0] = byte 0x00 (CSWSObject::GetGender)
//   [007441E0] = 0x0B (EVENT_DESTROY_OBJECT)  [007441DC] = 0x0A (event type)
//   [00746633] = byte 4 (object-type bound)   [0074666C] = byte 0 (GetGameObject success)
//   [00746644] = 0x7F000000 (OBJECT_INVALID)  [007455BC] = 900.0f (floaty range squared)
//   [0074C394] = 0, [0074C39C] = 2 (CSWSDoor walkmesh slot bounds)

// ===================================================================================== trivial slots
// 006FB8DE _purecall: typed slot adapter to the existing owner packed_purecall_006fb8de (never returns).
void __fastcall body_purecall_006fb8de(void*,void*){packed_purecall_006fb8de();}
// 00406D80 CResRef::CResRef(char const*): existing body menu_resources::Ref::construct_00406d80 (0030).
void* menu_resources_ref_construct_00406d80(void*out,const char*text){return static_cast<menu_resources::Ref*>(out)->construct_00406d80(text);}

// ===================================================================================== CSWSObject
// 004CAEE0 CSWSObject::GetDamageImmunity(uchar type): thiscall RET 4 -> int clamped to [-100,100].
int __fastcall body_004caee0(void*s,void*,unsigned type){
 M m;
 const signed char v=static_cast<signed char>(m.read8((type&255)+m.read(word(s)+0x1ac))); // 004CAEE0..AEEB
 if(v>=100)return 100;                                                                      // 004CAEEE..AEF0, AF09
 if(static_cast<int>(v)<-100)return -100;                                                   // 004CAEF2..AEFF
 return v;                                                                                  // 004CAF02..AF0E
}
// 004CAF70 CSWSObject::GetDamageImmunityByFlags(ushort flags): thiscall RET 4 -> int.
// Keeps the minimum signed immunity over the flagged types; a zero running value takes the next
// flagged value unconditionally (original quirk). Slot +0xB0 (GetDamageImmunity) is re-called on
// improvement exactly like the original (two calls per improving type).
int __fastcall body_004caf70(void*s,void*,unsigned flags){
 M m;using F=unsigned(__thiscall*)(void*,unsigned);
 short best=0;                                                                              // 004CAF78 ebx=0
 for(unsigned i=0;i<15;++i){                                                                // 004CAF7C, AFB4..AFB8
  if(!((static_cast<unsigned short>(flags)>>i)&1))continue;                                 // 004CAF80..AF8A
  if(best!=0){                                                                              // 004CAF8C..AF92
   const short v=static_cast<signed char>(reinterpret_cast<F>(m.read(m.read(word(s))+0xb0))(s,i)); // 004CAF94..AF9C
   if(v>=best)continue;                                                                     // 004CAFA0..AFA3
  }
  best=static_cast<signed char>(reinterpret_cast<F>(m.read(m.read(word(s))+0xb0))(s,i));   // 004CAFA5..AFB0
 }
 if(best>=100)return 100;                                                                   // 004CAFBA..AFBE, AFD4
 if(static_cast<int>(best)<-100)return -100;                                                // 004CAFC0..AFD1
 return best;                                                                               // 004CAFD9..AFDD
}
// 004CB190 CSWSObject::GetGender: mov al,[0073ECD0] (.rdata byte 0x00); ret.
unsigned char __fastcall body_004cb190(void*,void*){return 0;}
// 004CB160 CSWSObject::SetArea(CSWSArea*): thiscall RET 4.
void __fastcall body_004cb160(void*s,void*,void*area){
 M m;
 if(area)m.write(word(s)+0x8c,m.read(word(area)+0x120));                                   // 004CB160..B174
 else m.write(word(s)+0x8c,0x7f000000);                                                     // 004CB177..B183 [00746644]
}
// 004CC390 CSWSObject::ClearAction(CSWSObjectActionNode*, int): thiscall RET 8 -> 1.
// The `push 0` at 004CC3A4 is consumed by SetGlobalDialogState (GetInGameGui takes no argument).
unsigned __fastcall body_004cc390(void*s,void*,void*node,unsigned){
 M m;const U type=m.read(word(node));                                                       // 004CC390..C394
 if(type==0x18){                                                                            // 004CC396..C399
  void*gui=frame_R4_005ed690(ptr(m.read(word(kotor_menu_window_cells::manager_007a39fc)+4)),nullptr); // 004CC39B..C3A6
  kotor_fastcall_0062ec60(gui,nullptr,0);                                                   // 004CC3A4, C3AB..C3AD
  return 1;                                                                                 // 004CC3B2..C3B7
 }
 if(type==0x20)m.write(word(s)+0x50,0);                                                     // 004CC3BA..C3BF
 return 1;                                                                                  // 004CC3C6..C3CB
}
// 004CCF80 CSWSObject::DoDamage(int): thiscall RET 4.
void __fastcall body_004ccf80(void*s,void*,int damage){
 M m;using F=unsigned(__thiscall*)(void*,unsigned);
 const int hp=static_cast<short>(reinterpret_cast<F>(m.read(m.read(word(s))+0x9c))(s,1))-damage; // 004CCF83..CF9A
 m.write(word(s)+0xdc,static_cast<U>(hp));                                                  // 004CCF9E
 if(m.read(word(s)+0x200)&&hp<1)m.write(word(s)+0xdc,1);                                    // 004CCF91, CF9C..CFAB
}
// 004D0170 CSWSObject::GetDialogResref(CResRef* out): thiscall RET 4 -> out = CResRef("").
void* __fastcall body_004d0170(void*,void*,void*out){
 return menu_resources_ref_construct_00406d80(out,"");                                       // 004D0171..D184 -> 00406D80
}
// 004D01A0 CSWSObject::GetMaxHitPoints(int): mov ax,[ecx+0xE0]; ret 4.
unsigned short __fastcall body_004d01a0(void*s,void*,unsigned){M m;return static_cast<unsigned short>(m.read16(word(s)+0xe0));}
// 004D01B0 CSWSObject::GetLockOrientationToObject: mov eax,0x7F000000; ret.
unsigned __fastcall body_004d01b0(void*,void*){return 0x7f000000;}
// 004D01C0 CSWSObject::GetPortrait(CResRef* out): copies the 16-byte CResRef at +0x20; RET 4 -> out.
void* __fastcall body_004d01c0(void*s,void*,void*out){
 M m;for(U i=0;i<16;i+=4)m.write(word(out)+i,m.read(word(s)+0x20+i));                      // 004D01C0..D1DD
 return out;                                                                                // 004D01E1
}
// 004D09C0 CSWSObject scalar deleting destructor: thiscall(flags) RET 4 -> this.
void* __fastcall body_004d09c0(void*s,void*,unsigned flags){
 recovered_object_destroy_004d0220(s,nullptr);                                             // 004D09C3
 if(flags&1)live_free(s);                                                                   // 004D09C8..D9D5
 return s;                                                                                  // 004D09D8
}

// ===================================================================================== CGameObjectArray
// 004D7E90 CGameObjectArray::AddObjectAtPos(ulong id, CGameObject*): thiscall RET 8 -> 0/1/4 in AL.
// Original quirk kept: when the id already exists, the new node is freed, the existing node keeps its
// old object, and that displaced old object is re-added under the next free id (the counter value).
unsigned char __fastcall body_004d7e90(void*self,void*,unsigned id,void*object){
 M m;const U a=word(self);U obj=word(object);
 if(!obj)return 4;                                                                          // 004D7E90..7E9C, 7F97..7F9D
 U arg=id;                                                                                  // [esp+0x14]
 for(;;){
  const U top=(arg>>31)&1;                                                                  // 004D7EA6..7EAB ebx
  const U c=top&0xffff;                                                                     // 004D7EAE movzx ecx,bx
  const U esi=arg&0x7fffffff;                                                               // 004D7EB1, 7EBD
  const U bucket=(c<<12)+(arg&0xfff);                                                       // 004D7EB3..7EC3 edi
  if(esi&0x7f000000){                                                                       // 004D7EC5..7ECB
   if(esi<0x7f000001u||esi>0x7fffffffu)return 1;                                            // 004D7ECD..7EDF, 7FA0
   if(m.read(a+c*4+0xc)>=esi)m.write(a+c*4+0xc,esi-1);                                      // 004D7EE5..7EEE
   arg=m.read(a+c*4+0xc);                                                                   // 004D7EF2..7EF6
  }else{
   if(esi>0x7effffffu)return 1;                                                             // 004D7EFC..7F02
   if(m.read(a+c*4+4)<=esi)m.write(a+c*4+4,esi+1);                                          // 004D7F08..7F11
   arg=m.read(a+c*4+4);                                                                     // 004D7F15..7F19
  }
  const U node=word(live_operator_new(12));                                                 // 004D7F1D..7F26
  if(node){m.write(node,0);m.write(node+4,0);m.write(node+8,0);}                           // 004D7F29..7F35
  volatile U*const n=reinterpret_cast<volatile U*>(ptr(node));                              // failed allocation stores through null like the original
  n[0]=esi;n[1]=obj;n[2]=0;                                                                 // 004D7F3B..7F44
  const U slot=m.read(a)+bucket*4;                                                          // 004D7F47..7F4A
  U cur=m.read(slot);
  if(!cur){m.write(m.read(a)+bucket*4,node);return 0;}                                      // 004D7F4D..7F4F, 7FA9..7FB5
  U prev=0;                                                                                 // 004D7F51
  while(m.read(cur)>esi){                                                                   // 004D7F53..7F55
   prev=cur;cur=m.read(cur+8);                                                              // 004D7F57..7F59
   if(!cur){m.write(prev+8,node);return 0;}                                                 // 004D7F5C..7F69
  }
  if(m.read(cur)!=esi){                                                                     // 004D7F6C
   if(!prev){m.write(node+8,m.read(m.read(a)+bucket*4));m.write(m.read(a)+bucket*4,node);return 0;} // 004D7FB8..7FD1
   m.write(node+8,m.read(prev+8));m.write(prev+8,node);return 0;                            // 004D7FD4..7FE3
  }
  const U displaced=m.read(cur+4);                                                          // 004D7F6E
  live_free(ptr(node));                                                                     // 004D7F71..7F77
  if(top&0xffff)arg|=0x80000000;                                                            // 004D7F7A..7F7F
  obj=displaced;                                                                            // 004D7F8D
  if(!displaced)return 4;                                                                   // 004D7F87..7F91, 7F97..7F9D
 }
}

// 0063D520 CSWCObject::AttachmentFromServerObject(CSWSObject*): [ecx+0xF8]=arg; RET 4.
void __fastcall body_0063d520(void*c,void*,void*server){M m;m.write(word(c)+0xf8,word(server));}
// 0063D530 CSWCObject::DetachFromServerObject: mov [ecx+0xF8],0; ret.
void __fastcall body_0063d530(void*c,void*){M m;m.write(word(c)+0xf8,0);}
// 0059D9E0 CSWSExpression::~CSWSExpression (thiscall, no arguments).
// Quirk kept: the trailing free of [this+4] (0059DA79..DA85) runs after the array was already freed and nulled
// (frees NULL), exactly as the original.
void __fastcall body_0059d9e0(void*s,void*){
 M m;const U a=word(s);
 if(m.read(a+0x14)){                                                                        // 0059D9FE..DA0D
  recovered_expression_list_delete_list_0059d920(ptr(m.read(a+0x14)),nullptr);             // 0059DA0F
  if(m.read(a+0x14))live_free(ptr(m.read(a+0x14)));                                         // 0059DA14..DA1C
 }
 for(U i=0;static_cast<int>(i)<static_cast<int>(m.read(a+8));++i){                          // 0059DA24..DA2C, DA4A..DA50
  const U text=m.read(m.read(a+4)+4*i);                                                     // 0059DA30..DA33
  if(text){frame_LogStringDtor_005e5c20(ptr(text),nullptr);live_free(ptr(text));}           // 0059DA36..DA42
 }
 if(m.read(a+4)){live_free(ptr(m.read(a+4)));m.write(a+4,0);m.write(a+0xc,0);}              // 0059DA53..DA67
 m.write(a+8,0);                                                                            // 0059DA6D
 frame_LogStringDtor_005e5c20(ptr(a+0x18),nullptr);                                         // 0059DA6A..DA74
 live_free(ptr(m.read(a+4)));m.write(a+4,0);                                                // 0059DA79..DA91
}

// ===================================================================================== CSWSCreature
// 004EB160 CSWSCreature::SetId(ulong): thiscall RET 4.
void __fastcall body_004eb160(void*s,void*,unsigned id){
 M m;const U old=m.read(word(s)+4);                                                         // 004EB171
 recovered_creature_add_visible_0051b370(s,nullptr,id,1,1,0,0);                             // 004EB167..B175
 m.write(word(s)+4,id);                                                                     // 004EB17D
 recovered_creature_remove_visible_00517960(s,nullptr,old);                                 // 004EB17A..B180
}
// 004EB6B0 CSWSCreature::ResetUpdateTimes(ulong day, ulong time): thiscall RET 8.
void __fastcall body_004eb6b0(void*s,void*,unsigned day,unsigned time){
 M m;const U a=word(s);
 m.write(a+0x350,day);m.write(a+0x354,time);                                                // 004EB6B8..B6BE
 m.write(a+0x388,day);m.write(a+0x38c,time);                                                // 004EB6C4..B6CA
 m.write(a+0xa8,day);m.write(a+0xac,time);                                                  // 004EB6D0..B6D6
}
// 005BE780 CSWSFaction::GetMemberList(int** out): [out]=[this]; return [this+4]; RET 4.
unsigned __fastcall body_005be780(void*f,void*,void*out){M m;m.write(word(out),m.read(word(f)));return m.read(word(f)+4);}
// 004EC610 CSWSCreature::BroadcastFloatyData(CSWCCMessageData*): thiscall RET 4.
void __fastcall body_004ec610(void*s,void*,void*data){
 M m;const U me=word(s);
 U members;                                                                                 // [esp+8] (the pushed ecx slot)
 (void)body_005be780(combat_reuse_faction_00513fc0(s,nullptr),nullptr,&members);            // 004EC613..C621
 for(U i=0;static_cast<int>(i)<static_cast<int>(m.read(word(combat_reuse_faction_00513fc0(s,nullptr))+4));++i){ // 004EC626..C634, C6E4..C6EF
  const U id=m.read(members+4*i);                                                           // 004EC640..C64A
  const U player=word(kotor_fastcall_004ae910(ptr(m.server()),nullptr,id));                 // 004EC644..C651
  if(!player)continue;                                                                      // 004EC656..C65A
  U object=word(kotor_fastcall_00561080(ptr(player),nullptr));                              // 004EC660..C662
  if(!object)continue;                                                                      // 004EC667..C669
  if(m.read(me+0x8c)!=m.read(object+0x8c))continue;                                         // 004EC66B..C677
  object=word(kotor_fastcall_00561080(ptr(player),nullptr));                                // 004EC679..C67B
  float ox,oy,oz,mx,my,mz;
  std::memcpy(&ox,ptr(object+0x90),4);std::memcpy(&oy,ptr(object+0x94),4);std::memcpy(&oz,ptr(object+0x98),4);
  std::memcpy(&mx,ptr(me+0x90),4);std::memcpy(&my,ptr(me+0x94),4);std::memcpy(&mz,ptr(me+0x98),4);
  const double dx=static_cast<double>(ox)-static_cast<double>(mx);                         // 004EC680..C686
  const double dy=static_cast<double>(oy)-static_cast<double>(my);                         // 004EC691..C694
  const double dz=static_cast<double>(oz)-static_cast<double>(mz);                         // 004EC69A..C69D
  double d=dz*dz;                                                                           // 004EC6A3..C6A5
  d=d+dy*dy;                                                                                // 004EC6A7..C6AB
  d=d+dx*dx;                                                                                // 004EC6AD..C6B1
  if(!(d<=900.0))continue;                                                                  // 004EC6B3..C6C4 fcomp [007455BC]; test ah,41; jp (greater or unordered skips)
  const U player_id=m.read(player+4);                                                       // 004EC6C6
  void*message=recovered_server_sws_message_004aedb0(ptr(m.server()),nullptr);             // 004EC6C9..C6D8
  (void)recovered_cc_message_00568570(message,nullptr,player_id,0xb,data,0);                // 004EC6D2..C6DF
 }
}
// 004F8290 CSWSCreature::GetDialogResref(CResRef* out): thiscall RET 4 -> out.
void* __fastcall body_004f8290(void*s,void*,void*out){
 M m;const U stats=m.read(word(s)+0xa74);                                                   // 004F8290..8299
 if(!stats)return menu_resources_ref_construct_00406d80(out,"");                            // 004F829B..82AE
 for(U i=0;i<16;i+=4)m.write(word(out)+i,m.read(stats+0x44+i));                             // 004F82B1..82CF
 return out;                                                                                // 004F82B8, 82D3
}
// 004F82E0 CSWSCreature::GetDialogInterruptable: stats ? [stats+0x54] : 0.
unsigned __fastcall body_004f82e0(void*s,void*){M m;const U stats=m.read(word(s)+0xa74);return stats?m.read(stats+0x54):0;}
// 004F82F0 CSWSCreature::GetGender: stats ? byte [stats+0x64] : jmp 004CB190.
unsigned char __fastcall body_004f82f0(void*s,void*){M m;const U stats=m.read(word(s)+0xa74);if(!stats)return body_004cb190(s,nullptr);return static_cast<unsigned char>(m.read8(stats+0x64));}
// 004F8310 CSWSCreature::GetFirstName: stats ? stats+0x34 : this+0x0C.
void* __fastcall body_004f8310(void*s,void*){M m;const U stats=m.read(word(s)+0xa74);return stats?ptr(stats+0x34):ptr(word(s)+0xc);}
// 004F8330 CSWSCreature::GetLastName: stats ? stats+0x3C : this+0x0C.
void* __fastcall body_004f8330(void*s,void*){M m;const U stats=m.read(word(s)+0xa74);return stats?ptr(stats+0x3c):ptr(word(s)+0xc);}
// 004F8360 CSWSCreature::SetEffectSpellId(ulong): [+0x8F0]=arg; RET 4 (EAX = arg).
unsigned __fastcall body_004f8360(void*s,void*,unsigned v){M m;m.write(word(s)+0x8f0,v);return v;}

// ===================================================================================== door / placeable / item / waypoint
// 0058A1B0 CSWSDoor::GetDialogResref(CResRef* out): copies the CResRef at +0x2A2; RET 4 -> out.
void* __fastcall body_0058a1b0(void*s,void*,void*out){M m;for(U i=0;i<16;i+=4)m.write(word(out)+i,m.read(word(s)+0x2a2+i));return out;}
// 0058A1E0 CSWSDoor::GetFirstName: lea eax,[ecx+0x39C].
void* __fastcall body_0058a1e0(void*s,void*){return ptr(word(s)+0x39c);}
// 00588AB0 CSWSPlaceable::GetDead (door/placeable table +0x94): [+0xF8] ? 0 : jmp 004CB810.
unsigned __fastcall body_00588ab0(void*s,void*){M m;if(m.read(word(s)+0xf8))return 0;return trigger_dead_004cb810(s,nullptr);}
// 00589190 CSWSPlaceable::DoDamage(int): [+0xF8] ? ret 4 : jmp 004CCF80.
void __fastcall body_00589190(void*s,void*,int damage){M m;if(!m.read(word(s)+0xf8))body_004ccf80(s,nullptr,damage);}
// 00553200 CSWSItem GetFirstName thunk: lea eax,[ecx+0x270].
void* __fastcall body_00553200(void*s,void*){return ptr(word(s)+0x270);}
// 005B4770 CSWItem scalar deleting destructor: [this]=CSWItem table 0074B2A0; free on flags&1; RET 4.
void* __fastcall body_005b4770(void*s,void*,unsigned flags){
 M m;m.write(word(s),word(item_table_0074b2a0));                                            // 005B4778
 if(flags&1)live_free(s);                                                                   // 005B4770, 477E..4781
 return s;                                                                                  // 005B4789
}
// 005C7F10 CSWSWaypoint::EventHandler(ulong event,ulong,void*,ulong,ulong): RET 0x14.
void __fastcall body_005c7f10(void*s,void*,unsigned event,unsigned,void*,unsigned,unsigned){
 M m;
 if(event!=0x0b)return;                                                                     // 005C7F10..7F1A [007441E0]=0x0B
 if(!m.read(word(s)+0xec))return;                                                           // 005C7F1C..7F24
 using F=void*(__thiscall*)(void*,unsigned);reinterpret_cast<F>(m.read(m.read(word(s))))(s,1); // 005C7F26..7F2A scalar delete
}

// ===================================================================================== area removal cascade
// 0058FEC0 CSWArea::RemoveStaticBoundingBox(ulong id): thiscall RET 4 -> 0/1.
U body_0058fec0(U area,U id){
 M m;const U n=m.read(area+0x48);                                                           // 004FEC2 esi
 int i=0;
 if(static_cast<int>(n)<=0)return 0;                                                        // 0058FEC7..FECA
 for(;;){                                                                                   // 0058FED3..FEE0
  if(m.read(m.read(area+0x50)+4*static_cast<U>(i))==id)break;
  ++i;if(i>=static_cast<int>(m.read(area+0x48)))return 0;
 }
 if(i<0||i>=static_cast<int>(n))return 0;                                                   // 0058FEEA..FEF0
 m.write(area+0x48,n-1);                                                                    // 0058FEF2..FEF7
 if(i<static_cast<int>(n-1)){                                                               // 0058FEFA
  U off=static_cast<U>(i)*24;                                                               // 0058FEFC..FEFF
  do{
   const U ids=m.read(area+0x50);m.write(ids+4*static_cast<U>(i),m.read(ids+4*static_cast<U>(i)+4)); // 0058FF02..FF09
   U box=m.read(area+0x4c)+off;for(U k=0;k<12;k+=4)m.write(box+k,m.read(box+0x18+k));      // 0058FF0F..FF24
   box=m.read(area+0x4c)+off;for(U k=0;k<12;k+=4)m.write(box+0xc+k,m.read(box+0x24+k));    // 0058FF27..FF3F
   ++i;off+=0x18;                                                                           // 0058FF45..FF46
  }while(i<static_cast<int>(m.read(area+0x48)));                                            // 0058FF42, FF49..FF4B
 }
 return 1;                                                                                  // 0058FF4F
}
// 00507340 CSWSArea::DecreaseAILevelPriority: thiscall, no arguments.
void body_00507340(U area){
 M m;const U objects=m.read(m.read(m.server()+4)+0x1005c);                                 // 00507341..734F 004AED70
 void*const ai=recovered_server_ai_master_004aed80(ptr(m.server()),nullptr);               // 00507354..735F
 U found=0;                                                                                 // [esp+0x10] (the pushed ecx slot)
 for(U i=0;static_cast<int>(i)<static_cast<int>(m.read(area+0x194));++i){                  // 00507366..7370, 73BC..73C5
  const U id=m.read(m.read(area+0x190)+4*i);                                                // 00507372..7378
  if(kotor_fastcall_004d8230(ptr(objects),nullptr,id,reinterpret_cast<void**>(&found))!=0)continue; // 0050737B..738E [0074666C]=0
  if(static_cast<unsigned char>(m.read8(found+8))<=4)continue;                              // 00507390..739D [00746633]=4
  using F=void*(__thiscall*)(void*);
  if(!reinterpret_cast<F>(m.read(m.read(found)+0x30))(ptr(found)))continue;                // 0050739F..73A6
  if(m.read(found+0x78)!=1)continue;                                                        // 005073A8..73B0
  recovered_ai_level_004b08a0(ai,nullptr,ptr(found),0);                                     // 005073B2..73B7
 }
}
// 0050DC40 CSWSArea::RemoveObjectFromArea(ulong id): thiscall RET 4 -> 1.
U body_0050dc40(U area,U id){
 M m;
 U count=0;const U n=m.read(area+0x194);                                                    // 0050DC59..DC6B
 if(static_cast<int>(n)>0){const U data=m.read(area+0x190);for(U k=0;k<n;++k)if(m.read(data+4*k)==id)++count;} // 0050DC6D..DC7C
 if(count)recovered_int_list_remove_0066dcc0(ptr(area+0x190),nullptr,id);                  // 0050DC7E..DC83
 const U objects=m.read(m.read(m.server()+4)+0x1005c);                                     // 0050DC88..DC90 004AED70
 U object=id;                                                                               // [esp+0x20] (GetGameObject out reuses the argument slot)
 if(kotor_fastcall_004d8230(ptr(objects),nullptr,id,reinterpret_cast<void**>(&object))!=0)return 1; // 0050DC95..DCA8
 if(!object)return 1;                                                                       // 0050DCAE..DCB4
 using F=U(__thiscall*)(void*);
 if(static_cast<unsigned char>(m.read8(object+8))>4){                                       // 0050DCBA..DCC3
  if(reinterpret_cast<F>(m.read(m.read(object)+0x94))(ptr(object)))return 1;               // 0050DCC5..DCCF
 }
 if(!reinterpret_cast<F>(m.read(m.read(object)+0x30))(ptr(object)))return 1;               // 0050DCD5..DCE0
 if(kotor_fastcall_004ae910(ptr(m.server()),nullptr,id)){                                   // 0050DCE2..DCF2
  const U left=m.read(area+0x128)-1;m.write(area+0x128,left);                               // 0050DCF4
  if(!left)body_00507340(area);                                                             // 0050DCFA..DCFE
 }
 U event=word(live_operator_new(0x34));                                                     // 0050DD03..DD0A
 if(event)event=word(recovered_event_construct_004d7540(ptr(event),nullptr));               // 0050DD11..DD24
 *reinterpret_cast<volatile unsigned short*>(ptr(event))=0xd;                               // 0050DD29 (null store kept)
 const U area_id=m.read(area+0x120);                                                        // 0050DD33
 void*const ai=recovered_server_ai_master_004aed80(ptr(m.server()),nullptr);               // 0050DD39..DD51
 recovered_event_enqueue_004b08d0(ai,nullptr,0,0,id,area_id,0x0a,ptr(event));              // 0050DD28..DD58 [007441DC]=0x0A
 return 1;                                                                                  // 0050DD63
}
// Shared tail of every RemoveFromArea: SetArea(NULL); SetPosition(&zero,1,1,0).
void remove_from_area_tail(void*s){
 body_004cb160(s,nullptr,nullptr);                                                          // push 0; call 004CB160
 alignas(4) unsigned char zero[12];std::memset(zero,0,sizeof zero);                         // local vector = 0,0,0
 recovered_object_position_004cd1a0(s,nullptr,zero,1,1,0);                                  // push 0,1,1,&v; call 004CD1A0
}
U object_area(void*s){return word(recovered_object_area_004cb120(s,nullptr));}
// Count-then-remove of the object id in the area's +0x12C id list (trigger and door variants).
void remove_area_list_once(void*s){
 M m;const U id=m.read(word(s)+4);                                                          // 0058EE68 / 0058B4B2
 const U area=object_area(s);
 const U n=m.read(area+0x130);U count=0;
 if(static_cast<int>(n)>0){const U data=m.read(area+0x12c);for(U k=n,p=data;k;--k,p+=4)if(m.read(p)==id)++count;} // 0058EE72..EE8E
 if(count!=1)return;                                                                        // 0058EE90..EE93
 const U again=m.read(word(s)+4);                                                           // 0058EE95
 recovered_int_list_remove_0066dcc0(ptr(object_area(s)+0x12c),nullptr,again);              // 0058EE98..EEA6
}
// 005C8350 CSWSWaypoint::RemoveFromArea.
void body_005c8350(void*s){
 M m;
 if(object_area(s)){(void)body_0050dc40(object_area(s),m.read(word(s)+4));}                 // 005C8356..836C
 remove_from_area_tail(s);                                                                  // 005C8371..839F
}
// 0058EE30 CSWSTrigger::RemoveFromArea.
void body_0058ee30(void*s){
 M m;
 if(object_area(s)){                                                                        // 0058EE36..EE3D
  (void)body_0050dc40(object_area(s),m.read(word(s)+4));                                   // 0058EE3F..EE4D
  recovered_int_list_remove_0066dcc0(ptr(object_area(s)+0x1a0),nullptr,m.read(word(s)+4)); // 0058EE52..EE63
  remove_area_list_once(s);                                                                 // 0058EE68..EEA6
 }
 remove_from_area_tail(s);                                                                  // 0058EEAC..EEDA
}
// 00584950 CSWSPlaceable::RemoveFromArea.
void body_00584950(void*s){
 M m;
 if(object_area(s)){                                                                        // 00584956..495D
  if(static_cast<int>(m.read(word(s)+0x344))>=0){                                           // 0058495F..4967
   (void)body_0058fec0(object_area(s),m.read(word(s)+4));                                  // 00584969..4976
   m.write(word(s)+0x344,0xffffffff);                                                       // 0058497B
  }
  (void)body_0050dc40(object_area(s),m.read(word(s)+4));                                   // 00584985..4992
 }
 remove_from_area_tail(s);                                                                  // 00584997..49C5
}
// 0058B490 CSWSDoor::RemoveFromArea.
void body_0058b490(void*s){
 M m;
 if(object_area(s)){                                                                        // 0058B496..B49D
  (void)body_0050dc40(object_area(s),m.read(word(s)+4));                                   // 0058B49F..B4AD
  remove_area_list_once(s);                                                                 // 0058B4B2..B4F1
  (void)body_0058fec0(object_area(s),m.read(word(s)+4));                                   // 0058B4F6..B503
 }
 remove_from_area_tail(s);                                                                  // 0058B509..B537
}
// 005965D0 CSWCollisionMesh::~CSWCollisionMesh.  The leading vtable store ([this]=0074A068, and the
// 0074C32C / 0074C3A0 stores of the two surface-mesh wrappers 005CDAC0 / 005CE0D0) is a dead store on a
// dying object: no virtual call on `this` follows, and the rebuilt mesh tables are file-local to 0886.
void body_005965d0(U mesh){
 M m;
 if(m.read(mesh+0x4c)){                                                                     // 005965D3..65E1
  const U res=m.read(mesh+0x1c);                                                            // 005965E3
  m.write(mesh+0x54,0);m.write(mesh+0x60,0);m.write(mesh+0x64,0);m.write(mesh+0x6c,0);m.write(mesh+0x68,0); // 005965E6..65F2
  (void)c2da_release_demand_00409b80(ptr(res),nullptr);                                    // 005965F5
  if(!c2da_release_00409cf0(ptr(frame_control::resource_manager_007a39e8),nullptr,ptr(m.read(mesh+0x1c)))){ // 005965FA..660B
   const U r=m.read(mesh+0x1c);                                                             // 0059660D
   if(r){using F=void*(__thiscall*)(void*,unsigned);(void)reinterpret_cast<F>(m.read(m.read(r)))(ptr(r),1);} // 00596610..6618
  }
  m.write(mesh+0x1c,0);return;                                                              // 0059661A
 }
 static const U arrays[5]={0x54,0x60,0x64,0x68,0x6c};                                      // 00596620..665C
 for(U off:arrays){live_free(ptr(m.read(mesh+off)));m.write(mesh+off,0);}
}

// ===================================================================================== class destructors
// 005C8660 CSWSSoundObject::~CSWSSoundObject.
void body_005c8660(void*s){
 M m;m.write(word(s),word(sound_table_0074c0b8));                                           // 005C867D
 live_free(ptr(m.read(word(s)+0x25c)));m.write(word(s)+0x25c,0);                            // 005C8683..869C
 recovered_object_destroy_004d0220(s,nullptr);                                              // 005C86AE
}
// 005C84E0 CSWSWaypoint::~CSWSWaypoint.
void body_005c84e0(void*s){
 M m;m.write(word(s),word(waypoint_table_0074bf80));                                        // 005C84FD
 if(object_area(s))body_005c8350(s);                                                        // 005C850B..8516
 void*const module=server_get_module_004ae6b0(ptr(m.server()),nullptr);                    // 005C851B..8523
 if(module)recovered_module_remove_tag_004c5ef0(module,nullptr,ptr(word(s)+0x18),m.read(word(s)+4)); // 005C8528..8536
 exo_locstring_destroy_005ea000(ptr(word(s)+0x238),nullptr);                               // 005C853B..8546
 exo_locstring_destroy_005ea000(ptr(word(s)+0x230),nullptr);                               // 005C854B..8556
 recovered_object_destroy_004d0220(s,nullptr);                                              // 005C855B..8565
}
// Common: GetServerAIMaster()->RemoveObject(this); module->RemoveObjectFromLookupTable(tag,id).
void destroy_ai_and_tag(void*s){
 M m;
 (void)recovered_ai_remove_004af3d0(recovered_server_ai_master_004aed80(ptr(m.server()),nullptr),nullptr,s); // 0058EF48..EF58
 void*const module=server_get_module_004ae6b0(ptr(m.server()),nullptr);                    // 0058EF5D..EF66
 if(module)recovered_module_remove_tag_004c5ef0(module,nullptr,ptr(word(s)+0x18),m.read(word(s)+4)); // 0058EF6B..EF79
}
// 0058EEF0 CSWSTrigger::~CSWSTrigger.
void body_0058eef0(void*s){
 M m;const U a=word(s);
 m.write(a,word(trigger_table_00749a38));                                                   // 0058EF0D
 live_free(ptr(m.read(a+0x288)));                                                           // 0058EF13..EF22
 live_free(ptr(m.read(a+0x298)));                                                           // 0058EF27..EF2E
 if(object_area(s))body_0058ee30(s);                                                        // 0058EF36..EF43
 destroy_ai_and_tag(s);                                                                     // 0058EF48..EF79
 exo_locstring_destroy_005ea000(ptr(a+0x30c),nullptr);                                     // 0058EF7E..EF89
 live_free(ptr(m.read(a+0x2a8)));m.write(a+0x2a8,0);                                        // 0058EF8E..EFA8
 frame_LogStringDtor_005e5c20(ptr(a+0x27c),nullptr);                                        // 0058EFA2..EFB7
 c2da_destroy_array_006fa852(ptr(a+0x244),8,7,&local_record_destroy);                       // 0058EFBC..EFD1 (element dtor 005E5C20)
 frame_LogStringDtor_005e5c20(ptr(a+0x238),nullptr);                                        // 0058EFD6..EFE1
 frame_LogStringDtor_005e5c20(ptr(a+0x230),nullptr);                                        // 0058EFE6..EFF1
 exo_locstring_destroy_005ea000(ptr(a+0x228),nullptr);                                     // 0058EFF6..F001
 recovered_object_destroy_004d0220(s,nullptr);                                              // 0058F006..F010
}
// 005854B0 CSWSPlaceable::~CSWSPlaceable.
void body_005854b0(void*s){
 M m;const U a=word(s);
 m.write(a,word(placeable_table_007494d0));                                                 // 005854CE
 const U repository=m.read(a+0x36c);                                                        // 005854D4
 if(repository){kotor_fastcall_00555de0(ptr(repository),nullptr);live_free(ptr(repository));m.write(a+0x36c,0);} // 005854DA..54F6
 if(object_area(s))body_00584950(s);                                                        // 00585500..550D
 destroy_ai_and_tag(s);                                                                     // 00585512..5543
 if(m.read(a+0x440)){                                                                       // 00585548..5550
  const U client=m.read(word(kotor_menu_window_cells::manager_007a39fc)+4);                // 00585558..555E
  const U cid=kotor_fastcall_005eda50(ptr(client),nullptr,m.read(a+0x448));                 // 00585552..5562
  const U creature=word(frame_R4_005ed5b0(ptr(m.read(word(kotor_menu_window_cells::manager_007a39fc)+4)),nullptr,cid)); // 00585567..5571
  if(creature)m.write(creature+0x3dc,0x7f000000);                                           // 00585576..557A
 }
 body_005965d0(a+0x3a0);                                                                    // 00585584..558F 005CDAC0 (-> 005965D0)
 live_free(ptr(m.read(a+0x318)));                                                           // 00585594..55A0
 m.write(a+0x318,0);                                                                        // 005855B8
 c2da_destroy_array_006fa852(ptr(a+0x294),8,0x10,&local_record_destroy);                    // 005855A8..55C7 (element dtor 005E5C20)
 frame_LogStringDtor_005e5c20(ptr(a+0x264),nullptr);                                        // 005855CC..55D7
 exo_locstring_destroy_005ea000(ptr(a+0x234),nullptr);                                     // 005855DC..55E7
 exo_locstring_destroy_005ea000(ptr(a+0x228),nullptr);                                     // 005855EC..55F7
 recovered_object_destroy_004d0220(s,nullptr);                                              // 005855FC..5606
}
// 0058B550 CSWSDoor::~CSWSDoor.
void body_0058b550(void*s){
 M m;const U a=word(s);
 m.write(a,word(door_table_007498b0));                                                      // 0058B56F
 if(object_area(s))body_0058b490(s);                                                        // 0058B57D..B588
 destroy_ai_and_tag(s);                                                                     // 0058B58D..B5BE
 for(int i=0;i<=2;++i){                                                                     // 0058B5C3..B609 [0074C394]=0 .. [0074C39C]=2 (signed)
  const U slot=a+0x32c+static_cast<U>(i)*4;const U mesh=m.read(slot);                      // 0058B5D2, B5E0
  if(mesh){body_005965d0(mesh);live_free(ptr(mesh));m.write(slot,0);}                       // 0058B5E3..B5F7 005CE0D0 (-> 005965D0)
 }
 m.write(a+0x380,0);                                                                        // 0058B60C..B614
 exo_locstring_destroy_005ea000(ptr(a+0x3c8),nullptr);                                     // 0058B60E..B61F
 exo_locstring_destroy_005ea000(ptr(a+0x3a4),nullptr);                                     // 0058B624..B62F
 exo_locstring_destroy_005ea000(ptr(a+0x39c),nullptr);                                     // 0058B634..B63F
 frame_LogStringDtor_005e5c20(ptr(a+0x390),nullptr);                                        // 0058B644..B64F
 frame_LogStringDtor_005e5c20(ptr(a+0x388),nullptr);                                        // 0058B654..B65F
 live_free(ptr(m.read(a+0x2dc)));m.write(a+0x2dc,0);                                        // 0058B664..B67E
 frame_LogStringDtor_005e5c20(ptr(a+0x2d0),nullptr);                                        // 0058B678..B689
 c2da_destroy_array_006fa852(ptr(a+0x228),8,0xf,&local_record_destroy);                     // 0058B68E..B6A3 (element dtor 005E5C20)
 recovered_object_destroy_004d0220(s,nullptr);                                              // 0058B6A8..B6B2
}
// Scalar deleting destructors: call the class destructor, free on flags&1, return this; RET 4.
void* __fastcall body_0058f690(void*s,void*,unsigned flags){body_0058eef0(s);if(flags&1)live_free(s);return s;} // 0058F690..F6AB
void* __fastcall body_005c8630(void*s,void*,unsigned flags){body_005c84e0(s);if(flags&1)live_free(s);return s;} // 005C8630..864B
void* __fastcall body_005c9020(void*s,void*,unsigned flags){body_005c8660(s);if(flags&1)live_free(s);return s;} // 005C9020..903B
void* __fastcall body_00587a50(void*s,void*,unsigned flags){body_005854b0(s);if(flags&1)live_free(s);return s;} // 00587A50..7A6B
void* __fastcall body_0058c810(void*s,void*,unsigned flags){body_0058b550(s);if(flags&1)live_free(s);return s;} // 0058C810..C82B
