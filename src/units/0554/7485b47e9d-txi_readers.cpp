// TXI field readers 00463510/00463530/00463570/00463590/004635C0/004635E0/00463650/00463680 and the
// sscanf 006FAE08 provider (lane-font packet 6). Evidence: lane-gui-draw/exe-text.asm ranges
// 00463510..0046352D, 00463530..0046356C, 00463570..0046358F, 00463590..004635BB, 004635C0..004635D7,
// 004635E0..0046364D, 00463650..00463674, 00463680..00463707; strings 0073D5D0 "%s", 0073D720 "%d",
// 0073E2E4 "%f", 0073EE40 "%f %f %f", 00741E28 "true", 00741E20 "false", 00741E1C "1", 00741E18 "0",
// 00741E10 "TRUE", 00741E08 "FALSE".
#include "43e0af9c1e-font_txi.hpp"
#include <cstdarg>
#include <cstdio>

// 007FBAF8: firstword's static token buffer; 0x400 bytes up to the next referenced cell 007FBEF8.
extern "C" char kotor_firstword_buffer_007fbaf8[0x400]={};
// Original read-only format/literal cells (exact bytes incl. NUL).
extern "C" const char kotor_format_d_0073d720[]="%d";
namespace {
const char kFormatS[]="%s";            // 0073D5D0
const char kFormatF[]="%f";            // 0073E2E4
const char kFormatF3[]="%f %f %f";     // 0073EE40
}

// 006FAE08 sscanf. Lane snapshot: the UCRT vsscanf provider is removed; the single provider is now the faithful port
// of the original static-CRT sscanf / _input 00700858 in srcw5 actions/claude_snapshot_crt.cpp (kotor_sscanf_006fae08).
extern "C" int __cdecl kotor_sscanf_006fae08(const char* input,const char* format,...);
// 00463510: null -> null; else skip ' ' and '\t'. (Called with 7 when 00421FF0 hits EOF: original faults.)
extern "C" char* __cdecl skip_blanks_00463510(char* text){
 if(!text)return nullptr;
 while(*reinterpret_cast<volatile char*>(text)==' '||*reinterpret_cast<volatile char*>(text)=='\t')++text;
 return text;
}
// 00463530: copy first token (stop at NUL, ' ', '\t', '\n', '\r') into 007FBAF8, NUL-terminate, return it.
extern "C" char* __cdecl firstword_00463530(char* text){
 unsigned index=0;
 if(text){
  for(;;){char c=*text;if(!c||c==' '||c=='\t'||c=='\n'||c=='\r')break;kotor_firstword_buffer_007fbaf8[index++]=c;++text;}
 }
 kotor_firstword_buffer_007fbaf8[index]=0;   // 00463560
 return kotor_firstword_buffer_007fbaf8;     // 00463567
}
// 00463570: sscanf(text,"%f %f %f",v,v+1,v+2).
extern "C" void __cdecl parse_vector_00463570(char* text,volatile float* vector){
 kotor_sscanf_006fae08(text,kFormatF3,const_cast<float*>(vector),const_cast<float*>(vector+1),const_cast<float*>(vector+2));
}
// 00463590: sscanf(text,"%s",out); a zero result copies from address 0 (EAX) - dead for %s (it cannot
// match-fail without exhausting input, which yields EOF), transcribed for fidelity.
extern "C" void __cdecl parse_cstr_00463590(char* text,char* out){
 int result=kotor_sscanf_006fae08(text,kFormatS,out);
 if(result==0){
  const volatile char* from=reinterpret_cast<const volatile char*>(static_cast<std::uintptr_t>(result));
  char c;do{c=*from;out[reinterpret_cast<std::uintptr_t>(from)]=c;++from;}while(c);   // [eax+esi]=[eax]
 }
}
// 004635C0: sscanf(text,"%f",out).
extern "C" void __cdecl parse_float_004635c0(char* text,volatile float* out){
 kotor_sscanf_006fae08(text,kFormatF,const_cast<float*>(out));
}
// 004635E0: "%d"; only when sscanf returns exactly 0, read "%s" into a 64-byte local and map TRUE/FALSE.
extern "C" void __cdecl parse_int_004635e0(char* text,volatile std::int32_t* out){
 if(kotor_sscanf_006fae08(text,kotor_format_d_0073d720,const_cast<std::int32_t*>(out))!=0)return; // 004635FE
 char word[64];word[0]=0;                                                           // 00463600 (al==0)
 kotor_sscanf_006fae08(text,kFormatS,word);
 if(kotor_stricmp_0070acaf("TRUE",word)==0)*out=1;                                  // 0046362A
 if(kotor_stricmp_0070acaf("FALSE",word)==0)*out=0;                                 // 00463646 (eax==0)
}
// 00463650: push ecx reserves the int temp, so a failed "%d" stores the low word of the caller's ECX.
// Naked to keep that transport exactly (caller ECX is indeterminate in original and source alike).
extern "C" __declspec(naked) void __cdecl parse_short_00463650(char*,volatile std::int16_t*){
 __asm {
  push ecx
  mov ecx,dword ptr [esp+8]
  lea eax,[esp]
  push eax
  push offset kotor_format_d_0073d720
  push ecx
  call kotor_sscanf_006fae08
  mov edx,dword ptr [esp+18h]
  mov ax,word ptr [esp+0Ch]
  mov word ptr [edx],ax
  add esp,10h
  ret
 }
}
// 00463680: "%s" into an 8-byte local (unbounded, as original); true/false/1/0 (case-insensitive) in order.
extern "C" void __cdecl parse_bool_00463680(char* text,volatile bool* out){
 char word[8];word[0]=0;                                                            // 00463693
 kotor_sscanf_006fae08(text,kFormatS,word);
 volatile std::uint8_t* target=reinterpret_cast<volatile std::uint8_t*>(out);
 if(kotor_stricmp_0070acaf("true",word)==0)*target=1;                              // 004636B7
 if(kotor_stricmp_0070acaf("false",word)==0)*target=0;                             // 004636D0 (al==0)
 if(kotor_stricmp_0070acaf("1",word)==0)*target=1;                                 // 004636E8
 if(kotor_stricmp_0070acaf("0",word)==0)*target=0;                                 // 00463701
}
