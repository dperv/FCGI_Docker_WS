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

      std::vector<std::string> pass_params;
      pass_params.push_back("REQUEST_METHOD");
      pass_params.push_back("CONTENT_LENGTH");
      pass_params.push_back("REQUEST_URI");


      for(auto &param : pass_params)
      {
         char *value = FCGX_GetParam(param.c_str(), FCGXreq.envp);
         if( value )
            data.header[param] = std::string(value);
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

      if(data.header.count("CONTENT_LENGTH") && data.header["CONTENT_LENGTH"].length() && atoi(data.header["CONTENT_LENGTH"].c_str()) > 0)
      {
         int ilen = atoi(data.header["CONTENT_LENGTH"].c_str());
         if( ilen > 0 )
         {
            char *bufp = (char *)malloc(ilen+1);
            bzero(bufp, ilen+1);
            FCGX_GetStr((char *)bufp,ilen, FCGXreq.in);
            data.data = std::string(bufp);
            free(bufp);
         }
      }
   }
   return data;
}

int FCGI_Server::Response(response &resp)
{
   FCGX_PutS("Content-type: text/html\r\n", FCGXreq.out);

   switch (resp.status) {
      case OK:
         FCGX_PutS("Status: 200 OK\r\nConnection:close\r\n", FCGXreq.out);
      break;
      case DUPLICATE:
         FCGX_PutS("Status: 304 Not Modified\r\nConnection:close\r\n", FCGXreq.out);
      break;
      case NOT_FOUND:
         FCGX_PutS("Status: 404 Not Found\r\nConnection:close\r\n", FCGXreq.out);
      break;
      case WR_REQUEST:
         FCGX_PutS("Status: 412 ERROR\r\nConnection:close\r\n", FCGXreq.out);
      break;
      default:
         FCGX_PutS("Status: 500 ERROR\r\nConnection:close\r\n", FCGXreq.out);
      break;
   }

   FCGX_PutS("\r\n",      FCGXreq.out);
   FCGX_PutS( resp.data.c_str(), FCGXreq.out);
   FCGX_Finish_r(&FCGXreq);
   return 1;
}



