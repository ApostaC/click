#ifndef SERVER_HH
#define SERVER_HH
#include <click/element.hh>
#include <click/task.hh>

#define MESSAGE 0
#define DATA 1

CLICK_DECLS

class Server: public Element {
    public: 
        Server() : _task(this), _done(false) {}
        const char *class_name() const { return "Server";}
        const char *port_count() const { return "0/1"; }
        const char *process() const { return PUSH; }
        int configure(Vector<String> &, ErrorHandler *);
        int initialize(ErrorHandler*);
        //void push(int, Packet*);
        bool run_task(Task *);
        ~Server(){}
	private:
        bool _done;
	    String str;
        uint8_t type;
        uint32_t dst_ip;
        uint32_t dst_port;
        Task _task;
};

CLICK_ENDDECLS
#endif
