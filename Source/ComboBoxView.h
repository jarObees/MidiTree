#pragma once

class ComboBoxView :
	public jive::View
{
	ComboBoxView(std::string rawId, juce::StringArray rawComboBoxItems)
		: comboBoxId(rawId), comboBoxItems(rawComboBoxItems)
	{
	}
public:
	juce::ValueTree initialise() final
	{
		return juce::ValueTree{
			"ComboBox",
			{
				{"id", comboBoxId},
			},
			{
			},
		};
	}

	void setup(jive::GuiItem& item) final
	{
		// We confirm access to the comboBox itself.
		if (auto* comboBox = dynamic_cast<juce::ComboBox*>(item.getComponent().get()))
		{
			comboBox->addItemList(comboBoxItems, 0); // Adds the items to the comboBox.
		}
		else
		{
			jassertfalse;
		}
	}

private:
	const juce::String comboBoxId;
	const juce::StringArray comboBoxItems;
};