#include "stm32f10x.h"
#include "RTC.h"
#include "oled.h"
#include "Myview.h"
#include "matrix.h"
#include "Serial.h"
#include "Delay.h"
#include "password.h"
#include "RCC522.h"
#include "displayrc522.h"
#include "displayas608.h"
#include "Bee.h"
#include "Store.h"

uint8_t wire_as = 0xFF;
uint8_t wire_ic = 0xFF;
uint8_t root = 0xFF, ASGO = 0xFF;
uint8_t amend = 0xFF, again = 0xFF;
uint8_t hid = 0xFF;
uint8_t ok = 0xFF;
uint8_t sos = 0xFF;

uint8_t fail_count = 0;
uint8_t per = 0;

extern uint32_t password;
extern uint8_t matrix_num;
extern uint8_t row;
extern uint8_t card_numberbuf0[5];
uint32_t password_key;

uint8_t time_count = 15;

void wire_time_count(uint8_t count)
{
    time_count = count;
}

void add_fail_count(void)
{
    fail_count++;
}

void HC_XR(void)
{
    Serial_Init();
    if(Get_Serial_RxFlag())
    {
        switch(checkPack)
        {
            case 10:
                matrix_num = 0xFE;
                time_count = 15;
            break;
            case 0:
                Delay_ms(400);
                time_count = 15;
                if(per<9)
                {
                    matrix_num = 0;
                    password_make(0);
                    per++;
                }
            break;
            case 11:
                Delay_ms(400);
                time_count = 15;
                if(per<9)
                {
                    matrix_num = 1;
                    password_make(1);
                    per++;
                }
            break;
            case 12:
                Delay_ms(400);
                time_count = 15;
                if(per<9)
                {
                    matrix_num = 2;
                    password_make(2);
                    per++;
                }
            break;
            case 13:
                Delay_ms(400);
                time_count = 15;
                if(per<9)
                {
                    matrix_num = 3;
                    password_make(3);
                    per++;
                }
            break;
            case 14:
                Delay_ms(400);
                time_count = 15;
                if(per<9)
                {
                    matrix_num = 4;
                    password_make(4);
                    per++;
                }
            break;
            case 15:
                Delay_ms(400);
                time_count = 15;
                if(per<9)
                {
                    matrix_num = 5;
                    password_make(5);
                    per++;
                }
            break;
            case 16:
                Delay_ms(400);
                time_count = 15;
                if(per<9)
                {
                    matrix_num = 6;
                    password_make(6);
                    per++;
                }
            break;
            case 17:
                Delay_ms(400);
                time_count = 15;
                if(per<9)
                {
                    matrix_num = 7;
                    password_make(7);
                    per++;
                }
            break;
            case 18:
                Delay_ms(400);
                time_count = 15;
                if(per<9)
                {
                    matrix_num = 8;
                    password_make(8);
                    per++;
                }
            break;
            case 19:
                time_count = 15;
                Delay_ms(400);
                if(per<9)
                {
                    matrix_num = 9;
                    password_make(9);
                    per++;
                }
            break;
            case 20:
                time_count = 15;
                matrix_num = 10;
                Delay_ms(400);
                password_del();
                if(per!=0)
                    per--;
            break;
            case 30:
                time_count = 15;
                matrix_num = 20;
                Delay_ms(250);
                password_clear();
                if(per!=0)
                    per = 0;
            break;
            case 40:
                time_count = 15;
                matrix_num = 30;
                Delay_ms(250);
                password_clear();
                per = 0;
                amend = 1;
            break;
            case 50:
                matrix_num = 40;
                Delay_ms(400);
                time_count = 15;
                if(hid == 0xFF)
                    hid = 1;
                else
                    hid = 0xFF;
            break;
            case 60:
                matrix_num = 50;
                if(password == password_key)
                    ok = 1;
                else
                    ok = 0;
                time_count = 15;
            break;
            case 70:
                matrix_num = 60;
                Delay_ms(400);
                time_count = 15;
                sos = 0xFF;
                root = 1;
            break;
        }
    }
}

