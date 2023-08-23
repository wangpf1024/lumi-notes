## 第一步：登陆 Redis 集群

```shell
./src/redis-cli -h 172.27.2.48 -p 7000 -c
```

## 第二步：评估 Key Size 

### 方法一

```shell
# An iteration starts when the cursor is set to 0,
# 从 0 开始 
# and terminates when the cursor returned by the server is 0
# 返回 0 结束
172.27.2.48:7000> scan 0 match DX*

1) "25"

2) 1) "DX:system:organize:id:9ba98f2b8d494481958e6cfb23509cb9"

  2) "DX:login:token:zhenghao1"

  3) "DX:system:clientgroup:list:"

  4) "DX-"

  5) "DX:system:merchant:label:list:116649c6e87842abaea98323faf38a40"

  6) "DX:system:client:id17b5ee770a5a424ba9a777ff73343fc0"

  7) "DX:system:client:idde28b78188f4474d9191889c9fb94a0e"

  8) "DX:COMMON_BASESIMPLE:ALL"

  9) "DX:system:organize:id:8fae95fdddb94527ba6999292127d646"

  10) "DX:system:organize:id:fb2dca3aefb0418daf97e2d88bef05aa"

172.27.2.48:7000> scan 25 match DX*

1) "7"

2) 1) "DX:login:token:a3116e0bc89e48c99454d9fe4a1e37fd"

  2) "DX:system:organize:id:218db5ead84c42ecbef734f332384a3b"

  3) "DX:login:token:34a7c984aa82488b9fab850304eb71c7"

  4) "DX:login:token:17b5ee770a5a424ba9a777ff73343fc0"

  5) "DX:system:client:ida3116e0bc89e48c99454d9fe4a1e37fd"

  6) "DX:system:organize:id:2619dea09af94b8db64536e3dd950800"

  7) "DX:system:client:id20e2016306b246ed872b44efbb02d6c4"

  8) "DX:TTHIRD_CLIENTROUT"

  9) "DX:system:merchant:list"

  10) "DX:login:token:de28b78188f4474d9191889c9fb94a0e"

  11) "DX:system:organize:id:2a58244e3c3a494f9aebe4bb84ce96e6"

172.27.2.48:7000> scan 7 match DX*

1) "0"

2) 1) "DX:CLIENTROUT"
```

### 方法二

```shell
#Basically with COUNT the user specified the amount of work that should be done at every call in order to retrieve elements from the collection
# 指定查询数量
172.27.2.48:7000> scan 0 match DX* COUNT 1000
1) "0"
2)  1) "DX:system:organize:id:9ba98f2b8d494481958e6cfb23509cb9"
    2) "DX:login:token:zhenghao1"
    3) "DX:system:clientgroup:list:"
    4) "DX-"
    5) "DX:system:merchant:label:list:116649c6e87842abaea98323faf38a40"
    6) "DX:system:client:id17b5ee770a5a424ba9a777ff73343fc0"
    7) "DX:system:client:idde28b78188f4474d9191889c9fb94a0e"
    8) "DX:COMMON_BASESIMPLE:ALL"
    9) "DX:system:organize:id:8fae95fdddb94527ba6999292127d646"
   10) "DX:system:organize:id:fb2dca3aefb0418daf97e2d88bef05aa"
   11) "DX:login:token:a3116e0bc89e48c99454d9fe4a1e37fd"
   12) "DX:system:organize:id:218db5ead84c42ecbef734f332384a3b"
   13) "DX:login:token:34a7c984aa82488b9fab850304eb71c7"
   14) "DX:login:token:17b5ee770a5a424ba9a777ff73343fc0"
   15) "DX:system:client:ida3116e0bc89e48c99454d9fe4a1e37fd"
   16) "DX:system:organize:id:2619dea09af94b8db64536e3dd950800"
   17) "DX:system:client:id20e2016306b246ed872b44efbb02d6c4"
   18) "DX:TTHIRD_CLIENTROUT"
   19) "DX:system:merchant:list"
   20) "DX:login:token:de28b78188f4474d9191889c9fb94a0e"
   21) "DX:system:organize:id:2a58244e3c3a494f9aebe4bb84ce96e6"
   22) "DX:CLIENTROUT"
```

## 第三步：导出 KEYS

```shell
# 通过第一，二步骤确认可以数量
# 创建目录
mkdir dumpfiles

#导出keys值
./src/redis-cli -h 172.27.2.48 -p 7000 -c --scan 0 match DX* COUNT 1000 > ./dumpfiles/keys.txt
```

## 第四步：DUMP KEY

```shell l
# 创建 key 只目录
mkdir dumpfiles/files
```

```shell
# 创建到处脚步 dumpfilesBykeys.sh

#!/bin/bash
#要遍历的key
for key in `cat ./dumpfiles/keys.txt`
do
 echo "读取key --------> $key"
 # head -c -1 打印除最后 INT 字节以外的全部字节。去掉dump文件里的最后一个字符'\n'
 ./src/redis-cli -h 172.27.2.48 -p 7000 -c --raw dump $key |head -c -1 > ./dumpfiles/files/$key
done
```

