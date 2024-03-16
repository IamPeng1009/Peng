#include "link_list.h"



/* 功能：初始化结构体 */
LL *List_Init(void)
{
    LL *head = malloc(sizeof(LL));
    if(head == NULL)
    {
        perror("malloc");
        return NULL;
    }

    head->ID = -1;//设置头指针ID为-1

    head->next = head;
    head->prev = head;

    return head;//返回头指针
}

/* 功能：链表数据尾插法 */
void List_TailInsert(LL*head,LL Data)
{
    LL* NewNode = malloc(sizeof(LL));

    //数据域
    strcpy(NewNode->Name,Data.Name);
    strcpy(NewNode->PicPath,Data.PicPath);
    strcpy(NewNode->code,Data.code);
    NewNode->ID = Data.ID;
    NewNode->Price = Data.Price;
    NewNode->Remain = Data.Remain;
    NewNode->types = Data.types;

    //指针域
    LL *p = head->prev;
    p->next = NewNode;
    NewNode->next = head;
    head->prev = NewNode;
    NewNode->prev = p;
}

/* 功能：按ID来查找商品 */
LL* List_FindNode_ID(LL*head,int id)
{
    LL*p= head->next;//从第一个有效节点开始
    
    for(;p != head;p=p->next)
    {
        if(p->ID == id)
        {
            return p;//返回找到的结点的地址
        }
    }
    return NULL;//未找到
}

/* 功能：按名字查找商品 */
LL* List_FindNode_Name(LL*head,char *name)
{
    LL*p= head->next;//从第一个有效节点开始
    
    for(;p != head;p=p->next)
    {
        if(strstr(p->Name,name))
        {
            return p;//返回找到的结点的地址
        }
    }
    return NULL;//未找到
}

/* 功能：商品内部增加 */
void add(lv_event_t *arg)
{
    char tmp_price[8] = {0};
    if(((LL*)(arg->user_data))->buynum < ((LL*)(arg->user_data))->Remain + ((LL*)(arg->user_data))->buynum)
    {
        lv_label_set_text_fmt(((LL*)(arg->user_data))->rnum,"Remain %d",--(((LL*)(arg->user_data))->Remain));
        lv_label_set_text_fmt( (((LL*)(arg->user_data))->bnum),"%d",++(((LL *)(arg->user_data))->buynum));

        sprintf(tmp_price, "CNY:%.1f",(float)(((LL*)(arg->user_data))->buynum)*(((LL*)(arg->user_data))->Price));
        lv_label_set_text_fmt(((LL*)(arg->user_data))->bprice,"%s",tmp_price);
    }
}

/* 功能：商品内部减少 */
void sub(lv_event_t *arg)
{
    char tmp_price[8] = {0};
    if(((LL*)(arg->user_data))->buynum != 0)
    {
        lv_label_set_text_fmt(((LL*)(arg->user_data))->rnum,"Remain %d",++(((LL*)(arg->user_data))->Remain));
        lv_label_set_text_fmt( (((LL*)(arg->user_data))->bnum),"%d",--(((LL *)(arg->user_data))->buynum));

        sprintf(tmp_price, "CNY:%.1f",(float)(((LL*)(arg->user_data))->buynum)*(((LL*)(arg->user_data))->Price));
        lv_label_set_text_fmt(((LL*)(arg->user_data))->bprice,"%s",tmp_price);
    }
}

/* 功能：商品外部增加 */
void add_out(lv_event_t *arg)
{
    if(((LL*)(arg->user_data))->buynum < ((LL*)(arg->user_data))->Remain + ((LL*)(arg->user_data))->buynum)
    {
        lv_label_set_text_fmt(((LL*)(arg->user_data))->rnum,"Remain %d",--(((LL*)(arg->user_data))->Remain));
        lv_label_set_text_fmt( (((LL*)(arg->user_data))->bnum),"%d",++(((LL *)(arg->user_data))->buynum));
    }
}

