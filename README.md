# simple-diff

MIPT test task: diff and patch on binary file

Usage example:
```
>>> mkdir build && cd build && cmake .. && make -j8 && ctest
...
Test project /cygdrive/c/Users/rn3dk/CLionProjects/simple-diff/build
    Start 1: unit
1/1 Test #1: unit .............................   Passed    0.19 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.30 sec
>>> echo '1234' > 1.txt && echo '4321' > 2.txt
>>> bin/_diff 1.txt 2.txt > df.txt
>>> cat df.txt
@@ 0:4 0:4 @@
+      0x34
+      0x33
+      0x32
  0x31 0x31
- 0x32
- 0x33
- 0x34
  0x0a 0x0a

>>> bin/_patch 1.txt df.txt 2_.txt
>>> cat 2_.txt
4321
>>> bin/_diff 1.txt 2.txt

>>> echo '1234' > 1.txt && echo '23' > 2.txt
>>> bin/_diff 1.txt 2.txt > df.txt
>>> bin/_patch 1.txt df.txt 2_.txt
>>> cat 2_.txt
23
>>> cat df.txt
@@ 0:4 0:2 @@
- 0x31
  0x32 0x32
  0x33 0x33
- 0x34
  0x0a 0x0a

>>> echo '1222222222222222222222222222222222221' > 1.txt && echo '4222222222222222222222223' > 2.txt
>>> bin/_diff 1.txt 2.txt > df.txt
>>> cat df.txt
    @@ 0:1 0:1 @@
    * 0x31 0x34
      0x32 0x32
    @@ 23:37 23:25 @@
      0x32 0x32
    * 0x32 0x33
    - 0x32
    - 0x32
    - 0x32
    - 0x32
    - 0x32
    - 0x32
    - 0x32
    - 0x32
    - 0x32
    - 0x32
    - 0x32
    - 0x31
      0x0a 0x0a

>>> bin/_patch 1.txt df.txt 2_.txt
>>> cat 2_.txt
4222222222222222222222223
>>> bin/_diff 2.txt 2_.txt

>>>
```
