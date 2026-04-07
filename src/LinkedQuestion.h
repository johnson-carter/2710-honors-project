#pragma once
#include <string>

struct LinkedQuestion {

    struct LinkedAnswer {
        char letter;
        std::string answerContent;
        bool correctAnswer;
        LinkedAnswer* prevChoice = nullptr;
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
    LinkedAnswer* lastAnswer = nullptr;
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
};