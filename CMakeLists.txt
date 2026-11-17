cmake_minimum_required(VERSION 3.22)
project(PitchCorrectionPlugin VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(JUCE)

juce_add_plugin(PitchCorrectionPlugin
    COMPANY_NAME "StudentProject"
    PLUGIN_MANUFACTURER_CODE Stud
    PLUGIN_CODE Ptch
    FORMATS VST3 Standalone
    PRODUCT_NAME "PitchCorrectionPlugin"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

# Allows existing Projucer source files that include JuceHeader.h.
juce_generate_juce_header(PitchCorrectionPlugin)

target_sources(PitchCorrectionPlugin PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

target_compile_definitions(PitchCorrectionPlugin PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(PitchCorrectionPlugin PRIVATE
    juce::juce_audio_utils
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)