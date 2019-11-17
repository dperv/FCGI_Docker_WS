#include <string>
#include <memory>
#include "databasedriver.h"


databaseDriver::databaseDriver(std::string db,std::string address, std::string login, std::string pass): db_db(db),db_address(address), db_login(login), db_password(pass)
{
   con = std::unique_ptr<mysqlpp::Connection>(new mysqlpp::Connection(false));
}

bool databaseDriver::connect()
{
   if( con->connect( db_db.c_str(), db_address.c_str(), db_login.c_str(), db_password.c_str() ) )
   {

      return true;
   }
   else
   {
      std::cerr << "Ошибка при установлении соединения с БД: " << con->error() << std::endl;
      con->disconnect();
      return false;
   }
   return true;
}

bool databaseDriver::disconnect()
{
   con->disconnect();
   return true;
}

bool databaseDriver::objCreate(dataObject &obj, int &error_cade)
{
   mysqlpp::Query query = con->query( "insert into STORE ( `key`,`value` ) values ('"+obj.getKey()+"','"+obj.getValue()+"' )");
   if( query.exec() )
   {
      std::cout << "Object inserted. ["<<query.affected_rows()<<"]" << std::endl;
   }
   else
   {
      std::cout << "Object insert error [ "<< query.errnum() << query.error()<<"]" << std::endl;
      error_cade = query.errnum();
      return false;
   }
   return true;
}

bool databaseDriver::objRead(dataObject &obj)
{
   mysqlpp::Query query = con->query( "select `key`,`value` from STORE where `key`='"+obj.getKey()+"'");
   if( mysqlpp::StoreQueryResult res = query.store() )
   {
      if(res.num_rows() == 0)
         return false;
      for( size_t i = 0; i < res.num_rows(); i++ )
      {
         obj.setKey(res[i][0].c_str());
         obj.setValue(res[i][1].c_str());
      }
   }
   else
   {
      std::cout << "Object search error [ " << query.error()<<"]" << std::endl;
      return false;
   }
   return true;
}
bool databaseDriver::objRead(std::vector<dataObject> &obj)
{
   mysqlpp::Query query = con->query( "select `key`,`value` from STORE");
   if( mysqlpp::StoreQueryResult res = query.store() )
   {
      if(res.num_rows() == 0)
         return false;
      std::cout << "Found: "<<res.num_rows()<<"rows. " << std::endl;
      for( size_t i = 0; i < res.num_rows(); i++ )
      {
         dataObject tmp;
         tmp.setKey(res[i][0].c_str());
         tmp.setValue(res[i][1].c_str());
         tmp.print();
         obj.push_back(tmp);
      }
   }
   else
   {
      std::cout << "Object search error [ " << query.error()<<"]" << std::endl;
      return false;
   }
   return true;
}

bool databaseDriver::objUpdate(dataObject &obj, int &error, int &row)
{
   mysqlpp::Query query = con->query( "update STORE set `key`='"+obj.getKey()+"', `value`='"+obj.getValue()+"' where `key`='"+obj.getKey()+"'");
   if( query.exec()  )
   {
      std::cout << "Object updated. ["<<query.affected_rows()<<"]" << std::endl;
      row = query.affected_rows();
      if(!query.affected_rows())
         return false;
   }
   else
   {
      std::cout << "Object Update error [ " << query.error()<<"]" << std::endl;
      error = query.errnum();
      return false;
   }
   return true;
}

bool databaseDriver::objDelete(dataObject &obj, int &error, int &row)
{
   mysqlpp::Query query = con->query( "delete from STORE where `key`='"+obj.getKey()+"'");
   if(query.exec() )
   {
      row = query.affected_rows();
      if(!query.affected_rows())
         return false;
      std::cout << "Object deleted. ["<<query.affected_rows()<<"]" << std::endl;
   }
   else
   {
      std::cout << "Object search error [ " << query.error()<<"]" << std::endl;
      error = query.errnum();
      return false;
   }
   return true;
}

databaseDriver::~databaseDriver()
{
   disconnect();
}
