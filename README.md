# FastCGI Docker Workshop

Схема контейнеров:

c1       cXX             c2

nginx -> fcgi_db_crud -> mariadb

# Build:

mkdir build

cd build

cmake ../src

make

Run:

./fcgi_db_crud 127.0.0.1:9000 CRUD_DATA 192.168.56.102 crud crud1%  
    
         
         
