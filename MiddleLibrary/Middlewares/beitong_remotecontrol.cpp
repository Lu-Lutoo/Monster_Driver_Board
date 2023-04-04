/**
 * @file beitong_remotecontrol.cpp
 * @author Lutoo (e19135693@163.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "beitong_remotecontrl.h"


void BT_Controller_Classdef::Update(uint8_t* pData, uint16_t Size)
{
    if(Size != 22U)
    {
        /* ERROR 数据有问题*/
        return;
    }
    /* 方向键键值更新 */
    direction = *(pData + 11);
    /* 动作键键值更新 */
    movement = *(pData + 12);
    /* 其他键值更新 */
    Other = *(pData + 13);
    /* 跟新摇杆模拟值 */
    LeftRocker_TranverseValue = *(pData + 14);
    LeftRocker_VerticalValue = *(pData + 15);
    RightRocker_TranverseValue = *(pData + 16);
    RightRocker_VerticalValue = *(pData + 17);
    LT_Depth = *(pData + 18);
    RT_Depth = *(pData + 19);
}


