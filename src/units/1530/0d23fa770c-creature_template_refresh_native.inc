extern "C" void __fastcall kotor_fastcall_005cec90(void*,void*); // 005CEC90 CPathfindInformation::ComputeStepTolerance RET 0 (claude_w5_movement_actions)
struct NativeCreatureTemplateRefresh:CreatureTemplateRefreshPorts{NativeMemory&m;explicit NativeCreatureTemplateRefresh(NativeMemory&v):m(v){}Word table_float(Word a,Word b,Word c,Word d)override{return static_cast<Word>(kotor_m15b_c2da_get_float_entry_00413350(ptr(a),nullptr,static_cast<int>(b),static_cast<int>(c),static_cast<float*>(ptr(d))));}void collision_refresh(Word a)override{kotor_fastcall_005cec90(ptr(a),nullptr);}void float_copy(Word a,Word b)override{void*dst=ptr(a);void*src=ptr(b);__asm {mov eax,src
 fld dword ptr [eax]
 mov ecx,dst
 fstp dword ptr [ecx]}}
};
void native_creature_template_refresh(Word a){NativeMemory m;NativeCreatureTemplateRefresh p(m);unsigned scratch;creature_template_refresh_004ed6e0(m,p,a,word(&scratch));}
