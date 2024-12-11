add_subdirectory(GUI)

target_sources(${PROJECT_NAME}
        PRIVATE
        PluginEditor.cpp
        PluginEditor.h
        PluginProcessor.cpp
        PluginProcessor.h
)


target_sources(${PROJECT_NAME}
        PRIVATE
        core/NAMProcessor.cpp
        core/NAMProcessor.h
)


target_sources(${PROJECT_NAME}
        PRIVATE
        tonestack/ToneStack.cpp
        tonestack/ToneStack.h
)

target_sources(${PROJECT_NAME}
        PRIVATE
        impulse-response/ImpulseResponse.cpp
        impulse-response/ImpulseResponse.h
)

target_sources(${PROJECT_NAME}
        PRIVATE
        noise-gate/NoiseGate.cpp
        noise-gate/NoiseGate.h
)

target_sources(${PROJECT_NAME}
        PRIVATE
        resampler/Resampler.h
)

target_sources(${PROJECT_NAME}
        PRIVATE
        architecture/architecture.hpp
)

target_sources(${PROJECT_NAME}
        PRIVATE
        state/StateManager.cpp
        state/StateManager.h
)

target_sources(${PROJECT_NAME}
        PRIVATE
        rms/meter.h
)

