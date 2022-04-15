#include "LuaTable.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    cout<<"-----------------Create Table--------------------"<<endl;
    LuaTable table1("Test","LuaFilterParaments");
    LuaTable table2("Test",100); //构造的时候，访问数字键，输入数字
    LuaTable table3=table1.GetTable("100");//Get时，访问数字键，输入字符串
    cout<<"------------------Read Table---------------------"<<endl;
    double num=table2.GetNumber("RangeAngle");
    cout<<num<<endl;

    bool b=table2.GetBool("200");
    cout<<b<<endl;

    const char* str=table1.GetString("Type");
    cout<<str<<endl;

    table3=table3.GetTable("RangeAngle");
    cout<<table3.GetString("LuaFilter")<<endl;

    LuaData d=table3.GetData("200");
    cout<<d<<endl;

    cout<<"--------------------LuaData Opt---------------------"<<endl;
    cout<<"IsBool:"<<d.IsBool()<<endl;
    cout<<"IsNumber:"<<d.IsNumber()<<endl;
    cout<<d.Cast2Bool()<<endl;
    //cout<<d.Cast2Number()<<endl;   //类型检查
    cout<<"--------------------Traverse Table---------------------"<<endl;
    table1.TraverseTable([](string key,const LuaData& data)->bool
    {
        cout<<"key: "<<key<<"--------data: "<<data<<endl;
        return true;   //返回false的时候停止遍历
    });
    cout<<"------------Traverse Table Recursive-------------"<<endl;
    table1.TraverseTableRecursive([](string key,const LuaData& data)->bool
    {
        cout<<"key: "<<key<<"--------data: "<<data<<endl;
        return true;  
    });
    
}



