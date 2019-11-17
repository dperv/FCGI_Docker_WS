#ifndef JSONPARSER_H
#define JSONPARSER_H
#include <string>
#include <vector>
#include "dataobject.h"

class JsonParser
{
   public:
      JsonParser();
      bool parse(const std::string in,  dataObject& out );
      bool build(const dataObject& in,  std::string &out   );
      bool build(std::string &out);
      bool build(std::vector<dataObject> &in, std::string &out);
      bool buildCreate(const dataObject &in, std::string &out);
      bool buildUpdate(const dataObject &in,std::string &out);
      bool buildDelete(const dataObject &in,std::string &out);
      bool buildError(std::string &out);
};

#endif // JSONPARSER_H
