#include "1a29dc9d2e3ac481_system_token.hpp"
#include <cstdint>
namespace recovered_client_token {
unsigned parse_006754d0(const char*data,unsigned n,String*a,String*b,String*c,Ports&p){unsigned i=0,last=static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(c));while(i<n){last=static_cast<unsigned char>(data[i]);if(last=='.')break;if(last==' '){p.clear(a);p.clear(b);p.clear(c);return 0;}++i;}
 String first;p.counted(&first,data,i);p.assign(a,&first);p.destroy(&first);
 if(last!='.'){p.clear(b);p.clear(c);return i?1:0;}
 ++i;const unsigned start=i;while(i<n){last=static_cast<unsigned char>(data[i]);if(last==' ')break;if(last=='.'){p.clear(b);p.clear(c);return 0;}++i;}
 String second;p.counted(&second,data+start,i-start);p.assign(b,&second);p.destroy(&second);
 if(last==' '){++i;if(i>=n){p.clear(c);return 0;}String rest;p.counted(&rest,data+i,n-i);p.assign(c,&rest);p.destroy(&rest);return 3;}
 p.clear(c);return i>start?2:0;
}
}
