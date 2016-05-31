// Copyright (c) 2010
// All rights reserved.

#include <memory>

#include "ZackServer.hh"

int main(int argc, char* argv[]) {
  std::auto_ptr<zack::ZackServer> server;

  server.reset( new zack::ZackServer(argc, argv) );

  return 0;
}
