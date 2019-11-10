CREATE DATABASE CRUD_DATA CHARACTER SET utf8 COLLATE utf8_general_ci;

CREATE TABLE `STORE` (
  `key` varchar(100) NOT NULL,
  `value` varchar(100) NOT NULL,
  PRIMARY KEY (`key`),
  UNIQUE KEY `STORE_key_IDX` (`key`) USING HASH
) ENGINE=InnoDB DEFAULT CHARSET=utf8


CREATE USER 'crud_user'@'%';
UPDATE mysql.user SET Password=PASSWORD('crud_user1') WHERE User='crud_user' AND Host='%';
GRANT Alter ON CRUD_DATA.* TO 'crud_user'@'%';
GRANT Create ON CRUD_DATA.* TO 'crud_user'@'%';
GRANT Create view ON CRUD_DATA.* TO 'crud_user'@'%';
GRANT Delete ON CRUD_DATA.* TO 'crud_user'@'%';
GRANT Drop ON CRUD_DATA.* TO 'crud_user'@'%';
GRANT Grant option ON CRUD_DATA.* TO 'crud_user'@'%';
GRANT Index ON CRUD_DATA.* TO 'crud_user'@'%';
GRANT Insert ON CRUD_DATA.* TO 'crud_user'@'%';
GRANT References ON CRUD_DATA.* TO 'crud_user'@'%';
GRANT Select ON CRUD_DATA.* TO 'crud_user'@'%';
GRANT Show view ON CRUD_DATA.* TO 'crud_user'@'%';
GRANT Trigger ON CRUD_DATA.* TO 'crud_user'@'%';
GRANT Update ON CRUD_DATA.* TO 'crud_user'@'%';
FLUSH PRIVILEGES;
