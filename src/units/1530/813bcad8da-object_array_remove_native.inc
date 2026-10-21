struct NativeObjectArrayRemove final:ObjectArrayRemovePorts{void release(Word a)override{::operator delete(ptr(a));}};
extern "C" unsigned char __fastcall recovered_object_array_remove_004d81a0(void*self,void*,unsigned id,void**out){NativeMemory m;NativeObjectArrayRemove p;return static_cast<unsigned char>(object_array_remove_004d81a0(m,p,word(self),id,word(out)));}