/* 功能：商品外部减少 */
void sub_out(lv_event_t *arg)
{
    if(((LL*)(arg->user_data))->buynum != 0)
    {
        lv_label_set_text_fmt(((LL*)(arg->user_data))->rnum,"Remain %d",++(((LL*)(arg->user_data))->Remain));
        lv_label_set_text_fmt( (((LL*)(arg->user_data))->bnum),"%d",--(((LL *)(arg->user_data))->buynum));
    }
}

/* 功能：商品数据(从本地路径导入) */
void product_txt(LL *Head)
{
    FILE* fp = fopen("product.txt","r");
    if(fp == NULL)
    {
        perror("打开失败");
        exit(0);
    }

    LL data;

    while(!feof(fp))
    {
        fscanf(fp,"%d%s%f%s%d%s%d%d",&data.ID,data.Name,&data.Price,data.PicPath,&data.Remain,data.code,&data.buynum,&data.types);
        List_TailInsert(Head,data);
    }
    fclose(fp);
}

/* 功能：关联搜索按键 Name&ID */
void btn_search_event_bc(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);            // 获取事件编码
    LL * head = (LL *)lv_event_get_user_data(e);            // 获取键盘对象
    LL * head1 = (LL *)lv_event_get_user_data(e);           // 获取键盘对象

    LL * findnode =  List_FindNode_Name(head, txt);
    LL * findnode1 =  List_FindNode_ID(head1, atoi(txt));//atoi将字符型转成整型

    if (findnode) 
    {
        Deletemain();
        Buy_UI(findnode);
    }
    else if (findnode1) 
    {
        Deletemain();
        Buy_UI(findnode1);
    }
    else 
    {
        printf("\n");
    }
}

