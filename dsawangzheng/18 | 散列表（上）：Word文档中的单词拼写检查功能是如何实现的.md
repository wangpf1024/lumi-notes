### 假设我们有 10 万条 URL 访问日志，如何按照访问次数给 URL 排序？



1. **统计访问次数：** 遍历日志文件，统计每个URL的访问次数。可以使用一个数据结构，如哈希表（HashMap），其中URL作为键，访问次数作为值。
2. **排序：** 将统计得到的URL及其访问次数进行排序。可以使用任何常见的排序算法，如快速排序、归并排序等。排序的依据是访问次数，从高到低排序。
3. **输出结果：** 将排序后的URL及其访问次数输出或存储，以便进一步使用。



```java
package com.controller.bootweb.demo.dsa;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class URLSorter {

    public static void main(String[] args) {
        String logFilePath = "/Users/arvin/workspace/logs/access.log";

        try {
            Map<String, Integer> urlCounts = countURLVisits(logFilePath);
            List<Map.Entry<String, Integer>> sortedURLs = sortURLsByVisits(urlCounts);

            // 输出排序后的结果
            for (Map.Entry<String, Integer> entry : sortedURLs) {
                System.out.println(entry.getKey() + ": " + entry.getValue() + " visits");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // 读取日志文件并统计访问次数
    private static Map<String, Integer> countURLVisits(String logFilePath) throws IOException {
        Map<String, Integer> urlCounts = new HashMap<>();

        try (BufferedReader reader = new BufferedReader(new FileReader(logFilePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String url = extractURL(line);
                urlCounts.put(url, urlCounts.getOrDefault(url, 0) + 1);
            }
        }

        return urlCounts;
    }

    // 提取URL的辅助函数
    private static String extractURL(String logLine) {
        // 根据日志格式提取URL
        // 示例：假设日志行的格式是 "timestamp URL"
        String[] parts = logLine.split("https://", 2);
        return parts.length > 1 ? parts[1].trim() : "";
    }

    // 排序URL及其访问次数
    private static List<Map.Entry<String, Integer>> sortURLsByVisits(Map<String, Integer> urlCounts) {
        return urlCounts.entrySet()
                .stream()
                .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
                .collect(Collectors.toList());
    }
}
```

**access.log**

```
2022-01-01 10:00:00.000 https://time.geekbang.com/column/article/64586
2022-01-01 10:01:00.000 https://time.geekbang.com/column/article/64586
2022-01-01 10:02:00.000 https://time.geekbang.com/column/article/64586
2022-01-01 10:03:00.000 https://time.geekbang.com/column/article/64586
2022-01-01 10:04:00.000 https://time.geekbang.com/column/article/64522
2022-01-01 10:05:00.000 https://time.geekbang.com/column/article/64512
2022-01-01 10:06:00.000 https://time.geekbang.com/column/article/64586
2022-01-01 10:07:00.000 https://time.geekbang.com/column/article/64512
2022-01-01 10:09:00.000 https://time.geekbang.com/column/article/64186
```



### 有两个字符串数组，每个数组大约有 10 万条字符串，如何快速找出两个数组中相同的字符串？



```java
package com.controller.bootweb.demo.dsa;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

public class CommonStringsFinder {

    public static void main(String[] args) {
        // 两个字符串数组
        String[] array1 = {"apple", "banana", "orange", "kiwi", "grape"};
        String[] array2 = {"kiwi", "grape", "watermelon", "apple", "pear"};

        // 找出相同的字符串
        String[] commonStrings = findCommonStrings(array1, array2);

        // 输出结果
        System.out.println("Common Strings: " + Arrays.toString(commonStrings));
    }

    // 找出两个数组中相同的字符串
    private static String[] findCommonStrings(String[] array1, String[] array2) {
        Set<String> set1 = new HashSet<>(Arrays.asList(array1));
        Set<String> set2 = new HashSet<>(Arrays.asList(array2));

        // 使用retainAll方法找出两个集合的交集
        set1.retainAll(set2);

        // 将交集转换为数组
        String[] commonStrings = set1.toArray(new String[0]);

        return commonStrings;
    }
}
```