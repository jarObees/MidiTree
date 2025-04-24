#pragma once
#include <jive_core/jive_core.h>
namespace jiveGui
{
	class KnobWithLabel
	{
	public:
        // Customization =========================================================================
        
        // Set parameter ID
        [[nodiscard]] auto& withId(const juce::String& paramID) noexcept
        {
            knob_id = paramID;
            return *this;
        }

        // Set label id
        [[nodiscard]] auto& withLabelId(const juce::String& labelId) noexcept
        {
            label_id = labelId;
            return *this;
        }
        // Set label text
        [[nodiscard]] auto& withLabel(const juce::String& labelText) noexcept
        {
            label_text = labelText;
            return *this;
        }

        // Set knob size
        [[nodiscard]] auto& withSize(int knobSize) noexcept
        {
            size = knobSize;
            return *this;
        }

        // Set margin between label and knob
        [[nodiscard]] auto& withMargin(const juce::String& marginString) noexcept
        {
            margin = marginString;
            return *this;
        }
        // =====================================================================================
        // Main function that is called whenever we need to actually build the component.
        [[nodiscard]] auto operator()() const
        {
            return juce::ValueTree{
                // Main Component
                "Component",
                {
                    {"flex-direction", "column"},
                    {"align-items", "centre"},
                    {"margin", margin},
                },
                // Knob Component
                {
                    juce::ValueTree{
                        "Knob",
                        {
                            {"id", knob_id},
                            {"width", size},
                            {"height", size},
                        }
                    },
                // Label Component
                    juce::ValueTree{
                        "Text",
                        {
                            {"id", label_id},
                        },
                        {
                            juce::ValueTree{
                                "Text",
                                {
                                    {"text", label_text},
                                },
                            },
                        },
                    },
                },
            };
        }

	private:
        juce::String parameter;
        juce::String knob_id;
        juce::String label_id;
        juce::String label_text;
		int size = 80;
		juce::String margin = "10 0 0 0";
	};
}