/* UI：绘制一个初始的商品浏览界面 */
void Browse_UI(LL *Head, lv_obj_t *main, int type)
{
    //创建商品画布
    lv_obj_t *product_p = lv_obj_create(main);//父对象是活动屏幕
    lv_obj_set_size(product_p,520,600);
    lv_obj_align(product_p, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_flex_flow(product_p, LV_FLEX_FLOW_ROW_WRAP);//弹性布局设置
    lv_obj_set_scrollbar_mode(product_p,LV_SCROLLBAR_MODE_OFF);

    //创建广告画布
    lv_obj_t *ad = lv_obj_create(main);
    lv_obj_set_size(ad,180,600);
    lv_obj_align(ad, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_scrollbar_mode(ad,LV_SCROLLBAR_MODE_OFF);//设置无滚动条

    //根据搜索部件创建键盘
    lv_obj_t *keyboard = lv_keyboard_create(main);

    //创建搜索部件
    lv_obj_t *search = lv_textarea_create(ad);
    lv_obj_set_size(search,125,80);
    lv_obj_align(search, LV_ALIGN_TOP_MID, 0, 0);
    lv_textarea_set_one_line(search, true);     //设置只能单行输入
    lv_textarea_set_placeholder_text(search, "search");     //设置提示词
    lv_obj_add_state(search, LV_STATE_DEFAULT);     // 设置文本框为默认状态
    lv_obj_add_event_cb(search, get_txt, LV_EVENT_VALUE_CHANGED, NULL);//获取文本框信息

    //创建搜索按钮
    lv_obj_t *btn_search = lv_btn_create(ad);//创建按钮
    lv_obj_set_size(btn_search, 125, 50);//设置大小
    lv_obj_align(btn_search,  LV_ALIGN_TOP_MID, 0, 50);//按钮对齐
    lv_obj_set_style_bg_color(btn_search, lv_color_hex(0xff0000), LV_STATE_PRESSED);//按下后按钮背景颜色
    
    //创建搜索label
    lv_obj_t *label_search = lv_label_create(btn_search);
    lv_obj_set_style_bg_color(btn_search,lv_color_hex(0x00BFFF),0);
    lv_obj_set_style_bg_color(btn_search, lv_color_hex(0xff0000), LV_STATE_PRESSED);//按下后按钮背景颜色
    lv_label_set_text(label_search, "go");
    lv_obj_set_style_text_font(label_search, &lv_font_montserrat_24, LV_STATE_DEFAULT);//设置字体大小
    lv_obj_center(label_search);

    //购物车按钮
    lv_obj_t *btn_buy = lv_btn_create(ad);//创建按钮
    lv_obj_set_size(btn_buy, 130, 130);//设置大小
    lv_obj_set_style_bg_color(btn_buy,lv_color_hex(0xFFFFFF),0);//按钮颜色
    lv_obj_align(btn_buy,  LV_ALIGN_CENTER, 0, 50);//按钮对齐

    //创建按钮边框样式
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_border_width(&style_btn, 2);  // 设置边框宽度
    lv_style_set_border_color(&style_btn,  lv_color_hex(0xD3D3D3));  // 设置边框颜色
    lv_obj_add_style(btn_buy, &style_btn, LV_STATE_DEFAULT);    //将样式添加到按钮
    lv_obj_set_style_bg_color(btn_buy, lv_color_hex(0x87CEFA), LV_STATE_PRESSED);//按下后按钮背景颜色
    
    //购物车图标
    lv_obj_t *img_buy = lv_img_create(btn_buy);
    lv_obj_align(img_buy,LV_ALIGN_CENTER,0,0);
    lv_img_set_src(img_buy,"S:/root/pics/buycar.png");


    //添加pop.gif
    /*
    步骤：将gif解码并将解码文件放在同一路径即可
    缺点：计算空闲时间占空比的函数会认为屏幕一直在操作，导致进不了广告
    优点：可爱
    */
    LV_IMG_DECLARE(pop_gif);//用于声明一个图像

    lv_obj_t *img = lv_gif_create(ad);
    lv_gif_set_src(img, "S:/root/pics/pop.gif");
    lv_obj_align(img, LV_ALIGN_CENTER, 0, -90);

    
    /* 添加事件，调用购物车界面*/
    lv_obj_add_event_cb(btn_buy, Shop, LV_EVENT_CLICKED, Head);

    /* 添加事件，调用搜索函数(Name) */ 
    lv_obj_add_event_cb(btn_search, btn_search_event_bc, LV_EVENT_CLICKED, Head);

    /* 文本控件的所有事件 */
    lv_obj_add_event_cb(search, ta_event_cb, LV_EVENT_ALL, keyboard);

    /* 初始时将键盘隐藏 */
    lv_keyboard_set_textarea(keyboard, NULL);                     // 未将键盘与输入区绑定
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);                // 将表示将键盘隐藏

    /* 键盘的确认事件 */
    lv_obj_add_event_cb(keyboard, kb_event_cb, LV_EVENT_READY , NULL);

    //开始围绕链表在LCD屏创建若干个对象
    for(LL *p=Head->next;p!=Head;p=p->next)
    {
        if (p->types == type) 
        {
            //商品界面画布
            lv_obj_t *product = lv_obj_create(product_p);
            lv_obj_set_size(product,232,300);
            lv_obj_set_scrollbar_mode(product,LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_style_bg_color(product_p, lv_color_hex(	0x800080), 0);//设置次背景颜色为渐变从紫色开始
            lv_obj_set_style_bg_grad_color(product_p, lv_color_hex(0xFF1493), 0);//粉色结束
            lv_obj_set_style_bg_grad_dir(product_p, LV_GRAD_DIR_HOR, 0);//渐变设置

            lv_obj_t *img = lv_img_create(product);
            lv_obj_set_align(img,LV_ALIGN_TOP_MID);
            lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 20);
            lv_img_set_src(img,p->PicPath);//给图片对象添加一个图片源
            lv_img_set_zoom(img,450);  //比256大是放大，小是缩小

            //根据商品结点信息显示商品名称
            lv_obj_t *name = lv_label_create(product);//打印名字
            lv_obj_align(name, LV_ALIGN_CENTER, 0, 5);
            char tempbuf2[100] = {0};
            sprintf(tempbuf2, "%s",p->Name);
            lv_label_set_text( name,tempbuf2);
            lv_obj_set_style_text_font(name, &lv_font_montserrat_30, LV_STATE_DEFAULT);//设置字体大小

            //创建按钮
            lv_obj_t *btn = lv_btn_create(product);//创建按钮
            lv_obj_set_size(btn, 80, 40);//设置大小
            lv_obj_align(btn,  LV_ALIGN_CENTER, 0, 45);//按钮对齐
            lv_obj_set_style_bg_color(btn, lv_color_hex(0xff0000), LV_STATE_PRESSED);//按下后按钮背景颜色
            
            //按钮label
            lv_obj_t *btn_label = lv_label_create(btn);//创建按钮标签
            lv_obj_align(btn_label,LV_ALIGN_CENTER,0,0);//设置标签位置
            char tempbuf5[10]={0};
            sprintf(tempbuf5, "CNY:%.1f",p->Price);
            lv_label_set_text( btn_label,tempbuf5);//打印文本标签
            lv_obj_set_style_text_font(btn_label, &lv_font_montserrat_20, LV_STATE_DEFAULT);//设置字体大小

            //添加事件
            lv_obj_add_event_cb(btn, Buy, LV_EVENT_CLICKED,p);

            //商品剩余量
            p->rnum = lv_label_create(product);
            lv_label_set_text_fmt(p->rnum,"Remain %d",(p->Remain)-(p->buynum));
            lv_obj_align(p->rnum,LV_ALIGN_CENTER,0,77);
            lv_obj_set_style_text_font(p->rnum, &lv_font_montserrat_18, LV_STATE_DEFAULT);//设置字体大小

            //添加"+"按钮
            lv_obj_t *btn1 = lv_btn_create(product);
            lv_obj_set_size(btn1,45,45);
            lv_obj_align(btn1,LV_ALIGN_BOTTOM_RIGHT,0,0);
            lv_obj_t *label5 = lv_label_create(btn1);
            lv_obj_set_style_bg_color(btn1,lv_color_hex(0xFF0000),0);
            lv_label_set_text(label5, "+");
            lv_obj_center(label5);
            lv_obj_set_style_text_font(label5, &lv_font_montserrat_20, LV_STATE_DEFAULT);//设置字体大小
            lv_obj_add_event_cb(btn1,add_out,LV_EVENT_CLICKED,p);
            
            //添加"-"按钮
            lv_obj_t *btn2 = lv_btn_create(product);
            lv_obj_set_size(btn2,45,45);
            lv_obj_align(btn2,LV_ALIGN_BOTTOM_LEFT,0,0);
            lv_obj_t *label6 = lv_label_create(btn2);
            lv_obj_set_style_bg_color(btn2,lv_color_hex(0xFF0000),0);
            lv_label_set_text(label6, "-");
            lv_obj_center(label6);
            lv_obj_set_style_text_font(label6, &lv_font_montserrat_30, LV_STATE_DEFAULT);//设置字体大小
            lv_obj_add_event_cb(btn2,sub_out,LV_EVENT_CLICKED,p);

            //购买数量
            p->bnum = lv_label_create(product);
            lv_label_set_text_fmt(p->bnum,"%d",p->buynum);
            lv_obj_align(p->bnum,LV_ALIGN_BOTTOM_MID,0,0);
            lv_obj_set_style_text_font(p->bnum, &lv_font_montserrat_30, LV_STATE_DEFAULT);//设置字体大小
        }
    }
}

/* 功能：商品详细界面 */
void Buy_UI(void *user_data)
{
    /* 当前要购买商品的节点 */
    //创建一个全屏大画布
    lv_obj_t *main = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main,800,480);
    lv_obj_set_scrollbar_mode(main,LV_SCROLLBAR_MODE_OFF);//设置无滚动条
    lv_obj_set_style_bg_color(main, lv_color_hex(	0x800080), 0);//设置次背景颜色为渐变从紫色开始
    lv_obj_set_style_bg_grad_color(main, lv_color_hex(0xFF1493), 0);//粉色结束
    lv_obj_set_style_bg_grad_dir(main, LV_GRAD_DIR_HOR, 0);//渐变设置

    //商品信息窗口
    lv_obj_t *product_win = lv_obj_create(main);
    lv_obj_set_size(product_win,200,420);
    lv_obj_align_to(product_win,main,LV_ALIGN_TOP_MID,0,10);
    lv_obj_set_scrollbar_mode(product_win,LV_SCROLLBAR_MODE_OFF);//设置无滚动条

    //添加图片
    lv_obj_t *img = lv_img_create(product_win);
    lv_obj_align(img,LV_ALIGN_TOP_MID,0,20);
    lv_img_set_src(img,((LL*)(user_data))->PicPath);
    lv_img_set_zoom(img,400);  //比256大是放大，小是缩小
    
    //code图片
    lv_obj_t *img_code = lv_img_create(product_win);
    lv_obj_align(img_code,LV_ALIGN_BOTTOM_MID,0,20);
    lv_img_set_src(img_code,((LL*)(user_data))->code);
    //lv_img_set_zoom(img_code,400);  //比256大是放大，小是缩小

    //购买按钮
    lv_obj_t *btn_back = lv_btn_create(product_win);
    lv_obj_align(btn_back,LV_ALIGN_BOTTOM_MID,0,-110);//中间下面
    lv_obj_set_size(btn_back,80,40);

    //购买按钮label
    lv_obj_t *btn_back_label = lv_label_create(btn_back);//创建按钮标签
    lv_obj_align_to(btn_back_label,btn_back,LV_ALIGN_CENTER,-1,0);//设置标签位置
    lv_label_set_text( btn_back_label,"BUY");//打印文本标签
    lv_obj_set_style_text_font(btn_back_label, &lv_font_montserrat_16, LV_STATE_DEFAULT);//设置字体大小

    //返回按钮
    lv_obj_t *back_btn = lv_btn_create(main);
    lv_obj_align(back_btn,LV_ALIGN_TOP_LEFT,0,0);//中间下面
    lv_obj_set_size(back_btn,90,50);

    //返回按钮label
    lv_obj_t *back_btn_label = lv_label_create(back_btn);//创建按钮标签
    lv_obj_align_to(back_btn_label,back_btn,LV_ALIGN_CENTER,-10,-4);//设置标签位置
    lv_label_set_text( back_btn_label,"BACK");//打印文本标签
    lv_obj_set_style_text_font(back_btn_label, &lv_font_montserrat_18, LV_STATE_DEFAULT);//设置字体大小

    //按钮 "+"
    lv_obj_t *btn1 = lv_btn_create(product_win);
    lv_obj_set_size(btn1,40,40);
    lv_obj_align(btn1,LV_ALIGN_BOTTOM_RIGHT,0,-155);
    lv_obj_add_event_cb(btn1,add,LV_EVENT_CLICKED,((LL*)(user_data)));
    lv_obj_t *label5 = lv_label_create(btn1);
    lv_obj_set_style_bg_color(btn1,lv_color_hex(0xFF0000),0);
    lv_label_set_text(label5, "+");
    lv_obj_set_style_text_font(label5, &lv_font_montserrat_30, LV_STATE_DEFAULT);//设置字体大小
    lv_obj_center(label5);

    //按钮 "-"
    lv_obj_t *btn2 = lv_btn_create(product_win);
    lv_obj_set_size(btn2,40,40);
    lv_obj_align(btn2,LV_ALIGN_BOTTOM_LEFT,0,-155);
    lv_obj_add_event_cb(btn2,sub,LV_EVENT_CLICKED,((LL*)(user_data)));
    lv_obj_t *label6 = lv_label_create(btn2);
    lv_obj_set_style_bg_color(btn2,lv_color_hex(0xFF0000),0);
    lv_label_set_text(label6, "-");
    lv_obj_set_style_text_font(label6, &lv_font_montserrat_30, LV_STATE_DEFAULT);//设置字体大小
    lv_obj_center(label6);

    //商品剩余量
    ((LL*)(user_data))->rnum= lv_label_create(product_win);
    lv_label_set_text_fmt(((LL*)(user_data))->rnum,"Remain %d",(((LL*)(user_data))->Remain));
    lv_obj_align(((LL*)(user_data))->rnum,LV_ALIGN_CENTER,0,-20);
    lv_obj_set_style_text_font(((LL*)(user_data))->rnum, &lv_font_montserrat_18, LV_STATE_DEFAULT);//设置字体大小

    //购买数量
    ((LL*)(user_data))->bnum = lv_label_create(product_win);
    lv_label_set_text_fmt(((LL*)(user_data))->bnum,"%d",((LL*)(user_data))->buynum);
    lv_obj_align(((LL*)(user_data))->bnum,LV_ALIGN_BOTTOM_MID,0,-160);
    lv_obj_set_style_text_font(((LL*)(user_data))->bnum, &lv_font_montserrat_18, LV_STATE_DEFAULT);//设置字体大小

    /* 根据商品结点信息显示商品名称 */
    //ID
    lv_obj_t *img_label_ID = lv_label_create(product_win);
    lv_obj_align_to(img_label_ID,product_win,LV_ALIGN_CENTER,0,-67);
    char tempbuf1[10]={0};
    sprintf(tempbuf1, "ID:%d",((LL*)(user_data))->ID);
    lv_label_set_text( img_label_ID,tempbuf1);//打印文本标签
    lv_obj_set_style_text_font(img_label_ID, &lv_font_montserrat_18, LV_STATE_DEFAULT);//设置字体大小

    //商品金额
    ((LL*)(user_data))->bprice = lv_label_create(product_win);
    lv_obj_align(((LL*)(user_data))->bprice,LV_ALIGN_CENTER,0,-50);
    char tempbuf2[10]={0};
    sprintf(tempbuf2, "CNY:%.1f",(((LL*)(user_data))->Price)*(((LL*)(user_data))->buynum));
    lv_label_set_text( ((LL*)(user_data))->bprice,tempbuf2);//打印文本标签
    lv_obj_set_style_text_font(((LL*)(user_data))->bprice, &lv_font_montserrat_18, LV_STATE_DEFAULT);//设置字体大小

    //商品名字
    lv_obj_t *label_Name = lv_label_create(product_win);
    lv_obj_align(label_Name,LV_ALIGN_CENTER,0,-35);
    lv_label_set_text(label_Name,((LL*)(user_data))->Name);
    lv_obj_set_style_text_font(label_Name, &lv_font_montserrat_18, LV_STATE_DEFAULT);//设置字体大小
    
    lv_obj_add_event_cb(btn_back,btn_return,LV_EVENT_PRESSED,user_data);//按下购买按钮更新数据
    lv_obj_add_event_cb(back_btn,back_btn_back_cb,LV_EVENT_PRESSED,user_data);//按下返回按钮返回，不更新数据
}

