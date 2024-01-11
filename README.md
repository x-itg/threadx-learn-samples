# Eclipse ThreadX Learning Samples

此仓库已经把需要的两个子仓库提取出来原始仓库地址：https://github.com/eclipse-threadx/threadx-learn-samples.git

This repo contains sample projects for

- [Azure RTOS ThreadX Learning Path](https://learn.microsoft.com/training/paths/azure-rtos-threadx/)
- [Azure RTOS NetX Duo Learning path](https://learn.microsoft.com/training/paths/azure-rtos-netx-duo/)

## Get started

### 方式一:容器启动

- 安装VSCODE Docker DockerVS插件
- 启动wsl Docker
- 用VSCODE打开文件夹启动容器开发环境

### 方式二:Use GitHub Codespaces

- 打开Codespaces后需要把子仓库也克隆一遍
- [GitHub Codespaces](https://github.com/features/codespaces) is the preferred way to building and run these sample if you have your GitHub account enabled for this feature. Otherwise, you can still use it with the [local dev container](https://code.visualstudio.com/docs/remote/containers) or set up the toolchain by your own.

Follow the [Set up environment](https://learn.microsoft.com/training/modules/introduction-azure-rtos/2-set-up-environment) unit to get started with the samples.

#### Directory layout

    .
    ├── cmake                        # CMakelist files for building the project
    ├── docs                         # Documentation supplements
    ├── courses                      # Source code for learning paths
    │   ├── netxduo                  # NetX Duo samples
    │   └── threadx                  # ThreadX samples
    ├── libs                         # Submoduled ThreadX and NetX Duo source code
    └── tools                        # Required scripts for using NetX Duo within the container

### 方式三:Use Visual Studio

- 安装好Visual Studio 2022和 CMake
- 在示例路径下创建build文件夹如courses\threadx\ProjectEventChaining\build
- 在build文件夹下启动cmd输入cmake ..回车
- 打开build文件夹下的sln工程文件使用教程
- Visual Studio需要pthread支持
- You can also find the sample projects that can be built and run with Visual Studio in the [release page](https://github.com/eclipse-threadx/threadx-learn-samples/releases/tag/vs). An alternative for using the sample projects. Follow the [get started](#get-started) section above or the readme file in the `.zip` to learn how to use it.

### 方式四:在ubuntu中启动

- 在courses/threadx/ProjectEventChaining下面创建build文件夹
- cd build
- cmake ..
- make
- ./ProjectEventChaining  运行示例
- 也可以用vscode打开后运行指定任务

## Resources

- [Eclipse ThreadX on GitHub](https://github.com/eclipse-threadx)
- [Eclipse ThreadX documentation](https://github.com/eclipse-threadx/rtos-docs/)
