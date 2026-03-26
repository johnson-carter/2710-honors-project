#include <iostream>
#include <string>

struct LinkedQuestion{
    /*
        BIG QUESTION:
        Should every question have every answer type?
            meaning -
            A T/F question has a linkedanswer list, but never calls itf
            A MCQ has a "true/false" answer, but the program looks for linkedanswer based on qType
        
    */
    enum QuestionType{
        TFQ,
        MCQ,
        WRQ
    };
    
    // Question parameters
    std::string questionContent;
    QuestionType questionType;
    LinkedQuestion *nextQuestion;
    double pointValue;

    // Not implemented
    struct LinkedAnswer{
        char letter;
        std::string answerContent;
        bool correctAnswer;
        LinkedAnswer *nextChoice;
    };
};