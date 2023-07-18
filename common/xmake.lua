target("common")
    set_kind("static")
    add_files("**.cpp")
    add_includedirs("includes", {public = true})
    set_languages("cxx20")
    add_defines("_CRT_SECURE_NO_WARNINGS")
    set_policy("build.warning", true)
    set_warnings("allextra")
    add_linkdirs("libs")
    if is_plat("windows") then
        add_defines( "UNICODE", "_UNICODE")
        add_cxflags("/execution-charset:utf-8", "/source-charset:utf-8")
    end