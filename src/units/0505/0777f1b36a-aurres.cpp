// aurres.cpp - lane-aurres: Aurora resource stack + text/data stream (swkotor.exe 0044BD10..0044CB03).
// Every behavior cites the original instruction that establishes it (lane-aurres/disasm/range.txt).
// CRT family: original old-CRT fopen 006FAD9D / fread 006FA9D4 / fgets 006FC469 / fclose 006FACF0 /
// sprintf 006FADB0 are provided by the linked UCRT (/MT); FILE* objects never cross families.
#include "abe8f58c7a-aurres.hpp"
#include <cstring>

using aurres::U; using aurres::I; using aurres::Descriptor;

extern "C" {
aurres::GetCallback volatile aurres_get_callback_007b8fc4=nullptr;
aurres::ReleaseCallback volatile aurres_release_callback_007b8fc8=nullptr;
kotor_reconstruction::ArrayMutations aurres_stack_007b8fd8={};
char aurres_dofile_message_007b8fe8[0x100]={};
}

namespace {
kotor_reconstruction::ArrayMutations& stack=aurres_stack_007b8fd8;

Descriptor* volatile* slots(){return reinterpret_cast<Descriptor* volatile*>(static_cast<std::uintptr_t>(stack.data));}
I wrap_sub(I a,I b){return static_cast<I>(static_cast<U>(a)-static_cast<U>(b));}

// 0044BD70 GetResource: cdecl(selector). NULL -> data[size-1] with no empty guard (0044BD78..0044BD83).
// Non-NULL -> linear search over the count cached once at 0044BD89; 0 when absent (0044BDAA).
Descriptor* lookup_0044bd70(void* selector){
    if(!selector)return slots()[stack.size-1];
    const I count=stack.size;
    Descriptor* volatile* const data=slots();                                   // 0044BD95 loaded once
    for(I i=0;i<count;++i)
        if(data[i]==selector)return data[i];                                    // 0044BDA0 / 0044BDAE
    return nullptr;
}

// 0044BEE0/0044BF30/0044BFA0/0044BFD0 "top": data[size-1] read directly (0044BEE0..0044BEEC).
Descriptor* top(){return slots()[stack.size-1];}

// Lazy scratch shared by 0044BE3A, 0044BE91, 0044BEF7, 0044BF53, 0044CA68: new(+14), byte 0 = NUL.
void ensure_scratch(Descriptor* d){
    if(d->scratch==nullptr){
        char* s=static_cast<char*>(live_operator_new(static_cast<U>(d->scratch_size)));
        d->scratch=s;
        s[0]=0;
    }
}

// 0044BE20 getnextline_file (selector in EAX on the original). Line counter incremented before the
// lazy scratch (0044BE29..0044BE35); fgets(scratch, 0FA0h, file) result returned as is (0044BE58).
char* line_file_0044be20(void* selector){
    Descriptor* const d=lookup_0044bd70(selector);
    const I line=d->line;
    char* const scratch=d->scratch;
    d->line=line+1;
    if(scratch==nullptr){
        char* s=static_cast<char*>(live_operator_new(static_cast<U>(d->scratch_size)));
        d->scratch=s;
        s[0]=0;
    }
    return std::fgets(d->scratch,0xfa0,d->file);
}

// 0044BE70 getnextline_res (selector in EAX on the original).
char* line_memory_0044be70(void* selector){
    Descriptor* const d=lookup_0044bd70(selector);
    const I remaining=d->remaining;                                             // 0044BE7B
    const unsigned char* source=d->current;                                     // 0044BE7E
    I n=0;
    if(remaining<=0)return nullptr;                                            // 0044BE86 jle -> 0044BEC7
    ensure_scratch(d);                                                          // 0044BE8A..0044BEA0
    for(;;){                                                                    // 0044BEA5
        d->scratch[n]=static_cast<char>(*source);                               // 0044BEA5..0044BEAA (scratch reloaded)
        const I live=d->remaining;                                              // 0044BEAD reload
        ++n;++source;
        if(n>=live)break;                                                       // 0044BEB2
        if(d->scratch[n-1]=='\n')break;                                         // 0044BEB6..0044BEBD (LF retained)
    }
    const I left=wrap_sub(d->remaining,n);                                      // 0044BEBF sub [esi+10h],edi
    d->remaining=left;
    d->current=const_cast<unsigned char*>(source);                              // 0044BEC2
    if(left<0)return nullptr;                                                   // 0044BEC5 jns
    d->scratch[n]=0;                                                            // 0044BED0
    return d->scratch;                                                          // 0044BED4 reload
}

// 0044BEE0 file GetData(count): fread(scratch,1,count,file); no NUL; returns scratch (0044BF1C reload).
char* data_file_0044bee0(I count){
    Descriptor* const d=top();
    ensure_scratch(d);
    std::fread(d->scratch,1,static_cast<std::size_t>(static_cast<U>(count)),d->file);
    return d->scratch;
}

// 0044BF30 memory GetData(count): unconditional first byte (do-loop 0044BF65..0044BF78), no remaining>0 guard.
char* data_memory_0044bf30(I count){
    Descriptor* const d=top();
    ensure_scratch(d);                                                          // 0044BF46..0044BF62
    const unsigned char* source=d->current;                                     // 0044BF49 (read after +08)
    I n=0;
    do {
        d->scratch[n]=static_cast<char>(*source);                               // 0044BF65..0044BF6A
        const I live=d->remaining;                                              // 0044BF6D reload
        ++n;++source;
        if(n>=live)break;                                                       // 0044BF72
    } while(n<count);                                                           // 0044BF76 signed
    const I left=wrap_sub(d->remaining,n);                                      // 0044BF7A
    d->remaining=left;
    d->current=const_cast<unsigned char*>(source);                              // 0044BF7D
    if(left<0)return nullptr;                                                   // 0044BF80 jns
    d->scratch[n]=0;                                                            // 0044BF8C
    return d->scratch;
}

// Push shared by 0044C6FE..0044C731, 0044C810..0044C84C, 0044C8AC..0044C8E0: grow to 2*capacity or 8
// through 005E03C0 when size==capacity, then data[size]=d, ++size.
void push(Descriptor* d){
    const I capacity=stack.capacity;
    if(stack.size==capacity)
        menu_list_growth::allocate_005e03c0(&stack,nullptr,capacity!=0?static_cast<U>(capacity)+static_cast<U>(capacity):8u);
    slots()[stack.size]=d;
    stack.size=stack.size+1;
}

// 0044C680 internal_AurResGet: original custom ABI ECX=ext, EDX=name, one caller-popped stack word = out
// size pointer; EAX = descriptor or 0. Sole caller 0044C777, so it is internal here.
Descriptor* internal_get_0044c680(const char* name,const char* ext,I volatile* out){
    I size;                                                                     // [esp+0Ch] local (push ecx slot)
    unsigned char* const data=static_cast<unsigned char*>(aurres_get_callback_007b8fc4(name,ext,&size)); // 0044C68F
    if(data==nullptr)return nullptr;                                            // 0044C69E..0044C6A6
    Descriptor* d=static_cast<Descriptor*>(live_operator_new(0x1c));           // 0044C6AA
    if(d!=nullptr){                                                             // 0044C6B6..0044C6CF
        d->file=nullptr;d->line=-1;d->scratch_size=0x2000;
        d->scratch=nullptr;d->current=nullptr;d->remaining=0;d->base=nullptr;
    }
    // 0044C6D6: a failed 1Ch allocation leaves ESI=0 and the following stores fault (original quirk kept).
    Descriptor* volatile target=d;
    target->base=data;                                                          // 0044C6D8
    target->current=data;                                                       // 0044C6DB
    const I raw=size;
    target->remaining=raw>=0?raw:static_cast<I>(~static_cast<U>(raw));         // 0044C6E2..0044C6F0 (-1-raw)
    if(out!=nullptr)*out=size;                                                  // 0044C6F3..0044C6FB raw size
    push(target);                                                               // 0044C6FE..0044C731
    return target;
}
}

