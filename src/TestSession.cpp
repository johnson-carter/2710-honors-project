#include <iostream>
#include <string>
#include <assert.h>
#include "LinkedQuestion.h"

using namespace std;

struct TestSession{

    // Fields
    LinkedQuestion* head;
    LinkedQuestion* testQuestion;
    double totalPoints;
    double earnedPoints;
    int totalQuestions;
    int correctQuestions;
    bool isFinished;

    TestSession(LinkedQuestion* q){
        head = q;
        testQuestion = q;
        totalPoints = 0;
        earnedPoints = 0;
        totalQuestions = 0;
        correctQuestions = 0;
        isFinished = false;

        // traverse to initialize total stats
        LinkedQuestion* temp = head;
        while (temp != nullptr) {
            totalQuestions++;
            totalPoints += temp->pointValue;
            temp = temp->nextQuestion;
        }
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
        }
    }
    void processCurrentQuestion(int index) {       
        cout << "\n=== Question " << index << ": " << testQuestion->questionContent << " ===\n";
        if (testQuestion->questionType == LinkedQuestion::MCQ) {
            LinkedQuestion::LinkedAnswer* ans = testQuestion->firstAnswer;
            while (ans != nullptr) {
                cout << "  " << ans->letter << ". " << ans->answerContent << endl;
                ans = ans->nextChoice;
            }
        }
        askForNewAnswer();
    }
    void askForNewAnswer() {
        if(testQuestion->questionType == LinkedQuestion::MCQ){
            char answer = getMCQAnswer();
            testQuestion->studentAnswer = string(1, answer);
            testQuestion->isAnswered = true;
        }
        else if(testQuestion->questionType == LinkedQuestion::TFQ){
            bool answer = getTFAnswer();
            testQuestion->studentAnswer = answer ? "true" : "false";
            testQuestion->isAnswered = true;
        } else if(testQuestion->questionType == LinkedQuestion::WRQ){
            string answer = getWRAnswer();
            testQuestion->studentAnswer = answer;
            testQuestion->isAnswered = true;
        }
        cout << "\n================================\n";
    }

    bool checkIfFinished(){
        bool unfinished = false;
        LinkedQuestion* temp = head;
        while (temp != nullptr) {
            if (!temp->isAnswered) unfinished = true;
            temp = temp->nextQuestion;
        }
        return !unfinished;

    }
    void attemptSubmit() {
        // warning logic for Phase 2
        bool unfinished = !checkIfFinished();

        if (unfinished) {
            cout << "\n[WARNING]: You have unanswered questions. Submit anyway? (y/n): ";
            string confirm;
            getline(cin, confirm);
            if (confirm == "y" || confirm == "Y") isFinished = true;
        } else {
            isFinished = true;
        }
    }
    void moveToNextUnanswered() {
        LinkedQuestion* search = testQuestion->nextQuestion;
        // look forward from current spot
        while (search != nullptr) {
            if (!search->isAnswered) {
                testQuestion = search;
                return;
            }
            search = search->nextQuestion;
        }
        // wrap back to the beginning
        search = head;
        while (search != testQuestion && search != nullptr) {
            if (!search->isAnswered) {
                testQuestion = search;
                return;
            }
            search = search->nextQuestion;
        }
        cout << "\n[All questions answered! Select 'Submit' to finish.]\n";
    }
    void handleJumpInput() {
        cout << "Jump to question [1-" << totalQuestions << "]: "; // Dynamically calculate max in production
        string input;
        getline(cin, input);
        try {
            jumpToQuestion(stoi(input));
        } catch (...) {
            cout << "[Invalid input!]\n";
        }
    }
    void printSessionResults() {
        // reset stats here to ensure clean calculation
        earnedPoints = 0;
        correctQuestions = 0;
        
        cout << "\n=== SESSION LOG ===\n";
        LinkedQuestion* curr = head;
        int count = 1;

        while (curr != nullptr) {
            // CHEcK THIS HERE
            bool isCorrect = compareAnswers(curr->studentAnswer, curr->getAnswerString());
            if (isCorrect) {
                earnedPoints += curr->pointValue;
                correctQuestions++;
            }

            cout << "Question " << count << ": " << curr->getAnswerString() << endl;
            cout << "Your answer: " << curr->studentAnswer << endl;
            curr = curr->nextQuestion;
            count++;
        }

        cout << "\nCorrect answers: " << correctQuestions << "/" << totalQuestions << endl;
        cout << "Final score: " << earnedPoints << "/" << totalPoints << endl;
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

    int getCurrentQuestionIndex() {
        LinkedQuestion* temp = head;
        int index = 1;
        while (temp != nullptr) {
            if (temp == testQuestion) {
                return index;
            }
            temp = temp->nextQuestion;
            index++;
        }
        return -1; // Should never happen if testQuestion is always valid
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

            for (int i = 0; i < resultStr.length(); i++) {
                resultStr[i] = tolower(resultStr[i]);
            }
            
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
/*
    // Returns true if guessed correctly.
    bool askQuestion(){
        cout << "\nQuestion " << totalQuestions << ": " << testQuestion->questionContent << "\n";
        bool isCorrect = false;

        switch(testQuestion->questionType){
            //////
            case LinkedQuestion::MCQ:{
                cout << "Answer Choices: \n";
                LinkedQuestion::LinkedAnswer *ansChoices = testQuestion->firstAnswer;
                while(ansChoices != nullptr){
                    cout << "\t" << ansChoices->letter << ". " << ansChoices->answerContent << endl;
                    ansChoices = ansChoices->nextChoice;
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
    }*/

    void startQuiz() {
        if (head == nullptr) {
            cout << "No questions available to take.\n";
            return;
        }

        while (!isFinished) {
            if (testQuestion->isAnswered) {
                cout << "\n=== Question " << getCurrentQuestionIndex() << ": " << testQuestion->questionContent << " ===\n";
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
                    if(!checkIfFinished())processCurrentQuestion(getCurrentQuestionIndex());
                }
                else if (action == "3") {
                    handleJumpInput();
                }
                else if (action == "4") {
                    attemptSubmit();
                }
            } 
            else {
                cout << "\nDo you want to?\n";
                cout << "\t1. Go to next question\n";
                cout << "\t2. Jump to question.\n";
                cout << "\t3. Submit.\n";
                cout << "Select an action: ";
                string action;
                getline(cin, action);

                if (action == "1") {
                    processCurrentQuestion(getCurrentQuestionIndex());
                }
                else if (action == "2") {
                    handleJumpInput();
                }
                else if (action == "3") {
                    attemptSubmit();
                }
            //            int currentIdx = getCurrentQuestionIndex();
            }
        }
        printSessionResults();
    };
};
