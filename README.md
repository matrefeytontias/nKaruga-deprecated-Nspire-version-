nKaruga
=======

A clone of Treasure's *Ikaruga* for TI-Nspire calculators. Of course, this is still in early beta stages.

Building
========

To build the executable, you'll need everything that is in the nKaruga/src and nKaruga/gfx folder. Just build in nKaruga/ using your favorite tool (CLI or Ndless SDK). nKaruga.tns and nKaruga_emu will be generated in the main folder, ready to be sent to your TI-Nspire (any TI-Nspire) for the former, and to a TI-Nspire emulator for the latter.

If you use the CLI for building Ndless programs, running make in the root directory will be enough.

**make_nKarugaSprite.exe** is a purposely-built tool that should not be used for other projects than nKaruga.

**THE PROGRAM MUST NOT BE RAN IN CLI ! DRAP-AND-DROP ANY NUMBER OF FILES TO IT INSTEAD**

Running on a TI-Nspire
======================

You need **at least** Ndless 3.1 installed to run nKaruga on your TI-Nspire. That means nKaruga will only run on your TI-Nspire if the installed OS is Ndless 3.1 or higher-compatible.

Use nKaruga.tns on real hardware, and nKaruga_emu.tns on an Nspire emulator (else it's waaaaaay too fast).

Playing the game
================

Ikaruga is a danmaku (*bullet hell*) SHMUP (SHoot-theM-UP) with a twist : enemy ships and your own ship can be of one of two polarities, light or shadow. Your own ship is different though : you are free to switch between both polarities, when enemy ships are stuck with the same polarity. When in light polarity mode, only shadow bullets hit you, and when in shadow polarity mode, only light bullets hit you. When you get hit by a bullet of the same polarity than you, the bullet is absorbed and fills your power gauge instead of hurting you. When you have absorbed enough bullets, you can do a _power release_ and fire an amount of homing lasers given by the total number of bullets you absorbed divided per 10, rounded down. Your power gauge is then emptied.

When you fire at an enemy, there are two situations : either the bullet hitting the enemy is of the opposite polarity, in this case the bullet inflicts double damages ; or the bullet is of the same polarity, in this case the bullet only inflicts normal damage but the enemy will fireback bullets of its own polarity when dying.

Commands for this beta :
* CTRL   : fire
* SHIFT  : switch polarity
* DEL    : release power
* arrows : move
* ESC  : quit

Temporary debug commands:
* 4 : enable touchpad (obviously, it only works with touchpad Nspires)
* 5 : disable touchpad
* 7 : enable background
* 8 : disable background

The beta ends when all enemies are defeated or quitted the screen.

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
