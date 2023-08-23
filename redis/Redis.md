

**虚拟机：172.27.2.38，63，48 重复 1，2，3 操作 ，4，5 选择一台执行即可**

## 1.下载安装

```shell
# 下载 redis 包 并进行编译安装
wget https://download.redis.io/releases/redis-4.0.9.tar.gz
tar xzf redis-4.0.9.tar.gz
cd redis-4.0.9
make
```

## 2.编辑节点配置文件

```shell
# 编辑启用 redis 集群 ----- 共 3台虚拟机 部署策略 3 主，3 从 每台集群有2个节点 端口为 7000 7001
vi redis.conf
```

```shell
# 具体配置项 每个虚拟机两个节点
port 7000
cluster-enabled yes
cluster-config-file nodes-7000.conf
cluster-node-timeout 5000
appendonly yes
```

```shell
# 为方便配置文件区分，建立两个文件夹存放 redis.conf
mkdir 7000
cp redis.conf ./7000

mkdir 7001
cp redis.conf ./7001
```

```shell
#修改netip绑定
vi  ./7000/redis.conf
bind 172.27.3.38
port 7000
cluster-config-file nodes-7000.conf

vi  ./7001/redis.conf
bind 172.27.3.38
port 7001
cluster-config-file nodes-7001.conf
```

## 3.启动每一个节点

```shell
# 启动 3台机器重复以上操作
./src/redis-server ./7000/redis.conf &
./src/redis-server ./7001/redis.conf &
```

## 4.安装 gem

```shell
# for redis 3 or 4 为 执行 ./redis-trib.rb 前置条件 ----- 只需要在单一集群上执行即可
gem install redis
```

```shell
# have to update Ruby >=2.3.0 （安装or升级Ruby）
# https://www.ruby-lang.org/en/documentation/installation/#ruby-install
# https://www.ruby-lang.org/en/downloads/
# Ruby 2.7.3
$ ./configure
$ make
$ make install
```

## 5.创建集群

```shell
# 创建集群
./redis-trib.rb create --replicas 1 172.27.2.38:7000 172.27.2.38:7001 172.27.2.48:7000 172.27.2.48:7001 172.27.2.63:7000 172.27.2.63:7001
```

> Creating cluster
> Performing hash slots allocation on 6 nodes...
> Using 3 **masters**:
> 172.27.2.38:7000
> 172.27.2.48:7000
> 172.27.2.63:7000
> **Adding replica 172.27.2.48:7001 to 172.27.2.38:7000**
> **Adding replica 172.27.2.63:7001 to 172.27.2.48:7000**
> **Adding replica 172.27.2.38:7001 to 172.27.2.63:7000**
> **M:** 43837d1bc6faadd823a713e2a79670f2f7ea3f31 172.27.2.38:7000
> slots:**0-5460** (5461 slots) master
> S: 76efe3a8c213e27535648d5caa2bb9b881ebdcd5 172.27.2.38:7001
> replicates 9cd922525743bc14905016d0c4a66f13787dde58
> **M:** 87e91840bb2335a5b881d530ef97ebc9f89340fc 172.27.2.48:7000
> slots:**5461-10922** (5462 slots) master
> S: f187e478fd9c40e6c4b0ab66435e6bd7bbe46056 172.27.2.48:7001
> replicates 43837d1bc6faadd823a713e2a79670f2f7ea3f31
> **M:** 9cd922525743bc14905016d0c4a66f13787dde58 172.27.2.63:7000
> slots:**10923-16383** (5461 slots) master
> S: e3a3861e7d491deb38176556d3a5a4c903dc5446 172.27.2.63:7001
> replicates 87e91840bb2335a5b881d530ef97ebc9f89340fc
> Can I set the above configuration? (type 'yes' to accept): yes
> Nodes configuration updated
> **Assign a different config epoch to each node**
> Sending CLUSTER MEET messages to join the cluster
> Waiting for the cluster to join....
> Performing Cluster Check (using node 172.27.2.38:7000)
> **M: 43837d1bc6faadd823a713e2a79670f2f7ea3f31 172.27.2.38:7000**
> slots:0-5460 (5461 slots) master
> 1 additional replica(s)
> **M: 87e91840bb2335a5b881d530ef97ebc9f89340fc 172.27.2.48:7000**
> slots:5461-10922 (5462 slots) master
> 1 additional replica(s)
> ***S:** 76efe3a8c213e27535648d5caa2bb9b881ebdcd5 172.27.2.38:7001*
> slots: (0 slots) slave
> replicates 9cd922525743bc14905016d0c4a66f13787dde58
> ***S:** e3a3861e7d491deb38176556d3a5a4c903dc5446 172.27.2.63:7001*
> slots: (0 slots) slave
> replicates 87e91840bb2335a5b881d530ef97ebc9f89340fc
> **M: 9cd922525743bc14905016d0c4a66f13787dde58 172.27.2.63:7000**
> slots:10923-16383 (5461 slots) master
> 1 additional replica(s)
> ***S:** f187e478fd9c40e6c4b0ab66435e6bd7bbe46056 172.27.2.48:7001*
> slots: (0 slots) slave
> replicates 43837d1bc6faadd823a713e2a79670f2f7ea3f31
> [OK] All nodes agree about slots configuration.
> Check for open slots...
> Check slots coverage...
> [OK] All 16384 slots covered.
