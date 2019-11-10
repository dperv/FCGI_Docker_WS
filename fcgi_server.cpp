#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "fcgio.h"
#include "fcgi_config.h"  // HAVE_IOSTREAM_WITHASSIGN_STREAMBUF
#include "fcgi_server.h"
using namespace sbFCGI;

FCGI_Server::FCGI_Server(std::string socket)
{
   socketpath = socket;
   listenSocket = 0;
   FCGX_Init();
}

FCGI_Server::~FCGI_Server()
{
   FCGX_Finish();
   close(listenSocket);
}


int FCGI_Server::Start()
{
   listenSocket = FCGX_OpenSocket(socketpath.c_str(), 200);
   return 1;
}

request FCGI_Server::Accept()
{
   static request data;
   data.clear();

   FCGX_InitRequest(&FCGXreq, listenSocket, 0);
   if(FCGX_Accept_r(&FCGXreq) != 0)
   {
      std::cout<<"Got error. Exiting"<<std::endl;
      data.type = RQERROR;
   }
   else
   {
      std::cout<<"Incoming request:"<<std::endl;
      for(int i=0; FCGXreq.envp[i] != NULL; i+=2)
      {
         char *var = FCGXreq.envp[i];
         if(var != NULL)
         {
            char *p = strchr (var, '=');
            if(p)
            {
               p++;
            }
            data.header[std::string(var,(p-var-1))] = p;
         }
      }

      if(data.header["REQUEST_METHOD"] == "GET")
      {
         data.type = GET;
      }else if(data.header["REQUEST_METHOD"] == "POST")
      {
         data.type = POST;
      }else if(data.header["REQUEST_METHOD"] == "PUT")
      {
         data.type = PUT;
      }else if(data.header["REQUEST_METHOD"] == "DELETE")
      {
         data.type = DELETE;
      }



      if(data.header.count("CONTENT_LENGTH") && data.header["CONTENT_LENGTH"].length() && std::stoi(data.header["CONTENT_LENGTH"]) > 0)
      {
         int ilen = std::stoi(data.header["CONTENT_LENGTH"]);
         char *bufp = (char *)malloc(ilen+1);
         bzero(bufp, ilen+1);
         FCGX_GetStr((char *)bufp,ilen, FCGXreq.in);
         data.data = std::string(bufp);
         free(bufp);

      }



   }
   return data;
}

int FCGI_Server::Response(response &resp)
{
   FCGX_PutS("Content-type: text/html\r\n", FCGXreq.out);
   if(resp.status == OK)
      FCGX_PutS("Status: 200 OK\r\n", FCGXreq.out);
   else
      FCGX_PutS("Status: 500 ERROR\r\n", FCGXreq.out);
   FCGX_PutS("\r\n",      FCGXreq.out);
   FCGX_PutS( resp.data.c_str(), FCGXreq.out);
   FCGX_Finish_r(&FCGXreq);
   return 1;
}



