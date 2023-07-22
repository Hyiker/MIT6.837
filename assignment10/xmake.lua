add_requires("opengl", "glu", "tbb")

target("raytracerx")
    set_kind("binary")
    add_deps("common")
    add_packages("opengl", "glu", "tbb")

    add_includedirs("include")
    add_files("*.cpp")

    set_policy("build.warning", true)
    set_warnings("allextra")
    set_languages("c17", "cxx20")
    add_defines("_USE_MATH_DEFINES", "_HAS_STD_BYTE=0", "NOMINMAX")

task("raytracerx_scenes")
    set_category("plugin")
    on_run(function ()
        os.exec("xmake f -m release -c")
        os.exec("xmake build raytracerx")

        local assets = path.join(os.scriptdir(), "assets")
        local imgs = path.join(os.scriptdir(), "imgs")

        os.exec("xmake r raytracerx -input ".. assets .."/scene7_03_marble_vase.txt -size 200 200 -output ".. imgs .."/output7_03a.tga -grid 15 30 15 -shadows")

    end)
    set_menu{}
