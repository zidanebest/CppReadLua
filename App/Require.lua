local Register=function(moduleName)
    _G[moduleName]=require(moduleName)
end

-- Register("Test")

T={
    a={
        b=1
    }
}

-- local array=require "array"
-- a=array.new(5)
-- array.set(a,1,4)
-- b=array.get(a,1)