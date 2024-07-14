### 一.CAN总线

1. 标准：ISO11898
2. 差分通信：线与特性，隐形电平，使用更好的PHY芯片
3. 线与特性实现总线仲裁，0为隐形电平，能够主导总线，当多个设备发送时，优先级高的优先发送，其余设备进入等待模式
4. **`收发流程`**：
   - ![image-20240116220130469](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240116220130469.png)
5. 发送想关问题
   - ![image-20240116220413150](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240116220413150.png)
6. 接收过程
   - ![image-20240116220612170](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240116220612170.png)
7. 接收相关问题
   - ![image-20240116220749365](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240116220749365.png)
8. CAN错误处理：
   - ![image-20240116220902323](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240116220902323.png)
   - ![image-20240116220926421](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240116220926421.png)
   - ![image-20240116221337786](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240116221337786.png)
9. 波特率计算：

### 二.CANFD

1. 外设框图：
   - ![image-20240116222120377](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240116222120377.png)
2. 帧格式
   - ![image-20240116222956838](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240116222956838.png)
3. CANFD工作模式：正常模式外
   - 限制模式
   - 总线检测
   - 内部回环
4. CANFD波特率，位时间特性：
   - ![image-20240117213702368](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240117213702368.png)
5. CANFD的2560字RAM

### 三.CAN OPEN（301规范）

1. 协议介绍
   - ![image-20240120212515944](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240120212515944.png)
   - ![image-20240120212622023](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240120212622023.png)
2. 重点内容实现：
   - ![image-20240120213030692](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240120213030692.png)
   - ![image-20240120213115863](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240120213115863.png)
   - ![image-20240120213417236](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240120213417236.png)
3. CanOpenNode协议栈
4.  

### 四.实战

1. 组网
   - ![image-20240121141646472](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240121141646472.png)
   - ![image-20240121141845805](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240121141845805.png)



五、J1939协议栈（充电桩/BMS）

1. 发展
   - ![image-20240121210808698](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240121210808698.png)
   - ![image-20240121210824109](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240121210824109.png)
2. 报文格式
   - 具体细节查看图片
     - ![image-20240121214047465](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240121214047465.png)
     - ![image-20240121214116506](C:\Users\zhiqu\AppData\Roaming\Typora\typora-user-images\image-20240121214116506.png)
3. **`源码框架`**
   - ![image-20240121214303228](J:\Git-Space\Typora\image-20240121214303228.png)
   - ![image-20240121214507874](J:\Git-Space\Typora\image-20240121214507874.png)
   - ![image-20240121214654992](J:\Git-Space\Typora\image-20240121214654992.png)
   - 上电信息处理
4. PGN请求：ECU发送PGN请求，其他ECU响应回复信息（发送，返回）
   - 应答请求： 
   - 请求NAME信息
   - 请求Proprity A（这里面记录了厂商专用信息）
   - DM请求
   - `多帧传输`（FF填充）
   - TP_BAM批量响应：































































































































