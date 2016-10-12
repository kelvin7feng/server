
luaPath = "lua path"

G_tb = nil

function print_r()
    if not G_tb then
        print("tb is nil")
        return false;
    end

    for key, value in pairs(G_tb) do
        print("key:"..key)
        print("value:"..value)
    end
end

function lua_average()
    print("lua_average\r\n")
    nAvg = average(10, 20, 30, 40, 50)
    print("The average is ", nAvg)
end

function add (x, y)
    return x + y
end

function lua_average()
    local nAvg = luaEngine.Average(10, 20, 30, 40, 100, 80)
    return nAvg
end