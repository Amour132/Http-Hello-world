#include "Server.hpp"

int main(int argc,char* argv[])
{
  if(argc != 2)
  {
    std::cout << "Usage./diect" << std::endl;
    exit(1);
  }
  int port = atoi(argv[1]);
  Server* s = new Server(port);
  s->InitServer();
  s->Start();
  delete s;
  return 0;
}
