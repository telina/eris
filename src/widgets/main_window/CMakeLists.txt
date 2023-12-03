target_sources(erisLib
    PRIVATE
        main_window_actions.cpp
        main_window_actions_manager.cpp
        main_window_actions_manager.h
        main_window_buttons_group_manager.cpp
        main_window_buttons_group_manager.h
        main_window.cpp
        main_window.h
        main_window_manager.cpp
        main_window_manager.h
        main_window_tool_buttons_manager.cpp
        main_window_tool_buttons_manager.h
)

target_include_directories(erisLib PUBLIC .)