// 大头菜.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <string>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::getline;
using std::to_string;

int GetToday();
void ConvertNewTime(SYSTEMTIME* time);
void PrintTime(SYSTEMTIME* time, string dosomething, string reason);
void GetTargetDate(SYSTEMTIME* time, int target, int hour,int min, int week);
void GetSalePrice(int week, int* sale_price);
int StepOne(int week, int* mon_up_price, int* mon_down_price, int sale_price);
int StepTwo(int week, int mon_up_price, int sale_price);
int StepThree(int week, int mon_up_price, int sale_price);
bool ThisWeekFinal(int day_of_week, int hour, int week, int sale_price);

int main()
{
    //init all time point
    SYSTEMTIME time_point1, time_point2, time_point3, buy_time;
    GetLocalTime(&time_point1);
    GetLocalTime(&time_point2);
    GetLocalTime(&time_point3);
    GetLocalTime(&buy_time);

    int week = 1;

    //set time point
    //  buytime
    GetTargetDate(&buy_time, 7, 11, 0, week - 1);
    //  time p1
    GetTargetDate(&time_point1, 1, 11, 55, week);
    // time p2
    GetTargetDate(&time_point2, 3, 8, 0, week);
    //  time p3
    GetTargetDate(&time_point3, 4, 12, 0, week);


    string input;
    const int today = GetToday();
    int sale_price = 0;
    double buffer = 0.0;
    int mon_up_price = 0;
    int mon_down_price = 0;

    int step = 0;

    // start!
    GetSalePrice(week, &sale_price);
    system("cls");



    while (true)
    {
        step = StepOne(week, &mon_up_price, &mon_down_price, sale_price);
        if (step == 0)
        {
            system("cls");
            cout << "不符合三期型价格区间，请返回周一重置时间....." << endl << endl;
            continue;
        }
        else
        {
            system("cls");
            cout << "符合三期型！ 请进行下一步操作。。。。。" << endl << endl;
            if (step == 1)
            {
                // 点2
                step = StepTwo(week, mon_up_price, sale_price);
                system("cls");
                if (step == 0)
                {
                    // 节点3
                    step = StepThree(week, mon_up_price, sale_price);
                    system("cls");

                    if (step == 0)
                    {
                        //下周
                        cout << "很抱歉，本周为递减型，请跳转至下周日重新刷大头菜价格。。。。。\n下周 三期型概率为45%" << endl;
                        week++;
                        GetSalePrice(week, &sale_price);
                        system("cls");
                        continue;
                    }
                    if (step == 1)
                    {
                        //周五下午
                        if (ThisWeekFinal(5, 12, week, sale_price))
                        {
                            system("cls");
                            continue;
                        }
                        break;
                    }
                    if (step == 2)
                    {
                        //周五上午
                        if (ThisWeekFinal(5, 8, week, sale_price))
                        {
                            system("cls");
                            continue;
                        }
                        break;
                    }
                    if (step == 3)
                    {
                        //目前最高
                        cout << "************本日或为最高回收价***********" << endl;
                        if (ThisWeekFinal(4, 12, week, sale_price))
                        {
                            system("cls");
                            continue;
                        }
                        break;
                    }




                }
                else if (step == 1)
                {
                    // 周四上午
                    if (ThisWeekFinal(4, 8, week, sale_price))
                    {
                        system("cls");
                        continue;
                    }
                }
                else if (step == 2)
                {
                    // 周三下午
                    if (ThisWeekFinal(3, 13, week, sale_price))
                    {
                        system("cls");
                        continue;
                    }
                }
                else if (step == 3)
                {
                    // 自身
                    cout << "************本日或为最高回收价***********" << endl;
                    if (ThisWeekFinal(3, 8, week, sale_price))
                    {
                        system("cls");
                        continue;
                    }

                }









            }
            else if (step == 2)
            {
                // 周二下午最高
                if (ThisWeekFinal(2, 13, week, sale_price))
                {
                    system("cls");
                    continue;
                }
            }


        }



    }








    system("pause");


    return 0;
}


void GetSalePrice(int week, int* sale_price)
{
    string input;
    SYSTEMTIME time;
    GetLocalTime(&time);
    GetTargetDate(&time, 7, 11, 0, week - 1);
    PrintTime(&time, "前去查看购买价格，售价越高理论最高回收价格越高\n如果价格不够高，可以通过往前调时间刷新", "程序开始！");

    cout << "输入最终确认的价格 (如不输入，默认值100)： ";
    getline(cin, input);
    if (input == "")
    {
        //cout << "!!!!" << endl;
        *sale_price = 100;
    }
    else
    {
        *sale_price = atoi(input.c_str());
    }
}


