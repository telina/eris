target_sources(erisLib
    PRIVATE
        experiment.h
        prism.cpp
        prism.h
        prism_results_parser.cpp
        prism_results_parser.h
        xprism.cpp
        xprism.h
        octave.h
        octave.cpp
)

target_include_directories(erisLib PUBLIC .)