// Genuine OpenGL adapters; excluded from every executed fixture.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <gl/GL.h>
#include "c6ca4ec170-layers.h"
namespace aurora_gui {
void __stdcall get_integer(U32 word,I32* values){::glGetIntegerv(word,values);}
void __stdcall matrix_mode(U32 mode){::glMatrixMode(mode);}
void __stdcall push_matrix(){::glPushMatrix();}void __stdcall pop_matrix(){::glPopMatrix();}void __stdcall load_identity(){::glLoadIdentity();}
void __stdcall ortho(double l,double r,double b,double t,double n,double f){::glOrtho(l,r,b,t,n,f);}
void __stdcall blend_func(U32 s,U32 d){::glBlendFunc(s,d);}
void __stdcall viewport(I32 x,I32 y,I32 w,I32 h){::glViewport(x,y,w,h);}
void __stdcall begin(U32 mode){::glBegin(mode);}void __stdcall vertex4f(float x,float y,float z,float w){::glVertex4f(x,y,z,w);}void __stdcall end(){::glEnd();}
void __stdcall clear(U32 word){::glClear(word);}void __stdcall color4ubv(const unsigned char* bytes){::glColor4ubv(bytes);}
}
