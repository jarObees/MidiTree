#pragma once
namespace jiveGui
{
	namespace AnalogUserInput
	{
		namespace OctaveInput
		{
			class OctavesInputView : public jive::View
			{
			public:
				OctavesInputView(int rowNum, int columnNum)
					: rowNum(rowNum), columnNum(columnNum)
				{
					id = rowColIdMaker("oi", rowNum, columnNum);
				}
				
				juce::ValueTree initialise()
				{
					return juce::ValueTree
					{
						"Component",
						{
							{"width", "100%"},
							{"height", "25%"},
							{"flex-direction", "column"},
						},
						{
							getOctaveButton(rowColIdMaker("oi0", rowNum, columnNum)),
							getOctaveButton(rowColIdMaker("oi1", rowNum, columnNum)),
							getOctaveButton(rowColIdMaker("oi2", rowNum, columnNum)),
						}
					};
				}
				void setup(jive::GuiItem& item) final
				{
					const auto children = item.getChildren();
					for (auto* guiItem : children)
					{
						auto* button = dynamic_cast<juce::Button*>(guiItem->getComponent().get());
						jassert(button != nullptr);
					}
				}
			private:
				juce::String id;
				const int rowNum, columnNum;
				juce::ValueTree getOctaveButton(juce::String id)
				{
					juce::ValueTree button
					{
						"Button",
						{
							{"id", id},
							{"width", "100%"},
							{"height", "33%"},
							{"radio-group", 1},
							{"style",
								new jive::Object
								{
									{ "background", jive::toVar(colors::red)},
								},
							},
						},
					};
					return button;
				}
			};
		}
	}
}