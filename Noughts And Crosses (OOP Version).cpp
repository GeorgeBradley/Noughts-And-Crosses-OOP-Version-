#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include <random>
#include <ctime>
#include <cstdlib>

class Player
{
private:
    std::string m_type;
    unsigned char m_name;
    int m_winTally;
public:
    std::string GetType()const { return m_type; };
    Player(unsigned char name, std::string&& type = "Player", int wins = 0) :m_name(name), m_type(type), m_winTally(0) {}
    virtual unsigned char GetName()const { return m_name; }
    virtual bool ClaimSquare(std::map<int, unsigned char>& board, int move) = 0;
    virtual int NextMove(std::map<int, unsigned char>& board) = 0;
    virtual ~Player() = default;
    void AddWinToTally() { m_winTally++; }
    int GetWins() const { return m_winTally; }
};

class Human : public Player
{

public:
    Human(unsigned char name) :Player(name, "Human",0) {}
    virtual int NextMove(std::map<int, unsigned char>& board) override {
        int move;
        std::cout << "Enter a number on the board (e.g. 1): ";
        std::cin >> move;
        return move;
    }
    virtual bool ClaimSquare(std::map<int, unsigned char>& board, int move)
    {
        auto validSquare = std::find_if(board.begin(), board.end(), [&](auto pair) {
            return pair.first == move;
            });
        if (validSquare != board.end())
        {
            if (validSquare->second == '-')
            {
                validSquare->second = Player::GetName();
                return true;
            }
            else
            {
                std::cout << "This square has already been claimed. Choose a different square!" << std::endl;
                return false;
            }
        }
        return false;
    }
    virtual ~Human() = default;
};

class Robot : public Player
{

public:
    Robot(unsigned char name) :Player(name, "Robot", 0) {}

    bool CheckAvailability(std::map<int, unsigned char>& board, int number, std::vector<int>& keys) {
        for (auto& cell : board) {
            if (cell.first == number) {
                if (cell.second == '-') {
                    return true;
                }
            }
        }
        std::remove_if(keys.begin(), keys.end(), [&](auto& key) {
            return key == number;
            });
        return false;
    }
    virtual int NextMove(std::map<int, unsigned char>& board) override
    {
        std::vector<int>number = { 1,2,3,4,5,6,7,8,9 };
        int randNum = 0;
        std::srand(std::time(0));
        do
        {
            randNum = rand() % 9 + 1;
        } while (CheckAvailability(board, randNum, number) == false);

        std::cout << "Calcuating move. . .";
        return randNum;
    }
    virtual bool ClaimSquare(std::map<int, unsigned char>& board, int move)
    {
        auto validSquare = std::find_if(board.begin(), board.end(), [&](auto pair) {
            return pair.first == move;
            });
        if (validSquare != board.end())
        {
            if (validSquare->second == '-')
            {
                validSquare->second = Player::GetName();
                return true;
            }
            else
            {
                std::cout << "This square has already been claimed. Choose a different square!" << std::endl;
                return false;
            }
        }
        return false;
    }
    virtual ~Robot() = default;
};

class NoughtsAndCrosses
{
private:

