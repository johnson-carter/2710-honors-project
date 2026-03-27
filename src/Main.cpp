// Main.cpp
// Honors Project - COMP 2710
//
// Carter Johnson
//  
// To compile (from ~/honors_project/): g++ src/Main.cpp
// To run: ./a.out
//
//

#include <iostream>
#include <string>
#include "LinkedQuestion.cpp"
using namespace std;

// Function declarations:
void showGreeting();
void generateQuestion();
void editQuestion(){}
void deleteQuestion(){}
void buildMCQAnswers(LinkedQuestion *q);
void invalidInput(){cout << "[Command not recognized, please try again!]\n\n";}
// Note there is one other error message used in pdf/outline

// Fields:
LinkedQuestion* firstQuestion = nullptr;
int numQuestions = 0;

int main() {
    showGreeting();

    while(true){
        int menuOption;
        cout << "Do you want to?\n\t1. Create new question.\n\t2. Edit question.\n\t3. Delete question.\n\t4. Finish\n";
        cout << "Select an action: ";
        cin >> menuOption;
        switch(menuOption){
            case 1:
                generateQuestion();
                break;
            case 2:
                editQuestion();
                break;
            case 3:
                deleteQuestion();
                break;
            case 4:
                break;
            default:
                invalidInput();
                break;
        }
        cin.ignore(1000, '\n');
    }
    // Phase 2 in pdf shows a "what do u want to do here" menu


    return 0;
}


void showGreeting(){
    cout << "*** Welcome to the Honors' Testing Service ***\n\n";
}

void generateQuestion(){
    ++numQuestions;

    LinkedQuestion* q = new LinkedQuestion();
    string qType;
    string qContent;
    
    cout << "\n=== QUESTION " << numQuestions << " ===\n";
    cout << "Type of question [mcq/tf/wr]: ";
    cin >> qType;
    // String arent allowed in switch statments.
    while(qType != "mcq" && qType != "tf" && qType != "wr"){
        invalidInput();

        cout << "Type of question [mcq/tf/wr]: ";
        cin.ignore(1000, '\n');
        cin >> qType;
    }
    cout << "\nEnter a question: ";
    cin >> qContent;
    cin.ignore(1000, '\n');
    cout << endl; 
    // Is there anything that makes a question invalid?
    // I don't really know of any case where we cant accept it
    if(qType == "mcq"){
        q->questionType = LinkedQuestion::MCQ;
        
        buildMCQAnswers(q);
        // Need loop to select a correct answer.
    }
    else if(qType == "tf"){
        q->questionType = LinkedQuestion::TFQ;
        
        cout << "Select correct answer: ";
        string tOrF;

        cin >> tOrF;
        while(tOrF != "true" && tOrF != "false"){
            invalidInput();
            cout << "Select correct answer: ";
            cin >> tOrF;
            cin.ignore(1000, '\n');
        }

        if(tOrF == "true"){
            q->isTrue = true;
        } else {
            q->isTrue = false;
        }
        // ASSIGN TO LINKED QUESTION
    }
    else if(qType == "wr"){
        string qAns;
        q->questionType = LinkedQuestion::WRQ;
        cout << "Select correct answer: ";
        cin >> qAns;
        cin.ignore(1000, '\n');
        q->targetWord = qAns;
        // not sure if theres anything that needs to be checked here.
        
        // ASSIGN TO LINKED QUESTION
    }

    double pts;
    cout << "Enter point value: ";
    cin >> pts;
    while(pts < 0){
        invalidInput();
        cin.ignore(1000, '\n');
        cout << "Enter point value: ";
        cin >> pts;
    }
    q->pointValue = pts;

    cout << "Question saved.\n\n";
    // NEED TO IMPLEMENT QUESTION ANSWER SYSTEM
    // Have not done since each type has different asnwer format


    // Link new question to our current one
    if(firstQuestion == nullptr){
        firstQuestion = q;
        return;
    }
    
    LinkedQuestion* node = firstQuestion;
    while(node->nextQuestion != nullptr){
        node = node->nextQuestion;
    }

    node->nextQuestion = q;
}

void buildMCQAnswers(LinkedQuestion *q){
    q->lastAnswer = nullptr;
    cout << "[At any time, type 'quit()' to exit]\n\n";
    string userInput;
    char currLetter = 'A';
    cout << "Enter choice " << currLetter << ": ";
    cin >> userInput;
    while(userInput != "quit()"){
        // Add to node
        LinkedQuestion::LinkedAnswer *ansChoice = new LinkedQuestion::LinkedAnswer();
        ansChoice->letter = currLetter;
        ansChoice->answerContent = userInput;
        if(q->lastAnswer == nullptr){
            ansChoice->prevChoice = nullptr;
            q->lastAnswer = ansChoice;
        } else {
            ansChoice->prevChoice = q->lastAnswer;
            q->lastAnswer = ansChoice;
        }

        cin.ignore(1000, '\n');
        ++currLetter;
        cout << "Enter choice " << currLetter << ": ";
        cin >> userInput;
    }

    // NEED LOOP TO FIND CHAR OF CORRECT ANSWER,
    // CURRENTLY JUST BUILDS LINKED LIST - NO CORRECT

    // get target character
    // iterate up list until found
    
    // we might want to limit the questions to 26 so we don't go higher than z
    cout << endl;
}