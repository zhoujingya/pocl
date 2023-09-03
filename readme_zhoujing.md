## 本文档旨在用pocl构建本地CPU device或者其他设备的支持

### 编译命令
cmake . -DWITH_LLVM_CONFIG=/usr/lib/llvm-16/bin/llvm-config -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/home/triton/Codes/pocl/install -G Ninja -B build

### 运行poclcc之前的其他准备

其他情况参考ventus-llvm readme进行一些环境变量的设置

`export OCL_ICD_VENDORS=~/Codes/pocl/install/lib/libpocl.so `

如果libOpenCL.so这个库没有在系统库路径下，需要把包涵libOpenCL.so这个库的文件夹路径加载到`LD_LIBRARY_PATH`下

最后运行`./build/bin/poclcc -l`，我的机器上得到的输出如下:

```
LIST OF DEVICES:
0:
  Vendor:   AuthenticAMD
    Name:   cpu-znver3-AMD Ryzen 9 5900X 12-Core Processor
 Version:   OpenCL 3.0 PoCL HSTR: cpu-x86_64-pc-linux-gnu-znver3
```

也可以指定pocl支持的设备

```
export POCL_DEVICES="basic pthread" 
```