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

#define KEYS_TO_BIND 3
static const char *string_keys[KEYS_TO_BIND] = {
	"Fire\n",
	"Switch polarity\n",
	"Release power\n"
};

static const char *string_difficulties[3] = {
	"Easy\n",
	"Normal\n",
	"Hard\n"
};
	
int power_fill_offsets[] = { 1, 2, 1, 5, 1, 8, 1, 10, 1, 10, 1, 10, 1, 10, 3, 10, 6, 10, 9, 10 };
