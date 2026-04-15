// Main.cpp
// Honors Project - COMP 2710
//
// Carter Johnson, Natalie Nguyen
//  
// To compile (from ~/honors_project/): g++ src/Main.cpp
// To run: ./a.out
//
//

#include <iostream>
#include <string>
#include "LinkedQuestion.h"
#include "TestSession.cpp"

using namespace std;

// Function declarations:
void showGreeting();
void quizSetup();
void generateQuestion();
void editQuestion();
void deleteQuestion();
void buildMCQAnswers(LinkedQuestion *q);
void invalidInput(){cout << "[Command not recognized, please try again!]\n\n";}
// Note there is one other error message used in pdf/outline

// Fields:
LinkedQuestion* firstQuestion = nullptr;
int numQuestions = 0;

void clearMemory(LinkedQuestion* head) {
    LinkedQuestion* currentQ = head;
    while (currentQ != nullptr) {
        // Delete the nested Multiple Choice answers first
        LinkedQuestion::LinkedAnswer* currentA = currentQ->firstAnswer;
        while (currentA != nullptr) {
            LinkedQuestion::LinkedAnswer* nextA = currentA->nextChoice;
            delete currentA;
            currentA = nextA;
        }

        // Save the next question, then delete the current one
        LinkedQuestion* nextQ = currentQ->nextQuestion;
        delete currentQ;
        currentQ = nextQ;
    }
}

int main() {
    showGreeting();
    // Configure quiz questions
    quizSetup();
    
    // Generate test
    TestSession session(firstQuestion);
    session.startQuiz();

    if (firstQuestion != nullptr) {
        clearMemory(firstQuestion);
    }

    cout << "\n*** Thank you for using the testing service. Goodbye! ***\n";

    return 0;
}


void showGreeting(){
    cout << "*** Welcome to the Honors' Testing Service ***\n\n";
}

