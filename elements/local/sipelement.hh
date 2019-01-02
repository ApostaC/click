#ifndef _SIP_HH_
#define _SIP_HH_

#include <click/element.hh>
#include <click/timer.hh>
#include <click/hashtable.hh>
#include "packets.hh"
#include <cstring>
#include <string>
#include <iostream>


CLICK_DECLS

class SingleIPElement : public Element
{
    public:
		SingleIPElement();
		~SingleIPElement();
		const char *class_name() const { return "SingleIPElement";}
		const char *port_count() const { return "1-/1-";}
		const char *processing() const { return PUSH;}
		int configure(Vector<String>&,ErrorHandler*);
		void push(int, Packet*);
		int initialize(ErrorHandler*);
		//void run_timer(Timer* timer);
    public:
        std::string Int2Ipaddr(uint32_t addr);

    private:
        int _loss;
        uint32_t addr;
        uint32_t dest;
        Timer dumb;

    public:
        static const int DOWNSIDE = 1;
        static const int UPSIDE = 0;

};

CLICK_ENDDECLS

#endif
