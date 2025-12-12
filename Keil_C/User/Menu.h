#ifndef __MENU_H
#define __MENU_H

// 根据你的工程实际情况包含基础头文件
// 如果你的工程报错找不到 sc32.h，请改为 sc32_conf.h 或其他你工程里的头文件
#include "sc32.h"           
#include "TFT_Lcd.h"        

// ================= 1. 颜色安全定义 =================
// 防止 TFT_Lcd.h 没定义这些颜色导致报错
#ifndef RED
#define RED   0xF800
#endif
#ifndef BLACK
#define BLACK 0x0000
#endif
#ifndef WHITE
#define WHITE 0xFFFF
#endif
#ifndef BLUE
#define BLUE  0x001F
#endif

// ================= 2. 菜单配色配置 =================
#define MENU_BG_COLOR       RED    // 背景色
#define MENU_TEXT_SEL       WHITE  // 选中项颜色
#define MENU_TEXT_UNSEL     BLACK  // 未选中项颜色

// ================= 3. 函数声明 =================
void menu(void);

#endif /* __MENU_H */