void View_Init(void)
{
    Matrix_Init();
    MyRTC_Init();
    OLED_Init();
    OLED_Clear();
    while(matrix_num == 0xFE)
    {
        password_key = Store_Data[1] + Store_Data[2] * 65535;
        Add_Init();
        Bee_Init();
        kaisuo_OFF();
        TIM_Cmd(TIM2, DISABLE);
        time_count = 15;
        password_clear();
        per = 0;
        HC_XR();
        Matrix_row();
        MyRTC_ReadTime();
        OLED_ShowString(34, 11, "    -  -  ", OLED_6X8);
        OLED_ShowString(40, 25, "  :  :  ", OLED_6X8);
        OLED_DrawRectangle(1, 1, 125, 61, OLED_UNFILLED);
        OLED_ShowNum(34, 11, MyRTC_Time[0], 4, OLED_6X8);
        OLED_ShowNum(34+6*5, 11, MyRTC_Time[1], 2, OLED_6X8);
        OLED_ShowNum(34+6*5+6*3, 11, MyRTC_Time[2], 2, OLED_6X8);
        OLED_ShowNum(40, 25, MyRTC_Time[3], 2, OLED_6X8);
        OLED_ShowNum(40+6*3, 25, MyRTC_Time[4], 2, OLED_6X8);
        OLED_ShowNum(40+6*3+6*3, 25, MyRTC_Time[5], 2, OLED_6X8);
        switch(MyRTC_Time[6])
        {
            case 0: OLED_ShowChinese(40, 40, "星期日"); break;
            case 1: OLED_ShowChinese(40, 40, "星期一"); break;
            case 2: OLED_ShowChinese(40, 40, "星期二"); break;
            case 3: OLED_ShowChinese(40, 40, "星期三"); break;
            case 4: OLED_ShowChinese(40, 40, "星期四"); break;
            case 5: OLED_ShowChinese(40, 40, "星期五"); break;
            case 6: OLED_ShowChinese(40, 40, "星期六"); break;
            default: OLED_ShowChinese(40, 40, "星期六"); break;
        }
        OLED_Update();
    }
}

