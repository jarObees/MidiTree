#pragma once

//TODO: Double check to see if this works at all.
class TextEditorView 
	: public jive::View
	, private juce::TextEditor::Listener
{
	TextEditorView()
		: textValue{ getState(), "text" }
	{
	}
protected:
	juce::ValueTree initialise() final
	{
		return juce::ValueTree{
			"TextEditor",
			{
				{"width", width},
				{"height", height},
				{"text", jive::toVar(juce::String{"Default Text"})},
			},
		};
	}
	
	std::unique_ptr<juce::Component> createComponent(const juce::ValueTree& tree) final
	{
		if (tree.getType().toString() == "TextEditor")
			return std::make_unique<juce::TextEditor>();
		return nullptr;
	};

	void setup(jive::GuiItem& item) final
	{
		if (auto* textEditor = dynamic_cast<juce::TextEditor*>(item.getComponent().get()))
		{
			textEditor->addListener(this);
			textValue.onValueChange = [this, textEditor] {
				textEditor->setText(textValue);
				};
		}
		else
			jassertfalse;
	}

private:
	jive::Property<juce::String> textValue;
	size_t width, height;
};