#include "aec3b5f554-strlen.h"
static_assert(sizeof(void*)==4,"original x86 address arithmetic");
static uint32_t load_word(uint32_t address) {
 uint32_t value;
 // Explicit single DWORD read avoids C++ effective-type/alignment assumptions.
 __asm { mov eax,address
         mov eax,[eax]
         mov value,eax }
 return value;
}
extern "C" uint32_t __cdecl reconstructed_strlen_00701300(const char* text) {
 const uint32_t begin=reinterpret_cast<uint32_t>(text);uint32_t cursor=begin;
 while(cursor&3u){
  const unsigned char byte=*reinterpret_cast<volatile const unsigned char*>(cursor);++cursor;
  if(!byte)return cursor-1u-begin;
 }
 for(;;){
  const uint32_t word=load_word(cursor);
  const uint32_t candidate=(~word ^ (word+0x7efefeffu))&0x81010100u;
  cursor+=4;
  if(!candidate)continue;
  // Original rereads the DWORD on a mask hit, including false positives.
  const uint32_t check=load_word(cursor-4u);
  if(!(check&0xffu))return cursor-4u-begin;
  if(!(check&0xff00u))return cursor-3u-begin;
  if(!(check&0xff0000u))return cursor-2u-begin;
  if(!(check&0xff000000u))return cursor-1u-begin;
 }
}
