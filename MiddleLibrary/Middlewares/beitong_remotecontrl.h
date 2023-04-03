/**
 * @file beitong_remotecontrl.h
 * @author Lutoo (e19135693@163.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <stdint.h>

typedef enum
{
    BT_DNone = 0x0FU,
    BT_Forward = 0x00U,
    BT_Backward = 0x04U,
    BT_Left = 0x06U,
    BT_Right = 0x02U
} BT_Directoin;

typedef enum
{
    BT_MNone = 0x00U,
    BT_X = 0x10U,
    BT_Y = 0x08U,
    BT_A = 0x02U,
    BT_B = 0x01U,
    BT_LB = 0x40U,
    BT_RB = 0x80U
} BT_Movement;

typedef enum
{
    BT_ONone = 0x00U,
    BT_LeftRocker = 0x20U,
    RT_RightRocker = 0x40U,
    BT_Logo = 0x10U,
    BT_Start = 0x08U,
    BT_Select = 0x04U,
    BT_LT = 0x01U,
    BT_RT = 0x02U
} BT_Other;

class BT_Controller_Classdef
{
    public:
        BT_Controller_Classdef(){}
        ~BT_Controller_Classdef(){}

        void Update(uint8_t *pData, uint16_t Size);
        BT_Directoin get_direction_key(){
            return direction;
        }
        BT_Movement get_movement_key(){
            return Movement;
        }
        BT_Other get_other_key(){
            return Other;
        }
        uint8_t get_LeftRocker_TranverseValue(){
            return LeftRocker_TranverseValue;
        }
        uint8_t get_LeftRocker_VerticalValue(){
            return LeftRocker_VerticalValue;
        }
        uint8_t get_RightRocker_TranverseValue(){
            return RightRocker_TranverseValue;
        }
        uint8_t get_RightRocker_VerticalValue(){
            return RightRocker_VerticalValue;
        }
        uint8_t get_LT_Depth(){
            return LT_Depth;
        }
        uint8_t get_RT_Depth(){
            return RT_Depth;
        }
        

    private:
        BT_Directoin direction;
        BT_Movement movement;
        BT_Other Other;
        uint8_t LeftRocker_TranverseValue;
        uint8_t LeftRocker_VerticalValue;
        uint8_t RightRocker_TranverseValue;
        uint8_t RightRocker_VerticalValue;
        uint8_t LT_Depth;
        uint8_t RT_Depth;
};