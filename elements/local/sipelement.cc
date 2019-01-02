#include <vector>
#include<click/config.h>
#include<click/confparse.hh>
#include<click/error.hh>
#include <sstream>
#include "sipelement.hh"
#include "packets.hh"
#include <queue>
#include <ctime>
#include <cstdlib>

CLICK_DECLS

SingleIPElement::SingleIPElement() : dumb(this) {}
SingleIPElement::~SingleIPElement() {}
int SingleIPElement::configure(Vector<String>& conf, ErrorHandler *errh){
    dumb.initialize(this);
    if (cp_va_kparse(conf, this, errh,
                "MY_ADDRESS", cpkP+cpkM, cpIPAddress, &addr,
                "TO_ADDRESS", cpkP+cpkM, cpIPAddress, &dest,
                "LOSS_RATE", cpkP+cpkM, cpInteger, &_loss,
                cpEnd) < 0) {
        return -1;
    }
    return 0;
}

int SingleIPElement::initialize(ErrorHandler*)
{
    srand(std::time(0));
    return 0;
}

void SingleIPElement::push(int port, Packet *pkt)
{
    if(rand() % 100 < _loss)
        pkt->kill();
    if(port == UPSIDE)   // packet comes from upside, wrap it and forward it do downside
    {
        WritablePacket *ip_encap = Packet::make(NULL, sizeof(MyIPHeader) + pkt->length());
        //click_chatter("allocated new packet for ipencap, length: %zu\n", ip_encap->length());
        MyIPHeader *iphdr = (MyIPHeader *)ip_encap->data();
        iphdr->type = DATA;
        iphdr->TTL = 30;
        iphdr->src = addr;
        iphdr->dest = this->dest;
        iphdr->size = sizeof(MyIPHeader) + pkt->length();
        memcpy(iphdr + 1, pkt->data(), pkt->length());
        //click_chatter("sizeof MyIPHeader: %zu; iphdr = %lx, pld = %lx\n", 
        //        sizeof(MyIPHeader), iphdr, pld);
        //output(DOWNSIDE).push(ip_encap);
        output(DOWNSIDE).push(ip_encap);
    }
    else    // packet comes from downside, wrap it and forward it to upside
    {
        char *tgt = (char *)pkt->data();
        MyIPHeader *iphdr = (MyIPHeader*)tgt;
        if(iphdr->type != DATA)
        {
            click_chatter("Get wrong type of packet! return!\n");
            return;
        }
        if(iphdr->dest != addr)
        {
            click_chatter("I'm not %s, I'm %s, drop the wrong packet!\n",
                    Int2Ipaddr(iphdr->dest).c_str(), Int2Ipaddr(addr).c_str());
            return;
        }
        /* remove old header */
        tgt += sizeof(MyIPHeader);
        WritablePacket *out = Packet::make(tgt, pkt->length() - sizeof(MyIPHeader));
        output(UPSIDE).push(out);
    }
}

std::string SingleIPElement::Int2Ipaddr(uint32_t addr)
{
    struct Iaddr
    {
        int a0 : 8;
        int a1 : 8;
        int a2 : 8;
        int a3 : 8;
    }__attribute__((packed));
    Iaddr a = *(Iaddr *)(&addr);
    char buf[30];
    sprintf(buf, "%d.%d.%d.%d\0", a.a0, a.a1, a.a2, a.a3);
    std::string ret{buf};
    return ret;
}

CLICK_ENDDECLS


EXPORT_ELEMENT(SingleIPElement)
