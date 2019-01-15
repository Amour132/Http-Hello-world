#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;
using std::cerr;

void Usage()
{
  cout << "Usage./server[ip][port]" << endl;
}

int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    Usage();
    exit(1);
  }
  int sock = socket(AF_INET,SOCK_STREAM,0);
  if(sock < 0)
  {
    cerr << "sock error" << endl;
    exit(2);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));

  int ret = bind(sock,(sockaddr*)&addr,sizeof(addr));
  if(ret < 0)
  {
    cerr << "bind error" << endl;
    exit(2);
  }

  ret = listen(sock,10);
  if(ret < 0)
  {
    cerr << "listen error" << endl;
    exit(2);
  }
  for(;;)
  {
    struct sockaddr_in client_addr;
    socklen_t len;
    int new_sock = accept(sock,(sockaddr*)&client_addr,&len);
    if(new_sock < 0)
    {
      cerr << "accept error" << endl;
      continue;
    }
    char buff[1024] = {0};
    ssize_t read_size = read(new_sock,buff,sizeof(buff)-1);
    if(read_size < 0)
    {
      cerr << "read error" << endl;
      exit(2);
    }
    char message[1024] = {0};
    const char* hello = "<h1>hello world</h1>";
    sprintf(message,"HTTP/1.0 200 OK\nContent-Length:%lu\n\n%s",strlen(hello),hello);
    write(new_sock,message,strlen(message));
  }
  return 0;
}

