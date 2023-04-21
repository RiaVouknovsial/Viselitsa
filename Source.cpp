//Создайте консольную версию игры «Виселица».
//Правила игры можно посмотреть по этой ссылке https://ru.wikipedia.org/wiki/%D0%92%D0%B8%D1%81%D0%B5%D0%BB%D0%B8%D1%86%D0%B0_(%D0%B8%D0%B3%D1%80%D0%B0).
//Важно:
//■■ Слово выбирается компьютером из списка слов.
//■■ Список слов находится в файле в зашифрованном виде.
//■■ По завершении игры на экран выводится статистика игры:
//• количество времени;
//• количество попыток;
//• искомое слово;
//• буквы игрока.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;

class Game
{
public:
    string decryptedWord;
    string encryptedWord;

    vector<string> decryptedWords;
    vector<string> encryptedWords;

    vector<string> words;
    string word;
    int count;
    int shift;
    //функция для зашифровки английских слов методом Цезаря
    string  encryptWord(string input)
    {
        vector<char> word(input.begin(), input.end());
        string alphabet = "abcdefghijklmnopqrstuvwxyz";
        for (int i = 0; i < (int)input.length(); i++)
        {
            for (int j = 0; j < (int)alphabet.length(); j++)
            {
                if (word[i] == alphabet[j])
                {
                    word[i] = alphabet[(j + 7) % 26];
                    break;
                }
            }
        }
        string encryptedWord(word.begin(), word.end());
        return encryptedWord;
    }
    //функция для расшифровки английских слов методом Цезаря
    string  decryptWord(string input)
    {
        vector<char> word(input.begin(), input.end());
        string alphabet = "abcdefghijklmnopqrstuvwxyz";
        for (int i = 0; i < (int)input.length(); i++)
        {
            for (int j = 0; j < (int)alphabet.length(); j++)
            {
                if (word[i] == alphabet[j])
                {
                    word[i] = alphabet[(j + 19) % 26];
                    break;
                }
            }
        }
        string decryptedWord(word.begin(), word.end());
        return decryptedWord;
    }

    void createFile()
    {
        vector<string> words = { "ahisl", "jha","zavul" };
        ofstream outfile("file.txt");
        if (outfile.is_open())
        {
            for (const auto& word : words)
            {
                outfile << word << "\n";
            }
            outfile.close();
        }
        else
        {
            cout << "Failed to open file for writing" << endl;
        }
    }

    // функция для чтения файла со списком слов
    void readWords(string& word)
    {
        ifstream file("file.txt");
        if (file.is_open())
        {
            string encryptedWord;
            while (getline(file, encryptedWord))
            {
                string decryptedWord = decryptWord(encryptedWord);
                words.push_back(decryptedWord);
            }
            file.close();
        }
    }

    //функция для выбора случайного слова из списка
    string chooseWord(vector<string>& words)
    {
        srand(time(NULL));
        int index = rand() % words.size();
        return words[index];

    }

    int countHiddenLetters(const string& word, const vector<char>& guessedLetters)
    {
        int count = 0;
        for (char c : word)
        {
            if (find(guessedLetters.begin(), guessedLetters.end(), c) == guessedLetters.end())
            {
                count++;
            }
        }
        return count;
    }

    // функция для отображения текущего состояния игры на экране
    void displayGameState(string word, vector<char>& guessedLetters)
    {
        int hiddenLetters = countHiddenLetters(word, guessedLetters);
        cout << endl;
        cout << "Word: ";
        for (char c : word)
        {
            if (find(guessedLetters.begin(), guessedLetters.end(), c) != guessedLetters.end())
            {
                cout << c << " ";
            }
            else
            {
                cout << "_ ";
            }
        }
        cout << endl;
        static bool attemptsInfoDisplayed = false;
        if (!attemptsInfoDisplayed)
        {
            cout << "Attempts left: " << max(0, 6 - (int)guessedLetters.size()) << endl;
            attemptsInfoDisplayed = true;
        }
        cout << "Hidden letters: " << hiddenLetters << endl;
    }

