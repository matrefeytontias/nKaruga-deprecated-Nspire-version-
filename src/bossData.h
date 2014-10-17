#define BOSS_NB 1
/* Bosses format
 * - Number of patterns
 * - Number of filled HP bars per pattern * number of patterns
 * - Callback ID
 * 
 * The callback is supposed to handle all patterns by testing the amount of HP.
 * Exceptionally, drawing should be taken care of in the callback since bosses are going to have complex graphical behaviors.
 */
int bossDataArray[1][] = { { 3, 1, 1, 3, Boss_Pattern_1 }
};