参考文档 ：https://www.kafka-eagle.org/articles/docs/installation/linux-macos.html

下载地址：http://www.kafka-eagle.org/articles/docs/changelog/changelog.html

## 1.下载软件包

```shell
wget https://github.com/smartloli/kafka-eagle-bin/archive/v2.0.5.tar.gz
# 参考文档安装即可
```

## 2.修改kafka脚本（optional）

```shell
# 修改kafka 启动脚本 --- （解决 kafka 相关节点获取 内存使用信息） 需要重启服务
vi bin/kafka-server-start.sh 

if [ "x$KAFKA_HEAP_OPTS" = "x" ]; then
    export KAFKA_HEAP_OPTS="-Xmx1G -Xms1G"
    #新增JMX端口
    export JMX_PORT="9099"
fi

```

