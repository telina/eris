target_sources(erisLib
    PRIVATE
        checks.h
        command.cpp
        command.h
        file_manager.cpp
        file_manager_fields.h
        file_manager.h
        list.h
        logger.cpp
        logger.h
        memory.h
        node_settings_validator.cpp
        node_settings_validator.h
        qt_utils.cpp
        qt_utils.h
        string_utils.cpp
        string_utils.h
        task.h
        tokenizer.cpp
        tokenizer.h
        utils.h
)

target_include_directories(erisLib PUBLIC .)