#ifndef _PRODUCT_FUNC_H
#define _PRODUCT_FUNC_H

#include "./link_list.h"
#include <unistd.h>

float tmp_allprice;

/* 功能：关联文本对象 */
void ta_event_cb(lv_event_t *e);

/* 功能：键盘触发 */
void kb_event_cb(lv_event_t * e);

/* 获取搜索框文本内容 */
void get_txt(lv_event_t *arg);

/* 打印单件订单记录 */
void Buy_one(void *userdata);

/* 功能：按钮返回主界面(更新数据) */
void btn_return(lv_event_t *arg);

/* 功能：按钮返回主界面(不更新数据) */
void shop_btn_back_cb(lv_event_t *arg);

/* 功能：按钮返回主界面(数据清零) */
void back_btn_back_cb(lv_event_t *arg);

/* 按下返回主界面(数据不清零) */
void btn_back_cb(lv_event_t *arg);

/* 获取复选框信息 */
void checkbox_event_cb(lv_event_t *e);

#endif