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
#include "LinkedQuestion.cpp"

using namespace std;

// Function declarations:
void showGreeting();
void generateQuestion();
void editQuestion();
void deleteQuestion();
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
    getline(cin, qContent);
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


// NOTE - nothing in edit/delete functions is set for handling an empty list of questions !
// Should add this later
void editQuestion(){
    int input;
    cout << "Type a number to edit, or type quit(): ";
    cin >> input;
    while(input < 1 || input > numQuestions){
        invalidInput();
        cin.ignore(1000, '\n');
        invalidInput();
        cout << "Type a number to edit, or type quit(): ";
        cin >> input;
    }
    LinkedQuestion *qTarget = firstQuestion;
    for(int i = 1; i < input; ++i){
        qTarget = qTarget->nextQuestion;
    }

    cout << "=======================\n";
    cout << "=== Q" << input << "Saved Values ===\n";
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
        LinkedQuestion::LinkedAnswer *ansChoices = qTarget->lastAnswer;
        char targetChar;
        while(ansChoices != nullptr){
            cout << ansChoices->letter << ". " << ansChoices->answerContent << endl;
            if(ansChoices->correctAnswer == true)targetChar = ansChoices->letter;
            ansChoices = ansChoices->prevChoice;
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
    cin >> input;
    while(cin.fail() || input < 1 || input > numQuestions){
        cin.clear();
        cin.ignore(1000, '\n');
        invalidInput();
        cout << "Type a number to delete [1-" << numQuestions << "]: ";
        cin >> input;
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
        LinkedQuestion::LinkedAnswer *ans = qRemove->lastAnswer;
        while (ans != nullptr) {
            LinkedQuestion::LinkedAnswer *temp = ans;
            ans = ans->prevChoice;
            delete temp;
        }
    }

    delete qRemove;
    numQuestions--;
    cout << "Question deleted successfully.\n\n";
}

void buildMCQAnswers(LinkedQuestion *q){
    q->lastAnswer = nullptr;
    cout << "[At any time, type 'quit()' to exit]\n\n";
    string userInput;
    char currLetter = 'A';

    cin.ignore(1000, '\n');
    
    while(true){
        cout << "Enter choice " << currLetter << ": ";
        getline(cin, userInput);
        if(userInput == "quit()") break;
        // Add to node
        LinkedQuestion::LinkedAnswer *ansChoice = new LinkedQuestion::LinkedAnswer();
        ansChoice->letter = currLetter;
        ansChoice->answerContent = userInput;
        ansChoice->correctAnswer = false;
        
        ansChoice->prevChoice = q->lastAnswer;
        q->lastAnswer = ansChoice;

        currLetter++;
    }
    cout << "Which letter is the correct answer? ";
    char correct;
    cin >> correct;
    correct = toupper(correct);

    LinkedQuestion::LinkedAnswer *temp = q->lastAnswer;
    while (temp != nullptr) {
        if (temp->letter == correct) {
            temp->correctAnswer = true;
            q->correctLetter = correct;
        }
        temp = temp->prevChoice;
    }
}
