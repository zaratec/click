#ifndef INFINITESOURCE_HH
#define INFINITESOURCE_HH
#include "element.hh"

/*
 * =c
 * InfiniteSource([DATA, COUNT, BURSTSIZE])
 * =d
 * Creates packets consisting of DATA. Pushes BURSTSIZE such packets
 * out its single output every time it is scheduled (which will be often).
 * After COUNT packets are sent, stops the driver; but if COUNT is negative,
 * sends packets forever.
 * Default DATA is at least 64 bytes long. Default COUNT is -1 (send packets
 * forever). Default BURSTSIZE is 1.
 * =e
 * = InfiniteSource(\<0800>, -1, 5) -> Queue -> ...
 * =n
 * Useful for profiling and experiments.
 */

class InfiniteSource : public Element { protected:
  
  String _data;
  int _count;
  int _limit;
  int _total;
  Packet *_packet;
  
 public:
  
  InfiniteSource();
  
  const char *class_name() const		{ return "InfiniteSource"; }
  Processing default_processing() const		{ return PUSH; }
  
  InfiniteSource *clone() const;
  int configure(const String &, ErrorHandler *);
  int initialize(ErrorHandler *);
  void uninitialize();
  
  bool run_scheduled();
  
};

#endif