/* 禁止滚动动画 */
void scroll_begin_event(lv_event_t *e)
{
    /* 禁用滚动动画。点击标签按钮时触发 */
    if(lv_event_get_code(e) == LV_EVENT_SCROLL_BEGIN) 
    {
        lv_anim_t * a = lv_event_get_param(e);
        if(a)  a->time = 0;
    }
}

/* 创建标签界面 */
void Creat_tab(lv_obj_t *main)
{
    lv_obj_t *tabview = lv_tabview_create(main, LV_DIR_LEFT, 60);
    lv_obj_align(tabview, LV_ALIGN_TOP_LEFT,-20,-20);
    lv_obj_set_size(tabview,800,480);
    lv_obj_clear_flag(tabview,LV_OBJ_FLAG_SCROLL_CHAIN_HOR);
    lv_obj_add_event_cb(lv_tabview_get_content(tabview), scroll_begin_event, LV_EVENT_SCROLL_BEGIN, NULL);
    lv_obj_set_scrollbar_mode(tabview,LV_SCROLLBAR_MODE_OFF);

    tab1 = lv_tabview_add_tab(tabview, "water");
    lv_obj_set_scrollbar_mode(tab1,LV_SCROLLBAR_MODE_OFF);

    tab2 = lv_tabview_add_tab(tabview, "snake");
    lv_obj_set_scrollbar_mode(tab2,LV_SCROLLBAR_MODE_OFF);

    tab3 = lv_tabview_add_tab(tabview, "daily use");
    lv_obj_set_scrollbar_mode(tab3,LV_SCROLLBAR_MODE_OFF);

    lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);
}

