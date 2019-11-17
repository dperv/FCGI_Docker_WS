#include "jsonparser.h"
#include <iostream>
#include <memory>
#include <vector>
#include <json/json.h>
#include <json/config.h>
using namespace Json;


JsonParser::JsonParser()
{

}

bool JsonParser::parse(const std::string in, dataObject &out)
{
   std::cout << "string to parse [ "<<in << "]"<< in << std::endl;
   Json::Value root;
   std::string errors;
   //Json::Reader reader;

   Json::CharReaderBuilder builder;
   const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

   if (!reader->parse(in.c_str(), in.c_str() + in.length(), &root, &errors)) {
      std::cout << "error:"<<errors  << std::endl;
      return false;
   }

   out.setKey(   root["key"]  .asString());
   out.setValue( root["value"].asString());
   return true;
}
bool JsonParser::build(std::string &out)
{
   Json::Value root;
   root["result"] = "C++ APP.  OK";
   out = root.toStyledString();
   return true;
}

bool JsonParser::build(const dataObject &in, std::string &out)
{

   Json::Value root;
   root["key"]   = in.getKey();
   root["value"] = in.getValue();
   out = root.toStyledString();
   return true;
}

bool JsonParser::build(std::vector<dataObject> &in, std::string &out)
{
   Json::Value root;

   for(auto &obj : in)
   {
      Json::Value val;
      val["key"]   = obj.getKey();
      val["value"] = obj.getValue();
      root.append(val);
   }
   out = root.toStyledString();
   return true;
}


bool JsonParser::buildCreate(const dataObject &in, std::string &out)
{
   Json::Value root;
   root["message"] = "Key "+in.getKey()+" successfully added";
   out = root.toStyledString();
   return true;
}
bool JsonParser::buildUpdate(const dataObject &in,std::string &out)
{
   Json::Value root;
   root["message"] = "Key "+in.getKey()+" successfully added";
   out = root.toStyledString();
   return true;
}
bool JsonParser::buildDelete(const dataObject &in,std::string &out)
{
   Json::Value root;
   root["message"] = "C++ APP.  OK";
   out = root.toStyledString();
   return true;
}
bool JsonParser::buildError(std::string &out)
{
   Json::Value root;
   root["message"] = "Server Error";
   out = root.toStyledString();
   return true;
}

