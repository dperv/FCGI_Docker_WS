#ifndef FCGI_SERVER_H
#define FCGI_SERVER_H
#include <string>
#include <map>
#include <fcgi_config.h>
#include <fcgio.h>

namespace sbFCGI {
   enum  reqtypes
   {
      RQERROR,
      GET,
      POST,
      PUT,
      DELETE
   };

   enum  restypes
   {
      UNKNOWN,
      OK         = 200,
      DUPLICATE  = 304,
      NOT_FOUND  = 404,
      WR_REQUEST = 412,
      ERROR      = 500
   };

   struct request
   {
         reqtypes    type;
         std::map<std::string,std::string>    header;
         std::string data;
         void clear()
         {

          type =   RQERROR;
          header.clear();
          data.clear();
         }

   };

   struct response {
         int         status;
         std::string data;
   };


   class FCGI_Server
   {
      public:
         FCGI_Server(std::string socket);
         ~FCGI_Server();
         int Start();

         request Accept();
         int     Response(response &resp);

      private:
         std::string socketpath;
         int listenSocket;
         FCGX_Request FCGXreq;

   };

}
#endif // FCGI_SERVER_H
