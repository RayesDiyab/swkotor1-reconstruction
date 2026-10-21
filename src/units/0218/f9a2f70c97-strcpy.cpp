#include "f73fb8f2a5-strcpy.h"
#include <stdint.h>
static_assert(sizeof(void*)==4,"x86 memory access contract");
static uint32_t load32(uint32_t address){uint32_t value;
 __asm {mov eax,address
 mov eax,[eax]
 mov value,eax}
 return value;
}
static void store32(uint32_t address,uint32_t value){
 __asm {mov eax,address
 mov edx,value
 mov [eax],edx}
}
static void store16(uint32_t address,uint32_t value){
 __asm {mov eax,address
 mov edx,value
 mov word ptr [eax],dx}
}
extern "C" char* __cdecl reconstructed_strcpy_00705910(char* destination,const char* source){
 uint32_t in=reinterpret_cast<uint32_t>(source),out=reinterpret_cast<uint32_t>(destination);
 while(in&3u){
  const unsigned char byte=*reinterpret_cast<volatile const unsigned char*>(in);++in;
  *reinterpret_cast<volatile unsigned char*>(out)=byte;
  if(!byte)return destination;
  ++out;
 }
 for(;;){
  const uint32_t first=load32(in);
  const uint32_t candidate=(~first^(first+0x7efefeffu))&0x81010100u;
  const uint32_t word=load32(in); // Original second read occurs on EVERY word.
  in+=4;
  if(candidate){
   if(!(word&0xffu)){*reinterpret_cast<volatile unsigned char*>(out)=static_cast<unsigned char>(word);return destination;}
   if(!(word&0xff00u)){store16(out,word);return destination;}
   if(!(word&0xff0000u)){store16(out,word);*reinterpret_cast<volatile unsigned char*>(out+2)=0;return destination;}
   if(!(word&0xff000000u)){store32(out,word);return destination;}
  }
  store32(out,word);out+=4;
 }
}