extern "C" {
// 0044BD10 AurResGetDataBytes(count, descriptor), cdecl. Memory: return old cursor, cursor += count,
// remaining -= count (0044BD1A..0044BD27). File: new(count) then fread(buffer,1,count,file) (0044BD33..0044BD41).
void* __cdecl aurres_data_bytes_0044bd10(U count,void* descriptor){
    Descriptor* const d=static_cast<Descriptor*>(descriptor);
    if(d->file==nullptr){
        unsigned char* const old=d->current;
        d->current=old+count;
        d->remaining=wrap_sub(d->remaining,static_cast<I>(count));
        return old;
    }
    void* const buffer=live_operator_new(count);
    std::fread(buffer,1,count,d->file);
    return buffer;
}

// 0044BD50 AurResFreeDataBytes(descriptor, bytes): only file descriptors own the bytes (0044BD54..0044BD62).
void __cdecl aurres_free_data_bytes_0044bd50(void* descriptor,void* bytes){
    if(static_cast<Descriptor*>(descriptor)->file!=nullptr&&bytes!=nullptr)live_free(bytes);
}

// 0044BDC0 AurResRemove(descriptor): find with cached count; on a hit store size-1 and shift down,
// reloading data and size each step after the first (0044BDF0, 0044BE07). EAX is incidental.
void __cdecl aurres_remove_0044bdc0(void* descriptor){
    const I count=stack.size;
    I i=0;
    if(count<=0)return;                                                         // 0044BDCA
    Descriptor* volatile* data=slots();                                         // 0044BDCC
    while(data[i]!=descriptor){if(++i>=count)return;}                           // 0044BDD7..0044BDE2
    const I size=count-1;
    stack.size=size;                                                            // 0044BDE6
    if(i>=size)return;                                                          // 0044BDEC
    for(;;){
        data[i]=data[i+1];                                                      // 0044BE00..0044BE04
        ++i;
        if(i>=stack.size)return;                                                // 0044BE07..0044BE10
        data=slots();                                                           // 0044BDF0
    }
}

// 0044BFA0 AurResGetNextLine, cdecl, no arguments: empty stack -> 0 (EAX=size=0, 0044BFA9); otherwise
// selector 0 (xor eax,eax 0044BFB9/0044BFC0) into the file or memory leaf chosen by top->file.
char* __cdecl resource_next_0044bfa0(){
    if(stack.size==0)return nullptr;
    if(top()->file!=nullptr)return line_file_0044be20(nullptr);
    return line_memory_0044be70(nullptr);
}
const char* __cdecl aurres_next_line_const_0044bfa0(){return resource_next_0044bfa0();}

// 0044BFD0 AurResGetData(count), cdecl one word (tail jumps keep the argument slot).
void* __cdecl resource_read_0044bfd0(U count){
    if(stack.size==0)return nullptr;
    if(top()->file!=nullptr)return data_file_0044bee0(static_cast<I>(count));
    return data_memory_0044bf30(static_cast<I>(count));
}

// 0044C740 AurResGet(name, ext, int* size, char memory_only), cdecl.
void* __cdecl aurres_get_0044c740(const char* name,const char* ext,I volatile* size,U memory_only){
    char path[0x104];
    if(ext==nullptr)ext="";                                                     // 0044C752..0044C757 (73D71C "")
    if(aurres_get_callback_007b8fc4!=nullptr){                                  // 0044C75C
        Descriptor* const d=internal_get_0044c680(name,ext,size);               // 0044C777
        if(d!=nullptr)return d;                                                 // 0044C781 -> 0044C85D
        if(static_cast<char>(memory_only)!=0)return nullptr;                    // 0044C787 byte compare
    }
    // 0044C790..0044C79F: "" when ext starts with '.', otherwise "." (including an empty ext).
    const char* const separator=(ext[0]!=0&&ext[0]=='.')?"":".";
    std::sprintf(path,"%s%s%s",name,separator,ext);                             // 0044C7B1 (740CC4)
    std::FILE* const file=std::fopen(path,"rb");                                // 0044C7C0 (73D5B0)
    if(file==nullptr)return nullptr;                                            // 0044C7CA
    Descriptor* d=static_cast<Descriptor*>(live_operator_new(0x1c));           // 0044C7DD
    if(d!=nullptr){
        d->file=nullptr;d->line=-1;d->scratch_size=0x7a120;
        d->scratch=nullptr;d->current=nullptr;d->remaining=0;d->base=nullptr;
    }
    Descriptor* volatile target=d;                                              // 0044C809: failed new faults below
    target->file=file;                                                          // 0044C80B
    target->line=0;                                                             // 0044C80D
    push(target);                                                               // 0044C810..0044C84C
    if(size!=nullptr)*size=1;                                                   // 0044C84A..0044C854 after the push
    return target;
}

// 0044C870 AurResGet(FILE*), cdecl: file descriptor with line counter 0, pushed; no out size.
void* __cdecl aurres_get_file_0044c870(std::FILE* file){
    Descriptor* d=static_cast<Descriptor*>(live_operator_new(0x1c));           // 0044C873
    if(d!=nullptr){
        d->file=nullptr;d->line=-1;d->scratch_size=0x7a120;
        d->scratch=nullptr;d->current=nullptr;d->remaining=0;d->base=nullptr;
    }
    Descriptor* volatile target=d;
    target->file=file;                                                          // 0044C8A7
    target->line=0;                                                             // 0044C8A9
    push(target);                                                               // 0044C8AC..0044C8E0
    return target;
}

// 0044C8F0 AurResFree(descriptor, size), cdecl.
U __cdecl aurres_release_0044c8f0(void* descriptor,I size){
    Descriptor* const d=static_cast<Descriptor*>(descriptor);
    if(d==nullptr)return 0;                                                     // 0044C8F7 (EAX incidental)
    if(d->file!=nullptr){
        std::fclose(d->file);                                                   // 0044C904
        d->file=nullptr;                                                        // 0044C90C
    } else {
        aurres_release_callback_007b8fc8(d->base,size);                         // 0044C914..0044C91D, no null check
        d->base=nullptr;                                                        // 0044C926
    }
    const I count=stack.size;                                                   // 0044C92D
    I hits=0;
    if(count>0){
        Descriptor* volatile* const data=slots();                               // 0044C93C
        for(I i=0;i<count;++i)if(data[i]==d)++hits;                             // 0044C942..0044C94B
        if(hits!=0)stack.RemoveReverse32(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(d))); // 0044C957 00445080
    }
    char* const scratch=d->scratch;
    if(scratch!=nullptr){live_free(scratch);d->scratch=nullptr;}                // 0044C95C..0044C96D
    live_free(d);                                                               // 0044C975
    return 0;
}

