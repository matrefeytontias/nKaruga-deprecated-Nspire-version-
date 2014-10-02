nKaruga
=======

A clone of Treasure's *Ikaruga* for TI-Nspire calculators. Of course, this is still in early beta stages.

Building
========

To build the executable, you'll need everything that is in the nKaruga/src and nKaruga/gfx folder. Just build in nKaruga/ using your favorite tool (CLI or Ndless SDK). nKaruga.tns and nKaruga_emu will be generated in the main folder, ready to be sent to your TI-Nspire (any TI-Nspire) for the former, and to a TI-Nspire emulator for the latter.

If you use the CLI for building Ndless programs, running make in the root directory will be enough.

**make_nKarugaSprite.exe** is a purpose-built tool that should not be used for other projects than nKaruga.

**THE PROGRAM MUST NOT BE RAN IN CLI ! DRAG-AND-DROP ANY NUMBER OF IMAGES AT ONCE TO IT INSTEAD.**

Running on a TI-Nspire
======================

You need **at least** Ndless 3.1 installed to run nKaruga on your TI-Nspire. That means nKaruga will only run on your TI-Nspire if the installed OS is Ndless 3.1 or higher-compatible. nKaruga is also compatible with Ndless 3.6.

Use nKaruga.tns on real hardware, and nKaruga_emu.tns on an Nspire emulator (else it's waaaaaay too fast).

Playing the game
================

Ikaruga is a danmaku (*bullet hell*) SHMUP (SHoot-theM-UP) with a twist : enemy ships and your own ship can be of one of two polarities, light or shadow. Your own ship is different though : you are free to switch between both polarities, when enemy ships are stuck with the same polarity. When in light polarity mode, only shadow bullets hit you, and when in shadow polarity mode, only light bullets hit you. When you get hit by a bullet of the same polarity than you, the bullet is absorbed and fills your power gauge instead of hurting you. When you have absorbed enough bullets, you can do a _power release_ and fire an amount of homing lasers given by the total number of bullets you absorbed divided per 10, rounded down. Your power gauge is then emptied.

When you fire at an enemy, there are two situations : either the bullet hitting the enemy is of the opposite polarity, in this case the bullet inflicts double damages ; or the bullet is of the same polarity, in this case the bullet only inflicts normal damage but the enemy will fireback bullets of its own polarity when dying.

The main menu
=============

# Play

You get it I think. Press it to get in the game.

# Difficulty

Repeatedly press ENTER to loop through the three possible difficulties.

* Easy : enemies never fire back when killed.
* Normal : enemies fire back when killed by bullets of their polarity.
* Hard : enemies always fire back. Be ready for it.

# Use touchpad for movement

Enable this so you can sweep your fingers on the touchpad to move your ship instead of using the arrow keys. Obviously, this will only work if your calc has a touchpad.

# Use scrolling background

Disable this if you're really concerned about battery usage or use a TI-Nspire that doesn't have a color screen. If you don't understand why, just try.

# Configure controls

Here you can customize your controls for nKaruga. Simply press the key you want the displayed action to be binded to.

Default commands :
* CTRL   : fire
* SHIFT  : switch polarity
* DEL    : release power
* arrows : move
* P      : pause the game
* ESC    : quit

Scoring (a whole lot)
=====================

nKaruga implements the same complex scoring system as the original Ikaruga, and thus scoring takes an important part in the gameplay.

Each time one of your bullets hits an enemy, you earn 100 points, and each time you absorb a bullet of your polarity you get 100 points as well. Each time you kill an enemy, its polarity is remembered, and if you kill three enemies of the same polarity in a row, your _chain bonus_ doubles. Your _chain bonus_ is bonus points you get each time you complete a 3-kills streak of one polarity. It starts at 100 and doubles with each 3-kills streak you complete up to a maximum of 25 600.
But beware ! If you kill an enemy of the opposite polarity in the middle of a chain without it being finished (for example when you kill two blue ships and then one red ship), your chain breaks, your chain bonus goes back to 100 and your chain count to 0, and everything is to be done again.

One last thing, there is no other scoring during bosses than bullet hit and absorption.

Credits
=======

Code : matrefeytontias

GFX : Metaru, Hayleia, pierrotdu18, aeTIos, matrefeytontias
