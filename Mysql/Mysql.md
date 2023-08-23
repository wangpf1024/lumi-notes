下载地址：https://downloads.mysql.com/archives/community/

下载版本文件：

|                                                |                                                     |
| ---------------------------------------------- | --------------------------------------------------- |
| mysql-community-client-5.7.27-1.el7.x86_64.rpm | mysql-community-common-5.7.27-1.el7.x86_64.rpm      |
| mysql-community-libs-5.7.27-1.el7.x86_64.rpm   | mysql-community-libs-compat-5.7.27-1.el7.x86_64.rpm |
| mysql-community-server-5.7.27-1.el7.x86_64.rpm |                                                     |

安装参考文档：https://dev.mysql.com/doc/refman/8.0/en/linux-installation-rpm.html

## 1.安装

```shell
# Common files for server and client libraries
rpm -Uvh mysql-community-common-5.7.27-1.el7.x86_64.rpm
```

> warning: mysql-community-common-5.7.27-1.el7.x86_64.rpm: Header V3 DSA/SHA1 Signature, key ID 5072e1f5: NOKEY
> Preparing...                          ################################# [100%]
>   file /usr/share/mysql/czech/errmsg.sys from install of mysql-community-common-5.7.27-1.el7.x86_64 conflicts with file from package mariadb-libs-1:5.5.44-2.el7.centos.x86_64

```shell
# content os 7 默认安装 mysql 数据库移除 
yum -y remove mariadb-libs-1:5.5.44-2.el7.centos.x86_6`
```

```shell
# Common files for server and client libraries
rpm -Uvh mysql-community-common-5.7.27-1.el7.x86_64.rpm
```

```shell
# Shared libraries for MySQL database client applications
rpm -Uvh mysql-community-libs-5.7.27-1.el7.x86_64.rpm
```

```shell
# SMySQL client applications and tools
rpm -Uvh mysql-community-client-5.7.27-1.el7.x86_64.rpm
```

```shell
# Database server and related tools
mysql-community-server-5.7.27-1.el7.x86_64.rpm
```

## 2.启动数据库

```shell
# 启动数据库
systemctl start mysqld
```

## 3.权限修改

```shell
# 查看临时密码
grep 'temporary password' /var/log/mysqld.log
```

```shell
# 登录数据库
mysql -uroot -p
```

```shell
# 修改密码
ALTER USER 'root'@'localhost' IDENTIFIED BY 'MyNewPass4!';
```

```shell
# 修改密码设置权限--便于设置简单密码
SHOW VARIABLES LIKE 'validate_password%';
set global validate_password_length=6;
set global validate_password_policy=LOW; 
```

## 4.创建用户

```shell
# 创建测试用户
create user 'config-center'@'%' identified by '123456';
# 为测试用户赋值数据库访问权限
grant all privileges on `config-center`.* to 'config-center'@'%' identified by '123456'; 
# 创建测试数据库
CREATE DATABASE `config-center`;
```

```shell
# 开启数据库的binlog日志权限
vi /etc/my.cnf
server_id=1918
log_bin = mysql-bin
binlog_format = ROW
expire_logs_days=30

#查看设置结果 并重启数据库实例
show variables like '%log_bin%';
```

