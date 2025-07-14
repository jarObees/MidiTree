#pragma once
#include "Ids.h"


//TODO: Figure out how to make ur own lil listener guy.
namespace jiveGui
{
	namespace ForestUI
	{
		class TreeComponent : public juce::Component,
			private juce::ChangeBroadcaster
		{
		public:
			TreeComponent() : active(false), loaded(false) {}
			
			bool getActiveState() const { return active; }
			bool getLoadedState() const { return loaded; }
			
			void setLoadedState(bool newState) 
			{ 
				DBG("Setting loaded state...");
				if (loaded != newState)
					sendChangeMessage(); // Notifies any listeners.
			}
			void setActiveState(bool newState)
			{
				DBG("Setting active state...");
				if (active != newState)
					sendChangeMessage();
			}
			void addChangeListener(juce::ChangeListener * listener) { juce::ChangeBroadcaster::addChangeListener(listener); }
			void removeChangeListener(juce::ChangeListener* listener) { juce::ChangeBroadcaster::removeChangeListener(listener); }
		private:
			bool active, loaded;
		};

		class TreeComponentView : public jive::View,
				private TreeComponent,
			private juce::ChangeListener
		{
		public:
			TreeComponentView(int treeNum)
				: treeNum(treeNum)
			{
				id = treeIdMaker(treeNum);
			}

			juce::ValueTree initialise()
			{
				return juce::ValueTree{
					"TreeComponent",
					{
						{"id", id},
						{"width", 20},
						{"height", 20},
						{"active", jive::toVar(false)},
						{"loaded", jive::toVar(false)},
					},
					{
						juce::ValueTree
						{
							"Image",
							{
								{"source", jive::toVar(imageToShow)},
								{"width", "100%"},
								{"height", "100%"},
							}
						}
					}
				};
			}

			std::unique_ptr<juce::Component> createComponent(const juce::ValueTree& tree) final
			{
				if (tree.getType().toString() == "TreeComponent")
					return std::make_unique<TreeComponent>();

				return nullptr;
			}

			void setup(jive::GuiItem& item) final
			{
				imageSource = std::make_unique<jive::Property<juce::Image>>(item.getChildren().getFirst()->state, "source");
				activeProp = std::make_unique<jive::Property<bool>>(item.state, "active");
				loadedProp = std::make_unique<jive::Property<bool>>(item.state, "loaded");
				onStateChange = std::make_unique<jive::Event>(item.state, "on-change");
				if (auto* main = dynamic_cast<TreeComponent*>(item.getComponent().get()))
				{
					// Setup listener stuff.
					main->addChangeListener(this);
					activeProp->set(main->getActiveState());
					loadedProp->set(main->getLoadedState());

					// Dynamic image here.
					onStateChange->onTrigger = [this, main]()
					{
						DBG("State change detected!");
						if (main->getActiveState())
						{
							imageSource->set(juce::ImageCache::getFromMemory(BinaryData::TreeOn_png,
																			 BinaryData::TreeOn_pngSize));
						}
						else if (!main->getActiveState() && !main->getLoadedState())
						{
							imageSource->set(juce::ImageCache::getFromMemory(BinaryData::TreeOff_png,
																			 BinaryData::TreeOff_pngSize));
						}
					};
					onStateChange->trigger();
				}
			}
		private:
			void changeListenerCallback(juce::ChangeBroadcaster* source) override
			{
				DBG("Change Callback detected!");
				if (auto* treeComponent = dynamic_cast<TreeComponent*>(source))
				{
					activeProp->set(treeComponent->getActiveState());
					loadedProp->set(treeComponent->getLoadedState());
				}
			}
			std::unique_ptr<jive::Property<bool>> activeProp;
			std::unique_ptr<jive::Property<bool>> loadedProp;
			std::unique_ptr<jive::Property<juce::Image>> imageSource;
			std::unique_ptr<jive::Event> onStateChange;
			juce::Image imageToShow;
			int treeNum;
			juce::String id;
		};
	}
}