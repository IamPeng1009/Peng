#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include "lvgl/src/core/lv_disp.h"
#include "lvgl/src/core/lv_event.h"
#include "lvgl/src/core/lv_obj.h"
#include "lvgl/src/core/lv_obj_pos.h"
#include "lvgl/src/core/lv_obj_tree.h"
#include "lvgl/src/extra/widgets/keyboard/lv_keyboard.h"
#include "lvgl/src/extra/widgets/tabview/lv_tabview.h"
#include "lvgl/src/font/lv_font.h"
#include "lvgl/src/font/lv_symbol_def.h"
#include "lvgl/src/misc/lv_area.h"
#include "lvgl/src/misc/lv_color.h"
#include "lvgl/src/misc/lv_style.h"
#include "lvgl/src/widgets/lv_btn.h"
#include "lvgl/src/widgets/lv_checkbox.h"
#include "lvgl/src/widgets/lv_img.h"
#include "lvgl/src/widgets/lv_label.h"
#include "lvgl/src/widgets/lv_textarea.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include "link_list.h"
#include "product_func.h"

// 宏定义
#define DISP_BUF_SIZE (128 * 1024)
#define AD_WAIT 8000//广告时间
int time_count = 1;//计算屏幕空闲时间

/* 删除活动子画布 */
void Delself(lv_event_t *arg)
{
    lv_obj_del(arg->current_target);
    time_count = 1;
}

/* 广告页面 */
void AdvertisingPage()
{
    if (time_count > AD_WAIT) 
    {
        Deletemain();//把广告画布删除
    }

    lv_obj_t * img_advertisement = lv_img_create(lv_scr_act());
    lv_obj_set_size(img_advertisement, 800, 480); // 设置大小
    lv_obj_add_event_cb(img_advertisement, Delself, LV_EVENT_CLICKED, NULL);
    lv_obj_clear_flag(img_advertisement, LV_OBJ_FLAG_SCROLLABLE);

    //图片切换
    char *s[] = {"S:/root/pics/ad.bmp","S:/root/pics/ad1.bmp","S:/root/pics/ad2.bmp","S:/root/pics/ad3.bmp"};
    int size = sizeof(s)/sizeof(s[0]);
    lv_img_set_src(img_advertisement, s[(time_count/AD_WAIT)%size]);

    lv_obj_add_flag(img_advertisement, LV_OBJ_FLAG_CLICKABLE); // 使图片可点击
    lv_obj_set_scrollbar_mode(img_advertisement,LV_SCROLLBAR_MODE_OFF);

    // 滚动文字
    lv_obj_t * label1 = lv_label_create(img_advertisement);
    lv_label_set_long_mode(label1, LV_LABEL_LONG_SCROLL_CIRCULAR); // 滚动文字
    lv_obj_set_width(label1, 400);
    lv_obj_set_height(label1, 50);

    lv_label_set_text(label1, " Tap the screen to start buying! "
                                        " Tap the screen to start buying! "
                                        " Tap the screen to start buying! "); // 文本

    lv_obj_set_style_text_color(label1, lv_color_hex(0xED1C1C), LV_PART_SCROLLBAR); // 字体颜色 
    lv_obj_set_style_bg_opa(label1,200,LV_STATE_DEFAULT);//设置文本透明度
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_30, LV_STATE_DEFAULT);//设置广告字体大小
    lv_obj_align(label1, LV_ALIGN_BOTTOM_MID, 0, 0); // 位置；参考对象，对齐方式，x轴偏移量，y轴偏移量
}

/* 主函数 */
int main(void)
{
    /*初始化LVGL--杂项的  核心的 ....*/
    lv_init();

    /*初始化linux  fb设备  open设备文件 mmap映射LCD显存  /dev/fb0 */
    fbdev_init();
    
    /*注册fb设备到LVGL*/
    static lv_color_t buf[DISP_BUF_SIZE];
    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);
    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;//显示缓存
    disp_drv.flush_cb   = fbdev_flush;//LCD画点函数
    disp_drv.hor_res    = 800;
    disp_drv.ver_res    = 480;
    lv_disp_drv_register(&disp_drv);//将LCD屏的参数和操作-画点注册进LVGL

    //触摸屏的初始化
    evdev_init();

    //将初始化好的触摸屏设备注册进LVGL
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;//读取触摸屏坐标函数
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

    LL*Head = List_Init();
    product_txt(Head);

    lv_obj_t *main = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main,800,480);
    lv_obj_set_scrollbar_mode(main,LV_SCROLLBAR_MODE_OFF);//设置无滚动条
    
    txt = malloc(56);//文本缓冲区分配内存

    Creat_tab(main);
    Browse_UI(Head, tab1, 1);
    Browse_UI(Head, tab2, 2);
    Browse_UI(Head, tab3, 3);

    /*主函数主循环---待定*/
    while(1) 
    {
        lv_timer_handler();//LVGL事务处理函数 读坐标  遍历对象链表看哪个对象被操作
        usleep(5000);//每5ms调用

        /*屏幕无操作返回广告页面*/
        if (lv_timer_get_idle() >= 100) 
            time_count++;
        else 
            time_count = 1;

        printf("%d\n",time_count);
        if (time_count % AD_WAIT == 0) 
        {
            // printf("1:%d\n",time_count);
            AdvertisingPage();
        }
    }
    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) 
    {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
