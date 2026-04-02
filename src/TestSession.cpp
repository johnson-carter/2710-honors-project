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
            cout << "Your answer: ";
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
            cout << "Your answer [true/false]: ";
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
        // should answers be case sensitive? They are by default
        cout << "Your answer: ";
        getline(cin, result);
        return result;
    }

    // Returns true if guessed correctly.
    bool askQuestion(){
        cout << "\nQuestion " << totalQuestions << ": " << testQuestion->questionContent << "\n";
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
        // This is a linear execution, 1->2->......
        // Project manual shows action menu letting users progress 1->2->n while also jumping to input #
        // Implementation needed here 

        while(testQuestion != nullptr){
            ++totalQuestions;
            totalPoints += testQuestion->pointValue;
            if(askQuestion()){
                cout << "[Your answer is correct!]\n";
                ++correctQuestions;
                earnedPoints += testQuestion->pointValue;
            } else{
                cout << "[Your answer is incorrect!]\n";
            }

            testQuestion = testQuestion->nextQuestion;
        }

        printSessionResults();
    }
};