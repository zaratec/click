#ifndef FROMDEVICE_HH
#define FROMDEVICE_HH

/*
 * =c
 * FromDevice(devname)
 * =d
 * Intercepts all packets received by the Linux network interface
 * named devname and pushes them out output 0.
 * The packets include the link-level header.
 *
 * Linux won't see any packets from the device.
 * If you want Linux to process packets, you should
 * hand them to ToLinux.
 *
 * This element is only available inside the kernel module.
 *
 * =a ToDevice
 * =a ToLinux
 */

#include "element.hh"
#include "string.hh"
#include "glue.hh"

#define FROMDEV_QSIZE 64
class FromDevice : public Element {
  
 public:
  
  FromDevice();
  FromDevice(const String &);
  ~FromDevice();
  
  static void static_initialize();
  static void static_cleanup();
  
  const char *class_name() const		{ return "FromDevice"; }
  const char *processing() const	{ return PUSH; }
  
  FromDevice *clone() const;
  int configure(const String &, ErrorHandler *);
  int initialize(ErrorHandler *);
  void uninitialize();
  
  /* process a packet. return 0 if not wanted after all. */
  int got_skb(struct sk_buff *);
  
  void run_scheduled();
  
 private:
  
  String _devname;
  struct device *_dev;
  int _registered;
  
#ifdef CLICK_BENCHMARK
  // benchmark
  int _bm_done;
  void bm();
#endif
  
  Packet* _queue[FROMDEV_QSIZE];
  unsigned _puller_ptr;
  unsigned _pusher_ptr;
  unsigned next_i(int i) const	{ return (i!=(FROMDEV_QSIZE-1) ? i+1 : 0); } 
};

#endif
