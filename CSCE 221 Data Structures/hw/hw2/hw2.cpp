class card {};

int main() {
    int n = 52;
    Vector<card> deck;
    Vector<card> shuffled_deck;
    for (int i=0; i<n; ++i) {
        int index = i+randomInteger(n-i);
        shuffled_deck.push_back(deck[index]);
        
        card temp_card = deck[index];
        deck[index] = deck[i]
        deck[i] = temp_card;
    }
    deck = shuffled_deck;
    
    return 1;
}

void sort(int n, Vector<card> &deck) {
    Vector<card> shuffled_deck;
    for (int i=0; i<n; ++i) {
        int index = i+randomInteger(n-i);
        shuffled_deck.push_back(deck[index]);
        
        card temp_card = deck[index];
        deck[index] = deck[i]
        deck[i] = temp_card;
    }
    deck = shuffled_deck;
}
