local Register=function(moduleName)
    _G[moduleName]=require(moduleName)
end

Register("Test")

