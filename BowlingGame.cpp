// BowlingGame.cpp
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

// Frame stores the pins for each frame
class Frame {
private:
    int first;
    int second;
    int bonus;
    bool hasBonus;

public:
    Frame() : first(0), second(0), bonus(0), hasBonus(false) {}

    void setThrows(int f, int s) {
        if (f < 0 || f > 10 || s < 0 || s > 10) {
            throw invalid_argument("Pins must be between 0 and 10.");
        }
        if (f != 10 && (f + s > 10)) {
            throw invalid_argument("Total pins can't be more than 10 unless first is strike.");
        }
        first = f;
        second = s;
    }

    void setBonus(int b) {
        if (b < 0 || b > 10) {
            throw invalid_argument("Bonus pins must be between 0 and 10.");
        }
        bonus = b;
        hasBonus = true;
    }

    int getFirst() const { return first; }
    int getSecond() const { return second; }
    int getBonus() const { return bonus; }
    bool bonusAvailable() const { return hasBonus; }

    bool isStrike() const { return first == 10; }
    bool isSpare() const { return (first + second == 10) && first != 10; }
};

// Player keeps all frames and calculates score
class Player {
private:
    vector<Frame> gameFrames;

public:
    void addFrame(const Frame& frame) {
        gameFrames.push_back(frame);
    }

    int totalScore() const {
        int score = 0;
        for (size_t i = 0; i < gameFrames.size(); ++i) {
            score += gameFrames[i].getFirst() + gameFrames[i].getSecond();

            // Add bonuses if needed
            if (gameFrames[i].isStrike() && (i + 1 < gameFrames.size())) {
                score += gameFrames[i + 1].getFirst();
                if (gameFrames[i + 1].isStrike() && (i + 2 < gameFrames.size())) {
                    score += gameFrames[i + 2].getFirst();
                } else {
                    score += gameFrames[i + 1].getSecond();
                }
            } else if (gameFrames[i].isSpare() && (i + 1 < gameFrames.size())) {
                score += gameFrames[i + 1].getFirst();
            }

            // handle last frame bonus
            if (i == 9 && gameFrames[i].bonusAvailable()) {
                score += gameFrames[i].getBonus();
            }
        }
        return score;
    }

    const vector<Frame>& getFrames() const {
        return gameFrames;
    }
};

// BowlingGame drives the flow
class BowlingGame {
private:
    Player player;

public:
    void run() {
        cout << "Welcome to Bowling Score Calculator!\n\n";

        for (int frame = 1; frame <= 10; ++frame) {
            Frame current;
            int first = 0, second = 0, bonus = 0;

            cout << "Frame " << frame << ":\n";
            cout << "  First throw: ";
            cin >> first;

            if (first < 0 || first > 10) {
                throw invalid_argument("Invalid pins for first throw.");
            }

            if (first != 10 || frame == 10) {
                cout << "  Second throw: ";
                cin >> second;

                if (second < 0 || second > 10) {
                    throw invalid_argument("Invalid pins for second throw.");
                }
            }

            current.setThrows(first, second);

            if (frame == 10 && (current.isStrike() || current.isSpare())) {
                cout << "  Bonus throw: ";
                cin >> bonus;
                current.setBonus(bonus);
            }

            player.addFrame(current);
        }
    }

    void showScore() const {
        cout << "\nYour final score is: " << player.totalScore() << endl;
    }
};

int main() {
    try {
        BowlingGame game;
        game.run();
        game.showScore();
    } catch (const exception& ex) {
        cerr << "Oops! " << ex.what() << endl;
    }

    return 0;
}
