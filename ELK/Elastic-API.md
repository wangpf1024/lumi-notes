# Elastic-API

##  基础篇

### 基本概念索引文档和RESTAPI

```html
#查看索引相关信息
GET kibana_sample_data_ecommerce

#查看索引的文档总数
GET kibana_sample_data_ecommerce/_count

#查看前10条文档，了解文档格式
POST kibana_sample_data_ecommerce/_search
{
}

#_cat indices API
#查看indices
GET /_cat/indices/kibana*?v&s=index

#查看状态为绿的索引
GET /_cat/indices?v&health=green

#按照文档个数排序
GET /_cat/indices?v&s=docs.count:desc

#查看具体的字段
GET /_cat/indices/kibana*?pri&v&h=health,index,pri,rep,docs.count,mt

#How much memory is used per index?
GET /_cat/indices?v&h=i,tm&s=tm:desc

#相关阅读
- 为什么不再支持单个Index下，多个Tyeps https://www.elastic.co/cn/blog/moving-from-types-to-typeless-apis-in-elasticsearch-7-0
- CAT Index API https://www.elastic.co/guide/en/elasticsearch/reference/7.1/cat-indices.html 
```

### 节点，集群，分片及副本

```html
get _cat/nodes?v
GET /_nodes/es7_01,es7_02
GET /_cat/nodes?v
GET /_cat/nodes?v&h=id,ip,port,v,m


GET _cluster/health
GET _cluster/health?level=shards
GET /_cluster/health/kibana_sample_data_ecommerce,kibana_sample_data_flights
GET /_cluster/health/kibana_sample_data_flights?level=shards

#### cluster state
The cluster state API allows access to metadata representing the state of the whole cluster. This includes information such as
GET /_cluster/state

#cluster get settings
GET /_cluster/settings
GET /_cluster/settings?include_defaults=true

GET _cat/shards
GET _cat/shards?h=index,shard,prirep,state,unassigned.reason

#### 相关阅读

- CAT Nodes API https://www.elastic.co/guide/en/elasticsearch/reference/7.1/cat-nodes.html
- Cluster API https://www.elastic.co/guide/en/elasticsearch/reference/7.1/cluster.html
- CAT Shards API https://www.elastic.co/guide/en/elasticsearch/reference/7.1/cat-shards.html
```

### 文档的基本 CRUD 与批量操作

```html
############Create Document############
#create document. 自动生成 _id
POST users/_doc
{
	"user" : "Mike",
    "post_date" : "2019-04-15T14:12:12",
    "message" : "trying out Kibana"
}

#create document. 指定Id。如果id已经存在，报错
PUT users/_doc/1?op_type=create
{
    "user" : "Jack",
    "post_date" : "2019-05-15T14:12:12",
    "message" : "trying out Elasticsearch"
}

#create document. 指定 ID 如果已经存在，就报错
PUT users/_create/1
{
     "user" : "Jack",
    "post_date" : "2019-05-15T14:12:12",
    "message" : "trying out Elasticsearch"
}

### Get Document by ID
#Get the document by ID
GET users/_doc/1


###  Index & Update
#Update 指定 ID  (先删除，在写入)
GET users/_doc/1

PUT users/_doc/1
{
	"user" : "Mike"

}


#GET users/_doc/1
#在原文档上增加字段
POST users/_update/1/
{
    "doc":{
        "post_date" : "2019-05-15T14:12:12",
        "message" : "trying out Elasticsearch"
    }
}



### Delete by Id
# 删除文档
DELETE users/_doc/1


### Bulk 操作
#执行两次，查看每次的结果

#执行第1次
POST _bulk
{ "index" : { "_index" : "test", "_id" : "1" } }
{ "field1" : "value1" }
{ "delete" : { "_index" : "test", "_id" : "2" } }
{ "create" : { "_index" : "test2", "_id" : "3" } }
{ "field1" : "value3" }
{ "update" : {"_id" : "1", "_index" : "test"} }
{ "doc" : {"field2" : "value2"} }


#执行第2次
POST _bulk
{ "index" : { "_index" : "test", "_id" : "1" } }
{ "field1" : "value1" }
{ "delete" : { "_index" : "test", "_id" : "2" } }
{ "create" : { "_index" : "test2", "_id" : "3" } }
{ "field1" : "value3" }
{ "update" : {"_id" : "1", "_index" : "test"} }
{ "doc" : {"field2" : "value2"} }

### mget 操作
GET /_mget
{
    "docs" : [
        {
            "_index" : "test",
            "_id" : "1"
        },
        {
            "_index" : "test",
            "_id" : "2"
        }
    ]
}


#URI中指定index
GET /test/_mget
{
    "docs" : [
        {

            "_id" : "1"
        },
        {

            "_id" : "2"
        }
    ]
}


GET /_mget
{
    "docs" : [
        {
            "_index" : "test",
            "_id" : "1",
            "_source" : false
        },
        {
            "_index" : "test",
            "_id" : "2",
            "_source" : ["field3", "field4"]
        },
        {
            "_index" : "test",
            "_id" : "3",
            "_source" : {
                "include": ["user"],
                "exclude": ["user.location"]
            }
        }
    ]
}

### msearch 操作
POST kibana_sample_data_ecommerce/_msearch
{}
{"query" : {"match_all" : {}},"size":1}
{"index" : "kibana_sample_data_flights"}
{"query" : {"match_all" : {}},"size":2}


### 清除测试数据
#清除数据
DELETE users
DELETE test
DELETE test2

### 相关阅读
- Document API https://www.elastic.co/guide/en/elasticsearch/reference/7.1/docs.html
```
### 倒排索引