void quizSetup(){
    while(true){
        cout << "Do you want to?\n\t1. Create new question.\n\t2. Edit question.\n\t3. Delete question.\n\t4. Finish\n";
        cout << "Select an action: ";
        string optionStr;
        int menuOption = -1;

        getline(cin, optionStr);
        try {
            menuOption = stoi(optionStr);
        } catch (...) {
            menuOption = -1;
        }

        switch(menuOption) {
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
        if(menuOption == 4) break;
    }

    while(true){
        cout << "[!] Begin Assessment? [y/n]: ";
        string userInputStr;
        getline(cin, userInputStr);
        if(userInputStr == "y") break;
        else if(userInputStr == "n"){
            cout << "\nAssessment cancelled. Goodbye!\n";
            clearMemory(firstQuestion);
            firstQuestion = nullptr;
            return;
        }
    }
}
void generateQuestion(){
    ++numQuestions;

    LinkedQuestion* q = new LinkedQuestion();
    string qType;
    string qContent;
    
    cout << "\n=== QUESTION " << numQuestions << " ===\n";
    cout << "Type of question [mcq/tf/wr]: ";
    string qTypeStr;
    getline(cin, qTypeStr);
    qType = qTypeStr;

    while (qType != "mcq" && qType != "tf" && qType != "wr") {
        invalidInput();
        cout << "Type of question [mcq/tf/wr]: ";

        getline(cin, qType);
    }
    cout << "\nEnter a question: ";
    getline(cin, qContent);
    q->questionContent = qContent;
    
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
        string tfStr;
        getline(cin, tfStr);
        tOrF = tfStr;

        while (tOrF != "true" && tOrF != "false") {
            invalidInput();
            cout << "Select correct answer: ";

            getline(cin, tOrF);
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
        getline(cin, qAns);
        q->targetWord = qAns;
        // not sure if theres anything that needs to be checked here.
        
        // ASSIGN TO LINKED QUESTION
    }

    string ptsStr;
    double pts = -1;
    while(pts < 0){
        cout << "Enter point value: ";
        getline(cin, ptsStr);
        try
        {
            pts = stod(ptsStr);
        }
        catch(const std::exception& e)
        {
            invalidInput();
            pts = -1;
        }
        if(pts < 0)invalidInput();
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


// NOTE - nothing in edit/delete functions is set for handling an empty list of questions !
// Should add this later
void editQuestion(){
    int input;
    string inputStr;
    cout << "Type a number to edit, or type quit(): ";

    while (input < 1 || input > numQuestions) {
        invalidInput();
        cout << "Type a number to edit, or type quit(): ";

        string inputStr;
        getline(cin, inputStr);

        if (inputStr == "quit()") {
            break;
        }

        try {
            input = stoi(inputStr);
        } catch (...) {
            invalidInput();
        }
    }
    
    LinkedQuestion *qTarget = firstQuestion;
    for(int i = 1; i < input; ++i){
        qTarget = qTarget->nextQuestion;
    }

    cout << "=======================\n";
    cout << "=== Q" << input << " Saved Values ===\n";
    cout << "=======================\n";
    cout << "1. Type: " << qTarget->getTypeString() << endl;
    cout << "2. Question: " << qTarget->questionContent << endl;
    int nextLabel = 3;
    //
    // Alternate idea : add a getCorrectAnswer() function to linkedQuestion to simplify this logic.
    //
    if(qTarget->questionType == LinkedQuestion::MCQ){
        cout << nextLabel << ". Answer Choices: \n";
        ++nextLabel;
        LinkedQuestion::LinkedAnswer *ansChoices = qTarget->firstAnswer;
        char targetChar = ' ';
        while(ansChoices != nullptr){
            cout << "\t" << ansChoices->letter << ". " << ansChoices->answerContent << endl;
            if(ansChoices->correctAnswer == true) targetChar = ansChoices->letter;
            ansChoices = ansChoices->nextChoice;
        }    
        ++nextLabel;
        cout << nextLabel << ". Correct Answer: " << targetChar;
    } else if(qTarget->questionType == LinkedQuestion::TFQ){
        cout << nextLabel << ". Correct Answer: " << qTarget->isTrue;
    } else{
        cout << nextLabel << ". Correct Answer: " <<  qTarget->targetWord;
    }
    ++nextLabel;
    cout << endl << nextLabel << ". Point Value: " << qTarget->pointValue;
    cout << endl << endl;
    // ADD LOGIC TO EDIT QUESTION PROPERTIES HERE
    
}
void deleteQuestion(){
    if (numQuestions == 0) {
        cout << "No questions to delete.\n";
        return;
    }
    
    int input;
    cout << "Type a number to delete [1-" << numQuestions << "]: ";
    string inputNumStr;
    getline(cin, inputNumStr);

    try {
        input = stoi(inputNumStr);
    } catch (...) {
        input = -1; // force invalid to enter loop
    }

    while (input < 1 || input > numQuestions) {
        invalidInput();
        cout << "Type a number to delete [1-" << numQuestions << "]: ";

        getline(cin, inputNumStr);

        try {
            input = stoi(inputNumStr);
        } catch (...) {
            input = -1;
        }
    }
    // input is good:
    LinkedQuestion *qRemove = nullptr;
    
    // Case 1: Deleting the very first question
    if (input == 1) {
        qRemove = firstQuestion;
        firstQuestion = firstQuestion->nextQuestion;
    }
    // Case 2: Deleting any other question
    else {
        LinkedQuestion *prev = firstQuestion;
        for (int i = 1; i < input - 1; i++) {
            prev = prev->nextQuestion;
        }
        qRemove = prev->nextQuestion;
        prev->nextQuestion = qRemove->nextQuestion;
    }

    if (qRemove->questionType == LinkedQuestion::MCQ) {
        LinkedQuestion::LinkedAnswer *ans = qRemove->firstAnswer;
        while (ans != nullptr) {
            LinkedQuestion::LinkedAnswer *temp = ans;
            ans = ans->nextChoice;
            delete temp;
        }
    }

    delete qRemove;
    numQuestions--;
    cout << "Question deleted successfully.\n\n";
}

void buildMCQAnswers(LinkedQuestion *q) {
    q->firstAnswer = nullptr; // Initialize the head of the answer list
    cout << "[At any time, type 'quit()' to exit]\n\n";
    string userInput;
    char currLetter = 'A';
    
    while(true) {
        cout << "Enter choice " << currLetter << ": ";
        getline(cin, userInput);
        
        if(userInput == "quit()") {
            if(currLetter == 'A') {
                cout << "You must enter at least one answer choice.\n";
                continue;
            } else break;
        }

        // Create new node
        LinkedQuestion::LinkedAnswer *ansChoice = new LinkedQuestion::LinkedAnswer();
        ansChoice->letter = currLetter;
        ansChoice->answerContent = userInput;
        ansChoice->correctAnswer = false;
        ansChoice->nextChoice = nullptr;

        // Link node to the END of the list (Maintains A-B-C order)
        if(q->firstAnswer == nullptr) {
            q->firstAnswer = ansChoice;
        } else {
            LinkedQuestion::LinkedAnswer *temp = q->firstAnswer;
            while(temp->nextChoice != nullptr) {
                temp = temp->nextChoice;
            }
            temp->nextChoice = ansChoice;
        }
        currLetter++;
    }

    // Select the correct answer
    cout << "Which letter is the correct answer: ";
    string correctStr;
    char correct;
    while(true) {
        getline(cin, correctStr);
        if(!correctStr.empty()) {
            correct = toupper(correctStr[0]);
            if(correct >= 'A' && correct < currLetter) break;
        }
        invalidInput();
        cout << "Which letter is the correct answer: ";    
    }

    // Mark the correct answer in the list
    LinkedQuestion::LinkedAnswer *temp = q->firstAnswer;
    while (temp != nullptr) {
        if (temp->letter == correct) {
            temp->correctAnswer = true;
            q->correctLetter = correct;
        }
        temp = temp->nextChoice;
    }
}