    //std::vector<Player*> m_p;
    std::map<int, unsigned char>board;
    void DisplayBoard()
    {

        for (auto const& cell : board)
        {
            if (cell.first % 3 == 1) {
                std::cout << "\n\n";
            }
            if (cell.second != '-') {
                std::cout << cell.second << "        ";
            }
            else
            {
                std::cout << cell.first << "        ";
            }
        }
        std::cout << "\n\n";
    }
    auto CheckForAWinner(std::map<int, unsigned char>& board, const std::shared_ptr<Player>& player)
    {
        if (board.at(1) == player->GetName() && board.at(2) == player->GetName() && board.at(3) == player->GetName()) {
            std::cout << player->GetType() << ":" << player->GetName() << " is the winner" << std::endl;
            player->AddWinToTally();

            return true;
        }
        else if (board.at(4) == player->GetName() && board.at(5) == player->GetName() && board.at(6) == player->GetName()) {
            std::cout << player->GetType() << ":" << player->GetName() << " is the winner" << std::endl;
            player->AddWinToTally();
            return true;
        }
        else if (board.at(7) == player->GetName() && board.at(8) == player->GetName() && board.at(9) == player->GetName()) {
            std::cout << player->GetType() << ":" << player->GetName() << " is the winner" << std::endl;
            player->AddWinToTally();
            return true;
        }

        else if (board.at(1) == player->GetName() && board.at(4) == player->GetName() && board.at(7) == player->GetName()) {
            std::cout << player->GetType() << ":" << player->GetName() << " is the winner" << std::endl;
            player->AddWinToTally();
            return true;
        }
        else if (board.at(2) == player->GetName() && board.at(5) == player->GetName() && board.at(8) == player->GetName()) {
            std::cout << player->GetType() << ":" << player->GetName() << " is the winner" << std::endl;
            player->AddWinToTally();
            return true;
        }
        else if (board.at(3) == player->GetName() && board.at(6) == player->GetName() && board.at(9) == player->GetName()) {
            std::cout << player->GetType() << ":" << player->GetName() << " is the winner" << std::endl;
            player->AddWinToTally();
            return true;
        }

        else if (board.at(1) == player->GetName() && board.at(5) == player->GetName() && board.at(9) == player->GetName()) {
            std::cout << player->GetType() << ":" << player->GetName() << " is the winner" << std::endl;
            player->AddWinToTally();
            return true;
        }
        else if (board.at(7) == player->GetName() && board.at(5) == player->GetName() && board.at(3) == player->GetName()) {
            std::cout << player->GetType() << ":" << player->GetName() << " is the winner" << std::endl;
            player->AddWinToTally();
            return true;
        }
        else
        {
            return false;
        }
    }
    bool CheckForDraw(std::map<int, unsigned char>& board) {

        if (std::count_if(board.begin(), board.end(), [](auto& cell) {
            return cell.second == '-';
            }) == 0)
        {
            std::cout << "It's a draw!\n";
            return true;
        }
            return false;
    }
public:
    NoughtsAndCrosses()
    {
        board = { std::make_pair(1,'-'),std::make_pair(2,'-'),std::make_pair(3,'-'),
          std::make_pair(4,'-'),std::make_pair(5,'-'),std::make_pair(6,'-'),
          std::make_pair(7,'-'),std::make_pair(8,'-'),std::make_pair(9,'-') };

    }
  
    void ResetBoard() {
        for (auto& cell : board) {
            cell.second = '-';
        }
    }
    void play(std::shared_ptr<Player>& p1, std::shared_ptr<Player>& p2)
    {
        int currentPlayer = 1;
        std::vector<std::shared_ptr<Player>>m_player = { p1, p2 };
        do
        {
            currentPlayer = (currentPlayer + 1) % 2;
            do
            {
                DisplayBoard();
                std::cout << m_player.at(currentPlayer)->GetType() << ": " << m_player.at(currentPlayer)->GetName() << " turn: " << std::endl;
            } while (m_player.at(currentPlayer)->ClaimSquare(board, m_player.at(currentPlayer)->NextMove(board)) == false);
            system("pause");
            //std::cout << "\nPress enter to make the robot move. . .";
            //std::cin.get();
            //system("cls");

        } while (CheckForDraw(board) == false && CheckForAWinner(board, m_player.at(currentPlayer)) == false);
        DisplayBoard();
        ResetBoard();
    }
};

int main() {

    std::shared_ptr<Player> human1 = std::make_shared<Human>('O');
    std::shared_ptr<Player> human2 = std::make_shared<Human>('X');

    std::shared_ptr<Player> robot1 = std::make_shared<Robot>('O');
    std::shared_ptr<Player> robot2 = std::make_shared<Robot>('X');

    NoughtsAndCrosses game;

    int round = 1;
    do {
        std::cout << "Game " << round++ << ": Robot vs Robot\n";
        game.play(robot1, robot2);
    } 
    while (true);
  
 
    
    
    
    std::cout << "Game 2: Human vs Robot\n";
    game.play(human1, robot1);

    std::cout << "Game 3: Human vs Human\n";
    game.play(human1, human2);

    

}