```html
POST _analyze
{
  "analyzer": "standard",
  "text": "Mastering Elasticsearch"
}

POST _analyze
{
  "analyzer": "standard",
  "text": "Elasticsearch Server"
}

POST _analyze
{
  "analyzer": "standard",
  "text": "Elasticsearch Essentials"
}

#### 相关阅读

- https://zh.wikipedia.org/wiki/%E5%80%92%E6%8E%92%E7%B4%A2%E5%BC%95
- https://www.elastic.co/guide/cn/elasticsearch/guide/current/inverted-index.html

```

###  分析器进行分词

```html
#Simple Analyzer – 按照非字母切分（符号被过滤），小写处理
#Stop Analyzer – 小写处理，停用词过滤（the，a，is）
#Whitespace Analyzer – 按照空格切分，不转小写
#Keyword Analyzer – 不分词，直接将输入当作输出
#Patter Analyzer – 正则表达式，默认 \W+ (非字符分隔)
#Language – 提供了30多种常见语言的分词器
#2 running Quick brown-foxes leap over lazy dogs in the summer evening

#查看不同的analyzer的效果
#standard
GET _analyze
{
  "analyzer": "standard",
  "text": "2 running Quick brown-foxes leap over lazy dogs in the summer evening."
}

#simpe
GET _analyze
{
  "analyzer": "simple",
  "text": "2 running Quick brown-foxes leap over lazy dogs in the summer evening."
}


GET _analyze
{
  "analyzer": "stop",
  "text": "2 running Quick brown-foxes leap over lazy dogs in the summer evening."
}


#stop
GET _analyze
{
  "analyzer": "whitespace",
  "text": "2 running Quick brown-foxes leap over lazy dogs in the summer evening."
}

#keyword
GET _analyze
{
  "analyzer": "keyword",
  "text": "2 running Quick brown-foxes leap over lazy dogs in the summer evening."
}

GET _analyze
{
  "analyzer": "pattern",
  "text": "2 running Quick brown-foxes leap over lazy dogs in the summer evening."
}


#english
GET _analyze
{
  "analyzer": "english",
  "text": "2 running Quick brown-foxes leap over lazy dogs in the summer evening."
}


POST _analyze
{
  "analyzer": "icu_analyzer",
  "text": "他说的确实在理”"
}


POST _analyze
{
  "analyzer": "standard",
  "text": "他说的确实在理”"
}


POST _analyze
{
  "analyzer": "icu_analyzer",
  "text": "这个苹果不大好吃"
}

#### 相关阅读

- https://www.elastic.co/guide/en/elasticsearch/reference/7.1/indices-analyze.html
- https://www.elastic.co/guide/en/elasticsearch/reference/current/analyzer-anatomy.html

```

###  URI Search

