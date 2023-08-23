## 一，关系图解

![](https://static001.geekbang.org/infoq/3b/3b1b36a0cb4e57e270e72f23a83c68c8.png)



## 1.Config Servers as a 3 member replica set.

> Config servers store metadata and configuration settings for the cluster.
>
> 配置服务存储集群的 原数据（解释数据的数据）和配置设置。
>
>  As of MongoDB 3.4, config servers must be deployed as a **replica set** (CSRS).
>
> 3.4 版本之后配置方式必须以   replica set  部署。



## 2.Shard as a 3 member replica set

> Each shard contains a subset of the sharded data. 
>
> 每一个成员包括分配数据的子集。 
>
> A shard contains a subset of sharded data for a sharded cluster. Together, the cluster's shards hold the entire data set for the cluster. 
>
> 每一个分配是分片数据的自己。集群拥有正数据结合。
>
> As of MongoDB 3.6, shards must be deployed as a replica set.

*MongoDB does not guarantee that any two contiguous **chunks** reside on a single shard.*

### 2.1.Primary Shard

> Each database in a sharded cluster has a primary shard that holds all the un-sharded collections for that database. 
>
> 每一个数据库在集群中 主分片包括全部数据。
>
> Each database has its own primary shard. The primary shard has no relation to the primary in a replica set.
>
> 每一个数据库的主分片，这个主分片与Replica集合中的主结点没有关系。

![](https://static001.geekbang.org/infoq/02/02e3a980a93acf981e123405e7ff49e5.png)

> The mongos selects the primary shard when creating a new database by picking the shard in the cluster that has the least amount of data.
>
> 数据库创建时就会选择一个分片作为主分片。



## 3.One or more mongos routers.

> The mongos acts as a query router, providing an interface between client applications and the sharded cluster. 
>
> Client与集群之间的路由转发。
>
> Starting in MongoDB 4.4, mongos can support hedged reads to minimize latencies.

![](https://static001.geekbang.org/infoq/ae/aec1ae4ac37834d64fcad9daa15c4aa8.png) 



## 4.Replica set

> A replica set in MongoDB is a group of mongod processes that maintain the same data set. 
>
> 一个包含相同数据的集合。
>
> Replica sets provide redundancy and high availability, and are the basis for all production deployments. 
>
> 

![](https://static001.geekbang.org/infoq/e2/e2781c7328833fce43d499c87fd55346.png)

## 5.Chunk

> A contiguous range of shard key values within a particular shard. 
>
> Chunk ranges are inclusive of the lower boundary and exclusive of the upper boundary. 
>
> MongoDB splits chunks when they grow beyond the configured chunk size, which by default is 64 megabytes. 
>
> MongoDB migrates chunks when a shard contains too many chunks of a collection relative to other shards. 
>
> See Data Partitioning with Chunks and Sharded Cluster Balancer.











