# Makefile

# 指定交叉编译工具链
CC 				= arm-linux-gcc 

# 如果LVGL_DIR_NAME未定义，则将其定义为lvgl
LVGL_DIR_NAME 	?= lvgl

# 如果LVGL_DIR未定义，则将其定义为当前目录
LVGL_DIR 		?= .

# 编译器警告选项
WARNINGS		:= -Wall  -std=c99 

# 如果CFLAGS未定义，则将其定义为优化选项、调试选项和LVGL_DIR目录的头文件路径
CFLAGS 			?= -O3 -g0 -I$(LVGL_DIR)/ $(WARNINGS) -I$(LVGL_DIR)/src

# 链接器标志
LDFLAGS 		?= -lm

# 输出二进制文件名
BIN 			= demo

# 构建目录
BUILD_DIR 		= ./build
BUILD_OBJ_DIR 	= $(BUILD_DIR)/obj
BUILD_BIN_DIR 	= $(BUILD_DIR)/bin

# 安装目录
prefix 			?= /usr
bindir 			?= $(prefix)/bin

#收集要编译的文件
MAINSRC = 		./main.c
USERSRC =       $(shell find $(LVGL_DIR)/src -name '*.c')

# 收集要编译的文件
include $(LVGL_DIR)/lvgl/lvgl.mk
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk

# 在CSRCS后面追加赋值./mouse_cursor_icon.c
CSRCS 			+=$(LVGL_DIR)/mouse_cursor_icon.c 

# 目标文件拓展名
OBJEXT 			?= .o

# 汇编文件的目标文件列表
AOBJS 			= $(ASRCS:.S=$(OBJEXT)) # AOBJS: 汇编文件编译后的目标文件列表，将.S后缀替换为$(OBJEXT)即.o后得到AOBJS

# C文件的目标文件列表
COBJS 			= $(CSRCS:.c=$(OBJEXT))

# 主文件的目标文件
MAINOBJ 		= $(MAINSRC:.c=$(OBJEXT))
USEROBJ			= $(USERSRC:.c=$(OBJEXT))

# 所有源文件和目标文件的列表
SRCS 			= $(ASRCS) $(CSRCS) $(MAINSRC) $(USERSRC)
OBJS 			= $(AOBJS) $(COBJS) $(MAINOBJ) $(USEROBJ)

# 所有目标文件的完整路径
TARGET 			= $(addprefix $(BUILD_OBJ_DIR)/, $(patsubst ./%, %, $(OBJS)))

# 主目标，构建所有目标文件
all: default

# 生成每个目标文件的规则
# 第一行：定义了一个规则，说明每个目标文件（在./build/obj目录中）对应一个.c文件
$(BUILD_OBJ_DIR)/%.o: %.c 
# 第二行：确保目标文件所在的目录存在，如果不存在则创建
	@mkdir -p $(dir $@)  
# 第三行：使用 arm-linux-gcc 编译器编译对应的.c文件为目标文件(也就是当前目录下的main.c)
	@$(CC)  $(CFLAGS) -c $< -o $@
# 第四行：打印信息，说明正在编译哪个源文件
	@echo "CC $<"

# 默认目标，链接所有目标文件生成可执行文件
default: $(TARGET)
	@mkdir -p $(dir $(BUILD_BIN_DIR)/)
	$(CC) -o $(BUILD_BIN_DIR)/$(BIN) $(TARGET) $(LDFLAGS)

# 清理目标文件和生成的二进制文件
clean: 
	rm -rf $(BUILD_DIR)

# 安装二进制文件到指定目录
install:
	install -d $(DESTDIR)$(bindir)
	install $(BUILD_BIN_DIR)/$(BIN) $(DESTDIR)$(bindir)

# 卸载已安装的二进制文件
uninstall:
	$(RM) -r $(addprefix $(DESTDIR)$(bindir)/,$(BIN))
