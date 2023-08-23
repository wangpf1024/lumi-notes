下载地址：https://www.mongodb.com/download-center/community/releases

参考文档：https://docs.mongodb.com/manual/tutorial/install-mongodb-on-red-hat/

虚拟机配置：172.27.2.63/38/48

## 1.初始软件环境

参考文档 https://docs.mongodb.com/manual/tutorial/deploy-shard-cluster/

**以上机器分别安装以上软件**

```shell
# 创建mongodb 仓库地址
vi /etc/yum.repos.d/mongodb-org-4.4.repo

[mongodb-org-4.4]
name=MongoDB Repository
baseurl=https://repo.mongodb.org/yum/redhat/$releasever/mongodb-org/4.4/x86_64/
gpgcheck=1
enabled=1
gpgkey=https://www.mongodb.org/static/pgp/server-4.4.asc
```

```shell
# 安装db
yum install -y mongodb-org
```

```shell
# 观察配置文件信息
cat /etc/mongod.conf
```

## 2.创建配置文件

**以上机器分别配置**

### 第一步：Start each member of the config server replica set

```shell
# For a production deployment, deploy a config server replica set with at least three members
vi  /opt/soft/mongodb_sharding_config/03/shared.conf
```

```shell
# mongod.conf

# for documentation of all options, see:
#   http://docs.mongodb.org/manual/reference/configuration-options/

# where to write logging data.
systemLog:
  destination: file
  logAppend: true
  path: /opt/supp_app/data/mongodb/logs/03/mongod.log

# Where and how to store data.
storage:
  dbPath: /opt/supp_app/data/mongodb/03
  journal:
    enabled: true
#  engine:
#  wiredTiger:

# how the process runs
processManagement:
  fork: true  # fork and run in background
  pidFilePath: /opt/soft/mongodb_sharding_config/03/mongod.pid  # location of pidfile
  timeZoneInfo: /usr/share/zoneinfo

# network interfaces 默认端口为 27019 
net:
#  port: 27019
  bindIp: 172.27.2.63  # Enter 0.0.0.0,:: to bind to all IPv4 and IPv6 addresses or, alternatively, use the net.bindIpAll setting.


security:
  authorization: enabled
  keyFile: /opt/soft/mongodb_sharding_config/ssl_key # openssl rand -base64 756 > ssl_key

#operationProfiling:

#replication:

replication:
  oplogSizeMB: 4096
  replSetName: "confs"

sharding:
  clusterRole: configsvr

## Enterprise-Only Options

#auditLog:

#snmp:
```

### 第二步：Create the Shard Replica Sets "rep1"

```shell
# 第二步：
vi  /opt/soft/mongodb_sharding_config/01/shared.conf
```

```shell
# mongod.conf

# for documentation of all options, see:
#   http://docs.mongodb.org/manual/reference/configuration-options/

# where to write logging data.
systemLog:
  destination: file
  logAppend: true
  path: /opt/supp_app/data/mongodb/logs/01/mongod.log

# Where and how to store data.
storage:
  dbPath: /opt/supp_app/data/mongodb/01
  journal:
    enabled: true
#  engine:
#  wiredTiger:

# how the process runs
processManagement:
  fork: true  # fork and run in background
  pidFilePath: /opt/soft/mongodb_sharding_config/01/mongod.pid  # location of pidfile
  timeZoneInfo: /usr/share/zoneinfo

# network interfaces 默认端口 27018
net:
#  port: 27018
  bindIp: 172.27.2.63 # Enter 0.0.0.0,:: to bind to all IPv4 and IPv6 addresses or, alternatively, use the net.bindIpAll setting.


security:
  authorization: enabled
  keyFile: /opt/soft/mongodb_sharding_config/ssl_key

#operationProfiling:

replication:
  oplogSizeMB: 4096
  replSetName: "rep1"

sharding:
  clusterRole: shardsvr

## Enterprise-Only Options

#auditLog:

#snmp:
```

### 第三步：Create the Shard Replica Sets "rep2"

```shell
vi  /opt/soft/mongodb_sharding_config/02/shared.conf
```

