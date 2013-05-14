#### Android.mk #####
# 进入当前目录并取得当前目录的路径
# 其结果为: ~/android-ndk/apps/project_test/project/jni

LOCAL_PATH:= $(call my-dir)

# 清除全局变量
include $(CLEAR_VARS)

# 项目需要编译源文件列表
LOCAL_SRC_FILES:= \
test.c

# 项目需要的.h头文件目录列表
LOCAL_CFLAGS := 

# 项目需要的链接自编译出的.so 文件列表
LOCAL_SHARED_LIBRARIES :=

# 项目需要的链接非自编译出的.so 文件目录及列表
LOCAL_LDLIBS :=	

# 在执行此Android.mk 文件时，显示指定的变量信息
# 此次则显示 LOCAL_SRC_FILE变量内容在控制台上
$(info $(LOCAL_SRC_FILES))

# 项目编译出输文件名
LOCAL_MODULE:= test_c 

# 项目编译出输文件名，编译成哪种文件，
# BUILD_EXECUTABLE 是指.bin二进制执行文件
# BUILD_SHARED_LIBRARY 是指.so 动态库文件
# BUILD_STATIC_LIBRARY 是指.a 静态库文件
# 这里我们是生成二进制执行文件
include $(BUILD_EXECUTABLE)

