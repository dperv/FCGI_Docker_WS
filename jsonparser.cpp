#include "jsonparser.h"
#include <iostream>
#include <memory>
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

bool JsonParser::build(const dataObject &in, std::string &out)
{

   Json::Value root;
   root["key"]   = in.getKey();
   root["value"] = in.getValue();
   out = root.toStyledString();
   return true;
}
