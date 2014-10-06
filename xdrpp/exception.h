// -*- C++ -*-

/** \file exception.h Exceptions raised by RPC calls.  These depend on
 * the RPC message format.  Other exceptions may be found in \c
 * types.h.*/

#ifndef _XDRPP_EXCEPTION_H_HEADER_INCLUDED_
#define _XDRPP_EXCEPTION_H_HEADER_INCLUDED_ 1

#include <xdrpp/rpc_msg.hh>
#include <cerrno>
#include <cstring>

namespace xdr {

//! Translate one of the conditions in
//! [RFC5531](https://tools.ietf.org/html/rfc5531) for an unexecuted
//! call into a string.
const char *rpc_errmsg(accept_stat ev);
//! Translate one of the conditions in
//! [RFC5531](https://tools.ietf.org/html/rfc5531) for an unexecuted
//! call into a string.
const char *rpc_errmsg(auth_stat ev);
//! Return <tt>"rpcvers field mismatch"</tt> in response to constant
//! \c RPC_MISMATCH.
const char *rpc_errmsg(reject_stat ev);

//! Structure encoding all the various reasons a server can decline to
//! process an RPC call it received.
struct rpc_call_stat {
  enum { ACCEPT_STAT, AUTH_STAT, REJECT_STAT } type_;
  union {
    accept_stat accept_;
    auth_stat auth_;
    reject_stat reject_;
  };
  rpc_call_stat() : type_(ACCEPT_STAT), accept_(SUCCESS) {}
  rpc_call_stat(accept_stat s) : type_(ACCEPT_STAT), accept_(s) {}
  rpc_call_stat(auth_stat s) : type_(AUTH_STAT), auth_(s) {}
  rpc_call_stat(reject_stat s) : type_(REJECT_STAT), reject_(s) {}
  const char *message() const;
};

//! This is the exception raised in an RPC client when it reaches the
//! server and transmits a call, with no connection or communication
//! errors, but the server replies with an RPC-level message header
//! refusing to execute the call.
struct xdr_call_error : xdr_runtime_error {
  rpc_call_stat stat_;
  xdr_call_error(rpc_call_stat s) : xdr_runtime_error(s.message()), stat_(s) {}
};

//! Check that an RPC header precedes a result.  \throws
//! xdr_call_error if the reply does not contain a response.
void check_call_hdr(const rpc_msg &hdr);

//! This exception represents a system error encountered while
//! attempting to send RPC messages over a socket.
struct xdr_system_error : xdr_runtime_error {
  xdr_system_error(const char *what, int no = errno)
    : xdr_runtime_error(std::string(what) + ": " + std::strerror(no)) {}
};

}

#endif // !_XDRPP_EXCEPTION_H_HEADER_INCLUDED_
