### 调试技术

1. Event Recorder调试  RTT
   - Event Recorder调试技术为虚拟调试技术，不用硬件映射串口就可以通过调试接口进行信息打印以及执行时间测量
   - **RTT为Segger的调试技术，Jlink不支持时间戳以及中文打印，** RTT
     - RTT移植：
       - 初始化，上下行缓冲区配置
       - 简单的打印回环打印，
       - Cubemx进行移植--Event Recorder可以直接进行debug，
       - RTT需要进行移植目标代码