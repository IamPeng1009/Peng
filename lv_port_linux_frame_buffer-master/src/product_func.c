#include "./product_func.h"

/* 功能：关联文本对象 */
void ta_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);            // 获取事件编码
    lv_obj_t * ta = lv_event_get_target(e);                 // 获取文本对象
    lv_obj_t * kb = (lv_obj_t *)lv_event_get_user_data(e);  // 获取键盘对象

    /* 获取文本点击事件 */
    if(code == LV_EVENT_CLICKED) 
    {
        lv_keyboard_set_textarea(kb, ta);                   // 将键盘与文本进行关联
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);   // 清除键盘的隐身标志，显示键盘
        lv_obj_add_state(ta, LV_STATE_FOCUSED);  // 将文本框设置为聚焦状态
    }

    /* 获取文本失去焦点事件 */
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea(kb, NULL);              // 取消键盘的关联
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);     // 添加键盘隐藏标志，隐藏键盘
    }
}

/* 功能：键盘触发 */
void kb_event_cb(lv_event_t * e)
{
    lv_obj_t * kb = lv_event_get_target(e);                 // 获取当前事件对象，也就是键盘对象
    lv_obj_t * ta = lv_keyboard_get_textarea(kb);           // 获取与键盘绑定的输入缓冲区的对象

    lv_keyboard_set_textarea(kb, NULL);                  // 取消键盘的关联
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);         // 添加键盘隐藏标志，隐藏键盘

    lv_obj_clear_state(ta, LV_STATE_FOCUSED);    // 清除文本框的聚焦状态
}

/* 获取搜索框文本内容 */
void get_txt(lv_event_t *arg)
{
    lv_obj_t *target = lv_event_get_target(arg);//获取当前标签对象
    strcpy(txt,lv_textarea_get_text(target));
    printf("%s\n",txt);
}

/* 打印单件订单记录 */
void Buy_one(void *userdata)
{

    LL *p = (LL *)userdata;

    time_t t = time(NULL);
    char *s = ctime(&t);
    strtok(s,"\n");
    
    FILE *fp_list ;
    if(access("list_one.txt", F_OK)) // 不存在则创建
    {
        fp_list = fopen("list_one.txt", "w+");
    }
    else // 存在则打开
    {
        fp_list = fopen("list_one.txt", "a+");
    }

    setvbuf(fp_list, NULL, _IONBF, 0);
    fprintf(fp_list,"ID:%d\t Name:%s\t num:%d\t price:%f\t %s\n",p->ID,p->Name,p->buynum,(p->buynum)*(p->Price),s);

    fclose(fp_list);
}

/* 功能：按钮返回主界面(更新数据) */
void btn_return(lv_event_t *arg)
{
    Buy_one(arg->user_data);
    // 支付成功弹窗
    static const char * btns[] = {""};
    lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Done", "Payment completed.", btns, true);
    lv_obj_center(mbox1);

    LL *p = (LL*)(arg->user_data);

    //删除活动画布下所有，防止占内存
    Deletemain(); 

    for(LL*q = p;q->next != p; q = q->next)
    {
        q->Remain = (q->Remain);
        q->buynum = 0;
    }

    p->prev->Remain = p->prev->Remain;
    p->prev->buynum = 0;

    for(;p->ID != -1;p=p->next);//循环能结束  p就是指向头结点的头指针

    //创建一个大画布
    lv_obj_t *main = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main, 800,480);

    Creat_tab(main);
    Browse_UI(p,tab1,1);
    Browse_UI(p,tab2,2);
    Browse_UI(p,tab3,3);
}

/* 功能：按钮返回主界面(不更新数据) */
void shop_btn_back_cb(lv_event_t *arg)
{
    // 支付成功弹窗
    static const char * btns[] = {""};
    lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Done", "Payment completed.", btns, true);
    lv_obj_center(mbox1);

    LL *p = (LL*)(arg->user_data);

    //删除活动画布下的所有
    Deletemain();

    for(;p->ID != -1;p=p->next);//循环能结束  p就是指向头结点的头指针

    //循环为了给购买数量清零
    for(LL*q = p;q->next != p; q = q->next)
    {
        q->Remain = (q->Remain) - (q->buynum);
        q->buynum = 0;
    }
    p->prev->Remain = p->prev->Remain - p->prev->buynum;
    p->prev->buynum = 0;


    //创建一个大画布
    lv_obj_t *main = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main, 800,480);

    Creat_tab(main);
    Browse_UI(p,tab1,1);
    Browse_UI(p,tab2,2);
    Browse_UI(p,tab3,3);
}

/* 功能：按钮返回主界面(数据清零) */
void back_btn_back_cb(lv_event_t *arg)
{
    // 支付成功弹窗
    static const char * btns[] = {""};
    lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Failed", "Payment uncompleted.", btns, true);
    lv_obj_center(mbox1);

    LL *p = (LL*)(arg->user_data);

    //删除活动画布下的所有
    Deletemain();

    for(;p->ID != -1;p=p->next);//循环能结束  p就是指向头结点的头指针

    //循环为了给购买数量清零
    for(LL*q = p;q->next != p; q = q->next)
    {
        q->Remain = (q->Remain) + (q->buynum);
        q->buynum = 0;
    }
    p->prev->Remain = p->prev->Remain + p->prev->buynum;
    p->prev->buynum = 0;


    //创建一个大画布
    lv_obj_t *main = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main, 800,480);

    Creat_tab(main);
    Browse_UI(p,tab1,1);
    Browse_UI(p,tab2,2);
    Browse_UI(p,tab3,3);
}

/* 按下返回主界面(数据不清零) */
void btn_back_cb(lv_event_t *arg)
{
    LL *p = (LL*)(arg->user_data);

    //删除活动画布下所有，防止占内存
    Deletemain();

    //删除当前画布
    lv_obj_del(arg->current_target);

    //创建一个大画布
    lv_obj_t *main = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main, 800,480);

    Creat_tab(main);
    Browse_UI(p,tab1,1);
    Browse_UI(p,tab2,2);
    Browse_UI(p,tab3,3);
}

/* 获取复选框信息 */
void checkbox_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED) 
    {
        if (lv_obj_has_state(checkbox, LV_STATE_CHECKED)) 
        {
            printf("ON\n");
        }
        else 
        {
            printf("OFF\n");
        }
    }
}