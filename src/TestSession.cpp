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
    LinkedQuestion* head;
    LinkedQuestion* testQuestion;
    double totalPoints;
    double earnedPoints;
    int totalQuestions;
    int correctQuestions;

    TestSession(LinkedQuestion* q){
        head = q;
        testQuestion = q;
        totalPoints = 0;
        earnedPoints = 0;
        totalQuestions = 0;
        correctQuestions = 0;
    }

    // logic for Phase 2 "Jump to Question"
    void jumpToQuestion(int targetNum) {
        if (targetNum < 1) {
            cout << "[That question does not exist!]\n";
            return;
        }
        LinkedQuestion* temp = head;
        for (int i = 1; i < targetNum && temp != nullptr; i++) {
            temp = temp->nextQuestion;
        }
        if (temp == nullptr) {
            cout << "[That question does not exist!]\n";
        }
        else {
            testQuestion = temp;
            processCurrentQuestion(targetNum);
        }
    }
    void processCurrentQuestion(int index) {
        if (testQuestion->isAnswered) {
            cout << "Question " << targetNum << ": " << testQuestion->questionContent << endl;
            cout << "You answered: " << testQuestion->studentAnswer << endl;
                
            cout << "\nDo you want to?\n";
            cout << "1. Edit this answer.\n";
            cout << "2. Go to next question\n";
            cout << "3. Jump to question.\n";
            cout << "4. Submit.\n";
            cout << "Select an action: ";
            string action;
            getline(cin, action);

            if (action == "1") {
                askForNewAnswer();
            }
            else if (action == "2") {
                moveToNextUnanswered();
            }
            else if (action == "3") {
                handleJumpInput();
            }
        }
        else {
            cout << "\nQuestion " << index << ": " << testQuestion->questionText << endl;
            askForNewAnswer();
        }
    }
    void askForNewAnswer() {
        cout << "Your answer: ";
        getline(cin, testQuestion->studentAnswer);
        testQuestion->isAnswered = true;
    }
    void moveToNextUnanswered() {
        LinkedQuestion* search = testQuestion->nextQuestion;
        while (search != nullptr) {
            if (!search->isAnswered) {
                testQuestion = search;
                return;
            }
            search = search->nextQuestion;
        }
        search = head;
        while (search != testQuestion && search != nullptr) {
            if (!search->isAnswered) {
                testQuestion = search;
                return;
            }
            search = search->nextQuestion;
        }
        cout << "[All questions answered! Select 'Submit' to finish.]\n";
    }
    void handleJumpInput() {
        cout << "Jump to question [1-4]: "; // Dynamically calculate max in production
        string input;
        getline(cin, input);
        try {
            jumpToQuestion(stoi(input));
        } catch (...) {
            cout << "[Invalid input!]\n";
        }
    }
    void printSessionResults() {
        cout << "\n=== SESSION LOG ===\n";
        LinkedQuestion* curr = head;
        int count = 1;

        while (curr != nullptr) {
            bool isCorrect = compareAnswers(curr->studentAnswer, curr->correctAnswer);
            if (isCorrect) {
                earnedPoints += curr->points;
                correctQuestions++;
            }
            cout << "Question " << count << ": " << curr->correctAnswer << endl;
            cout << "Your answer: " << curr->studentAnswer << endl;
            curr = curr->nextQuestion;
            count++;
        }

        cout << "\nCorrect answers: " << correctQuestions << "/" << totalQuestions << endl;
        cout << "Final score: " << earnedPoints << "/" << totalPoints << endl;
        cout << "*** Thank you for using the testing service. Goodbye! ***\n";
    }
    bool compareAnswers(string s1, string s2) {
        if (s1.length() != s2.length()) {
            return false;
        }
        for (size_t i = 0; i < s1.length(); i++) {
            if (toupper(s1[i]) != toupper(s2[i])) {
                return false;
            }
        }
        return true;
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
