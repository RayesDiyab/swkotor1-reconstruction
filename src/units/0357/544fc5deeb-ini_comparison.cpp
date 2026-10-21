#include "66d32dd555-ini_comparison.h"
#include "5408f4c951-compare.h"
namespace ini_comparison {
std::int32_t __fastcall CompareNC005e58c0(
    kotor_reconstruction::StartupString* self,void*,const char* input) {
    const char* stored=self->data; // Original loads this volatile pointer once.
    if(!input)return stored==nullptr?1:0;
    if(!stored)return 0;
    return kotor_stricmp_0070acaf(input,stored)==0?1:0;
}
}
