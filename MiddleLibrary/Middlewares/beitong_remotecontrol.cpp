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
    uint8_t temp;
    if(Size != 22U)
    {
        /* ERROR 数据有问题*/
        return;
    }

    /*shi mountain*/
    /* 方向键键值更新 */
    temp = *(pData + 11);
    switch (temp)
    {
    case BT_DNone:          direction = BT_DNone;           break;
    case BT_Forward:        direction = BT_Forward;         break;
    case BT_Backward:       direction = BT_Backward;        break;
    case BT_Left:           direction = BT_Left;            break;
    case BT_Right:          direction = BT_Right;           break;
    default:
        /* ERROR 数据有问题*/
        break;
    }
    /* 动作键键值更新 */
    temp = *(pData + 12);
    switch (temp)
    {
    case BT_MNone:      movement = BT_MNone;    break;
    case BT_X:          movement = BT_X;        break;
    case BT_Y:          movement = BT_Y;        break;
    case BT_A:          movement = BT_A;        break;
    case BT_B:          movement = BT_B;        break;
    case BT_LB:         movement = BT_LB;       break;
    case BT_RB:         movement = BT_RB;       break;
    default:
        /* ERROR 数据有问题*/
        break;
    }
    /* 其他键值更新 */
    temp = *(pData + 13);
    switch (temp)
    {
    case BT_ONone:              Other = BT_ONone;           break;
    case BT_LeftRocker:         Other = BT_LeftRocker;      break;
    case RT_RightRocker:        Other = RT_RightRocker;     break;
    case BT_Logo:               Other = BT_Logo;            break;
    case BT_Start:              Other = BT_Start;          break;
    case BT_Select:             Other = BT_Select;          break;
    case BT_LT:                 Other = BT_LT;              break;
    case BT_RT:                 Other = BT_RT;              break;
    
    default:
        break;
    }
    /* 跟新摇杆模拟值 */
    LeftRocker_TranverseValue = *(pData + 14);
    LeftRocker_VerticalValue = *(pData + 15);
    RightRocker_TranverseValue = *(pData + 16);
    RightRocker_VerticalValue = *(pData + 17);
    LT_Depth = *(pData + 18);
    RT_Depth = *(pData + 19);
}


