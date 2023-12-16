

#### 贪心算法

**贪心算法（Greedy Algorithm）** 是一种基于贪心策略进行问题求解的算法。在贪心算法中，每一步都选择当前状态下最优的解，最终期望通过局部最优的选择达到全局最优。贪心算法通常不保证能够得到全局最优解，但在某些问题中，贪心算法能够得到足够好的解，并且具有高效性。

#### 贪心算法的基本思路

1. **建立解的选择集合。** 对于问题中的每一个子问题，建立一个解的选择集合。
2. **确定选择的标准。** 定义一种选择标准，选择当前最优解。
3. **选择当前最优解。** 根据选择标准，从选择集合中选择当前最优解。
4. **更新当前状态。** 根据选择的解，更新当前问题的状态。
5. **重复以上步骤。** 重复执行上述步骤，直到达到最终的解或无法继续选择。

#### 适用场景

- **贪心选择性质：** 问题的最优解可以通过一系列局部最优的选择得到。
- **子问题无关性：** 子问题的解不会影响到其他子问题的选择。
- **可行性：** 对于每一个子问题，选择最优解后，问题的解仍然是可行的。

#### 经典应用

1. **活动选择问题（Activity Selection Problem）：** 选择一系列互不相容的活动，使得能够参与的活动数量最大。
2. **背包问题的贪心解法：** 贪心策略用于选择价值/权重最高的物品，以填满背包。

#### 优缺点

**贪心算法的优点：**

- **简单高效：** 贪心算法通常思路简单、易于实现，并且具有较高的执行效率。
- **适用性广泛：** 在一些问题中，贪心算法能够得到可接受的解。

**贪心算法的缺点：**

- **不保证最优解：** 贪心算法不一定能够得到全局最优解，因为每一步只考虑当前状态的局部最优解。





在一个非负整数 a 中，我们希望从中移除 k 个数字，让剩下的数字值最小，如何选择移除哪 k 个数字呢？



具体步骤如下：

1. **初始化一个空栈，用于存放最终结果的数字。**
2. 从高位到低位遍历非负整数 a 的每一位。
   - 如果栈不为空，且当前数字小于栈顶数字，弹出栈顶数字，直到栈为空或者已经移除 k 个数字。
   - 将当前数字入栈。
3. **如果仍需移除 k 个数字，从栈底开始弹出数字，直到移除 k 个数字。**
4. **构造最终的结果字符串。**



```java
package com.controller.bootweb.demo.dsa;

import java.util.Stack;

public class RemoveKDigits {

 
    public static String removeKdigits(String num, int k) {
        if (num == null || num.length() == 0 || k >= num.length()) {
            return "0";
        }

        Stack<Character> stack = new Stack<>();
        for (char digit : num.toCharArray()) {
            while (!stack.isEmpty() && k > 0 && digit < stack.peek()) {
                stack.pop();
                k--;
            }
            stack.push(digit);
        }

        // 如果仍需移除 k 个数字，从栈底开始弹出数字
        while (k > 0) {
            stack.pop();
            k--;
        }

        // 构造最终结果字符串
        StringBuilder result = new StringBuilder();
        while (!stack.isEmpty()) {
            result.insert(0, stack.pop());
        }

        // 移除结果字符串前导的 '0'
        int index = 0;
        while (index < result.length() && result.charAt(index) == '0') {
            index++;
        }
        result.delete(0, index);

        return result.length() == 0 ? "0" : result.toString();
    }

    public static void main(String[] args) {
        String num = "1432219";
        int k = 3;
        String result = removeKdigits(num, k);
        System.out.println(result); // Output: "1219"
    }
}
```



假设有 n 个人等待被服务，但是服务窗口只有一个，每个人需要被服务的时间长度是不同的，如何安排被服务的先后顺序，才能让这 n 个人总的等待时间最短？



贪心策略是按照每个人需要服务的时间长度从小到大进行排序，然后按照排好序的顺序进行服务。这样可以最小化每个人的等待时间。

```java
public class ShortestJobFirst {

    public static int minimizeTotalWaitTime(int[] serviceTimes) {
        if (serviceTimes == null || serviceTimes.length == 0) {
            return 0;
        }

        Arrays.sort(serviceTimes);  // 按照服务时间从小到大排序

        int totalWaitTime = 0;
        int currentWaitTime = 0;

        for (int serviceTime : serviceTimes) {
            totalWaitTime += currentWaitTime;  // 累加当前人的等待时间
            currentWaitTime += serviceTime;    // 累加当前人的服务时间
        }

        return totalWaitTime;
    }

    public static void main(String[] args) {
        int[] serviceTimes = {5, 2, 8, 4};
        int result = minimizeTotalWaitTime(serviceTimes);
        System.out.println("最小总等待时间：" + result); // 输出：最小总等待时间：19
    }
}
```