```html
#URI Query
GET kibana_sample_data_ecommerce/_search?q=customer_first_name:Eddie
GET kibana*/_search?q=customer_first_name:Eddie
GET /_all/_search?q=customer_first_name:Eddie


#REQUEST Body
POST kibana_sample_data_ecommerce/_search
{
	"profile": true,
	"query": {
		"match_all": {}
	}
}

#基本查询
GET /movies/_search?q=2012&df=title&sort=year:desc&from=0&size=10&timeout=1s

#带profile
GET /movies/_search?q=2012&df=title
{
	"profile":"true"
}


#泛查询，正对_all,所有字段
GET /movies/_search?q=2012
{
	"profile":"true"
}

#指定字段
GET /movies/_search?q=title:2012&sort=year:desc&from=0&size=10&timeout=1s
{
	"profile":"true"
}


# 查找美丽心灵, Mind为泛查询
GET /movies/_search?q=title:Beautiful Mind
{
	"profile":"true"
}

# 泛查询
GET /movies/_search?q=title:2012
{
	"profile":"true"
}

#使用引号，Phrase查询
GET /movies/_search?q=title:"Beautiful Mind"
{
	"profile":"true"
}

#分组，Bool查询
GET /movies/_search?q=title:(Beautiful Mind)
{
	"profile":"true"
}


#布尔操作符
# 查找美丽心灵
GET /movies/_search?q=title:(Beautiful AND Mind)
{
	"profile":"true"
}

# 查找美丽心灵
GET /movies/_search?q=title:(Beautiful NOT Mind)
{
	"profile":"true"
}

# 查找美丽心灵
GET /movies/_search?q=title:(Beautiful %2BMind)
{
	"profile":"true"
}


#范围查询 ,区间写法
GET /movies/_search?q=title:beautiful AND year:[2002 TO 2018%7D
{
	"profile":"true"
}


#通配符查询
GET /movies/_search?q=title:b*
{
	"profile":"true"
}

//模糊匹配&近似度匹配
GET /movies/_search?q=title:beautifl~1
{
	"profile":"true"
}

GET /movies/_search?q=title:"Lord Rings"~2
{
	"profile":"true"
}


#### 相关阅读

- https://www.elastic.co/guide/en/elasticsearch/reference/7.1/search-search.html
- https://searchenginewatch.com/sew/news/2065080/search-engines-101
- https://www.huffpost.com/entry/search-engines-101-part-i_b_1104525
- https://www.entrepreneur.com/article/176398
- https://www.searchtechnologies.com/meaning-of-relevancy
-https://baike.baidu.com/item/%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8E%E5%8F%91%E5%B1%95%E5%8F%B2/2422574

- https://www.elastic.co/guide/en/elasticsearch/reference/7.0/search-uri-request.html
- https://www.elastic.co/guide/en/elasticsearch/reference/7.0/search-search.html
```

### Request Body 与 Query DSL

```html
#ignore_unavailable=true，可以忽略尝试访问不存在的索引“404_idx”导致的报错
#查询movies分页
POST /movies,404_idx/_search?ignore_unavailable=true
{
  "profile": true,
	"query": {
		"match_all": {}
	}
}

POST /kibana_sample_data_ecommerce/_search
{
  "from":10,
  "size":20,
  "query":{
    "match_all": {}
  }
}


#对日期排序
POST kibana_sample_data_ecommerce/_search
{
  "sort":[{"order_date":"desc"}],
  "query":{
    "match_all": {}
  }

}

#source filtering
POST kibana_sample_data_ecommerce/_search
{
  "_source":["order_date"],
  "query":{
    "match_all": {}
  }
}


#脚本字段
GET kibana_sample_data_ecommerce/_search
{
  "script_fields": {
    "new_field": {
      "script": {
        "lang": "painless",
        "source": "doc['order_date'].value+'hello'"
      }
    }
  },
  "query": {
    "match_all": {}
  }
}


POST movies/_search
{
  "query": {
    "match": {
      "title": "last christmas"
    }
  }
}

POST movies/_search
{
  "query": {
    "match": {
      "title": {
        "query": "last christmas",
        "operator": "and"
      }
    }
  }
}

POST movies/_search
{
  "query": {
    "match_phrase": {
      "title":{
        "query": "one love"

      }
    }
  }
}

POST movies/_search
{
  "query": {
    "match_phrase": {
      "title":{
        "query": "one love",
        "slop": 1

      }
    }
  }
}

#### 相关阅读
- https://www.elastic.co/guide/en/elasticsearch/reference/7.0/search-uri-request.html
- https://www.elastic.co/guide/en/elasticsearch/reference/7.0/search-search.html
```

### Query & Simple Query String Query

```html
PUT /users/_doc/1
{
  "name":"Ruan Yiming",
  "about":"java, golang, node, swift, elasticsearch"
}

PUT /users/_doc/2
{
  "name":"Li Yiming",
  "about":"Hadoop"
}


POST users/_search
{
  "query": {
    "query_string": {
      "default_field": "name",
      "query": "Ruan AND Yiming"
    }
  }
}


POST users/_search
{
  "query": {
    "query_string": {
      "fields":["name","about"],
      "query": "(Ruan AND Yiming) OR (Java AND Elasticsearch)"
    }
  }
}


#Simple Query 默认的operator是 Or
POST users/_search
{
  "query": {
    "simple_query_string": {
      "query": "Ruan AND Yiming",
      "fields": ["name"]
    }
  }
}


POST users/_search
{
  "query": {
    "simple_query_string": {
      "query": "Ruan Yiming",
      "fields": ["name"],
      "default_operator": "AND"
    }
  }
}


GET /movies/_search
{
	"profile": true,
	"query":{
		"query_string":{
			"default_field": "title",
			"query": "Beafiful AND Mind"
		}
	}
}


# 多fields
GET /movies/_search
{
	"profile": true,
	"query":{
		"query_string":{
			"fields":[
				"title",
				"year"
			],
			"query": "2012"
		}
	}
}



GET /movies/_search
{
	"profile":true,
	"query":{
		"simple_query_string":{
			"query":"Beautiful +mind",
			"fields":["title"]
		}
	}
}
```