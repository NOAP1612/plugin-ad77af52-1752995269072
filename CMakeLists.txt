CMakeLists.txt

cmake_minimum_required(VERSION 3.15)

project(StereoReverb VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# JUCE setup
find_package(JUCE CONFIG REQUIRED)

juce_add_plugin(StereoReverb
    COMPANY_NAME "YourCompany"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE Srvb
    FORMATS VST3 AU
    PRODUCT_NAME "StereoReverb"
)

target_sources(StereoReverb PRIVATE
    PluginProcessor.cpp
    PluginEditor.cpp
    PluginProcessor.h
    PluginEditor.h
)

target_link_libraries(StereoReverb PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)
```