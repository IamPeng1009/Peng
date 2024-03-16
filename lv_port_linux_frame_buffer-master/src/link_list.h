#ifndef _LINK_LIST_H
#define _LINK_LIST_H

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>

#include "product_func.h"

// 全局变量的声明
lv_obj_t *tab1;    //标签
lv_obj_t *tab2;
lv_obj_t *tab3;
lv_obj_t *checkbox; //复选框
char *txt;         // 将输入文本传入缓冲区

/* 售货机商品信息节点构建 */
typedef struct List
{
    int   ID;           //商品编号
    char  Name[32];     //存放商品的名字
    float Price;        //商品的单价
    char  PicPath[128]; //存放商品的图片在本机的路径及名字
    int   Remain;       //商品的余量
    char  code[128];    //二维码图片
    int   buynum;       //购买数量
    int   types;        //tab标签

    float totalPrice;   //总价格
    lv_obj_t *rnum;     //剩余数量的基本事件
    lv_obj_t *bnum;     //购买数量的基本事件
    lv_obj_t *bprice;   //购买金额的基本事件

    lv_obj_t *total_price;//显示价格的基本事件
    lv_obj_t *all_price; //购物车总价的基本事件
    struct List *next;
    struct List *prev;
}LL;

/* 功能：初始化结构体 */
LL *List_Init(void);

/* 功能：链表数据尾插法 */
void List_TailInsert(LL*head,LL Data);

/* 功能：按ID来查找商品 */
LL* List_FindNode_ID(LL*head,int id);

/* 功能：按名字查找商品 */
LL* List_FindNode_Name(LL*head,char *name);

/* 功能：商品内部增加 */
void add(lv_event_t *arg);

/* 功能：商品内部减少 */
void sub(lv_event_t *arg);

/* 功能：商品外部增加 */
void add_out(lv_event_t *arg);

/* 功能：商品外部减少 */
void sub_out(lv_event_t *arg);

/* 功能：商品数据(从本地路径导入) */
void product_txt(LL *Head);

/* 功能：关联搜索按键 Name&ID */
void btn_search_event_bc(lv_event_t* e);

/* UI：绘制一个初始的商品浏览界面 */
void Browse_UI(LL *Head, lv_obj_t *main, int type);

/* 功能：商品详细界面 */
void Buy_UI(void *user_data);

/* 禁止滚动动画 */
void scroll_begin_event(lv_event_t *e);

/* 创建标签界面 */
void Creat_tab(lv_obj_t *main);

/* 测试函数 */
void Buy(lv_event_t *arg);

/* 重新绘制Shop_UI画布 */
void Shop(lv_event_t *arg);

/* 购物车界面 */
void Shop_UI(void *user_data);

/* 删除活动画布的第一个子画布 */
void Deletemain();

#endif