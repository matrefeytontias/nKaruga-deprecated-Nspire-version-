#ifndef INC_MISC
#define INC_MISC
static const char string_nKaruga_config[] = "nKaruga.cfg.tns";

static const char string_title[] = "Press Enter to start or Del to quit";

static const char string_continue[] = "Continue ? (Enter/Del : yes/no)";

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

static const int power_fill_offsets[] = { 1, 2, 1, 5, 1, 8, 1, 10, 1, 10, 1, 10, 1, 10, 3, 10, 6, 10, 9, 10 };
#endif