// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2013 CohortFS, LLC
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 *
 */

#include "xio_dispatcher.h"
#include "messages/MPing.h"
#include "messages/MDataPing.h"

XioDispatcher::XioDispatcher(Messenger *msgr) :
  Dispatcher(msgr->cct),
  active(false),
  messenger(msgr),
  dcount(0)
{
  // nothing
}

XioDispatcher::~XioDispatcher() {
  // nothing
}

bool XioDispatcher::ms_dispatch(Message *m)
{
  ConnectionRef conn;
  uint64_t dc = 0;

  dc = dcount++;

  switch (m->get_type()) {
  case CEPH_MSG_PING:
    break;
  case MSG_DATA_PING:
  {
    MDataPing* mdp __attribute__((unused)) = static_cast<MDataPing*>(m);
    //cout << "MDataPing " << mdp->tag << " " << mdp->counter << std::endl;
    //mdp->get_data().hexdump(cout);
  }
    break;
  default:
    abort();
  }

  if (unlikely(m->get_magic() & MSG_MAGIC_TRACE_CTR)) {
    if (unlikely(dc % 65536) == 0) {
      struct timespec ts;
      clock_gettime(CLOCK_REALTIME_COARSE, &ts);
      std::cout << "ping " << dc << " nanos: " <<
	ts.tv_nsec + (ts.tv_sec * 1000000000)  << std::endl;
    }
  } /* trace ctr */

  m->put();

  return true;
}

bool XioDispatcher::ms_handle_reset(Connection *con)
{
  return true;
}

void XioDispatcher::ms_handle_remote_reset(Connection *con)
{
  // nothing
}

