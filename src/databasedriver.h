#ifndef DATABASEDRIVER_H
#define DATABASEDRIVER_H
#include <memory>
#include <string>
#include <mysql++/mysql++.h>
#include "dataobject.h"

class databaseDriver
{
   public:
      databaseDriver(std::string db, std::string address, std::string login, std::string pass);
      bool connect();
      bool disconnect();

      bool objCreate( dataObject &obj );
      bool objRead( dataObject &obj );
      bool objUpdate( dataObject &obj );
      bool objDelete( dataObject &obj );
      ~databaseDriver();


   private:
      std::unique_ptr<mysqlpp::Connection> con;
      bool connected = false;
      std::string db_db;
      std::string db_address;
      std::string db_login;
      std::string db_password;
};

#endif // DATABASEDRIVER_H
