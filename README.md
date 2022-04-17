# CppReadLua
提供访问Lua Table的API

要先将表注册到Require.lua中
- Test.lua
```
return {
	ID = "Sphere_200_360",
	LuaFilter = "CheckRangeHit",
	LuaFilterParaments = {
		Type = "Sphere",
		Radius = 200.0,
		RangeAngle = 360.0
	},
	[100] = {
    	    RangeAngle=10,
    	    [200]=true
    	},
	CampFilter = "Enemy"
	
}

```
```
cout<<"-----------------Create Table--------------------"<<endl;
    LuaTable table1("Test","LuaFilterParaments");
    LuaTable table2("Test",100); //构造的时候，访问数字键，输入数字
    std::shared_ptr<LuaTable> table3=table1.GetTable("100");//Get时，访问数字键，输入字符串
    cout<<"------------------Read Table---------------------"<<endl;
    double num=table2.GetNumber("RangeAngle");
    cout<<num<<endl;
    
    bool b=table2.GetBool("200");
    cout<<b<<endl;
    
    std::string str=table1.GetString("Type");
    cout<<str<<endl;
    
    LuaData d=table3->GetData("200");
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
```
