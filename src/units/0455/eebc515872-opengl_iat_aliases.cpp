// Reference views over genuine selected OPENGL32 loader IAT cells. No backing
// function-pointer cells, function bodies, value copies or runtime setter.
#include "15d4c2890e-canonical_views.h"
extern "C" {
extern decltype(&::glDisable) kotor_gui_import_glDisable;
extern decltype(&::glBegin) kotor_gui_import_glBegin;
extern decltype(&::glColor4ubv) kotor_gui_import_glColor4ubv;
extern decltype(&::glTexCoord2f) kotor_gui_import_glTexCoord2f;
extern decltype(&::glVertex4f) kotor_gui_import_glVertex4f;
extern decltype(&::glEnd) kotor_gui_import_glEnd;
extern decltype(&::glEnable) kotor_gui_import_glEnable;
}




#pragma comment(linker,"/alternatename:_kotor_gui_import_glDisable=__imp__glDisable@4")

#pragma comment(linker,"/alternatename:_kotor_gui_import_glBegin=__imp__glBegin@4")
#pragma comment(linker,"/alternatename:_kotor_gui_import_glColor4ubv=__imp__glColor4ubv@4")
#pragma comment(linker,"/alternatename:_kotor_gui_import_glTexCoord2f=__imp__glTexCoord2f@8")
#pragma comment(linker,"/alternatename:_kotor_gui_import_glVertex4f=__imp__glVertex4f@16")
#pragma comment(linker,"/alternatename:_kotor_gui_import_glEnd=__imp__glEnd@0")
#pragma comment(linker,"/alternatename:_kotor_gui_import_glEnable=__imp__glEnable@4")
namespace kotor_gui_canonical_required {
decltype(&::glDisable) const volatile& disable_0073d22c=kotor_gui_import_glDisable;
decltype(&::glBegin) const volatile& begin_0073d2d0=kotor_gui_import_glBegin;
decltype(&::glColor4ubv) const volatile& color_0073d2e8=kotor_gui_import_glColor4ubv;
decltype(&::glTexCoord2f) const volatile& texcoord_0073d2cc=kotor_gui_import_glTexCoord2f;
decltype(&::glVertex4f) const volatile& vertex_0073d324=kotor_gui_import_glVertex4f;
decltype(&::glEnd) const volatile& end_0073d2c4=kotor_gui_import_glEnd;
decltype(&::glEnable) const volatile& enable_0073d390=kotor_gui_import_glEnable;
}