void View_First(void)
{
    OLED_Init();
    Serial_Init();
    Matrix_Init();
    MyRTC_Init();
    OLED_Clear();

    while(matrix_num != 0xFE)
    {
        TIM_Cmd(TIM2, ENABLE);
        HC_XR();

        if(root == 0xFF)
        {
            ASGO = 0xFF;
            Matrix_row();
            MyRTC_ReadTime();

            OLED_ShowString(5, 5, "    -  -  ", OLED_6X8);
            OLED_ShowString(80, 5, "  :  :  ", OLED_6X8);
            OLED_ShowNum(5, 5, MyRTC_Time[0], 4, OLED_6X8);
            OLED_ShowNum(5+6*5, 5, MyRTC_Time[1], 2, OLED_6X8);
            OLED_ShowNum(5+6*5+6*3, 5, MyRTC_Time[2], 2, OLED_6X8);
            OLED_ShowNum(80, 5, MyRTC_Time[3], 2, OLED_6X8);
            OLED_ShowNum(80+6*3, 5, MyRTC_Time[4], 2, OLED_6X8);
            OLED_ShowNum(80+6*3+6*3, 5, MyRTC_Time[5], 2, OLED_6X8);

            OLED_DrawRectangle(21, 21, 85, 20, OLED_UNFILLED);
            OLED_ShowNum(28, 23, password, 9, OLED_8X16);
            if(hid == 0xFF)
            {
                switch(per)
                {
                    case 1: OLED_ShowString(28, 23, "*", OLED_8X16); break;
                    case 2: OLED_ShowString(28, 23, "**", OLED_8X16); break;
                    case 3: OLED_ShowString(28, 23, "***", OLED_8X16); break;
                    case 4: OLED_ShowString(28, 23, "****", OLED_8X16); break;
                    case 5: OLED_ShowString(28, 23, "*****", OLED_8X16); break;
                    case 6: OLED_ShowString(28, 23, "******", OLED_8X16); break;
                    case 7: OLED_ShowString(28, 23, "*******", OLED_8X16); break;
                    case 8: OLED_ShowString(28, 23, "********", OLED_8X16); break;
                    case 9: OLED_ShowString(28, 23, "*********", OLED_8X16); break;
                }
            }
            OLED_ClearArea(28+per*8, 23, 8*(9-per), 16);
            OLED_Update();

            if(matrix_num != 50)
            {
                OLED_ClearArea(18, 47, 120, 16);
                OLED_ShowImage(53, 47, 16, 16, suo);
                OLED_Update();
            }
            else if(matrix_num == 50)
            {
                OLED_ShowChinese(33, 47, "正在解锁");
                OLED_Update();
                Delay_s(1);

                if(ok == 1)
                {
                    OLED_ClearArea(18, 47, 120, 16);
                    OLED_ShowImage(53, 47, 16, 16, suokai);
                    matrix_num = 0xFF;
                    ok = 0xFF;
                    per = 0;
                    password_clear();
                    OLED_Update();
                    Delay_s(1);
                    while(1)
                    {
                        Bee_Init();
                        OLED_ClearArea(18, 47, 120, 16);
                        OLED_ShowChinese(33, 47, "解锁成功");
                        fail_count = 0;
                        OLED_Update();
                        kaisuo_ON();
                        Delay_s(3);
                        kaisuo_OFF();
                        break;
                    }
                }
                else if(ok == 0)
                {
                    OLED_ClearArea(18, 47, 120, 16);
                    OLED_ShowChinese(33, 47, "解锁失败");
                    matrix_num = 0xFF;
                    ok = 0xFF;
                    per = 0;
                    password_clear();
                    OLED_Update();
                    Delay_s(1);
                    fail_count++;
                }
            }

            if(RC522_lock() == 1)
            {
                time_count = 15;
                matrix_num = 0xFF;
                OLED_ClearArea(18, 47, 120, 16);
                OLED_ShowChinese(33, 47, "正在解锁");
                OLED_Update();
                Delay_s(1);
                OLED_ClearArea(18, 47, 120, 16);
                OLED_ShowImage(53, 47, 16, 16, suokai);
                OLED_Update();
                Delay_s(1);
                while(1)
                {
                    Bee_Init();
                    OLED_ClearArea(18, 47, 120, 16);
                    OLED_ShowChinese(33, 47, "解锁成功");
                    OLED_Update();
                    kaisuo_ON();
                    Delay_s(3);
                    kaisuo_OFF();
                    fail_count = 0;
                    matrix_num = 0xFF;
                    password_clear();
                    ok = 0xFF;
                    per = 0;
                    break;
                }
            }

            if(fail_count == 10)
            {
                sos = 1;
                Bee_Init();
                while(sos == 1)
                {
                    time_count = 15;
                    OLED_Clear();
                    Bee_ON();
                    Delay_ms(250);
                    Bee_OFF();
                    Delay_ms(250);
                    OLED_ShowChinese(15, 5, "警告警告");
                    OLED_Update();
                    HC_XR();
                }
            }
            else if(fail_count == 7)
            {
                fail_count = 3;
                while(fail_count--)
                {
                    time_count = 15;
                    Delay_ms(1000);
                    OLED_Clear();
                    OLED_ShowChinese(15, 5, "还剩两次机会");
                    OLED_ShowNum(20, 25, fail_count, 2, OLED_8X16);
                    OLED_ShowChinese(45, 25, "秒后重试");
                    OLED_Update();
                }
                OLED_Clear();
                OLED_Update();
                fail_count = 8;
                matrix_num = 0xFF;
            }
            else if(fail_count == 2)
            {
                fail_count = 2;
                while(fail_count--)
                {
                    time_count = 15;
                    Delay_ms(1000);
                    OLED_Clear();
                    OLED_ShowChinese(15, 5, "还剩六次机会");
                    OLED_ShowNum(20, 25, fail_count, 2, OLED_8X16);
                    OLED_ShowChinese(45, 25, "秒后重试");
                    OLED_Update();
                }
                OLED_Clear();
                OLED_Update();
                fail_count = 3;
                matrix_num = 0xFF;
            }
        }

        if(root == 1)
        {
            TIM_Cmd(TIM2, DISABLE);
            time_count = 15;

            OLED_Clear();
            OLED_ShowChinese(35, 5, "一键解锁");
            OLED_ShowChinese(35, 25, "录入指纹");
            OLED_ShowChinese(35, 45, "录入卡号");
            OLED_DrawRectangle(5, 3, 120, 60, OLED_UNFILLED);
            OLED_Update();

            while(root == 1)
            {
                HC_XR();
                Matrix_row();

                if(wire_as == 1)
                {
                    Add_FR();
                    wire_as = 0xFF;
                }

                if(wire_ic == 1)
                {
                    Add_Rfid();
                    wire_ic = 0xFF;
                }
            }
        }
    }
}


