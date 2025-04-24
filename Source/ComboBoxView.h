#pragma once

// Args: (int width, int height, juce::StringArray comboBoxItems, std::string comboBoxId)
//TODO: Why is this shit invisible.
namespace jiveGui
{
	class ComboBoxView : public jive::View
	{
	public:
		ComboBoxView(int rawWidth, int rawHeight, juce::String rawComboBoxId)
			: width(rawWidth), height(rawHeight), comboBoxId(rawComboBoxId)
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
					{"focus-outline", true},
					{
					"style",
						new jive::Object{
							{"background", jive::toVar(colors::sky_primary)},
						}
					}
				},
				{
				}
			};
		}

		std::unique_ptr<juce::Component> createComponent(const juce::ValueTree& tree) final
		{
			if (tree.getType().toString() == "ComboBox")
			{
				DBG("Found a ComboBox!");
				return std::make_unique<juce::ComboBox>();
			}
			else if (tree.getType().toString() == "Component")
			{
				DBG("Found the placeholder!");
				//DBG(tree.toXmlString());
				return std::make_unique<juce::Component>();
			}
			else
				return nullptr;
		};

		void setup(jive::GuiItem& item) final
		{
			// We confirm access to the comboBox itself.
			if (auto* comboBox = dynamic_cast<juce::ComboBox*>(item.getComponent().get()))
			{
				comboBox->setVisible(true);
			}
			else
			{
				jassertfalse;
			}
		}

	private:
		int width, height;
		const juce::String comboBoxId;
	};
}