#include <iostream>
#include "dataobject.h"

dataObject::dataObject()
{

}

dataObject::dataObject(const dataObject &rignt)
{
   key    = rignt.key;
   value  = rignt.value;

}

std::string dataObject::getKey() const
{
   return key;
}

void dataObject::setKey(const std::string &value)
{
   key = value;
}

std::string dataObject::getValue() const
{
   return value;
}

void dataObject::setValue(const std::string &lvalue)
{
   value = lvalue;
}

dataObject& dataObject::operator=(const dataObject &right)
{
   if (this == &right) {
      return *this;
   }
   key   = right.key;
   value = right.value;

   return *this;

}

void dataObject::print()
{
   std::cout << "Pair: "<<key<<" : "<<value<<std::endl;
}