void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) == SET)
    {
        if(root == 1)
        {
        }
        else
        {
            switch(row)
            {
                case 1:
                    time_count = 15;
                    matrix_num = 10;
                    Delay_ms(150);
                    password_del();
                    if(per != 0)
                        per--;
                break;
                case 2:
                    time_count = 15;
                    matrix_num = 20;
                    Delay_ms(250);
                    password_clear();
                    if(per != 0)
                        per = 0;
                break;
                case 3:
                    time_count = 15;
                    matrix_num = 30;
                    Delay_ms(250);
                break;
                case 4:
                    time_count = 15;
                    matrix_num = 60;
                break;
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) == SET)
    {
        if(root == 1)
        {
            switch(row)
            {
                case 1:
                    wire_ic = 1;
                break;
                case 2:
                    wire_as = 1;
                break;
                case 3:
                    Bee_Init();
                    kaisuo_ON();
                    Delay_s(3);
                    kaisuo_OFF();
                break;
                case 4:
                    root = 0xFF;
                    OLED_Clear();
                break;
            }
        }
        else
        {
            switch(row)
            {
                case 1:
                    time_count = 15;
                    matrix_num = 9;
                    Delay_ms(400);
                    if(per < 9)
                    {
                        password_make(9);
                        per++;
                    }
                break;
                case 2:
                    time_count = 15;
                    matrix_num = 6;
                    Delay_ms(400);
                    if(per < 9)
                    {
                        password_make(6);
                        per++;
                    }
                break;
                case 3:
                    time_count = 15;
                    matrix_num = 3;
                    Delay_ms(400);
                    if(per < 9)
                    {
                        password_make(3);
                        per++;
                    }
                break;
                case 4:
                    time_count = 15;
                    matrix_num = 50;
                    if(password == password_key)
                        ok = 1;
                    else
                        ok = 0;
                break;
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI2_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line2) == SET)
    {
        if(root == 1)
        {
        }
        else
        {
            switch(row)
            {
                case 1:
                    time_count = 15;
                    matrix_num = 8;
                    Delay_ms(250);
                    if(per < 9)
                    {
                        password_make(8);
                        per++;
                    }
                break;
                case 2:
                    time_count = 15;
                    matrix_num = 5;
                    Delay_ms(250);
                    if(per < 9)
                    {
                        password_make(5);
                        per++;
                    }
                break;
                case 3:
                    time_count = 15;
                    matrix_num = 2;
                    Delay_ms(400);
                    if(per < 9)
                    {
                        password_make(2);
                        per++;
                    }
                break;
                case 4:
                    time_count = 15;
                    matrix_num = 0;
                    Delay_ms(400);
                    if(per < 9)
                    {
                        password_make(0);
                        per++;
                    }
                break;
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

void EXTI3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line3) == SET)
    {
        if(root == 1)
        {
        }
        else
        {
            switch(row)
            {
                case 1:
                    time_count = 15;
                    matrix_num = 7;
                    Delay_ms(400);
                    if(per < 9)
                    {
                        password_make(7);
                        per++;
                    }
                break;
                case 2:
                    time_count = 15;
                    matrix_num = 4;
                    Delay_ms(400);
                    if(per < 9)
                    {
                        password_make(4);
                        per++;
                    }
                break;
                case 3:
                    time_count = 15;
                    matrix_num = 1;
                    Delay_ms(400);
                    if(per < 9)
                    {
                        password_make(1);
                        per++;
                    }
                break;
                case 4:
                    time_count = 15;
                    Delay_ms(400);
                    root = 1;
                    OLED_Clear();
                break;
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        time_count--;
        if(time_count == 0)
        {
            time_count = 15;
            matrix_num = 0xFE;
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
