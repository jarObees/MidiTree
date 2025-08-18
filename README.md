MidiTree - Sequencer MIDI FX Plugin
=================================
MidiTree is a sequencer plugin where the user can define their own [L-Systems](https://en.wikipedia.org/wiki/L-system) in order to generate a string of notes.
When a note is played by the user, it will become the root of the sequence, and will begin arpeggiating through the generated notes.
These generated notes are relative to the note input.


<img width="432" height="876" alt="TINGY" src="https://github.com/user-attachments/assets/e37c2b45-547a-4314-8866-2fcb37f8e344" />

# ✨ Features
- Define your own L-System rules to generate your own note sequence!
- Supports VST3.
- The "Forest" helps manage and automate through presets on the fly!

# 🤔How to use
## Basic Use
_(0. Familiarize yourself with [L-Systems](https://en.wikipedia.org/wiki/L-system) if unfamiliar)_
#### 1. Define your rules.
- Hover over the note wheel to select your interval.
- Adjust octaves by hovering over the notewheel and scrolling.
- Adjust direction by clicking the ascending/descending arrow.
#### 2. Select how many generations you want to iterate through.
#### 3. Grow it!
#### 4. Adjust note type (quarter, dotted, triplet), velocity and note rate at the bottom tab.
#### 5. Save it and name your new tree!
#### 6. You can load in trees by clicking the DNA button.
## Forest
Once you have a saved tree, you can load it into the forest.
#### 1. Adjust forest slider to the desired tree slot.
#### 2. Click the shovel button to plant it. 
Now, unless bypassed, if the slider is over a loaded tree slot, that will be the tree that will be actively played. Feel free to load in all 7 slots, and smoothly slide between different trees live!
#### 3. To remove a tree from a slot, click the skull button. Hold click to clear out all the slots.
# 🎯 Future Features
- Support non-deterministic, [Stochastic L-Systems](https://en.wikipedia.org/wiki/L-system#Stochastic_grammars).
- Improve UI further for clarity and better user-feedback.
- Bundle in fun tree presets.
- Bundle in a how-to guide that can be opened from the plugin.

# ⚠️ Notes
- Currently an ALPHA release. Expect bugs and quirks!
- Report bugs, suggestions or comments to me e-mail:

# For use in 🅰️bleton
Ableton is clunky with MIDI FX, idk why so follow these steps and you should be good.
1. Load MidiTree on it's own AUDIO track.
2. Route the MidiTree Track to your instrument's track.
Should look something like this:
<img width="361" height="218" alt="image" src="https://github.com/user-attachments/assets/d70413d2-b0e7-45e3-a218-9a6ee7259042" />


3. VOILÀ
