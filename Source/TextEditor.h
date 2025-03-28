#pragma once
#include <jive_core/jive_core.h>

// View is used to generate a juce::TextEditor component.
class TextEditorView :
	public jive::View,
	private juce::TextEditor::Listener
{
public:
	TextEditorView(int rawWidth, int rawHeight, juce::String rawId, juce::String rawWidthPercentage = "0%", juce::String rawHeightPercentage = "0%")
		: width(rawWidth), widthPercentage(rawWidthPercentage), height(rawHeight), heightPercentage(rawHeightPercentage), id(rawId)
	{}

	juce::ValueTree initialise() final
	{
		DBG("width percentage: " << widthPercentage);
		auto _width = (widthPercentage == "0%") ? jive::toVar(width) : jive::toVar(widthPercentage);
		auto _height = (heightPercentage == "0%") ? jive::toVar(height) : jive::toVar(heightPercentage);
		return juce::ValueTree
		{
			"TextEditor",
			{
				{"id", id},
				{"width", _width},
				{"height", _height},
										{
							"style",
								new jive::Object
								{
									{ "background", jive::toVar(jiveUI::colors::ground_tertiary)},
								},
						},
			},
		};
	}

	std::unique_ptr<juce::Component> createComponent(const juce::ValueTree& tree) final
	{
		if (tree.getType().toString() == "TextEditor")
		{
			return std::make_unique<juce::TextEditor>();
		}
		else
		{
			return nullptr;
		}
	}

	void setup(jive::GuiItem& item) final
	{
		if (auto* textEditor = dynamic_cast<juce::TextEditor*>(item.getComponent().get()))
		{
			textEditorComponent = textEditor;
			setTextEditorSettings(textEditorComponent);
		}
		else
			jassertfalse;
	}

	juce::TextEditor* textEditorComponent;

private:

	void setTextEditorSettings(juce::TextEditor* textEditor)
	{
		DBG("Setting text editor settings...");
		textEditor->setText("HERE IS THE TEXT");
		textEditor->setMultiLine(true);
		textEditor->setReturnKeyStartsNewLine(true);
		textEditor->setReadOnly(false);
		textEditor->setWantsKeyboardFocus(true);
		textEditor->addListener(this);
	}

	// This function is called whenever text is changed.
	void textEditorTextChanged(juce::TextEditor& editor) override
	{
		DBG("TEXT CHANGED");
	}

	juce::String id;
	int width, height;
	juce::String widthPercentage, heightPercentage;
	std::unique_ptr<jive::Event> onValueChange;
	std::unique_ptr<jive::Property<juce::String>> textValue;
};