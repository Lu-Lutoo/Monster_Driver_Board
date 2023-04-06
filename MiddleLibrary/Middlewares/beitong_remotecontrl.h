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
    BT_RightRocker = 0x40U,
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
        inline uint8_t get_movment(){
            return movement;
        }
        inline bool is_X(){
            return movement & BT_X;
        }
        inline bool is_A(){
            return movement & BT_A;
        }
        inline bool is_B(){
            return movement & BT_B;
        }
        inline bool is_Y(){
            return movement & BT_Y;
        }
        inline bool is_LB(){
            return movement & BT_LB;
        }
        inline bool is_RB(){
            return movement & BT_RB;
        }
        inline bool is_LeftRocker(){
            return Other & BT_LeftRocker;
        }
        inline bool is_RightRocker(){
            return Other & BT_RightRocker;
        }
        inline bool is_Logo(){
            return Other & BT_Logo;
        }
        inline bool is_Start(){
            return Other & BT_Start;
        }
        inline bool is_Select(){
            return Other & BT_Select;
        }
        inline bool is_LT(){
            return Other & BT_LT;
        }
        inline bool is_RT(){
            return Other & BT_RT;
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
        uint8_t direction;
        uint8_t movement;
        uint8_t Other;
        uint8_t LeftRocker_TranverseValue;
        uint8_t LeftRocker_VerticalValue;
        uint8_t RightRocker_TranverseValue;
        uint8_t RightRocker_VerticalValue;
        uint8_t LT_Depth;
        uint8_t RT_Depth;
};