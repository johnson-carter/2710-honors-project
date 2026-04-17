#pragma once
#include <string>

struct LinkedQuestion {

    struct LinkedAnswer {
        char letter;
        std::string answerContent;
        bool correctAnswer;
        LinkedAnswer* nextChoice = nullptr;
    };

    bool isTrue = false;
    std::string targetWord;
    char correctLetter;

    enum QuestionType {
        TFQ,
        MCQ,
        WRQ
    };

    std::string questionContent;
    QuestionType questionType;
    LinkedQuestion* nextQuestion = nullptr;
    LinkedAnswer* firstAnswer = nullptr;
    double pointValue = 0.0;

    bool isAnswered = false;
    std::string studentAnswer;

    std::string getTypeString() const {
        switch(questionType) {
            case MCQ: return "mcq";
            case TFQ: return "t/f";
            case WRQ: return "wr";
            default: return "null";
        }
    }
    std::string getAnswerString() const {
        switch(questionType) {
            case MCQ: return std::string(1, correctLetter);
            case TFQ: return isTrue ? "true" : "false";
            case WRQ: return targetWord;
            default: return "";
        }
    }

    void getQuestionDetails(int qNum) const{
        std::cout << "=================================" << std::endl;
        std::cout << "=== Q" << qNum << " Saved Values ===" << std::endl;
        std::cout << "=================================" << std::endl;
        int label = 0;
        std::cout << "\t" << ++label << " Type: " << getTypeString() << std::endl;
        std::cout << "\t" << ++label << " Question: " << questionContent << std::endl;
        std::cout << "\t" << ++label << " Point Value: " << pointValue << std::endl;
        if(questionType == MCQ){
            std::cout << "\t" << ++label << " Answer Choices: " << std::endl;
            LinkedAnswer* current = firstAnswer;
            while(current != nullptr){
                std::cout << "\t\t" << current->letter << ". " << current->answerContent << std::endl;
                current = current->nextChoice;
            }
            std::cout << "\t" << ++label << " Correct Answer: " << getAnswerString() << std::endl;
        }
        else if(questionType == TFQ){
            std::cout << "\t" << ++label << " Correct Answer: " << getAnswerString() << std::endl;
        }
        else if(questionType == WRQ){
            std::cout << "\t" << ++label << " Correct Answer: " << targetWord << std::endl;
        }    
        std::cout << "=================================" << std::endl;
    }

    int getNumMCQChoices() const {
        if(questionType != MCQ) return 0;
        int count = 0;
        LinkedAnswer* current = firstAnswer;
        while(current != nullptr){
            count++;
            current = current->nextChoice;
        }
        return count;
    }
};
