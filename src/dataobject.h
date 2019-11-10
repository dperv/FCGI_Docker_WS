#ifndef DATAOBJECT_H
#define DATAOBJECT_H
#include <string>

class dataObject
{
   public:
      dataObject();
      dataObject(const dataObject &rignt);
      std::string getKey() const;
      void setKey(const std::string &value);

      std::string getValue() const;
      void setValue(const std::string &lvalue);



      dataObject &operator=( const dataObject &right);

      void print();

   private:
      std::string key;
      std::string value;
};

#endif // DATAOBJECT_H
