#ifndef JSONPARSER_H
#define JSONPARSER_H
#include <string>
#include "dataobject.h"

class JsonParser
{
   public:
      JsonParser();
      bool parse(const std::string in,  dataObject& out );
      bool build(const dataObject& in,  std::string &out   );
};

#endif // JSONPARSER_H
