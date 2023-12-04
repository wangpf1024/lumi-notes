如果字符串是通过单链表来存储的，那该如何来判断是一个回文串呢？



回文串是一种具有特殊对称性质的字符串。它正着读和倒着读是一样的。换句话说，如果一个字符串从左到右读和从右到左读是一样的，那么它就是一个回文串。

例如，下面是一些回文串的示例：

- "level"
- "deed"
- "radar"
- "madam"
- "civic"

在回文串中，通常忽略非字母和非数字的字符，只考虑字母和数字字符，而且通常不区分大小写。

判断一个字符串是否是回文串是常见的编程问题，有多种方法可以实现，包括使用双指针、栈、递归等。



### 单链表方式：

1. **构建单链表：** 遍历字符串构建单链表，时间复杂度为 O(n)，其中 n 是字符串的长度。
2. **查找中点：** 使用快慢指针找到链表中点，时间复杂度为 O(n/2)，即 O(n)。
3. **反转后半部分链表：** 对后半部分链表进行反转，时间复杂度为 O(n/2)，即 O(n)。
4. **比较操作：** 比较前半部分和反转后的后半部分链表，时间复杂度为 O(n/2)，即 O(n)。

综合起来，单链表方式的时间复杂度为 O(n)。

在单链表中判断一个字符串是否是回文串，可以采用以下步骤：

1. **找到链表的中点：**
   - 使用快慢指针找到链表的中点。慢指针每次移动一个节点，而快指针每次移动两个节点，当快指针到达链表尾部时，慢指针指向链表中点。
2. **反转后半部分链表：**
   - 从中点开始，将后半部分链表进行反转。可以使用迭代或递归实现反转。
3. **比较前半部分和反转后的后半部分链表：**
   - 从链表头开始，与反转后的后半部分链表逐个比较节点的值。
   - 如果所有节点的值都相同，则字符串是回文串；否则，不是回文串。

```java
class ListNode {
    char val;
    ListNode next;

    public ListNode(char val) {
        this.val = val;
    }
}

public class PalindromeLinkedList {

    public static boolean isPalindrome(ListNode head) {
        if (head == null || head.next == null) {
            return true; // 空链表或只有一个节点的链表是回文串
        }

        // 使用快慢指针找到链表的中点
        ListNode slow = head;
        ListNode fast = head;

        while (fast != null && fast.next != null) {
            slow = slow.next;
            fast = fast.next.next;
        }

        // 反转后半部分链表
        ListNode reversedHalf = reverseLinkedList(slow);

        // 比较前半部分和反转后的后半部分链表
        while (reversedHalf != null) {
            if (head.val != reversedHalf.val) {
                return false;
            }
            head = head.next;
            reversedHalf = reversedHalf.next;
        }

        return true;
    }

    // 反转链表的函数
    private static ListNode reverseLinkedList(ListNode head) {
        ListNode prev = null;
        ListNode current = head;

        while (current != null) {
            ListNode nextNode = current.next;
            current.next = prev;
            prev = current;
            current = nextNode;
        }

        return prev;
    }

    public static void main(String[] args) {
        // 示例
        ListNode head = new ListNode('a');
        head.next = new ListNode('b');
        head.next.next = new ListNode('c');
        head.next.next.next = new ListNode('b');
        head.next.next.next.next = new ListNode('a');

        System.out.println(isPalindrome(head)); // 输出 true
    }
}
```



### 栈方式：

1. **构建栈：** 遍历字符串构建栈，时间复杂度为 O(n)，其中 n 是字符串的长度。
2. **查找中点：** 不需要查找中点，直接将前半部分字符压入栈中。
3. **比较操作：** 比较栈中的字符与字符串后半部分的字符是否相同，时间复杂度为 O(n/2)，即 O(n)。

综合起来，栈方式的时间复杂度为 O(n)。



```java
public class PalindromeChecker {

    public static boolean isPalindrome(String str) {
        if (str == null || str.isEmpty()) {
            return true; // 空字符串是回文串
        }

        // 将字符串中的非字母和非数字字符去除，并转换为小写
        str = str.replaceAll("[^a-zA-Z0-9]", "").toLowerCase();

        int length = str.length();
        Stack<Character> stack = new Stack<>();

        // 将字符串的前半部分压入栈中
        for (int i = 0; i < length / 2; i++) {
            stack.push(str.charAt(i));
        }

        // 跳过字符串中间的字符，如果字符串长度为奇数
        int startIndex = (length % 2 == 0) ? length / 2 : length / 2 + 1;

        // 比较栈中的字符与字符串后半部分的字符是否相同
        for (int i = startIndex; i < length; i++) {
            if (stack.isEmpty() || stack.pop() != str.charAt(i)) {
                return false;
            }
        }

        return true;
    }

    public static void main(String[] args) {
        // 示例
        String palindrome1 = "A man, a plan, a canal, Panama!";
        String palindrome2 = "Race a car";
        String palindrome3 = "abcba";

        System.out.println(isPalindrome(palindrome1)); // 输出 true
        System.out.println(isPalindrome(palindrome2)); // 输出 false
        System.out.println(isPalindrome(palindrome3)); // 输出 true
    }
}
```



### LRU：

```java
public class LRUCache<K, V> extends LinkedHashMap<K, V> {
    private final int capacity;

    public LRUCache(int capacity) {
        super(capacity, 0.75f, true);
        this.capacity = capacity;
    }

    @Override
    protected boolean removeEldestEntry(Map.Entry<K, V> eldest) {
        return size() > capacity;
    }

    public static void main(String[] args) {
        LRUCache<Integer, String> lruCache = new LRUCache<>(3);

        lruCache.put(1, "One");
        lruCache.put(2, "Two");
        lruCache.put(3, "Three");

        System.out.println(lruCache); // 输出：{1=One, 2=Two, 3=Three}

        lruCache.get(2); // 访问 2，使其变为最近使用，不会触发 removeEldestEntry

        lruCache.put(4, "Four"); // 添加 4，会触发 removeEldestEntry，将最久未使用的元素移除

        System.out.println(lruCache); // 输出：{2=Two, 3=Three, 4=Four}
    }
}
```