// 0044C980 dofile(file_name), cdecl; returns the 007B8FE8 message buffer.
char* __cdecl aurres_dofile_0044c980(const char* file_name){
    char ext[0x10];                                                             // [esp+8]  (local +00)
    char base[0x40];                                                            // [esp+18h](local +10)
    const char* p=file_name;
    I n=0;
    char c=*p;
    while(c!=0&&c!='.'){base[n++]=c;c=*++p;}                                    // 0044C989..0044C99F
    base[n]=0;                                                                  // 0044C9A3
    n=0;
    c=*p;
    while(c!=0){ext[n++]=c;c=*++p;}                                             // 0044C9B0..0044C9BB (p ends on NUL)
    ext[n]=0;                                                                   // 0044C9C5
    Descriptor* const d=static_cast<Descriptor*>(aurres_get_0044c740(base,ext,nullptr,0)); // 0044C9BD..0044C9D0
    if(d==nullptr){
        std::sprintf(aurres_dofile_message_007b8fe8,"ERROR:  file '%s' doesn't exist",p); // 0044C9DE (7414A0); p == ""
        return aurres_dofile_message_007b8fe8;
    }
    for(;;){
        const I count=stack.size;                                               // 0044CA00
        I i=0;
        Descriptor* entry=nullptr;
        if(count>0){
            Descriptor* volatile* const data=slots();                           // 0044CA0C
            while(i<count&&data[i]!=d)++i;                                      // 0044CA12..0044CA1A
            if(i<count)entry=data[i];                                           // 0044CA43
        }
        if(entry==nullptr)break;                                                // 0044CA1C / 0044CA48
        char* line;
        if(entry->file!=nullptr){                                               // 0044CA4A: inline file reader
            Descriptor* const f=lookup_0044bd70(entry);                         // 0044CA50 (non-NULL selector)
            const I number=f->line;
            char* const scratch=f->scratch;
            f->line=number+1;                                                   // 0044CA57..0044CA63
            if(scratch==nullptr){
                char* s=static_cast<char*>(live_operator_new(static_cast<U>(f->scratch_size)));
                f->scratch=s;s[0]=0;                                            // 0044CA68..0044CA77
            }
            line=std::fgets(f->scratch,0xfa0,f->file);                          // 0044CA86
        } else {
            line=line_memory_0044be70(entry);                                   // 0044CA90 (EAX = entry)
        }
        if(line==nullptr)break;                                                 // 0044CA97
        while(*line==' '||*line=='\t')++line;                                   // 0044CAA0..0044CAAD
        if(*line==0||*line=='#')continue;                                       // 0044CAB1 / 0044CAB9
        char* q=line+std::strlen(line)-1;                                       // 0044CAC2..0044CAD0
        while(static_cast<I>(reinterpret_cast<std::uintptr_t>(q))>=static_cast<I>(reinterpret_cast<std::uintptr_t>(line))){ // jl signed
            const char t=*q;
            if(t!='\n'&&t!='\r'&&t!='\t'&&t!=' ')break;                        // 0044CAD8..0044CAEC
            *q=0;--q;                                                           // 0044CAEE..0044CAF4
        }
        aurres_func_interp_0044c1f0(line);                                      // 0044CAF7
    }
    aurres_release_0044c8f0(d,0);                                               // 0044CA1C..0044CA1F
    std::sprintf(aurres_dofile_message_007b8fe8,"Done executing file '%s'",p);  // 0044CA24 (741484)
    return aurres_dofile_message_007b8fe8;
}
}
