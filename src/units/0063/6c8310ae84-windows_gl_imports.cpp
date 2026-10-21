// Genuine Win32/OpenGL adapters. Compile/link evidence only in this packet;
// this translation unit is excluded from every executed headless fixture.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <gl/GL.h>
#include "85474cc56b-renderer_initialize.h"
#include "ecf744e644-aa_probe.h"
#include "27a630ccc9-renderer_gl_imports.h"
#include "fad9ebe559-renderer_gl_imports_gen.h"
namespace kotor_renderer_init {
static_assert(sizeof(DEVMODEA)==156 && sizeof(PIXELFORMATDESCRIPTOR)==40 && sizeof(RECT)==16,"original Win32 layouts");
I32 __stdcall EnumDisplaySettingsA_0073d3e4(const char* name,U32 index,DisplayMode* mode){return ::EnumDisplaySettingsA(name,index,reinterpret_cast<DEVMODEA*>(mode));}
I32 __stdcall ChangeDisplaySettingsA_0073d3e8(DisplayMode* mode,U32 flags){return ::ChangeDisplaySettingsA(reinterpret_cast<DEVMODEA*>(mode),flags);}
I32 __stdcall SetWindowLongA_0073d3ec(void* window,I32 index,U32 value){return ::SetWindowLongA(static_cast<HWND>(window),index,static_cast<LONG>(value));}
I32 __stdcall AdjustWindowRect_0073d3e0(Rect* rect,U32 style,I32 menu){return ::AdjustWindowRect(reinterpret_cast<RECT*>(rect),style,menu);}
I32 __stdcall SetWindowPos_0073d40c(void* window,void* after,I32 x,I32 y,U32 width,U32 height,U32 flags){return ::SetWindowPos(static_cast<HWND>(window),static_cast<HWND>(after),x,y,static_cast<int>(width),static_cast<int>(height),flags);}
I32 __stdcall SendMessageA_0073d428(void* window,U32 message,U32 first,U32 second){return ::SendMessageA(static_cast<HWND>(window),message,first,static_cast<LPARAM>(second));}
I32 __stdcall ShowWindow_0073d400(void* window,I32 show){return ::ShowWindow(static_cast<HWND>(window),show);}
void* __stdcall GetDC_0073d408(void* window){return ::GetDC(static_cast<HWND>(window));}
I32 __stdcall SetPixelFormat_0073d014(void* dc,I32 format,const PixelFormat* pfd){return ::SetPixelFormat(static_cast<HDC>(dc),format,reinterpret_cast<const PIXELFORMATDESCRIPTOR*>(pfd));}
I32 __stdcall ChoosePixelFormat_0073d018(void* dc,const PixelFormat* pfd){return ::ChoosePixelFormat(static_cast<HDC>(dc),reinterpret_cast<const PIXELFORMATDESCRIPTOR*>(pfd));}
I32 __stdcall DescribePixelFormat_0073d008(void* dc,I32 format,U32 size,PixelFormat* pfd){return ::DescribePixelFormat(static_cast<HDC>(dc),format,size,reinterpret_cast<PIXELFORMATDESCRIPTOR*>(pfd));}
void* __stdcall wglCreateContext_0073d2b8(void* dc){return ::wglCreateContext(static_cast<HDC>(dc));}
I32 __stdcall wglMakeCurrent_0073d2b0(void* dc,void* context){return ::wglMakeCurrent(static_cast<HDC>(dc),static_cast<HGLRC>(context));}
I32 __stdcall wglDeleteContext_0073d30c(void* context){return ::wglDeleteContext(static_cast<HGLRC>(context));}
I32 __stdcall ReleaseDC_0073d404(void* window,void* dc){return ::ReleaseDC(static_cast<HWND>(window),static_cast<HDC>(dc));}
void __stdcall glEnable_0073d390(U32 value){::glEnable(value);}
void __stdcall glDisable_0073d22c(U32 value){::glDisable(value);}
// ---- Revision (lane-renderer-bodies): additional entries of this single owner.
// Each name suffix is the original IAT cell; every body calls that real import.
static_assert(sizeof(WNDCLASSA)==40 && offsetof(WNDCLASSA,hInstance)==16 && offsetof(WNDCLASSA,lpszClassName)==36,"WindowClass layout");
static_assert(sizeof(OSVERSIONINFOA)==148 && offsetof(OSVERSIONINFOA,dwMajorVersion)==4,"VersionInfo layout");
void* __stdcall GetStockObject_0073d01c(I32 object){return ::GetStockObject(object);}
U16 __stdcall RegisterClassA_0073d41c(const WindowClass* window_class){return ::RegisterClassA(reinterpret_cast<const WNDCLASSA*>(window_class));}
void* __stdcall CreateWindowExA_0073d410(U32 ex_style,const char* class_name,const char* title,U32 style,I32 x,I32 y,I32 width,I32 height,void* parent,void* menu,void* instance,void* parameter){
    return ::CreateWindowExA(ex_style,class_name,title,style,x,y,width,height,static_cast<HWND>(parent),static_cast<HMENU>(menu),static_cast<HINSTANCE>(instance),parameter);}
I32 __stdcall DestroyWindow_0073d3fc(void* window){return ::DestroyWindow(static_cast<HWND>(window));}
const char* __stdcall glGetString_0073d290(U32 name){return reinterpret_cast<const char*>(::glGetString(name));}
I32 __stdcall GetVersionExA_0073d060(VersionInfo* version){
#pragma warning(suppress:4996) // the original imports and calls GetVersionExA itself
    return ::GetVersionExA(reinterpret_cast<OSVERSIONINFOA*>(version));}
void __stdcall glGenTextures_0073d24c(I32 count,U32* names){::glGenTextures(count,names);}
void __stdcall glBindTexture_0073d240(U32 target,U32 name){::glBindTexture(target,name);}
void __stdcall glCopyTexImage2D_0073d2d8(U32 target,I32 level,U32 internal_format,I32 x,I32 y,U32 width,U32 height,I32 border){
    ::glCopyTexImage2D(target,level,internal_format,x,y,static_cast<GLsizei>(width),static_cast<GLsizei>(height),border);}
void __stdcall glTexParameteri_0073d23c(U32 target,U32 name,I32 value){::glTexParameteri(target,name,value);}
I32 __stdcall wglShareLists_0073d2b4(void* first,void* second){return ::wglShareLists(static_cast<HGLRC>(first),static_cast<HGLRC>(second));}
void __stdcall glGetFloatv_0073d218(U32 name,float* values){::glGetFloatv(name,values);}
I32 __stdcall DefWindowProcA_0073d3b0(void* window,U32 message,U32 first,I32 second){
    return static_cast<I32>(::DefWindowProcA(static_cast<HWND>(window),message,first,second));}
IatRawProcedure __stdcall wglGetProcAddress_0073d310(const char* name){
    // The only function-pointer conversion in this owner: wglGetProcAddress's PROC
    // result is an untyped procedure address by API contract (original 0042E040/
    // 00436490 store it raw into typed extension cells).
    return reinterpret_cast<IatRawProcedure>(::wglGetProcAddress(name));}
const IatWindowProcedure iat_DefWindowProcA_0073d3b0=&DefWindowProcA_0073d3b0;
const IatLoadProcedure iat_wglGetProcAddress_0073d310=&wglGetProcAddress_0073d310;
const IatStringProcedure iat_glGetString_0073d290=&glGetString_0073d290;
void (__stdcall* iat_glEnable_0073d390)(U32)=&glEnable_0073d390;
void (__stdcall* iat_glDisable_0073d22c)(U32)=&glDisable_0073d22c;
void (__stdcall* iat_glGenTextures_0073d24c)(I32,U32*)=&glGenTextures_0073d24c;
void (__stdcall* iat_glBindTexture_0073d240)(U32,U32)=&glBindTexture_0073d240;
void (__stdcall* iat_glTexParameteri_0073d23c)(U32,U32,I32)=&glTexParameteri_0073d23c;
void (__stdcall* iat_glTexImage2D_0073d280)(U32,I32,I32,I32,I32,I32,U32,U32,const void*)=&glTexImage2D_0073d280;
void (__stdcall* iat_glTexImage1D_0073d2f0)(U32,I32,I32,I32,I32,U32,U32,const void*)=&glTexImage1D_0073d2f0;
void (__stdcall* iat_glCopyTexImage2D_0073d2d8)(U32,I32,U32,I32,I32,U32,U32,I32)=&glCopyTexImage2D_0073d2d8;
I32 (__stdcall* iat_wglMakeCurrent_0073d2b0)(void*,void*)=&wglMakeCurrent_0073d2b0;
void* (__stdcall* iat_wglCreateContext_0073d2b8)(void*)=&wglCreateContext_0073d2b8;
I32 (__stdcall* iat_wglShareLists_0073d2b4)(void*,void*)=&wglShareLists_0073d2b4;
// ---- BEGIN GENERATED opengl32/glu32 adapters (tools/gen_gl_adapters.py)
void __stdcall glClear_0073d21c(U32 a0){::glClear(a0);}
void __stdcall glClearColor_0073d220(float a0, float a1, float a2, float a3){::glClearColor(a0, a1, a2, a3);}
void __stdcall glColor4f_0073d224(float a0, float a1, float a2, float a3){::glColor4f(a0, a1, a2, a3);}
void __stdcall glMatrixMode_0073d228(U32 a0){::glMatrixMode(a0);}
void __stdcall glLoadIdentity_0073d230(){::glLoadIdentity();}
void __stdcall glOrtho_0073d234(double a0, double a1, double a2, double a3, double a4, double a5){::glOrtho(a0, a1, a2, a3, a4, a5);}
void __stdcall glAlphaFunc_0073d238(U32 a0, float a1){::glAlphaFunc(a0, a1);}
void __stdcall glGetIntegerv_0073d244(U32 a0, I32* a1){::glGetIntegerv(a0, a1);}
void __stdcall glDeleteTextures_0073d248(I32 a0, const U32* a1){::glDeleteTextures(a0, a1);}
void __stdcall glReadPixels_0073d250(I32 a0, I32 a1, I32 a2, I32 a3, U32 a4, U32 a5, void* a6){::glReadPixels(a0, a1, a2, a3, a4, a5, a6);}
void __stdcall glFinish_0073d254(){::glFinish();}
void __stdcall glDrawElements_0073d258(U32 a0, I32 a1, U32 a2, const void* a3){::glDrawElements(a0, a1, a2, a3);}
void __stdcall glMultMatrixf_0073d25c(const float* a0){::glMultMatrixf(a0);}
void __stdcall glEnableClientState_0073d260(U32 a0){::glEnableClientState(a0);}
void __stdcall glDisableClientState_0073d264(U32 a0){::glDisableClientState(a0);}
void __stdcall glPushAttrib_0073d268(U32 a0){::glPushAttrib(a0);}
void __stdcall glPopAttrib_0073d26c(){::glPopAttrib();}
void __stdcall glStencilOp_0073d270(U32 a0, U32 a1, U32 a2){::glStencilOp(a0, a1, a2);}
void __stdcall glFrontFace_0073d274(U32 a0){::glFrontFace(a0);}
void __stdcall glStencilMask_0073d278(U32 a0){::glStencilMask(a0);}
U8 __stdcall glIsTexture_0073d27c(U32 a0){return ::glIsTexture(a0);}
void __stdcall glTexImage2D_0073d280(U32 a0, I32 a1, I32 a2, I32 a3, I32 a4, I32 a5, U32 a6, U32 a7, const void* a8){::glTexImage2D(a0, a1, a2, a3, a4, a5, a6, a7, a8);}
void __stdcall glTranslatef_0073d284(float a0, float a1, float a2){::glTranslatef(a0, a1, a2);}
void __stdcall glRotatef_0073d288(float a0, float a1, float a2, float a3){::glRotatef(a0, a1, a2, a3);}
void __stdcall glDepthMask_0073d28c(U8 a0){::glDepthMask(a0);}
void __stdcall glTexSubImage2D_0073d294(U32 a0, I32 a1, I32 a2, I32 a3, I32 a4, I32 a5, U32 a6, U32 a7, const void* a8){::glTexSubImage2D(a0, a1, a2, a3, a4, a5, a6, a7, a8);}
void __stdcall glStencilFunc_0073d298(U32 a0, I32 a1, U32 a2){::glStencilFunc(a0, a1, a2);}
void __stdcall glTexCoordPointer_0073d29c(I32 a0, U32 a1, I32 a2, const void* a3){::glTexCoordPointer(a0, a1, a2, a3);}
void __stdcall glColorPointer_0073d2a0(I32 a0, U32 a1, I32 a2, const void* a3){::glColorPointer(a0, a1, a2, a3);}
void __stdcall glNormalPointer_0073d2a4(U32 a0, I32 a1, const void* a2){::glNormalPointer(a0, a1, a2);}
void __stdcall glVertexPointer_0073d2a8(I32 a0, U32 a1, I32 a2, const void* a3){::glVertexPointer(a0, a1, a2, a3);}
void __stdcall glDrawArrays_0073d2ac(U32 a0, I32 a1, I32 a2){::glDrawArrays(a0, a1, a2);}
void __stdcall glViewport_0073d2bc(I32 a0, I32 a1, I32 a2, I32 a3){::glViewport(a0, a1, a2, a3);}
U32 __stdcall glGetError_0073d2c0(){return ::glGetError();}
void __stdcall glEnd_0073d2c4(){::glEnd();}
void __stdcall glVertex3f_0073d2c8(float a0, float a1, float a2){::glVertex3f(a0, a1, a2);}
void __stdcall glTexCoord2f_0073d2cc(float a0, float a1){::glTexCoord2f(a0, a1);}
void __stdcall glBegin_0073d2d0(U32 a0){::glBegin(a0);}
void __stdcall glNormal3f_0073d2d4(float a0, float a1, float a2){::glNormal3f(a0, a1, a2);}
void __stdcall glVertex3fv_0073d2e4(const float* a0){::glVertex3fv(a0);}
void __stdcall glColor4ubv_0073d2e8(const U8* a0){::glColor4ubv(a0);}
void __stdcall glColor3f_0073d2ec(float a0, float a1, float a2){::glColor3f(a0, a1, a2);}
void __stdcall glTexImage1D_0073d2f0(U32 a0, I32 a1, I32 a2, I32 a3, I32 a4, U32 a5, U32 a6, const void* a7){::glTexImage1D(a0, a1, a2, a3, a4, a5, a6, a7);}
void __stdcall glTexEnvi_0073d2f4(U32 a0, U32 a1, I32 a2){::glTexEnvi(a0, a1, a2);}
void __stdcall glFlush_0073d2f8(){::glFlush();}
void __stdcall glPopMatrix_0073d2fc(){::glPopMatrix();}
void __stdcall glPushMatrix_0073d300(){::glPushMatrix();}
void __stdcall glDrawBuffer_0073d304(U32 a0){::glDrawBuffer(a0);}
void __stdcall glCopyTexSubImage2D_0073d308(U32 a0, I32 a1, I32 a2, I32 a3, I32 a4, I32 a5, I32 a6, I32 a7){::glCopyTexSubImage2D(a0, a1, a2, a3, a4, a5, a6, a7);}
void __stdcall glTexGeni_0073d314(U32 a0, U32 a1, I32 a2){::glTexGeni(a0, a1, a2);}
void __stdcall glNormal3fv_0073d318(const float* a0){::glNormal3fv(a0);}
void __stdcall glCallLists_0073d31c(I32 a0, U32 a1, const void* a2){::glCallLists(a0, a1, a2);}
void __stdcall glListBase_0073d320(U32 a0){::glListBase(a0);}
void __stdcall glVertex4f_0073d324(float a0, float a1, float a2, float a3){::glVertex4f(a0, a1, a2, a3);}
void __stdcall glEndList_0073d328(){::glEndList();}
void __stdcall glBitmap_0073d32c(I32 a0, I32 a1, float a2, float a3, float a4, float a5, const U8* a6){::glBitmap(a0, a1, a2, a3, a4, a5, a6);}
void __stdcall glNewList_0073d330(U32 a0, U32 a1){::glNewList(a0, a1);}
U32 __stdcall glGenLists_0073d334(I32 a0){return ::glGenLists(a0);}
void __stdcall glPixelStorei_0073d338(U32 a0, I32 a1){::glPixelStorei(a0, a1);}
void __stdcall glDeleteLists_0073d33c(U32 a0, I32 a1){::glDeleteLists(a0, a1);}
void __stdcall glRasterPos2f_0073d340(float a0, float a1){::glRasterPos2f(a0, a1);}
void __stdcall glPushClientAttrib_0073d344(U32 a0){::glPushClientAttrib(a0);}
void __stdcall glPopClientAttrib_0073d348(){::glPopClientAttrib();}
void __stdcall glFogi_0073d34c(U32 a0, I32 a1){::glFogi(a0, a1);}
void __stdcall glFogf_0073d350(U32 a0, float a1){::glFogf(a0, a1);}
void __stdcall glFogfv_0073d354(U32 a0, const float* a1){::glFogfv(a0, a1);}
void __stdcall glColorMask_0073d358(U8 a0, U8 a1, U8 a2, U8 a3){::glColorMask(a0, a1, a2, a3);}
void __stdcall glColor4fv_0073d35c(const float* a0){::glColor4fv(a0);}
void __stdcall glMaterialfv_0073d360(U32 a0, U32 a1, const float* a2){::glMaterialfv(a0, a1, a2);}
void __stdcall glColorMaterial_0073d364(U32 a0, U32 a1){::glColorMaterial(a0, a1);}
void __stdcall glDepthFunc_0073d368(U32 a0){::glDepthFunc(a0);}
void __stdcall glClearStencil_0073d36c(I32 a0){::glClearStencil(a0);}
void __stdcall glLightModelfv_0073d370(U32 a0, const float* a1){::glLightModelfv(a0, a1);}
void __stdcall glScalef_0073d374(float a0, float a1, float a2){::glScalef(a0, a1, a2);}
void __stdcall glPolygonMode_0073d378(U32 a0, U32 a1){::glPolygonMode(a0, a1);}
U8 __stdcall glIsEnabled_0073d37c(U32 a0){return ::glIsEnabled(a0);}
void __stdcall glLineWidth_0073d380(float a0){::glLineWidth(a0);}
void __stdcall glPointSize_0073d384(float a0){::glPointSize(a0);}
void __stdcall glLightfv_0073d388(U32 a0, U32 a1, const float* a2){::glLightfv(a0, a1, a2);}
void __stdcall glBlendFunc_0073d38c(U32 a0, U32 a1){::glBlendFunc(a0, a1);}
// ---- END GENERATED
// ---- Revision (lane-m48-a, D53 handoff "MessageBoxA_0073d418 adapter (renderer-bodies adapter owner)"): user32 IAT cell 0073D418.
// Caller 0047B92F..0047B940 (Material::HasBumpMap) does call dword ptr [0073D418]; stdcall, 4 args, EAX = MessageBoxA result.
I32 __stdcall MessageBoxA_0073d418(void* window,const char* text,const char* caption,U32 type){return ::MessageBoxA(static_cast<HWND>(window),text,caption,type);}
}