```shell
# mongod.conf

# for documentation of all options, see:
#   http://docs.mongodb.org/manual/reference/configuration-options/

# where to write logging data.
systemLog:
  destination: file
  logAppend: true
  path: /opt/supp_app/data/mongodb/logs/02/mongod.log

# Where and how to store data.
storage:
  dbPath: /opt/supp_app/data/mongodb/02
  journal:
    enabled: true
#  engine:
#  wiredTiger:

# how the process runs
processManagement:
  fork: true  # fork and run in background
  pidFilePath: /opt/soft/mongodb_sharding_config/02/mongod.pid  # location of pidfile
  timeZoneInfo: /usr/share/zoneinfo

# network interfaces
net:
  port: 27017
  bindIp: 172.27.2.63  # Enter 0.0.0.0,:: to bind to all IPv4 and IPv6 addresses or, alternatively, use the net.bindIpAll setting.


security:
  authorization: enabled
  keyFile: /opt/soft/mongodb_sharding_config/ssl_key

#operationProfiling:

#replication:

replication:
  oplogSizeMB: 4096
  replSetName: "rep2"


sharding:
  clusterRole: shardsvr

## Enterprise-Only Options

#auditLog:

#snmp:
```

### 第四步：Start a `mongos` for the Sharded Cluster

```shell
vi  /opt/soft/mongodb_sharding_config/mongos.conf
```

```shell
systemLog:
  destination: file
  logAppend: true
  path: /opt/supp_app/data/mongodb/logs/mongod.log

processManagement:
  fork: true  # fork and run in background
  pidFilePath: /opt/soft/mongodb_sharding_config/mongos.pid  # location of pidfile
  timeZoneInfo: /usr/share/zoneinfo

security:
  keyFile: /opt/soft/mongodb_sharding_config/ssl_key
  
sharding:
  configDB: confs/172.27.2.38:27019,172.27.2.48:27019,172.27.2.63:27019
net:
  port: 3000
  bindIp: 172.27.2.63
```

## 3.启动mongo

**以上机器分别执行**


```shell
# 1.启动 Shard Replica 集合 rep1
/usr/bin/mongod -f /opt/soft/mongodb_sharding_config/01/shared.conf
# 2.启动 Shard Replica 集合 rep2
/usr/bin/mongod -f /opt/soft/mongodb_sharding_config/02/shared.conf
# 3.启动 Config Server
/usr/bin/mongod -f /opt/soft/mongodb_sharding_config/03/shared.conf
# 4.启动集群 启动集群
/usr/bin/mongos -f /opt/soft/mongodb_sharding_config/mongos.conf
# -----------------------------------------------------------------------停止
/usr/bin/mongod -f /opt/soft/mongodb_sharding_config/01/shared.conf --shutdown
```

## 4.构建集群 

**以上机器分别执行**

```shell
# 1.初始 Shard Replica confs
/usr/bin/mongo --host 172.27.2.48:27019
#初始成员
rs.initiate( {
   _id : "confs",
   configsvr: true,
   members: [
      { _id: 0, host: "172.27.2.38:27019" },
      { _id: 1, host: "172.27.2.48:27019" },
      { _id: 2, host: "172.27.2.63:27019" } 
   ]
})
# To identify the primary in the replica set.
rs.status() 

# 2.初始 Shard Replica 集合 rep1
/usr/bin/mongo --host 172.27.2.48:27018
#初始成员
rs.initiate( {
   _id : "rep1",
   members: [
      { _id: 0, host: "172.27.2.38:27018" },
      { _id: 1, host: "172.27.2.48:27018" },
      { _id: 2, host: "172.27.2.63:27018" } 
   ]
})
# To identify the primary in the replica set.
rs.status() 

# 3.初始 Shard Replica 集合 rep2
/usr/bin/mongo --host 172.27.2.48:27017
#初始成员
rs.initiate( {
   _id : "rep2",
   members: [
      { _id: 0, host: "172.27.2.38:27017" },
      { _id: 1, host: "172.27.2.48:27017" },
      { _id: 2, host: "172.27.2.63:27017" } 
   ]
})
# To identify the primary in the replica set.
rs.status() 

# 连接集群
/usr/bin/mongo --host 172.27.2.48:3000

# 添加集群 shardAdded
sh.addShard( "rep1/172.27.2.38:27018,172.27.2.48:27018,172.27.2.63:27018")
sh.addShard( "rep2/172.27.2.38:27017,172.27.2.48:27017,172.27.2.63:27017")
```

