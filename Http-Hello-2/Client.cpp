#include "Client.hpp"

int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    std::cout << "Usage./dicet" << std::endl;
    exit(1);
  }
  int port = atoi(argv[2]);
  std::string ip = argv[1];
  Client *c = new Client(ip,port);
  c->InitClient();
  c->Connect();
  c->Start();
  delete c;
  return 0;
}
