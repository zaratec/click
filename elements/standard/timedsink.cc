/*
 * timedsink.{cc,hh} -- element pulls packets periodically, discards them
 * Eddie Kohler
 *
 * Copyright (c) 1999 Massachusetts Institute of Technology.
 *
 * This software is being provided by the copyright holders under the GNU
 * General Public License, either version 2 or, at your discretion, any later
 * version. For more information, see the `COPYRIGHT' file in the source
 * distribution.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "timedsink.hh"
#include "confparse.hh"
#include "error.hh"
#include "glue.hh"

TimedSink::TimedSink()
  : _timer(this)
{
  add_input();
}

TimedSink *
TimedSink::clone() const
{
  return new TimedSink;
}

int
TimedSink::configure(const String &conf, ErrorHandler *errh)
{
  return cp_va_parse(conf, this, errh,
		     cpInterval, "packet pull interval", &_interval,
		     0);
}

int
TimedSink::initialize(ErrorHandler *)
{
  _timer.schedule_after_ms(_interval);
  return 0;
}

void
TimedSink::uninitialize()
{
  _timer.unschedule();
}

bool
TimedSink::wants_packet_upstream() const
{
  return true;
}

bool
TimedSink::run_scheduled()
{
  // if timer is scheduled, we don't care if a packet is upstream.
  if (_timer.scheduled())
    return false;

  Packet *p = input(0).pull();
  if (p) {
    p->kill();
    _timer.schedule_after_ms(_interval);
  }
  // otherwise, rely on PACKET_UPSTREAM to wake us up
  return false;
}

EXPORT_ELEMENT(TimedSink)