## 第五步：Restore KEY

```shell
# 创建倒入脚步 restoreBykeys.sh

#!/bin/bash
#要遍历的key
for key in `cat ./dumpfiles/keys.txt`
do
 echo "恢复 key --------> $key"
 cat ./dumpfiles/files/$key | ./src/redis-cli -h 172.17.33.45 -p 7000 -c -x restore $key 0
done
```

## 第六步：查看输出结果

```shell
恢复 key --------> DX:system:organize:id:9ba98f2b8d494481958e6cfb23509cb9
OK
恢复 key --------> DX:login:token:zhenghao1
OK
恢复 key --------> DX:system:clientgroup:list:
OK
恢复 key --------> DX-
OK
恢复 key --------> DX:system:merchant:label:list:116649c6e87842abaea98323faf38a40
OK
恢复 key --------> DX:system:client:id17b5ee770a5a424ba9a777ff73343fc0
OK
恢复 key --------> DX:system:client:idde28b78188f4474d9191889c9fb94a0e
OK
恢复 key --------> DX:COMMON_BASESIMPLE:ALL
OK
恢复 key --------> DX:system:organize:id:8fae95fdddb94527ba6999292127d646
OK
恢复 key --------> DX:system:organize:id:fb2dca3aefb0418daf97e2d88bef05aa
OK
恢复 key --------> DX:login:token:a3116e0bc89e48c99454d9fe4a1e37fd
OK
恢复 key --------> DX:system:organize:id:218db5ead84c42ecbef734f332384a3b
OK
恢复 key --------> DX:login:token:34a7c984aa82488b9fab850304eb71c7
OK
恢复 key --------> DX:login:token:17b5ee770a5a424ba9a777ff73343fc0
OK
恢复 key --------> DX:system:client:ida3116e0bc89e48c99454d9fe4a1e37fd
OK
恢复 key --------> DX:system:organize:id:2619dea09af94b8db64536e3dd950800
OK
恢复 key --------> DX:system:client:id20e2016306b246ed872b44efbb02d6c4
OK
恢复 key --------> DX:TTHIRD_CLIENTROUT
OK
恢复 key --------> DX:system:merchant:list
OK
恢复 key --------> DX:login:token:de28b78188f4474d9191889c9fb94a0e
OK
恢复 key --------> DX:system:organize:id:2a58244e3c3a494f9aebe4bb84ce96e6
OK
恢复 key --------> DX:CLIENTROUT
OK
```

# 测试环境运用

## 登陆Redis集群

```shell
./src/redis-cli -h 172.17.47.20 -p 7001 -c
```

```shell
# 评估 KEYS 大小 ---- 观察得出需要多次查询才可获取全部 key值
172.17.47.20:7001> scan 0 match DX* count 1000
1) "49344"
2) 1) "DX:login:token:283951e1f0c24eb9893a437742f358a2"
172.17.47.20:7001> 
```

```shell
#!/bin/bash
#要遍历的key

./src/redis-cli -h 172.17.47.20 -p 7001 -c scan 0 match DX* count 1000 | head -n 1 > ./dkrediskey/curent_index.txt
# 获取第一次遍历key值
./src/redis-cli -h 172.17.47.20 -p 7001 -c scan 0 match DX* count 1000 |grep 'DX' >> ./dkrediskey/keys.txt

a=0

while [ $a -eq 0 ]

do

for idx in `cat ./dkrediskey/curent_index.txt`

do

 echo "读取idx --------> $idx"

 ./src/redis-cli -h 172.17.47.20 -p 7001 -c scan $idx match DX* count 1000 |grep 'DX' >> ./dkrediskey/keys.txt

 if [ $idx -eq 0 ]
 then
    echo "完成"
    break 2
 else
  echo "."
  ./src/redis-cli -h 172.17.47.20 -p 7001 -c scan $idx match DX* count 1000 | head -n 1 > ./dkrediskey/curent_index.txt
 fi
done

sleep 1

done
```

```shell
# 创建到处脚步 dumpDKfilesBykeys.sh

#!/bin/bash
#要遍历的key
for key in `cat ./dkrediskey/keys.txt`
do
 echo "读取key --------> $key"
 # head -c -1 打印除最后 INT 字节以外的全部字节。去掉dump文件里的最后一个字符'\n'
 ./src/redis-cli -h 172.17.47.20 -p 7001 -c --raw dump $key |head -c -1 > ./dkrediskey/files/$key
done
```

```shell
# 创建倒入脚步 restoreDKBykeys.sh

#!/bin/bash
#要遍历的key
for key in `cat ./dkrediskey/keys.txt`
do
 echo "恢复 key --------> $key"
 cat ./dkrediskey/files/$key | ./src/redis-cli -h 172.17.33.45 -p 7000 -c -x restore $key 0
done
```

