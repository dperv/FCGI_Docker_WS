#include <stdlib.h>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include "fcgi_server.h"
#include "jsonparser.h"
#include <json/json.h>
#include "databasedriver.h"
using namespace std;
using namespace sbFCGI;
using namespace Json;

// args: 127.0.0.1:9000 CRUD_DATA 192.168.56.102 crud crud1

int main(int argc, char **argv)
{
   if(argc < 6)
   {
      cout << "Usage:"<<argv[0]<<"<api address> <db name> <db address> <db login> <db password>";
      return 1;
   }

   databaseDriver  storage(argv[2], argv[3], argv[4], argv[5] );
   FCGI_Server srv(argv[1]);

   storage.connect();


   srv.Start();


   while ( true )
   {

      try {
      request rq = srv.Accept();
      if(rq.type  == RQERROR)
      {
         cout<<"Error. Exitiong"<<endl;
         break;
      }
      auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());


      std::cout<<"---------- New request -------------------------------"<<std::endl;
      std::cout<< ctime(&timenow) ;

      std::cout<<"______________ Headers ___________________"<<std::endl;
      for(auto hd : rq.header)
      {
         std::cout<<hd.first<<":"<<hd.second<<std::endl;
      }
      std::cout<<"__________________________________________"<<std::endl;

      response resp;
      resp.status = OK;


      dataObject data;
      if(rq.data.length())
      {
         std::cout<<"Data = "<<rq.data<<endl;

         JsonParser j;
         if(rq.data.length() > 0)
         {
            cout << "Got incoming data for insert" << endl;

            if( j.parse(rq.data, data) )
            {
               cout << "Got incoming values for insert" << endl;
               data.print();
            }else
            {
               resp.status = ERROR;
            }
         }
      }



      switch (rq.type)
      {

         case GET:
         {
            resp.status = ERROR;
            if(rq.header.count("REQUEST_URI") > 0 )
            {

               std::string rqs = rq.header["REQUEST_URI" ];

               if(rqs.find("?") != std::string::npos)
               {

                  std::string key = rqs.substr(rqs.find("?"));

                  if(key.find("=") != std::string::npos)
                  {
                     data.setKey(key.substr(key.find("=")+1));

                     JsonParser j;

                     if(storage.objRead(data))
                     {
                        j.build(data, resp.data);
                        resp.status = OK;
                     }
                  }
               }

            }else
            {
               resp.status = ERROR;
            }
         }
         break;
         case POST:
         {
            if(!storage.objCreate(data))
            {
               resp.status = ERROR;
               //resp.data = "Inserted data" + rq.data;
            }

         }
         break;
         case PUT:
         {
            if(!storage.objUpdate(data))
            {
               resp.status = ERROR;
               //resp.data = "Inserted data" + rq.data;
            }
            //resp.data = "Hello from "+std::to_string(pid) + "  - "+ std::to_string(count++);
         }
         break;
         case DELETE:
         {
            resp.status = ERROR;
            if(rq.header.count("REQUEST_URI") > 0 )
            {

               std::string rqs = rq.header["REQUEST_URI" ];

               if(rqs.find("?") != std::string::npos)
               {

                  std::string key = rqs.substr(rqs.find("?"));

                  if(key.find("=") != std::string::npos)
                  {
                     data.setKey(key.substr(key.find("=")+1));

                     JsonParser j;

                     if(storage.objDelete(data))
                     {
                        resp.status = OK;
                     }
                  }
               }

            }
         }
         break;
         case RQERROR:
         default:
            resp.status = ERROR;
         break;


      }
      srv.Response(resp);
      std::cout<<"==========================================================="<<std::endl;
      } catch(...)
      {
         std::cout<<"SOme error. Waiting for new request."<<std::endl;
         continue;
      }
   }


   return 0;
}
