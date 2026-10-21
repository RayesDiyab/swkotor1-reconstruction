#include "4ff7034f3a-mainloop.h"
namespace callable_frame {
struct StringLease {
 frame_log::String object;
 explicit StringLease(const char* text){frame_LogStringCtor_005e5a90(&object,nullptr,text);}
 ~StringLease() noexcept {frame_LogStringDtor_005e5c20(&object,nullptr);}
};
struct FileLease {
 frame_log::File object;bool constructed=false;
 void construct(frame_log::String* filename,frame_log::String* mode){frame_LogFileCtor_005e68d0(&object,nullptr,filename,14,mode);constructed=true;}
 ~FileLease() noexcept {if(constructed)frame_LogFileDtor_005e69e0(&object,nullptr);}
};
void active_log_00603fe3(){
 FileLease file;
 {
  StringLease mode(frame_log::empty_0073d5d4);StringLease filename(frame_log::filename_0074edcc);
  file.construct(&filename.object,&mode.object);
 } // exact successful order:filename then mode, before FileOpened.
 auto* log=reinterpret_cast<frame_log::String*>(frame_control::log_request_008328d8);
 if(frame_LogFileOpened_005e6a10(&file.object,nullptr)!=0){
  if(frame_LogFileSize_005e6950(&file.object,nullptr)==0){frame_LogWriteText_005e6a40(&file.object,nullptr,frame_log::header_0074ed88);frame_LogWriteText_005e6a40(&file.object,nullptr,frame_log::header_0074ed30);}
  char date[9];frame_LogDate_006fe4a7(date);
  const char* text=frame_LogCStr_005e5670(log,nullptr);
  frame_LogFormat_005e5680(log,frame_log::format_0074ed28,date,text);
  frame_LogWriteString_005e6a70(&file.object,nullptr,log);

  frame_LogAssign_005e5140(log,nullptr,frame_log::clear_0073d71c);

 }else frame_LogFormat_005e5680(log,frame_log::error_0074ed00);
}
}
