static const char string_nKaruga_config[] = "nKaruga.cfg.tns";

static const char string_title[] = "Press Enter to start or Del to quit";

#define TITLE_OPTIONS 5
static const char *string_options[TITLE_OPTIONS] = {
	"nPlay\n",
	"iDifficulty:",
	"bUse touchpad for movement:",
	"bUse scrolling background:",
	"nConfigure controls\n"
};

#define KEYS_TO_BIND 4
static const char *string_keys[KEYS_TO_BIND] = {
	"Fire\n",
	"Switch polarity\n",
	"Release power\n",
	"Pause the game\n"
};

static const char *string_difficulties[3] = {
	"Easy\n",
	"Normal\n",
	"Hard\n"
};

static const char *string_results[6] = {
	"BATTLE REPORT\n\n\n",
	"Boss destroy bonus\n\n",
	"Score (this chapter)\n\n",
	"Max ",
	" chains\n\n",
	"Grade\n\n"
};

int power_fill_offsets[] = { 1, 2, 1, 5, 1, 8, 1, 10, 1, 10, 1, 10, 1, 10, 3, 10, 6, 10, 9, 10 };

// Colors for bosses' HP bars
// from low to high HP
// red (no HP), orange, yellow, green-yellow, dark green, green, cyan, navy, blue, purple, magenta, olive, dark gray, white
unsigned short color_HPbars[] = { 0xf800, 0xfd20, 0xffe0, 0xafe5, 0x03e0, 0x07e0, 0x07ff, 0x000f, 0x001f, 0x780f, 0xf81f, 0x7be0, 0x7bef, 0xffff };