现在你有 10 个接口访问日志文件，每个日志文件大小约 300MB，每个文件里的日志都是按照时间戳从小到大排序的。你希望将这 10 个较小的日志文件，合并为 1 个日志文件，合并之后的日志仍然按照时间戳从小到大排列。如果处理上述排序任务的机器内存只有 1GB，你有什么好的解决思路，能“快速”地将这 10 个日志文件合并吗？



### 步骤

1. **分割文件：** 将每个大日志文件拆分成小块，每块大小适应内存限制，例如，可以拆分成若干个小文件。

2. **小文件内排序：** 对每个小文件进行内部排序，确保每个小文件内的日志按照时间戳从小到大排列。

3. **合并有序文件：** 利用归并排序的思想，将所有小文件按照时间戳顺序合并为一个大文件。在这个过程中，可以利用优先队列（Priority Queue）来保持每个小文件当前处理的位置，确保合并后的文件依然有序。

4. **输出结果：** 得到一个合并后的大日志文件，其中的日志仍然按照时间戳从小到大排列。

### 注意事项

- 在进行文件合并时，需要逐步读取和写入数据，而不是一次性加载所有数据到内存中。
- 合并过程中可以采用多路归并（Multiway Merge）的策略，以降低对内存的需求。
- 在实际实现中，可以使用流式处理，逐行读取和写入数据，以降低内存占用。

这种外部排序的方法可以在有限的内存条件下，对大量日志文件进行高效合并和排序。



### Java 参考代码

```java
public class ExternalSort {

    public static void externalSort(List<File> files, File output) throws IOException {
        PriorityQueue<LogEntry> priorityQueue = new PriorityQueue<>(Comparator.comparing(LogEntry::getTimestamp));

        // Open readers for each file
        List<BufferedReader> readers = new ArrayList<>();
        for (File file : files) {
            BufferedReader reader = new BufferedReader(new FileReader(file));
            readers.add(reader);
            // Read the first line from each file and add it to the priority queue
            String line = reader.readLine();
            if (line != null) {
                LogEntry logEntry = new LogEntry(file.getAbsoluteFile().getAbsolutePath(), line);
                priorityQueue.add(logEntry);
            }
        }
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(output))) {
            // Process the priority queue until it's empty
            while (!priorityQueue.isEmpty()) {
                LogEntry currentEntry = priorityQueue.poll();
                writer.write(currentEntry.getLine());
                writer.newLine();
                // Read the next line from the file and add it to the priority queue
                BufferedReader currentReader = readers.get(files.indexOf(new File(currentEntry.getFileName())));
                String nextLine = currentReader.readLine();
                if (nextLine != null) {
                    LogEntry nextLogEntry = new LogEntry(currentEntry.getFileName(), nextLine);
                    priorityQueue.add(nextLogEntry);
                }
            }
        } finally {
            // Close all readers after processing
            for (BufferedReader reader : readers) {
                reader.close();
            }
        }
    }

    // LogEntry class to represent each log entry with timestamp and file name



    private static long getTimestamp(BufferedReader reader) throws IOException {
        String line = reader.readLine();
        if(StrUtil.isEmpty(line)){
            return 0;
        }
        String date = line.split(" \\[")[0];
        return DateUtil.parse(date).getTime();// Assuming timestamp is at the beginning of each log entry
    }

    public static void main(String[] args) {
        try {
            List<File> inputFiles = Arrays.asList(
                    new File("/Users/arvin/workspace/logs/log1.txt"),
                    new File("/Users/arvin/workspace/logs/log2.txt")
                    // Add other log files here
            );
            File outputFile = new File("/Users/arvin/workspace/logs/mergedLog.txt");

            externalSort(inputFiles, outputFile);

            System.out.println("Log files merged successfully!");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

class LogEntry {
    private final String fileName;
    private final String line;

    public LogEntry(String fileName, String line) {
        this.fileName = fileName;
        this.line = line;
    }

    public String getFileName() {
        return fileName;
    }

    public String getLine() {
        return line;
    }

    public long getTimestamp() {
        // Implement the logic to extract and return the timestamp from the log entry
        return 0;
    }
}
```



### Log1.txt

```java
2022-01-01 10:00:00.000 [main] DEBUG com.example.MyClass - This is a debug message.
2022-01-01 10:01:15.500 [main] INFO com.example.MyClass - Application started.
2022-01-01 10:05:30.200 [worker-1] WARN com.example.MyClass - Connection timeout.
2022-01-01 10:10:45.800 [main] ERROR com.example.MyClass - Critical error occurred.
2022-01-01 10:12:20.100 [main] INFO com.example.MyClass - Processing data for user: john_doe.
2022-01-01 10:15:05.300 [worker-2] DEBUG com.example.MyClass - Task completed successfully.
```

### Log2.txt

```java
2023-01-01 10:00:00.000 [main] DEBUG com.example.MyClass - This is a debug message.
2023-01-01 10:01:15.500 [main] INFO com.example.MyClass - Application started.
2023-01-01 10:05:30.200 [worker-1] WARN com.example.MyClass - Connection timeout.
2023-01-01 10:10:45.800 [main] ERROR com.example.MyClass - Critical error occurred.
2023-01-01 10:12:20.100 [main] INFO com.example.MyClass - Processing data for user: john_doe.
2023-01-01 10:15:05.300 [worker-2] DEBUG com.example.MyClass - Task completed successfully.
```

### mergedLog.txt

```java
2022-01-01 10:00:00.000 [main] DEBUG com.example.MyClass - This is a debug message.
2023-01-01 10:00:00.000 [main] DEBUG com.example.MyClass - This is a debug message.
2022-01-01 10:01:15.500 [main] INFO com.example.MyClass - Application started.
2023-01-01 10:01:15.500 [main] INFO com.example.MyClass - Application started.
2022-01-01 10:05:30.200 [worker-1] WARN com.example.MyClass - Connection timeout.
2023-01-01 10:05:30.200 [worker-1] WARN com.example.MyClass - Connection timeout.
2022-01-01 10:10:45.800 [main] ERROR com.example.MyClass - Critical error occurred.
2023-01-01 10:10:45.800 [main] ERROR com.example.MyClass - Critical error occurred.
2022-01-01 10:12:20.100 [main] INFO com.example.MyClass - Processing data for user: john_doe.
2023-01-01 10:12:20.100 [main] INFO com.example.MyClass - Processing data for user: john_doe.
2022-01-01 10:15:05.300 [worker-2] DEBUG com.example.MyClass - Task completed successfully.
2023-01-01 10:15:05.300 [worker-2] DEBUG com.example.MyClass - Task completed successfully.
```

