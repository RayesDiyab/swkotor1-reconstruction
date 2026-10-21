#include "04a491e27d-font_info.hpp"
#include <new>



namespace font_info_producer {



FontInfo* __fastcall construct_004221c0(void* memory,void*){
 FontInfo* self=::new(memory)FontInfo;
 font_info_zero_word(*self,7U);font_info_zero_word(*self,8U);

 font_info_zero_word(*self,6U);

 font_info_zero_word(*self,10U);

 font_info_zero_word(*self,11U);

 font_info_zero_word(*self,9U);

 font_info_zero_word(*self,0U);

 font_info_zero_word(*self,1U);font_info_zero_word(*self,2U);font_info_zero_word(*self,3U);font_info_zero_word(*self,4U);font_info_zero_word(*self,5U);



 return self;

}

}
