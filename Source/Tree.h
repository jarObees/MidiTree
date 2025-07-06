#pragma once

namespace jiveGui
{
	namespace ForestUI
	{
		enum TreeState
		{
			ACTIVE,
			LOADED,
			UNLOADED,
		};

		class TreeComponent : public juce::Component
		{
		public:
			TreeComponent()
			{
				currentState = TreeState::UNLOADED;
			}
			TreeState getTreeState() const { return currentState; } 
			void setTreeState(TreeState newState)
			{
				currentState = newState;
			}
		private:
			TreeState currentState;
		};

		class TreeComponentView : public jive::View
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
				onStateChange = std::make_unique<jive::Event>(item.state, "on-change");
				if (auto* main = dynamic_cast<TreeComponent*>(item.getComponent().get()))
				{
					onStateChange->onTrigger = [this, main]()
					{
						switch (main->getTreeState())
						{
						case TreeState::ACTIVE:
							imageSource->set(juce::ImageCache::getFromMemory(BinaryData::TreeOn_png,
																			 BinaryData::TreeOff_pngSize));
							break;

						case TreeState::UNLOADED:
							imageSource->set(juce::ImageCache::getFromMemory(BinaryData::TreeOff_png,
																			 BinaryData::TreeOff_pngSize));
							break;
						case TreeState::LOADED:
							imageSource->set(juce::ImageCache::getFromMemory(BinaryData::TreeLoaded_png,
																			 BinaryData::TreeLoaded_pngSize));
							break;
						}
					};
					onStateChange->trigger();
				}
			}
		private:
			std::unique_ptr<jive::Property<bool>> enabledProperty;
			std::unique_ptr<jive::Property<juce::Image>> imageSource;
			std::unique_ptr<jive::Event> onStateChange;
			juce::Image imageToShow;
			int treeNum;
			juce::String id;
		};
	}
}