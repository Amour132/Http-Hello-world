#pragma once 

#include <iostream>
#include <unistd.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

class Client 
{
  public:

    Client(const std::string &ip,int &port):_ip(ip),_port(port),_sock(-1)
    {}

    void InitClient()
    {
      _sock = socket(AF_INET,SOCK_STREAM,0);
      if(_sock < 0)
      {
        std::cerr << "socket error" <<std::endl;
        exit(2);
      }
    }

    void Connect()
    {
      struct sockaddr_in peer;
      peer.sin_family = AF_INET;
      peer.sin_port = htons(_port);
      peer.sin_addr.s_addr = inet_addr(_ip.c_str());

      if(connect(_sock,(sockaddr*)&peer,sizeof(peer)) == 0)
      {
        std::cout << "connect success" << std::endl;
      }
      else 
      {
        exit(3);
      }
    }

    void Start()
    {
      std::string message;
      char buf[1024] = {0};
      for(;;)
      {
        std::cout << "Please Entet#" << std::endl;
        std::cin >> message;
        write(_sock,message.c_str(),message.size());
        read(_sock,buf,sizeof(buf)-1);
        std::cout << "echo#" << buf << std::endl;
      }
    }

    ~Client()
    {
      close(_sock);
    }
  private:
    int _sock; 
    std::string _ip;
    int _port;
};
