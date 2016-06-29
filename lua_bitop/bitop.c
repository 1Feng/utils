#include <stdint.h>
#include <inttypes.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static int rshift(lua_State* l) {
    int n = lua_gettop(l);
    if (n == 2) {
        int64_t a = lua_tonumber(l, 1);
        int64_t b = lua_tonumber(l, 2);
        lua_pushnumber(l, a >> b);
        return 1;
    }
    return 0;
}

static int lshift(lua_State* l) {
    int n = lua_gettop(l);
    if (n == 2) {
        int64_t a = lua_tonumber(l, 1);
        int64_t b = lua_tonumber(l, 2);
        lua_pushnumber(l, a << b);
        return 1;
    }
    return 0;
}

static int band(lua_State* l) {
    int n = lua_gettop(l);
    if (n == 2) {
        int64_t a = lua_tonumber(l, 1);
        int64_t b = lua_tonumber(l, 2);
        lua_pushnumber(l, a & b);
        return 1;
    }
    return 0;
}

static int bor(lua_State* l) {
    int n = lua_gettop(l);
    if (n == 2) {
        int64_t a = lua_tonumber(l, 1);
        int64_t b = lua_tonumber(l, 2);
        lua_pushnumber(l, a | b);
        return 1;
    }
    return 0;
}

static int tostring(lua_State* l) {
    int n = lua_gettop(l);
    if (n == 2) {
        uint64_t a = lua_tonumber(l, 1);
        uint64_t b = lua_tonumber(l, 2);
        //printf("%"PRIu64" %"PRIu64"\n", a, b);
        //printf("%"PRIX64" %"PRIX64"\n", a, b);
        char str[65];
        if (b == 16) {
            sprintf(str, "%"PRIx64, a);
            lua_pushstring(l, str);
        }
        if (b == 10) {
            sprintf(str, "%"PRIu64, a);
            lua_pushstring(l, str);
        }
        if (b == 2) {
            uint64_t mask = 0x8000000000000000ULL;
            int i = 0;
            for (int j = 0; j < 64; ++j) {
                if (a & mask) {
                    str[i++] = '1';
                } else {
                    if (i) {
                        str[i++] = '0';
                    }
                }
                a = a << 1;
            }
            if (i == 0) {
                str[i++] = '0';
            }
            str[i] = '\0';
            lua_pushstring(l, str);
        }
        return 1;
    }
    return 0;
}

static const luaL_Reg lib[] = {
    {"lshift", lshift},
    {"rshift", rshift},
    {"band", band},
    {"bor", bor},
    {"tostring", tostring},
    {NULL, NULL}
};

int luaopen_bitop(lua_State* l) {
    luaL_register(l, "bitop", lib);
    return 1;
}
