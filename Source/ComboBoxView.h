#pragma once

class ComboBoxView :
	public jive::View
{
public:
	ComboBoxView(juce::StringArray rawComboBoxItems, std::string rawComboBoxId)
		: comboBoxId(rawComboBoxId), comboBoxItems(rawComboBoxItems)
	{
	}

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
			comboBox->addItemList(comboBoxItems, 1); // Adds the items to the comboBox.
			//TODO: Make sure that it has a default value set. Might not need to do this once connected to the parameter.
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