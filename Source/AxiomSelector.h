#pragma once

namespace jiveGui
{
	namespace AnalogUserInput
	{
		const int NUMBLOCKCOLUMNS = 8;	// Recall: Cols start at -1.
		const int NUMBLOCKROWS = 5;
		const int BLOCKHEIGHT = 75;
		const int BLOCKWIDTH = 25;

		namespace AxiomSelecta
		{
			// This component is for use within the note wheel, hence the width and height being at 100%
			class AxiomSelectorView
				: public jive::View
			{
			public:
				AxiomSelectorView(int rowNum, int columnNum)
					: rowNum(rowNum), columnNum(columnNum)
				{
					DBG("Making an AxiomSelecta!");
					id = idRowColMaker(IdPrefix::axiomToggle, rowNum, columnNum);
					bgImage = juce::Image();
				}

				juce::ValueTree initialise() final
				{
					juce::ValueTree axiomSelectaComponent =
					{
						"Component",
						{
							{"display", "block"},
							{"width", "100%"},
							{"height", "100%"},
						},
						{
							juce::ValueTree
							{
								"Image",
								{
									{"source", jive::toVar(bgImage)},
									{"width", "100%"},
									{"height", "100%"},
								}
							},
							juce::ValueTree
							{
								"Button",
								{
									{"id", id},
									{"width", "100%"},
									{"height", "20%"},
									{"centre-x", "50%"},
									{"always-on-top", true},
									{"toggle-on-click", false},		// Helps us manually control the toggle state.
									{"toggleable", true},			// Gives access to setToggeState()
								}
							}
						}
					};
					return axiomSelectaComponent;
				}

				void setup(jive::GuiItem& item) final
				{
					DBG("Setting up the axiom selector...");
					analogUserInput = findAncestorWithIdPrefix(&item);
					jassert(analogUserInput != nullptr);

					activeAxiomId = std::make_unique<jive::Property<juce::String>>(analogUserInput->state, StringIds::activeAxiom);
					jassert(activeAxiomId != nullptr);

					imageSource = std::make_unique<jive::Property<juce::Image>>(item.state.getChild(0), "source");
					jassert(imageSource != nullptr);

					DBG("Connected activeAxiomId to shared value. Current activeAxiomId = " << activeAxiomId->get());

					auto* imageComponent = dynamic_cast<juce::Component*>(item.getChildren().getFirst()->getComponent().get());
					jassert(imageComponent != nullptr);
					imageComponent->setInterceptsMouseClicks(false, false);
					
					auto* parentBlockComponent = item.getComponent().get();
					jassert(parentBlockComponent != nullptr);
					parentBlockComponent->setInterceptsMouseClicks(false, true);

					auto* button = dynamic_cast<juce::Button*>(item.getChildren().getLast()->getComponent().get());
					jassert(button != nullptr);
					button->setMouseCursor(juce::MouseCursor::PointingHandCursor);

					// After a button is clicked, each AxiomSelector executes this lambda.
					activeAxiomId->onValueChange = [this, button]()
						{
							DBG("Change detected in: " << button->getComponentID());
							if (button->getComponentID() == activeAxiomId->get())
							{
								DBG("Setting: " << id << "to ON");
								imageSource->set(juce::ImageCache::getFromMemory(BinaryData::AxiomSelectorOn_png,
																				 BinaryData::AxiomSelectorOn_pngSize));
								button->setToggleState(true, false);
							}
							else
							{
								DBG("Setting: " << id << "to OFF");
								imageSource->set(juce::Image{});
								button->setToggleState(false, false);
							}
						};
					button->onClick = [this, button]()
						{
							DBG("Previous ACTIVEAXIOM: " << activeAxiomId->get());
							DBG("New      ACTIVEAXIOM: " << id);
							activeAxiomId->set(id);
						};
					if (instanceCount.get() == 0)
					{
						button->triggerClick();
						instanceCount = instanceCount.get() + 1;
					}
				};
			private:
				jive::GuiItem* findAncestorWithIdPrefix(jive::GuiItem* item)
				{
					while (item != nullptr)
					{
						if (item->state.hasProperty("id"))
						{
							auto idValue = item->state["id"].toString();
							DBG("Inspecting item with id: " << idValue);

							if (idValue.startsWith(StringIds::analogUserInput))
								return item;
						}
						DBG("Moving up tree...");
						item = item->getParent(); // Move up the tree
					}
					DBG("Found jack...");
					return nullptr; // Not found
				}
				inline static juce::ThreadLocalValue<int> instanceCount;
				std::vector<juce::Button*> axiomButtonsGroup;
				jive::GuiItem* analogUserInput = nullptr;
				juce::String id;
				const int rowNum, columnNum;
				juce::Image bgImage;
				std::unique_ptr<jive::Property<juce::Image>> imageSource;
				std::unique_ptr<jive::Property<juce::String>> activeAxiomId;
			};

		}
	}
}