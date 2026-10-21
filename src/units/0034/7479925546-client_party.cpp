// Original CSWCMessage Party handler0064F1A0..0064F25E, thiscall RET4.
// All subtype branches preserved; selected W2 readers and0457 lookup are reused.
#include <cstdint>
#include <cstring>
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
unsigned __fastcall kotor_fastcall_004d6230(void*,void*,unsigned);
unsigned __fastcall kotor_fastcall_004d6780(void*,void*,unsigned);
unsigned __fastcall kotor_fastcall_00692630(void*,void*);
unsigned char __fastcall kotor_fastcall_004d8230(void*,void*,unsigned,void**);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long,const char*);
}
namespace {
static_assert(sizeof(void*)==4,"original x86 layout");
void* pointer(const void* object,unsigned offset) {
 void* value;std::memcpy(&value,static_cast<const unsigned char*>(object)+offset,4);return value;
}
std::int32_t count(void* self) {
 unsigned value=kotor_fastcall_004d6780(self,nullptr,32);
 std::int32_t result;std::memcpy(&result,&value,4);return result;
}
}
extern "C" unsigned __fastcall recovered_client_party_0064f1a0(void* self,void*,unsigned subtype) {
 if(kotor_fastcall_004d6230(self,nullptr,1))return 0;                        //0064F1A1..B1
 switch(static_cast<unsigned char>(subtype)) {                          //0064F1B4; byte-index table0064F270
 case 9:
  (void)kotor_fastcall_00692630(self,nullptr);return 1;                    //0064F1D2..E0
 case 1:case 3: {
  void* array=pointer(pointer(pointer(kotor_menu_window_cells::manager_007a39fc,4),4),0x14); //0064F1E3..EF
  std::int32_t remaining=count(self);                                  //0064F1F2..FD; signed JLE
  while(remaining>0) {
   unsigned id=kotor_fastcall_00692630(self,nullptr);                    //0064F202..04
   void* object;
   if(kotor_fastcall_004d8230(array,nullptr,id,&object)==0) {             //0064F209..1C; original0074666C byte is0
    //0064F21E..22: the original reads the object vtable unchecked (a null object faults there)
    using F=void*(__thiscall*)(void*);                                  //creature2C=00641DB0 self; base2C=0063E7F0 zero
    reinterpret_cast<F>(pointer(pointer(object,0),0x2c))(object);        //0064F21E..24; ignored EAX
   }
   --remaining;                                                       //0064F227..28
  }
  return 1;                                                           //0064F22A..32 /0064F24E..55
 }
 case 4: {
  std::int32_t remaining=count(self);                                  //0064F235..42
  while(remaining>0) {(void)kotor_fastcall_00692630(self,nullptr);--remaining;} //0064F244..4C
  return 1;
 }
 default:return 0;                                                    //0064F258..5C
 }
}
