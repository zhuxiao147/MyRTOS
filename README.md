# MyRTOS

MyRTOS is a lightweight real-time operating system (RTOS) designed for embedded systems, focusing on clarity, modularity, and ease of use. This project is suitable for learning, research, and small embedded applications.

本项目旨在弥补轻量化 RTOS 缺口，很多开发者和学生有时只需要一个简单的 RTOS，快速移植。

本项目代码结构简单易懂，只包含 RTOS 基础功能。

## 项目结构

```
src/
	arch/           # 架构相关代码（如Cortex-M端口）
	include/        # 公共头文件
	ipc/            # 信号量、互斥锁、队列等同步机制
	kernel/         # 内核核心模块（任务、调度、时间等）
```

## 主要功能完成度

- [x] 任务管理（创建、切换、空闲任务）
- [x] 时间片轮转调度
- [x] 基本的任务优先级支持
- [x] 任务延时与时间管理
- [ ] 信号量、互斥锁、队列等同步机制（接口已定义，部分实现中）
- [x] 低功耗支持（接口预留）
- [ ] 板级支持包（BSP）扩展

## 目录说明

- `arch/`：架构相关适配代码（如端口文件）
- `include/`：RTOS 对外头文件接口
- `ipc/`：同步机制（os_mutex, os_queue, os_semphr 等）
- `kernel/`：内核核心（任务、调度、时间等）

## 快速开始

1. 克隆本仓库
2. 在 `src/` 目录下根据目标平台配置架构和 BSP
3. 编译并烧录到目标硬件

## 适用平台

- 目前以 ARM Cortex-M 架构为主，其他平台可扩展
