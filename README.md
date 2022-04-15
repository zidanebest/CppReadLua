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
int main(int argc, char* argv[])
{
    cout<<"----------------------Table1--------------------------"<<endl;
    LuaTable table("Test");
    cout<<table.GetString("LuaFilterParaments","Type")<<endl;
    cout<<table.GetNumber("100","RangeAngle")<<endl;

    
    cout<<"----------TraverseRecursive----------"<<endl;
    table.TraverseTableRecursive([](std::string key,const LuaData& value)->bool
    {
        cout<<key<<" --- "<<value<<endl;
        return true;
    });
    
    cout<<"----------TraverseTable--------------"<<endl;
    table.TraverseTable([](std::string key,const LuaData& value)->bool
    {
        cout<<key<<" --- "<<value<<endl;
        return true;
    });
    
    cout<<"----------------------Table2--------------------------"<<endl;
    LuaTable t2=table.GetTable("LuaFilterParaments");
    cout<<t2.GetData("Radius").Cast2Number()<<endl;
    cout<<t2.GetData("RangeAngle").IsNumber()<<endl;
    cout<<t2.GetData("RangeAngle").IsTable()<<endl;
    
    cout<<"----------------------Table3--------------------------"<<endl;
    LuaTable t3("Test",100);        //lua api访问数字键的时候必须提供一个数值，而非字符串，因此这里跟Get类型函数不一样
    cout<<t3.GetBool("200")<<endl;
    
    //cout<<t3.GetString("RangeAngle")<<endl;  //类型检查
    return 0;
}
```
