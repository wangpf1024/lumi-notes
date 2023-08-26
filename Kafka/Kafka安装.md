参考文档：http://kafka.apache.org/documentation/#introduction

下载地址：http://kafka.apache.org/downloads

## 配置更新

```shell
# 解压安装包接口
vi config/server.properties 
# 修改配置文件信息 id 不可重复
broker.id=0
zookeeper.connect=172.27.2.38:2181,172.27.2.48:2181,172.27.2.63:2181
# 集群情况下需要增加默认失效时长
zookeeper.connection.timeout.ms=180000
```

## 启动broker

```shell
# 启动
./bin/kafka-server-start.sh ./config/server.properties &
# 守护进程方式启动
./bin/kafka-server-start.sh  -daemon ./config/server.properties &
```

> The producer sends data directly to the broker that is the leader for the partition without any intervening routing tier. 
>
> To help the producer do this all Kafka nodes can answer a request for metadata about which servers are alive and where the leaders for the partitions of a topic are at any given time to allow the producer to appropriately direct its requests.

## 创建topic

```shell
# 创建topic replication  需要于 <= broker size (节点数量)
./kafka-topics.sh --create --zookeeper 172.27.2.38:2181,172.27.2.48:2181,172.27.2.63:2181 --replication-factor 3 --partitions 4 --topic dx_datacenter_topic
./kafka-topics.sh --create --zookeeper 172.27.2.38:2181,172.27.2.48:2181,172.27.2.63:2181 --replication-factor 3 --partitions 4 --topic dx_guijiaicall_topic
./kafka-topics.sh --create --zookeeper 172.27.2.38:2181,172.27.2.48:2181,172.27.2.63:2181 --replication-factor 3 --partitions 4 --topic dx_trfailureretry_topic
./kafka-topics.sh --create --zookeeper 172.27.2.38:2181,172.27.2.48:2181,172.27.2.63:2181 --replication-factor 3 --partitions 12 --topic dx_trcallback_topic
```
