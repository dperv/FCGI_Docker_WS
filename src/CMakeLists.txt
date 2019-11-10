cmake_minimum_required(VERSION 3.5)

project(fcgi_db_crud LANGUAGES CXX)
include_directories( /usr/local/Cellar/fcgi/2.4.0/include  /usr/local/Cellar/jsoncpp/1.9.1/include /usr/include/jsoncpp /usr/local/Cellar/mysql++/3.2.5/include  /usr/local/Cellar/mysql-client/5.7.28/include/mysql /usr/include/mysql)
link_directories(/usr/local/Cellar/fcgi/2.4.0/lib /usr/local/Cellar/jsoncpp/1.9.1/lib /usr/local/Cellar/mysql++/3.2.5/lib /usr/local/Cellar/mysql-client/5.7.28/lib)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable( fcgi_db_crud main.cpp fcgi_server.cpp dataobject.cpp jsonparser.cpp databasedriver.cpp)
target_link_libraries(fcgi_db_crud fcgi jsoncpp mysqlpp )
