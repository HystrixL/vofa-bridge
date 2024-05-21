set_project("vofa_bridge")
set_version("1.0.0")

target("vofa_bridge")
    set_kind("static")
    add_files("./vofa_bridge.hpp",{rule = "c++"})