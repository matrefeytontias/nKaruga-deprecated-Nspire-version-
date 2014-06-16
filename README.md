nKaruga
=======

A clone of Treasure's *Ikaruga* for TI-Nspire calculators. Of course, this is still in early beta stages.

Building
========

To build the executable, you'll need everything that is in the nKaruga/src and nKaruga/gfx folder. Just build in nKaruga/src using your favorite tool (CLI or Ndless SDK). nKaruga.tns will be generated in the main folder, ready to be sent to your TI-Nspire (any TI-Nspire).

**Building using alternative graphics**

Two sets of sprites are available to build nKaruga, sprite at resolution 3:4 or 1:2. To change the sprite set, uncomment the corresponding #include line in nKaruga/src/buildGameLUTs.cpp :
* ../gfx/gfx.h holds the 3:4 sprite set
* ../gfx/testMidSize/gfx.h holds the 1:2 sprite set

Running on a TI-Nspire
======================

You need **at least** Ndless 3.1 installed to run nKaruga on your TI-Nspire. That means nKaruga will only run on your TI-Nspire if the installed OS is Ndless 3.1-compatible.

Playing the game
================

Ikaruga is a danmaku (*bullet hell*) SHMUP (SHoot-theM-UP) with a twist : enemy ships and your own ship can be of one of two polarities, light or darkness. Your own ship is different though : you are free to switch between both polarities, when enemy ships are stuck with the same polarity. When in light polarity mode, only darkness bullets hit you, and when in darkness polarity mode, only light bullets hit you. When you get hit by a bullet of the same polarity than you, the bullet is absorbed and fills your power gauge _(this feature is not implemented yet)_ instead of hurting you.

When you fire at an enemy, there are two situations : either the bullet hitting the enemy is of the opposite polarity, in this case the bullet inflicts 2 damages instead of 1 ; or the bullet is of the same polarity, in this case the bullet only inflicts 1 damage but the enemy will fireback a lot of bullets of its own polarity when dying.

Commands for this beta :
* ESC    : fire
* TAB    : switch polarity
* HOME   : release power _(this feature is not implemented yet)_
* arrows : move
* CLEAR  : quit

The beta ends when all enemies are defeated or quitted the screen.