/* 测试函数 */
void Buy(lv_event_t *arg)
{
    LL *p = (LL*)(arg->user_data);

    Deletemain();

    //绘制新的购买界面
    Buy_UI(arg->user_data);
}

/* 重新绘制Shop_UI画布 */
void Shop(lv_event_t *arg)
{
    Deletemain();
    
    Shop_UI(arg->user_data);
}

/* 购物车界面 */
void Shop_UI(void *user_data)
{
    /* 显示购物车界面 */
    //创建一个全屏大画布
    lv_obj_t *main = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main,800,480);
    lv_obj_set_scrollbar_mode(main,LV_SCROLLBAR_MODE_OFF);//设置无滚动条

    //商品信息窗口
    lv_obj_t *product_win = lv_obj_create(main);
    lv_obj_set_size(product_win,500,460);
    lv_obj_set_pos(product_win,100,-10);
    lv_obj_set_style_bg_color(product_win, lv_color_hex(	0x800080), 0);//设置次背景颜色为渐变从紫色开始
    lv_obj_set_style_bg_grad_color(product_win, lv_color_hex(0xFF1493), 0);//粉色结束
    lv_obj_set_style_bg_grad_dir(product_win, LV_GRAD_DIR_HOR, 0);//渐变设置
    lv_obj_set_scrollbar_mode(product_win,LV_SCROLLBAR_MODE_OFF);//设置无滚动条
    lv_obj_set_flex_flow(product_win,LV_FLEX_FLOW_ROW_WRAP);//弹性布局设置

    //购买窗口
    lv_obj_t *buy_win = lv_obj_create(main);
    lv_obj_set_size(buy_win,160,460);
    lv_obj_set_pos(buy_win,600,-10);

    //二维码code图片
    lv_obj_t *img_code = lv_img_create(buy_win);
    lv_obj_align(img_code,LV_ALIGN_TOP_MID,0,0);
    lv_img_set_src(img_code,"S:/root/pics/2code.png");

    //创建购买按钮
    lv_obj_t *shop_btn = lv_btn_create(buy_win);
    lv_obj_set_size(shop_btn,100,60);
    lv_obj_align(shop_btn,LV_ALIGN_CENTER,0,50);
    //购买标签
    lv_obj_t *back = lv_label_create(shop_btn);
    lv_obj_align(back,LV_ALIGN_CENTER,0,0);
    lv_label_set_text(back, "BUY");
    lv_obj_set_style_text_font(back, &lv_font_montserrat_24, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(shop_btn,shop_btn_back_cb,LV_EVENT_PRESSED,user_data);

    //创建订单(购物车订单)
    time_t t = time(NULL);//增加时间信息
    char *s = ctime(&t);
    strtok(s,"\n");

    FILE *fp = fopen("list_more.txt","a+");
    if(fp == NULL)
    {
        perror("error");
        exit(0);
    }
    setvbuf(fp,NULL,_IONBF,0);//无缓冲

    LL *p=((LL*)user_data)->next;
    float allprice = 0.0;
    char tmp_price[8];
    //开始围绕链表在LCD屏创建若干个对象
    for(LL *p=((LL*)user_data)->next;p!=(user_data);p=p->next)
    {
        if(p->buynum > 0 && p->Remain > 0)
        {
            //商品界面画布
            lv_obj_t *product = lv_obj_create(product_win);
            lv_obj_set_size(product,222,300);
            lv_obj_add_event_cb(product,Buy,LV_EVENT_LONG_PRESSED_REPEAT,p);
            lv_obj_set_scrollbar_mode(product,LV_SCROLLBAR_MODE_OFF);
            //lv_obj_clear_flag(product,LV_OBJ_FLAG_SCROLLABLE);  //清除滚动标志
            lv_obj_set_style_bg_color(product_win, lv_color_hex(	0x800080), 0);//设置次背景颜色为渐变从紫色开始
            lv_obj_set_style_bg_grad_color(product_win, lv_color_hex(0xFF1493), 0);//粉色结束
            lv_obj_set_style_bg_grad_dir(product_win, LV_GRAD_DIR_HOR, 0);//渐变设置
            
            // //创建复选框部件
            // checkbox = lv_checkbox_create(product);
            // lv_obj_align(checkbox,LV_ALIGN_TOP_LEFT,-5,-5);
            // lv_checkbox_set_text(checkbox, "");//默认有文本提示
            // lv_obj_set_style_pad_column( checkbox, 20, LV_STATE_DEFAULT );     //设置文本和勾选框的间距
            // lv_obj_add_state(checkbox, LV_STATE_CHECKED | LV_STATE_DISABLED);	        //添加状态：默认选中且不可修改
            // lv_obj_clear_state(checkbox, LV_STATE_CHECKED | LV_STATE_DISABLED);	        //清除复选框的状态
            // lv_obj_add_event_cb(checkbox, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);//测试复选框状态

            //商品图片
            lv_obj_t *img = lv_img_create(product);
            lv_obj_align(img,LV_ALIGN_TOP_MID,0,15);
            lv_img_set_src(img,p->PicPath);//给图片对象添加一个图片源
            lv_img_set_zoom(img,500);

            tmp_allprice = ((float)(p->buynum) * (p->Price));//转成浮点型
            
            //显示购买数量
            p->bnum = lv_label_create(product);
            lv_label_set_text_fmt(p->bnum, "count:%d", p->buynum);
            lv_obj_set_style_text_font(p->bnum, &lv_font_montserrat_24, LV_STATE_DEFAULT);//设置字体大小
            lv_obj_align(p->bnum,LV_ALIGN_CENTER,0,30);

            //显示price
            p->total_price = lv_label_create(product);
            sprintf(tmp_price,"CNY:%.1f",tmp_allprice);
            lv_label_set_text_fmt(p->total_price,"%s",tmp_price);
            lv_obj_set_style_text_font(p->total_price, &lv_font_montserrat_24, LV_STATE_DEFAULT);//设置字体大小
            lv_obj_align(p->total_price,LV_ALIGN_CENTER,0,65);

            //单件商品信息写入表格中
            if (p->buynum > 0) 
            {
                fprintf(fp, "ID:%d\t Name:%s\t num:%d\t price:%.2f\t \n",p->ID,p->Name,p->buynum,(p->buynum)*(p->Price));
            }

            allprice += tmp_allprice;
        }
    }
    //更新价格
    p->all_price = lv_label_create(buy_win);
    sprintf(tmp_price,"CNY:%.1f",allprice);
    lv_label_set_text_fmt(p->all_price, "%s", tmp_price);
    lv_obj_set_style_text_font(p->all_price, &lv_font_montserrat_24, LV_STATE_DEFAULT);//字体放大
    lv_obj_align(p->all_price,LV_ALIGN_CENTER,0,0);

    //创建返回按钮
    lv_obj_t *shop_btn_back = lv_btn_create(main);
    lv_obj_set_size(shop_btn_back,100,60);
    lv_obj_set_pos(shop_btn_back,-5,0);
    //返回标签
    lv_obj_t *btn_back = lv_label_create(shop_btn_back);
    lv_obj_align(btn_back,LV_ALIGN_CENTER,0,0);
    lv_label_set_text(btn_back, "BACK");
    lv_obj_set_style_text_font(btn_back, &lv_font_montserrat_24, LV_STATE_DEFAULT);//设置字体大小
    lv_obj_add_event_cb(shop_btn_back,back_btn_back_cb,LV_EVENT_PRESSED,user_data);

    //购物车总价写入表格中
    if (allprice > 0) 
    {
        fprintf(fp, "Total price:%.3f\t %s\n******************************\n",allprice,s);
    }
    fclose(fp);
}

/* 删除活动画布的第一个子画布 */
void Deletemain()
{
    lv_obj_del(lv_obj_get_child(lv_scr_act(), -1));
}


