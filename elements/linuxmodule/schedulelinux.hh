#ifndef SCHEDULELINUX_HH
#define SCHEDULELINUX_HH
#include <click/element.hh>
#include <click/task.hh>

/*
 * =c
 * ScheduleLinux
 * =s
 * returns to Linux scheduler
 * =io
 * None
 * =d
 *
 * Returns to Linux's scheduler every time it is scheduled by Click. Use
 * ScheduleInfo to specify how often this should happen.
 *
 * =a ScheduleInfo */

class ScheduleLinux : public Element { public:
  
  ScheduleLinux();
  ~ScheduleLinux();
  
  const char *class_name() const		{ return "ScheduleLinux"; }
  
  ScheduleLinux *clone() const;
  int configure(const Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *);
  void add_handlers();

  void run_scheduled();

 private:

  Task _task;
  
};

#endif
