#include "common.h"

KeyEvent getk(void)
{
	static KeyEvent k;
	k = (((((isKeyPressed(KEY_NSPIRE_8) << 1) + isKeyPressed(KEY_NSPIRE_7)) << 1) + isKeyPressed(KEY_NSPIRE_5)) << 1) + isKeyPressed(KEY_NSPIRE_4);
	k <<= 4;
	k |= (((((isKeyPressed(KEY_NSPIRE_DEL) << 1) + isKeyPressed(KEY_NSPIRE_HOME)) << 1) + isKeyPressed(KEY_NSPIRE_TAB)) << 1) + isKeyPressed(KEY_NSPIRE_ESC);
	k <<= 4;
	k |= (((((isKeyPressed(KEY_NSPIRE_UP) << 1) + isKeyPressed(KEY_NSPIRE_RIGHT)) << 1) + isKeyPressed(KEY_NSPIRE_LEFT)) << 1) + isKeyPressed(KEY_NSPIRE_DOWN);
	if(isKeyPressed(KEY_NSPIRE_UPRIGHT)) k |= 0x0c;
	if(isKeyPressed(KEY_NSPIRE_RIGHTDOWN)) k |= 0x05;
	if(isKeyPressed(KEY_NSPIRE_DOWNLEFT)) k |= 0x03;
	if(isKeyPressed(KEY_NSPIRE_LEFTUP)) k |= 0x0a;
	return k;
}
