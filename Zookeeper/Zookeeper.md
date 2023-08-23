下载地址 ：https://zookeeper.apache.org/releases.html

帮助文档：https://zookeeper.apache.org/doc/r3.7.0/zookeeperOver.html

## 1.安装软件

```shell
# 安装zk 集群-------最新集群 使用3台虚拟机
# 解压zk安装包
tar apache-zookeeper-3.6.3-bin.tar.gz
cd  apache-zookeeper-3.6.3-bin
```

## 2.修改配置文件

```shell
# 复制配置文件
cp zoo_sample.cfg zoo.cfg
```

```shell
# 编辑配置信息
vi zoo.cfg
tickTime=2000
dataDir=/opt/supp_app/data/zookeeper
clientPort=2181
initLimit=5
syncLimit=2
#节点选举及数据同步配置
server.1=172.27.2.38:2888:3888
server.2=172.27.2.48:2888:3888
server.3=172.27.2.63:2888:3888
```

## 3.写入id标识

```shell
# 在 dataDir 目录下创建一个名称为 myid 的文件 并将当前主机对应的 server.X 中的 X 写入文件。
cd /opt/supp_app/data/zookeeper
# 将 1  复制到 myid 每一个ip对应唯一的 x
vi myid
```

## 4.启动服务

```shell
# 启动命令 将 zoo.cfg 相关配置复制到其他主机并执行以上操作即可。
./bin/zkServer.sh start
./bin/zkServer.sh status
```