int StepOne(int week, int* mon_up_price, int* mon_down_price,int sale_price)
{
    string input="";

    SYSTEMTIME time;
    GetLocalTime(&time);
    GetTargetDate(&time, 1, 11, 55, week);



    

    PrintTime(&time,  "查看周一上午的售价以及周一下午的售价", "设置售价为" + to_string(sale_price) + "\t理论最高回收价为" + to_string(sale_price * 6));
    //周一上午
    cout << "请输入周一上午的售价：";
    getline(cin, input);
    while (input == "")
    {
        cout << "请重新输入： ";
        getline(cin, input);
    }

    try
    {
        *mon_up_price = atoi(input.c_str());
    }
    catch (...)
    {
        cout << "输入错误，退出中。。。。";
        exit(-1);
    }

    //check price
    if (*mon_up_price > sale_price * 0.9 || *mon_up_price < sale_price * 0.85)
    {
        return 0;
    }

    cout << "请等待几分钟 直至获得周一下午售价......" << endl;


    //周一下午
    cout << "请输入周一下午的售价：";
    getline(cin, input);
    while (input == "")
    {
        cout << "请重新输入： ";
        getline(cin, input);
    }

    try
    {
        *mon_down_price = atoi(input.c_str());
    }
    catch (...)
    {
        cout << "输入错误，退出中。。。。";
        exit(-1);
    }

    
    if (*mon_down_price <= *mon_up_price)
    {
        return 1;
    }
    else
    {
        return 2;
    }

}

/* 周三上午
    0 节点3
    1 周四上午
    2 周三下午
    3 高于两倍 自身时间
*/
int StepTwo(int week, int mon_up_price, int sale_price)
{
    string input="";
    SYSTEMTIME time;
    GetLocalTime(&time);
    GetTargetDate(&time, 3, 8, 0, week);


    PrintTime(&time, "请输入周三上午售价", "请查看周三上午的售价......");

    int price = 0;
    getline(cin, input);
    price = atoi(input.c_str());
    while (input == ""||price==0)
    {
        cout << "请重新输入： ";
        getline(cin, input);
        price = atoi(input.c_str());
    }

    // 判断0
    if (price <= mon_up_price)
    {
        return 0;
    }
    // 1
    if (price > mon_up_price && price <= 1.4 * sale_price)
    {
        return 1;
    }
    if (price > 1.4 * sale_price && price <= 2 * sale_price)
    {
        return 2;
    }
    if (price > 2 * sale_price)
    {
        return 3;
    }


    return 4;
}

/*
0 week++, 进行下周日

*/
int StepThree(int week,int mon_up_price,int sale_price)
{
    string input="";
    SYSTEMTIME time;
    GetLocalTime(&time);
    GetTargetDate(&time, 4, 12, 0, week);


    PrintTime(&time, "请输入周四下午售价", "请查看周四下午的售价......");
    int price = 0;

    getline(cin, input);
    price = atoi(input.c_str());
    while (input == "" || price == 0)
    {
        cout << "请重新输入： ";
        getline(cin, input);
        price = atoi(input.c_str());
    }

    if (price <= mon_up_price)
    {
        return 0;
    }
    if(price > mon_up_price && price <= 1.4 * sale_price)
    {
        return 1;
    }
    if (price > 1.4 * sale_price && price <= 2 * sale_price)
    {
        return 2;
    }
    if (price > 2 * sale_price)
    {
        return 3;
    }





    return 4;
}
















bool ThisWeekFinal(int day_of_week,int hour,int week,int sale_price)
{
    string input="";
    SYSTEMTIME time;
    GetLocalTime(&time);
    GetTargetDate(&time, day_of_week, hour, 0, week);
    PrintTime(&time, "如果价格不满意或不高于"+to_string(sale_price*2)+"输入任意符号返回周一并重新开始，直接回车退出程序", "找到可能的最高回收价时间.....");

    getline(cin, input);
    while (input == "")
    {
        exit(0);
    }
    return true;
}







int GetToday()
{
    SYSTEMTIME time;
    GetLocalTime(&time);


    /*test
    
    

    string test = "";
    SYSTEMTIME time1,time2,time3,time4,time5,time6;

    GetLocalTime(&time1);
    GetTargetDate(&time1, 5, 6, false);
    PrintTime(&time1, test, test);


    */

    return time.wDayOfWeek;


}

void PrintTime(SYSTEMTIME*time,string dosomething,string reason)
{
    cout << reason << endl<<endl;
    cout << "现在更改时间前往： " << time->wMonth << "月" << time->wDay << "日" << time->wHour << "点" <<time->wMinute<<"分"<< "\t周"<<time->wDayOfWeek<< endl<<endl;
    cout << dosomething << endl;

}

void ConvertNewTime(SYSTEMTIME* time)
{
    int month_max;
    switch(time->wMonth)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        month_max = 31;
        break;

    case 2:
        if ((time->wYear % 4 == 0 && time->wYear % 100 != 0) || time->wYear % 400 == 0)
        {
            month_max = 29;
        }
        else
        {
            month_max = 28;
        }
        break;

    default:
        month_max = 30;
        break;
    }

    if (time->wDay > month_max)
    {
        time->wDay -= month_max;
        time->wMonth++;
    }

    if (time->wMonth > 12)
    {
        time->wMonth -= 12;
        time->wYear++;
    }

    if (time->wMonth > 12 && time->wDay > month_max)
    {
        ConvertNewTime(time);
    }

    
}


void GetTargetDate(SYSTEMTIME* time, int target, int hour, int min, int week)
{
    int current_day = time->wDay;
    int current_dow = time->wDayOfWeek;

    time->wHour = hour;
    time->wMinute = min;

    time->wDayOfWeek = target;

    time->wDay += target - current_dow + week * 7;


    ConvertNewTime(time);

}


