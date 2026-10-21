#include "600d93cb23d5f323_client_status.hpp"
namespace recovered_client_status {
unsigned status_0064f280(void*self,unsigned subtype,Ports&p){String text;p.empty(&text);if(p.overrun(self,1)||p.underrun(self,1)){p.destroy(&text);return 0;}(void)p.gui(p.app());switch(static_cast<unsigned char>(subtype)){
 case 3:p.state(p.app(),2);if(p.downloaded(p.app()))p.set_downloaded(p.app(),0);else if(p.has_character(p.app())){Ref ref;p.ref_from_string(&ref,p.character(p.app()));p.send_character(self,&ref,0);}break;
 case 2:p.state(p.app(),0);break;
 case 1:{p.state(p.app(),1);p.format_run(&text,'s',"Module","Run");void*app=p.app();unsigned length=p.length(&text);const char*data=p.text(&text);unsigned recipient=p.recipient();void*network=p.network(app);p.send(network,recipient,data,length,0);break;}
 default:p.destroy(&text);return 0;
 }p.destroy(&text);return 1;}
}
