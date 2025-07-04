-- input.lua 是原始数据文件名
local raw = dofile("/home/dove/CdomRush/tmp/gui_common.lua")

local function extractPrefixAndIndex(name)
    -- 比如 bandit_0001 -> prefix=bandit, index=1
    local prefix, num = name:match("^(.-)_(%d+)$")
    if prefix and num then
        return prefix, tonumber(num)
    end
    return nil, nil
end

local grouped = {}

for name, data in pairs(raw) do
    local prefix, idx = extractPrefixAndIndex(name)
    if prefix and idx then
        grouped[prefix] = grouped[prefix] or {}
        grouped[prefix][idx] = {
            a_name = data.a_name,
            size = data.size,
            trim = data.trim,
            a_size = data.a_size,
            f_quad = data.f_quad
        }

        if data.alias then
            for _, aliasName in ipairs(data.alias) do
                local aliasPrefix, aliasIdx = extractPrefixAndIndex(aliasName)
                if aliasPrefix == prefix and aliasIdx then
                    grouped[prefix][aliasIdx] = {
                        a_name = data.a_name,
                        size = data.size,
                        trim = data.trim,
                        a_size = data.a_size,
                        f_quad = data.f_quad
                    }
                end
            end
        end
    else
        -- 🟡 没有数字后缀的帧，保留完整名称
        grouped[name] = grouped[name] or {}
        grouped[name][1] = {
            a_name = data.a_name,
            size = data.size,
            trim = data.trim,
            a_size = data.a_size,
            f_quad = data.f_quad
        }
    end
end


-- 生成输出字符串
local function serializeTable(t, indent)
    indent = indent or ""
    local lines = {"{"}
    local subIndent = indent .. "    "
    for k, v in pairs(t) do
        local key
        if type(k) == "string" and k:match("^%a[%w_]*$") then
            key = k .. " = "
        else
            key = "[" .. tostring(k) .. "] = "
        end

        if type(v) == "table" then
            table.insert(lines, subIndent .. key .. serializeTable(v, subIndent) .. ",")
        elseif type(v) == "string" then
            table.insert(lines, subIndent .. key .. string.format("%q", v) .. ",")
        else
            table.insert(lines, subIndent .. key .. tostring(v) .. ",")
        end
    end
    table.insert(lines, indent .. "}")
    return table.concat(lines, "\n")
end

print("return " .. serializeTable(grouped))
