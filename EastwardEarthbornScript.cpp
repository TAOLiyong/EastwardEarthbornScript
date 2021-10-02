#include <stdio.h>
#include <iostream>
#include <easyx.h>
#include <windows.h>
#include <string>
#include<cmath>

RECT rect;

BYTE scan_code(DWORD pKey)//虚拟信号转成扫描码,突破游戏对虚拟键盘的屏蔽
{
    const DWORD result = MapVirtualKey(pKey, MAPVK_VK_TO_VSC);
    return static_cast<BYTE>(result);
}

void presskey(DWORD pKey)
{
    keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), 0, 0);
    Sleep(100);
    keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), KEYEVENTF_KEYUP, 0);
    Sleep(100);
}

bool isSame(IMAGE* pimg1,IMAGE* pimg2,int x,int y)
{
    for(int i=0;i<x;i++)
    {
        for (int j = 0; j < y; j++)
        {
            SetWorkingImage(pimg1);
            COLORREF c1 = getpixel(i, j);
            SetWorkingImage(pimg2);
            COLORREF c2 = getpixel(i, j);
            int B = abs(BYTE GetBValue(c1) - BYTE GetBValue(c2));
            int R = abs(BYTE GetRValue(c1) - BYTE GetRValue(c2));
            if (B > 80 || R > 80)//如果有一个不一样
            { 
                return false;
            }
        }
    }
    return true;
}

bool isObject(IMAGE O[5], IMAGE N[5])//判断是否为目标职业组合
{
    for (int i = 0; i < 5; i++)
    {
        if (!isSame(&O[i], &N[i], 46, 46))//只要有一个不一样
        {
            return false;
        }
            
    }
    return true;
}

void reset()//重开
{
    //按下TAB
    presskey(9);
    //按下s键
    presskey('S');
    //按下s键
    presskey('S');
    //按下s键
    presskey('S');
    //按下s键
    presskey('S');
    //按下空格
    presskey(' ');

    Sleep(7500);//点击重新开始后等7.5s
    //按下空格
    presskey(' ');

    Sleep(10000);
    for (int i = 0; i < 24; i++)
    {
        //按下空格
        presskey(' ');
        Sleep(1000);
    }
}

using std::cout; using std::cin;
using std::endl; using std::string;
int main()
{
    const char* name[] = { "圣盾骑士.jpg","苦行武僧.jpg","陷阱猎人.jpg","耗竭法师.jpg","治愈神官.jpg" };//目标职业名的图标的文件名
    IMAGE imgO[5];//目标职业图标
    IMAGE imgN[5];//当前职业图标截取
    IMAGE image(1920, 1080);//游戏窗口
    IMAGE imgNull;//空图
    IMAGE imgFlag;//停止跳对话的标志
    HWND gamehwnd = FindWindow("SDL_app", "Eastward");  //定义接受窗口ID的变量
    HDC gamehdc = GetDC(gamehwnd); //获得gamehwnd游戏窗口的图像
    HDC imghdc = GetImageHDC(&image);

    loadimage(&imgFlag, "flag.jpg");

    for (int i = 0; i < 5; i++)
    {
        loadimage(&imgO[i], name[i]);
    }

    SetForegroundWindow(gamehwnd);//把游戏窗口放在桌面的最前边
    Sleep(2000);

    while (true)//不停运行
    {
        BitBlt(imghdc, 0, 0, 1920, 1080, gamehdc, 0, 0, SRCCOPY);//把游戏窗口图像gamehdc转给图片变量imghdc
        SetWorkingImage(&image);//设置切割图片的工作区
        getimage(&imgN[0], 838, 745, 46, 46);//骑士的职业图标获取

        if (!isSame(&imgN[0], &imgO[0], 46, 46))
        {
            reset();
            continue;
        }

        SetForegroundWindow(gamehwnd);//把游戏窗口放在桌面的最前边
        //按下s键
        presskey('S');

        //按下空格
        presskey(' ');

        //按下D键
        presskey('D');

        //按下D键
        presskey('D');

        for (int i = 1; i < 5; i++)//获得其他职业图标
        {
            BitBlt(imghdc, 0, 0, 1920, 1080, gamehdc, 0, 0, SRCCOPY);//把游戏窗口图像gamehdc转给图片变量imghdc
            SetWorkingImage(&image);//设置切割图片的工作区
            getimage(&imgN[i], 1401, 271, 46, 46);
            //按下D键
            presskey('D');
        }
        if (isObject(imgO, imgN))//判断是否为目标职业组合
            break;

        reset();
    }

    return 0;
}