#pragma once

// Args: (int width, int height, std::string id)
class TextEditorView 
	: public jive::View
	, private juce::TextEditor::Listener
{
public:

	TextEditorView(int rawWidth, int rawHeight, std::string rawId)
		: width{ rawWidth }, height{ rawHeight }, textValue{ getState(), "text" }, id{ rawId }
	{}

	juce::ValueTree initialise() final
	{
		return juce::ValueTree{
			"TextEditor",
			{
				{"width", width},
				{"height", height},
				{"text", jive::toVar(juce::String{"Default Text"})},
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
									{"background", jive::toVar(jiveUI::colors::crown_highlight)},
								}
						}
					}
				},
			}
		};
	}
	
	std::unique_ptr<juce::Component> createComponent(const juce::ValueTree& tree) final
	{
		if (tree.getType().toString() == "TextEditor")
			return std::make_unique<juce::TextEditor>();
		else
			return nullptr;
	};

	void setup(jive::GuiItem& item) final
	{
		if (auto* textEditor = dynamic_cast<juce::TextEditor*>(item.getComponent().get()))
		{
			setTextEditorSettings(textEditor);
			//textValue.onValueChange = [this, textEditor] {
			//	textEditor->setText(textValue);
			//	};
		}
		else
			jassertfalse;
	}

private:
	void setTextEditorSettings(juce::TextEditor* textEditor)
	{
		textEditor->addListener(this);
		textEditor->setMultiLine(true);
		textEditor->setReturnKeyStartsNewLine(true);
		textEditor->setFont(juce::Font(15.0));
		textEditor->setColour(juce::TextEditor::backgroundColourId, juce::Colours::darkgrey);
		textEditor->setColour(juce::TextEditor::textColourId, juce::Colours::white);
		textEditor->setColour(juce::TextEditor::outlineColourId, juce::Colours::black);
	}
	int width, height;
	jive::Property<juce::String> textValue;
	juce::String id;
};