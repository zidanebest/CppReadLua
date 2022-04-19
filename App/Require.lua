local Register=function(moduleName)
    _G[moduleName]=require(moduleName)
end

Register("Test")

local json= require("json")

local RegisterJson=function(path,tableName)
    local contentPath = EnvPath["contentDir"]
    
    local fullPath=contentPath.."Script/"..path
print(fullPath)
    local file = io.open(fullPath, "r")
    local info = file:read("*a")
    file:close()
    _G[tableName] = json.decode(info)
end

RegisterJson("Datas/Houdini_data/Prologue/connect_level.json","ConnectLevel")

