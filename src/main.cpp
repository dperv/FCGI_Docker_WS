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

   if(!storage.connect())
      return 1;
   if(!srv.Start())
      return 2;


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
                  cout << "Got incoming data:" << endl;
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

                  if( rqs.find("/info") != std::string::npos )
                  {
                     JsonParser j;
                     j.build( resp.data);
                     resp.status = OK;

                  }else
                     if( rqs.find("/storage") != std::string::npos )
                     {
                        cout << "Storage request" << endl;

                        size_t key_pos = std::string::npos;
                        key_pos = rqs.find("/storage/");
                        if ( key_pos != std::string::npos)
                        {/// Key request

                           key_pos+=8;   // + length /storage
                           data.setKey(rqs.substr(key_pos+1));
                           cout << "Searching for key "<< data.getKey() << endl;
                           JsonParser j;
                           if(storage.objRead(data))
                           {
                              j.build(data, resp.data);
                              resp.status = OK;
                           }else
                           {
                              resp.status = NOT_FOUND;
                           }
                        }else
                        {/// Global request
                           std::vector<dataObject> out;
                           JsonParser j;
                           if(storage.objRead(out))
                           {
                              j.build(out, resp.data);
                              resp.status = OK;
                           }else
                           {
                              resp.status = NOT_FOUND;
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
               int errcode = 0;
               if(data.getKey().length() == 0)
                  resp.status = WR_REQUEST;
               else if(!storage.objCreate( data, errcode ))
               {
                  if(errcode == 1062)
                  {
                     resp.status = DUPLICATE;
                  }else
                     resp.status = ERROR;
               }else
               {
                  resp.status = OK;
               }

            }
            break;
            case PUT:
            {
               int error = 0;
               int count = 0;
               if(data.getKey().length() == 0 || data.getValue().length() == 0)
                  resp.status = WR_REQUEST;
               else if(!storage.objUpdate(data, error, count))
               {
                  if(count == 0 )  resp.status = NOT_FOUND;
                  else
                     resp.status = ERROR;
               }else
               {
                  JsonParser j;
                  j.buildUpdate(data, resp.data);
               }

            }
            break;
            case DELETE:
            {
               resp.status = ERROR;

               int error = 0;
               int count = 0;
               if(data.getKey().length() == 0 )
                  resp.status = WR_REQUEST;
               else if(!storage.objDelete(data, error, count))
               {
                  if(count == 0 )  resp.status = NOT_FOUND;
                  else
                     resp.status = ERROR;
               }else
               {
                  JsonParser j;
                  j.buildDelete(data, resp.data);
                  resp.status = OK;
               }
            }
            break;
            case RQERROR:
            default:
               resp.status = ERROR;
            break;


         }


         {
            JsonParser j;
            switch (resp.status) {
               case ERROR:
                  j.buildError( resp.data);
               break;
               default:
               break;
            }
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
