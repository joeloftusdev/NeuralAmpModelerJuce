
juce_add_binary_data(BinaryData SOURCES
        knob_background.png
        Michroma-Regular.ttf
        Roboto-Regular.ttf
        background.jpg
        IRIcon.svg
)

set_target_properties(BinaryData PROPERTIES
        POSITION_INDEPENDENT_CODE TRUE)