    // функция для проверки, есть ли в слове данная буква
    bool checkGuess(string word, char guess, vector<char>& guessedLetters, int& attemptsLeft)
    {
        bool found = false;
        for (char c : word)
        {
            if (c == guess)
            {
                if (find(guessedLetters.begin(), guessedLetters.end(), guess) == guessedLetters.end())
                {
                    attemptsLeft--;
                }
                found = true;
            }
        }
        if (!found)
        {
            if (find(guessedLetters.begin(), guessedLetters.end(), guess) == guessedLetters.end())
            {
                attemptsLeft--;
            }
        }
        guessedLetters.push_back(guess);
        if (attemptsLeft < 0)
        {
            attemptsLeft = 0;
        }
        return found;
    }

    bool isGameOver(const string& word, const vector<char>& guessedLetters, int attemptsLeft)
    {
        int hiddenLetters = countHiddenLetters(word, guessedLetters);
        if (attemptsLeft == 0 || hiddenLetters == 0)
        {
            return true;
        }
        return false;
    }

    void displayGameStats(int attemptsLeft, vector<char>& guessedLetters)
    {
        cout << endl;
        cout << "Attempts left: " << attemptsLeft << endl;
        cout << "Guessed letters: ";
        vector<char> uniqueLetters;
        for (char c : guessedLetters)
        {
            if (find(uniqueLetters.begin(), uniqueLetters.end(), c) == uniqueLetters.end())
            {
                uniqueLetters.push_back(c);
                cout << c << " ";
            }
        }
        cout << endl;
    }

    void Start()
    {
       
        {
            createFile(); // Создаем файл со словами и заполняем его

             // Читаем файл со словами и заносим их в список слов
             readWords(word);

             //  Выбираем случайное слово из списка
             string word =chooseWord(words);

             vector<char> guessedLetters; // Список угаданных букв
             int attemptsLeft = 6; // Количество оставшихся попыток
            cout << "\t\tWelcome to the game of Hangman!" << endl;
            cout << "\tThe principle of the game: guess the intended word.\n";
            cout << "   The word must be a noun\, common noun in the nominative singular\,\n";
            cout << "\tor plural in the absence of a singular form in the word.\n";
            cout << "\t\tYou are given 6 attempts to guess the word."<< endl;
             while (!isGameOver(word, guessedLetters, attemptsLeft))
             {
                displayGameState(word, guessedLetters); // Отображаем текущее состояние игры

                 char guess;
                 cout << "Enter a letter: ";
                 cin >> guess;

                 //   Проверяем, есть ли данная буква в слове
                 if (checkGuess(word, guess, guessedLetters, attemptsLeft))
                 {
                     cout << "Letter " << guess << " found!" << endl;
                 }
                 else
                 {
                     cout << "Letter " << guess << " not found." << endl;
                 }
             }

             //  Выводим результат игры
             displayGameState(word, guessedLetters);
             if (countHiddenLetters(word, guessedLetters) == 0)
             {
                 cout << "Congratulations! You guessed the word!";
             }
             else
             {
                 cout << "You lost! The word was: " << word << endl;
             }
           
             displayGameStats(attemptsLeft, guessedLetters);
        }
    }
};


int main()
{
        bool StartAgain = true;
        while (StartAgain)
        {
         Game game;
      ////encrypt-decrypt 
     /*string text="elephant";
      cout << text << endl;
      string textencrypt = game.encryptWord(text);
      cout << textencrypt<<endl;
     string textdecrypt = game.decryptWord(textencrypt);
     cout << textdecrypt << endl;*/

    srand(time(0));
    
    game.Start();
  
   cout << "Time code working = " << clock() / 1000.0 << " seconds" << endl;
   
   cout << "Do you want to play again? (y/n): ";
   char StartAgainChar;
   cin >> StartAgainChar;
   StartAgain = (tolower(StartAgainChar) == 'y' || tolower(StartAgainChar) == 'Y');
   cout<<endl;
     }
    cout << "Thanks for playing Hangman!" << endl;
    system("pause");
   cout << endl;
    return 0;
}


