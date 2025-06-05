### 题目：数字识别 & 神经网络

### 要求：

1. 在一个8*8的点阵中绘制（模拟）手写的数字
1. 设计并训练一个神经网络，以识别手写的数字

### 参考：

[神经网络入门](http://blog.csdn.net/zzwu/article/details/574931)

本目录包含 Python 实现 `digit_nn.py` 与纯 C 实现 `digit_nn.c`。

编译 C 程序：

```bash
gcc digit_nn.c -lm -o digit_nn
```

运行：

```bash
./digit_nn
```
