#include <iostream>
#include <string>

struct LinkedQuestion{
    // Use switch logic to define rules based on question type
    enum QuestionType{
        TFQ,
        MCQ,
        WRQ
    };
    
    // Question parameters
    std::string questionContent;
    QuestionType questionType;
    LinkedQuestion *nextQuestion;
    int pointValue;

    // Not implemented
    struct LinkedAnswer{
        char letter;

    };
};