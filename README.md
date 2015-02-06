nKaruga
=======

A clone of Treasure's *Ikaruga* for TI-Nspire calculators. Of course, this is still in beta stages. Source is available at https://github.com/matrefeytontias/nKaruga

Building
========

To build the executable, you'll need everything that is in the nKaruga/src and nKaruga/gfx folder. Just build in nKaruga/ using [the latest Ndless SDK](https://github.com/ndless-nspire/Ndless) by running `make` in the `src/` directory. nKaruga.tns, nKaruga.prg.tns, nKaruga_emu.tns and nKaruga_emu.prg.tns will be generated in the main folder, ready to be used. If you use Ndless 3.1, use the .prg.tns files, and if you use Ndless 3.6 use the .tns files, nKaruga being for your actual Nspire (any Nspire) and nKaruga_emu for your TI-Nspire emulator.

Running the game
================

You need **at least** Ndless 3.1 installed to run nKaruga on your TI-Nspire. That means nKaruga will only run on your TI-Nspire if the installed OS is Ndless 3.1 or higher-compatible. nKaruga is also compatible with Ndless 3.6.

Playing the game
================

Ikaruga is a danmaku (*bullet hell*) SHMUP (SHoot-theM-UP) with a twist : enemy ships and your own ship can be of one of two polarities, light or shadow. Your own ship is different though : you are free to switch between both polarities, when enemy ships are stuck with the same polarity. When in light polarity mode, only shadow bullets hit you, and when in shadow polarity mode, only light bullets hit you. When you get hit by a bullet of the same polarity than you, the bullet is absorbed and fills your power gauge instead of hurting you. When you have absorbed enough bullets, you can do a _power release_ and fire as many power fragments (homing lasers) as filled power cells (a power cell is 10 bullets). Your power gauge is then emptied.

When you fire at an enemy, there are two situations : either the bullet hitting the enemy is of the opposite polarity, in this case the bullet inflicts double damages ; or the bullet is of the same polarity, in this case the bullet only inflicts normal damage.

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
* CTRL     : fire
* SHIFT    : switch polarity
* DEL      : release power
* touchpad : disabled
* P        : pause the game
* ESC      : quit

Scoring (a whole lot)
=====================

nKaruga implements the same complex scoring system as the original Ikaruga, and thus scoring takes an important part in the gameplay.

Each time one of your bullets hits an enemy, you earn 20 points if it's of the same polarity and 40 if it's of opposite polarity, and each time you absorb a bullet of your polarity you get 100 points. Each time you kill an enemy, its polarity is "remembered", and if you kill three enemies of the same polarity in a row, your _chain bonus_ doubles. Your _chain bonus_ is bonus points you get each time you complete a 3-kills streak of one polarity. It starts at 100 and doubles with each 3-kills streak you complete, up to a maximum of 25 600.
But beware ! If you kill an enemy of the wrong polarity in the middle of a chain without it being finished (for example when you kill two blue ships and then one red ship), your chain breaks, your chain bonus goes back to 100 and your chain count to 0, and everything is to be done again.

Boss battles
============

During boss battles, chaining is not taken into account. When you battle a boss, you have a limited amount of time to take it down. Bosses may or may not respect several patterns ; if you're too slow, the boss will move to the next pattern by itself. 

Tips for boss battles :
- watch for the time ! The faster you beat the boss, the bigger the bonus at the end !
- try to figure out what polarity the boss is vulnerable to. It can change during the battle.
- also try to figure out what the hitbox is. It's not always obvious.

Credits
=======

Code : matrefeytontias
With occasional help from : Choups314, geekboy

GFX : Metaru, Hayleia, pierrotdu18, aeTIos, matrefeytontias
