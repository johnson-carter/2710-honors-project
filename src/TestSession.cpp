#include "LinkedQuestion.h"
using namespace std;

struct TestSession{
    // Methods
    /*bool askQuestion();
    char getMCQAnswer();
    bool getTFAnswer();
    string getWRAnswer();
    void printSessionResults();*/

    // Fields
    LinkedQuestion* testQuestion;
    double totalPoints;
    double earnedPoints;
    int totalQuestions;
    int correctQuestions;

    TestSession(LinkedQuestion* q){
        testQuestion = q;
        totalPoints = 0;
        earnedPoints = 0;
        totalQuestions = 0;
        correctQuestions = 0;
    }

    char getMCQAnswer(){
        char result;
        while(true){
            string resultStr;
            getline(cin, resultStr);
            if(resultStr.length() == 1 && isalpha(resultStr[0])){
                result = toupper(resultStr[0]);
                break;
            } else{
                cout << "[Answer not recognized, please try again!]\n\n";
            }
        }
        return result;
    }

    bool getTFAnswer(){
        bool result;
        while(true){
            string resultStr;
            getline(cin, resultStr);
            if(resultStr == "true"){
                result = true;
                break;
            } else if(resultStr == "false"){
                result = false;
                break;
            } else{
                cout << "[Answer not recognized, please try again!]\n\n";
            }
        }
        return result;
    }

    string getWRAnswer(){
        string result;
        getline(cin, result);
        return result;
    }

    // Returns true if guessed correctly.
    bool askQuestion(){
        cout << "Question " << totalQuestions << ": " << testQuestion->questionContent << "\n\n";
        bool isCorrect = false;

        switch(testQuestion->questionType){
            //////
            case LinkedQuestion::MCQ:{
                cout << "Answer Choices: \n";
                LinkedQuestion::LinkedAnswer *ansChoices = testQuestion->lastAnswer;
                while(ansChoices != nullptr){
                    cout << "\t" << ansChoices->letter << ". " << ansChoices->answerContent << endl;
                    ansChoices = ansChoices->prevChoice;
                }
                char userAnswer = getMCQAnswer();
                if(userAnswer == testQuestion->correctLetter){
                    isCorrect = true;
                }
            }
                break;
            //////
            case LinkedQuestion::TFQ:{
                cout << "True or False? \n";
                bool tfAnswer = getTFAnswer();
                
                if(tfAnswer == testQuestion->isTrue){
                    isCorrect = true;
                }

                break;
            }
            //////
            case LinkedQuestion::WRQ:{
                cout << "Type your answer: \n";
                string wrAnswer = getWRAnswer();
                
                if(wrAnswer == testQuestion->targetWord){
                    isCorrect = true;
                }
                break;
            }
        }
        return isCorrect;
    }

    

    void printSessionResults(){
        cout << "\n=== SESSION LOG ===\n";
        cout << "Correct answers: " << correctQuestions << "/" << totalQuestions << endl;
        cout << "Final score: " << earnedPoints << "/" << totalPoints << endl;
    }

    void startQuiz(){
        while(testQuestion != nullptr){
            ++totalQuestions;
            totalPoints += testQuestion->pointValue;
            if(askQuestion()){
                ++correctQuestions;
                earnedPoints += testQuestion->pointValue;
            }

            testQuestion = testQuestion->nextQuestion;
        }
        printSessionResults();
    }
};