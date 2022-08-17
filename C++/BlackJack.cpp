#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <fstream>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]) {
    int bet = 0, playerCard = 0, dealerCard = 0, playerTotal = 0, dealerTotal = 0, iterator = 0, money = 0;
    char choice, pCardValue, dCardValue, hit, outcome, goAgain;
    ofstream outfile;
    ifstream infile;
    int cardOne = 0, cardTwo = 0, cardThree = 0;

    outfile.open("LastNameHistory.txt");
    outfile << "Card1" << "    Card2" << "    Card3" << "    Bet" << "    Dealer" << "    W/L" << "    Balanace" << endl;

    goAgain = 'Y';
    while(goAgain == 'Y' || goAgain == 'y') {
        bet = 0, playerCard = 0, dealerCard = 0, playerTotal = 0, dealerTotal = 0;

        cout << "(1) Play BlackJack\n(2) View Game History\n(3) Quit Game" << endl;
        cout << "\n\nSelect a choice from the menu: ";
        cin >> choice;
        cout << endl;

        switch(choice) {
            case '1':
                if(money == 0) {
                    cout << "What is your current balance?: ";
                    cin >> money;
                }
                cout << "\nLet's play!" << endl;
                cout << "Place a bet (minimum of $10 and a maximum of $150): ";
                cin >> bet;

                while(bet < 10 || bet > 150) {
                    cout << "You have either placed too low or too high of a bet." << endl;
                    cout << "Please place a bet between $10 and $150: ";
                    cin >> bet;
                }

                srand(time(NULL));
                for(int i = 1; i < 3; i++) {
                    playerCard = rand() % 13 + 1;
                    //playerCard = 1;
                    dealerCard = rand() % 13 + 1;

                    if(playerCard == 10) {
                        playerTotal += playerCard;
                        pCardValue = 'J';
                        cout << "Player card " << i << ": " << pCardValue << endl;
                    }
                    else if(playerCard == 11) {
                        playerCard = 10;
                        playerTotal += playerCard;
                        pCardValue = 'Q';
                        cout << "Player card " << i << ": " << pCardValue << endl;
                    }
                    else if(playerCard == 12) {
                        playerCard = 10;
                        playerTotal += playerCard;
                        pCardValue = 'K';
                        cout << "Player card " << i << ": " << pCardValue << endl;
                    }
                    else if(playerCard == 13) {
                        playerCard = 11;
                        playerTotal += playerCard;
                        pCardValue = 'A';
                        cout << "Player card " << i << ": " << pCardValue << endl;
                    }
                    else {
                        playerCard++;
                        pCardValue = 'X';
                        playerTotal += playerCard;
                        cout << "Player card " << i << ": " << playerCard << endl;
                    }


                    if(dealerCard == 10) {
                        dealerTotal += dealerCard;
                        dCardValue = 'J';
                        cout << "Dealer card: HIDDEN" << endl;
                    }
                    else if(dealerCard == 11) {
                        dealerCard = 10;
                        dealerTotal += dealerCard;
                        dCardValue = 'Q';
                        cout << "Dealer card: HIDDEN" << endl;
                    }
                    else if(dealerCard == 12) {
                        dealerCard = 10;
                        dealerTotal += dealerCard;
                        dCardValue = 'K';
                        cout << "Dealer card: HIDDEN" << endl;
                    }
                    else if(dealerCard == 13) {
                        dealerCard = 11;
                        dealerTotal += dealerCard;
                        dCardValue = 'A';
                        cout << "Dealer card: HIDDEN" << endl;
                    }
                    else {
                        dealerCard++;
                        dealerTotal += dealerCard;
                        cout << "Dealer card: HIDDEN" << endl;
                    }

                    if(pCardValue == 'J' || pCardValue == 'Q' || pCardValue == 'K' || pCardValue == 'A') {
                        outfile << pCardValue << "        ";
                    }
                    else {
                        outfile << playerCard << "        ";
                    }
                }
                cout << "\nPlayer Total: " << playerTotal << endl;

                if(dealerTotal == 21 && playerTotal == 21) {
                    cout << "Dealer and player both hit 21. Push!" << endl;
                    outcome = 'P';
                    outfile << "0        " << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                    break;
                }
                else if(dealerTotal == 21 && playerTotal != 21) {
                    cout << "Dealer hit 21. You lose." << endl;
                    outcome = 'L';
                    money -= bet;
                    outfile << "0        " << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                    break;
                }
                else if(dealerTotal != 21 && playerTotal == 21) {
                    cout << "You hit 21. You Win!" << endl;
                    outcome = 'W';
                    money += bet * 2;
                    outfile << "0        " << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                    break;
                }


                cout << "Would you like to hit?(Y/N): ";
                cin >> hit;
                if(hit == 'N' || hit == 'n') {
                    outfile << "0        ";
                }
                cout << endl;

                if(hit == 'Y' || hit == 'y') {
                    playerCard = rand() % 13 + 1;

                    if(playerCard == 10) {
                        playerTotal += playerCard;
                        pCardValue = 'J';
                        cout << "Player card 3: " << pCardValue << endl;
                    }
                    else if(playerCard == 11) {
                        playerCard = 10;
                        playerTotal += playerCard;
                        pCardValue = 'Q';
                        cout << "Player card 3: " << pCardValue << endl;
                    }
                    else if(playerCard == 12) {
                        playerCard = 10;
                        playerTotal += playerCard;
                        pCardValue = 'K';
                        cout << "Player card 3: " << pCardValue << endl;
                    }
                    else if(playerCard == 13) {
                        playerCard = 11;
                        playerTotal += playerCard;
                        pCardValue = 'A';
                        cout << "Player card 3: " << pCardValue << endl;
                    }
                    else {
                        playerCard++;
                        playerTotal += playerCard;
                        cout << "Player card 3: " << playerCard << endl;
                    }

                    cout << "\nPlayer Total: " << playerTotal << endl;

                    if(playerTotal > 21) {
                        cout << "Bust. You lose.\n" << endl;
                        outcome = 'L';
                        money -= bet;
                        if(pCardValue == 'J' || pCardValue == 'Q' || pCardValue == 'K' || pCardValue == 'A') {
                            outfile << pCardValue << "        " << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;

                        }
                        else {
                            outfile << playerCard << "        " << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                        }
                        break;
                    }

                    if(pCardValue == 'J' || pCardValue == 'Q' || pCardValue == 'K' || pCardValue == 'A') {
                        outfile << pCardValue << "        ";
                    }
                    else {
                        outfile << playerCard << "        ";
                    }
                }

                if(dealerTotal < 17 && playerTotal < 21) {
                    dealerCard = rand() % 13 + 1;

                    if(dealerCard == 10) {
                        dealerTotal += dealerCard;
                        dCardValue = 'J';
                        cout << "Dealer card  3: " << dCardValue << endl;
                    }
                    if(dealerCard == 11) {
                        dealerCard = 10;
                        dealerTotal += dealerCard;
                        dCardValue = 'Q';
                        cout << "Dealer card  3: " << dCardValue << endl;
                    }
                    if(dealerCard == 12) {
                        dealerCard = 10;
                        dealerTotal += dealerCard;
                        dCardValue = 'K';
                        cout << "Dealer card  3: " << dCardValue << endl;
                    }
                    if(dealerCard == 13) {
                        dealerCard = 1;
                        dealerTotal += dealerCard;
                        dCardValue = 'A';
                        cout << "Dealer card  3: " << dCardValue << endl;
                    }
                    if(dealerCard < 10) {
                        dealerCard++;
                        dealerTotal += dealerCard;
                        cout << "Dealer card  3: " << dealerCard << endl;
                    }

                    cout << "Dealer Total: " << dealerTotal << endl;

                    if(dealerTotal > 21) {
                        cout << "Dealer busts. You Win!" << endl;
                        outcome = 'W';
                        money += bet * 2;
                        outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                        break;
                    }
                    if(dealerTotal < playerTotal && dealerTotal >= 17 && dealerTotal < 21) {
                        cout << "You Win!" << endl;
                        outcome = 'W';
                        money += bet;
                        outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                        break;
                    }
                    if(dealerTotal > playerTotal && dealerTotal >= 17 && dealerTotal < 21) {
                        cout << "You Lose!" << endl;
                        outcome = 'L';
                        money -= bet;
                        outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                        break;
                    }
                    if(dealerTotal == 21 && playerTotal == 21) {
                        cout << "Dealer and player both hit 21. Push!" << endl;
                        outcome = 'P';
                        outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                        break;
                    }
                    if(dealerTotal == 21 && playerTotal != 21) {
                        cout << "Dealer hit 21. You lose." << endl;
                        outcome = 'L';
                        money -= bet;
                        outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                        break;
                    }
                    if(dealerTotal == 17 && playerTotal == 17) {
                        cout << "Dealer and player both hit 17. Push!" << endl;
                        outcome = 'P';
                        outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                        break;
                    }
                    if(dealerTotal == playerTotal && dealerTotal >= 17) {
                        cout << "Dealer and player both hit " << playerTotal << ". Push!" << endl;
                        outcome = 'P';
                        outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                        break;
                    }
                    iterator++;
                }

                if(iterator == 0) {
                    cout << "Dealer Total: " << dealerTotal << endl;
                }

                if(dealerTotal < playerTotal && playerTotal < 21 && dealerTotal < 21) {
                    cout << "You Win!" << endl;
                    outcome = 'W';
                    money += bet * 2;
                    outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                    break;
                }
                if(dealerTotal > playerTotal && playerTotal < 21 && dealerTotal < 21) {
                    cout << "You Lose!" << endl;
                    outcome = 'L';
                    money -= bet;
                    outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                    break;
                }
                if(dealerTotal == 21 && playerTotal == 21) {
                    cout << "Dealer and player both hit 21. Push!" << endl;
                    outcome = 'P';
                    outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                    break;
                }
                if(dealerTotal == 21 && playerTotal != 21) {
                    cout << "Dealer hit 21. You lose." << endl;
                    outcome = 'L';
                    money -= bet;
                    outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                    break;
                }
                if(dealerTotal != 21 && playerTotal == 21) {
                    cout << "You hit 21. You Win!" << endl;
                    outcome = 'W';
                    money += bet * 2;
                    outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                    break;
                }
                if(dealerTotal == 17 && playerTotal == 17) {
                    cout << "Dealer and player both hit 17. Push!" << endl;
                    outcome = 'P';
                    outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                    break;
                }
                if(dealerTotal == playerTotal) {
                    cout << "Dealer and player both hit " << playerTotal << ". Push!" << endl;
                    outcome = 'P';
                    outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                    break;
                }
                if(dealerTotal > 21) {
                    cout << "Dealer busts. You Win!" << endl;
                    outcome = 'W';
                    money += bet * 2;
                    outfile << bet << "     " << dealerTotal << "        " << outcome << "      " << money << endl;
                    break;
                }
                break;

            case '2':
                infile.open("LastNameHistory.txt");
                infile >> cardOne >> cardTwo >> cardThree >> bet >> dealerTotal >> outcome >> money;
                cout << cardOne << cardTwo << cardThree << bet << dealerTotal << outcome << money;
                infile.close();

                break;

            case '3':
                cout << "See ya!\n\n" << endl;
                exit(1);
        }

        cout << "Go again?(Y/N): ";
        cin >> goAgain;
    }
    outfile.close();
    return 0;
}
