#include <iostream>
#include <string>

struct LinkedQuestion{
    
    struct LinkedAnswer{
        char letter;
        std::string answerContent;
        bool correctAnswer;
        LinkedAnswer *prevChoice;
    };
    bool isTrue;
    std::string targetWord;
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
    LinkedAnswer *lastAnswer;
    double pointValue;

    // Not implemented
    

    std::string getTypeString(){
        switch(questionType){
            case MCQ:
                return "mcq";
                break;
            case TFQ:
                return "t/f";
                break;
            case WRQ:
                return "wr";
                break;
            default: 
                return "null";
                break;
        }
    }
};