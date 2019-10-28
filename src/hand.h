#ifndef HAND_H
#define HAND_H

#include <gtkmm.h>

using namespace std;

struct DefineCard
{
    string suit;
    string face;
    int pointsValue;   // FACE => 10 POINTS & ACE's => 1 OR 11 (WHICHEVER IS BEST FOR PLAYER/DEALER
    int cardStatus;   // 0 = InPlay , 1 = InDeck, 2 = DiscardPile
} Deck[53]; // Deck[52] will be used when shuffling deck

class Hand
{
public:
    Hand();
    virtual ~Hand();

}

#endif