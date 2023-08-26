参考文档：https://www.elastic.co/guide/en/elasticsearch/reference/8.7/targz.html


## ELK

> 软件包版本应保持一致


###  Elastic

```shell
./bin/elasticsearch -E node.name=node0 -E cluster.name=geektime -E path.data=node0_data -d
./bin/elasticsearch -E node.name=node1  -E cluster.name=geektime -E path.data=node1_data -d
./bin/elasticsearch -E node.name=node2 -E cluster.name=geektime -E path.data=node2_data -d
./bin/elasticsearch -E node.name=node3 -E cluster.name=geektime -E path.data=node3_data -d
```

###  验证集群

#### 更新秘密
```shell
bin/elasticsearch-reset-password -u elastic
```

#### 集群信息

```shell
curl --cacert ./config/certs/http_ca.crt -u elastic https://localhost:9200
```

#### 输出结果
```json
{
  "name" : "node0",
  "cluster_name" : "geektime",
  "cluster_uuid" : "uneVb-yQT6CzFZhxAHNrag",
  "version" : {
  "number" : "8.7.1",
  "build_flavor" : "default",
  "build_type" : "tar",
  "build_hash" : "f229ed3f893a515d590d0f39b05f68913e2d9b53",
  "build_date" : "2023-04-27T04:33:42.127815583Z",
  "build_snapshot" : false,
  "lucene_version" : "9.5.0",
  "minimum\_wire\_compatibility_version" : "7.17.0",
  "minimum\_index\_compatibility_version" : "7.0.0"
  },
  "tagline" : "You Know, for Search"
}
```

#### 安装分词插件

```shell
./elasticsearch-8.7.1/bin/elasticsearch-plugin install analysis-icu
```

###  kibana

#### 启动

```shell
./bin/kibana
```

#### 配置kibana用户

```json
## error msg with kibana_system
You do not have permission to access the requested page
## 
The [kibana_system](https://www.elastic.co/guide/en/elasticsearch/reference/current/built-in-users.html) is a built-in role used to connect to Elasticsearch. It's not intended as a login to Kibana. I would login to Kibana using the elastic account and then create a new user and then use that one.
```


###  Logstash

下载地址 ： https://www.elastic.co/downloads/past-releases/logstash-8-7-1

### 测试数据集

下载地址 ： https://grouplens.org/datasets/movielens/

#### 编辑logstash倒入数据集配置文件

```powershell
vi  logstash.conf
```

```yaml
# movies.csv must copy to bin dir
input {
  file {
  path => "/Users/arvin/workspace/soft/logstash-8.7.1/bin/movies.csv"
  start_position => "beginning"
  sincedb_path => "/dev/null"
  }
}

filter {
  csv {
  separator => ","
  columns => \["id","content","genre"\]
  }
  mutate {
  split => { "genre" => "|" }
  remove_field => \["path", "host","@timestamp","message"\]
  }
  mutate {
  split => \["content", "("\]
  add_field => { "title" => "%{\[content\]\[0\]}"}
  add_field => { "year" => "%{\[content\]\[1\]}"}
  }
  mutate {
  convert => {
  "year" => "integer"
  }
  strip => \["title"\]
  remove_field => \["path", "host","@timestamp","message","content"\]
  }
}

output {
  elasticsearch {
  hosts => "https://localhost:9200"
  index => "movies"
  user => "elastic"
  password => "iHwXTgHDaUmje440Sea6"
  ssl\_certificate\_verification => false
  document_id => "%{id}"
  }
  stdout {}
}
```

#### 倒入数据

```shell
 sudo ./bin/logstash -f ./logstash.conf
```
