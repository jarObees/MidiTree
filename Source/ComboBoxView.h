#pragma once

// Args: (int width, int height, juce::StringArray comboBoxItems, std::string comboBoxId)
//TODO: Why is this shit invisible.
class ComboBoxView :
	public jive::View
{
public:
	ComboBoxView(int rawWidth, int rawHeight, juce::StringArray rawComboBoxItems, std::string rawComboBoxId)
		: width(rawWidth), height(rawHeight), comboBoxId(rawComboBoxId), comboBoxItems(rawComboBoxItems)
	{
	}

	juce::ValueTree initialise() final
	{
		return juce::ValueTree{
			"ComboBox",
			{
				{"id", comboBoxId},
				{"width", width},
				{"height", height},
			},
			{
				// Placeholder block of color.
				juce::ValueTree{
					"Component",
					{
						{"align-items", "centre"},
						{"justify-content", "centre"},
						{"width", "100%"},
						{"height", "100%"},
						{
							"style",
								new jive::Object{
									{"background", jive::toVar(jiveUI::colors::ground_highlight)},
								}
						}
					}
				},
			},
		};
	}

	void setup(jive::GuiItem& item) final
	{
		// We confirm access to the comboBox itself.
		if (auto* comboBox = dynamic_cast<juce::ComboBox*>(item.getComponent().get()))
		{
			comboBox->addItemList(comboBoxItems, 1); // Adds the items to the comboBox.
			//TODO: Make sure that it has a default value set. Might not need to do this once connected to the parameter.
		}
		else
		{
			jassertfalse;
		}
	}

private:
	int width, height;
	const juce::String comboBoxId;
	const juce::StringArray comboBoxItems;
};