## 5.数据备份与恢复

```shell
/usr/bin/mongodump --host=192.168.100.118 --port=27017 --db=dxLogDB --out=/opt/data/
/usr/bin/mongorestore --host=172.27.2.63 --port=3000 /opt/data/
```

> 2021-06-17T10:51:18.452+0800    preparing collections to restore from
> 2021-06-17T10:51:18.456+0800    reading metadata for dxLogDB.dXLog from /opt/data/dxLogDB/dXLog.metadata.json
> 2021-06-17T10:51:18.456+0800    reading metadata for dxLogDB.callPlanDTO from /opt/data/dxLogDB/callPlanDTO.metadata.json
> 2021-06-17T10:51:18.456+0800    reading metadata for dxLogDB.taskCreateBackDTO from /opt/data/dxLogDB/taskCreateBackDTO.metadata.json
> 2021-06-17T10:51:18.456+0800    reading metadata for dxLogDB.dxClientLogVo from /opt/data/dxLogDB/dxClientLogVo.metadata.json
> 2021-06-17T10:51:18.814+0800    restoring dxLogDB.dXLog from /opt/data/dxLogDB/dXLog.bson
> 2021-06-17T10:51:18.870+0800    restoring dxLogDB.callPlanDTO from /opt/data/dxLogDB/callPlanDTO.bson
> 2021-06-17T10:51:18.982+0800    restoring dxLogDB.taskCreateBackDTO from /opt/data/dxLogDB/taskCreateBackDTO.bson
> 2021-06-17T10:51:19.061+0800    restoring dxLogDB.dxClientLogVo from /opt/data/dxLogDB/dxClientLogVo.bson
> 2021-06-17T10:51:19.437+0800    no indexes to restore
> 2021-06-17T10:51:19.437+0800    finished restoring dxLogDB.taskCreateBackDTO (98 documents, 0 failures)
> 2021-06-17T10:51:19.437+0800    reading metadata for dxLogDB.taskCallBackDTO from /opt/data/dxLogDB/taskCallBackDTO.metadata.json
> 2021-06-17T10:51:19.437+0800    no indexes to restore
> 2021-06-17T10:51:19.437+0800    finished restoring dxLogDB.callPlanDTO (319 documents, 0 failures)
> 2021-06-17T10:51:19.437+0800    no indexes to restore
> 2021-06-17T10:51:19.438+0800    finished restoring dxLogDB.dxClientLogVo (30 documents, 0 failures)
> 2021-06-17T10:51:19.593+0800    restoring dxLogDB.taskCallBackDTO from /opt/data/dxLogDB/taskCallBackDTO.bson
> 2021-06-17T10:51:19.790+0800    no indexes to restore
> 2021-06-17T10:51:19.790+0800    finished restoring dxLogDB.taskCallBackDTO (99 documents, 0 failures)
> 2021-06-17T10:51:21.451+0800    [######..................]  dxLogDB.dXLog  36.4MB/135MB  (27.0%)
> 2021-06-17T10:51:24.451+0800    [################........]  dxLogDB.dXLog  90.6MB/135MB  (67.2%)
> 2021-06-17T10:51:26.821+0800    [########################]  dxLogDB.dXLog  135MB/135MB  (100.0%)
> 2021-06-17T10:51:26.821+0800    no indexes to restore
> 2021-06-17T10:51:26.821+0800    finished restoring dxLogDB.dXLog (63481 documents, 0 failures)
> 2021-06-17T10:51:26.821+0800    64027 document(s) restored successfully. 0 document(s) failed to restore.