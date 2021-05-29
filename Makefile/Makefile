SRC=$(wildcard ./*.c)                # 获取当前目录下所有的.c文件

OBJS=$(patsubst %.c, %.o, $(SRC))    # 将SRC中所有出现的.c替换成.o

TARGET=all			     # 目标文件

$(TARGET):$(OBJS)		     # $@ 目标  $< 所有依赖  $^ 第一个依赖
	gcc $^ -o $@                       

%.o:%.c                              # 将所有.c生成对应的.o
	gcc -c $< -o $@              # 在命令前加@,不显示命令本身,只显示结果

.PHONY:clean                         # 声明伪目标
clean:
	rm -rf $(OBJS) $(TARGET)     # 在命令前加-,即使本条命令出错,仍然会执行之后的命令
