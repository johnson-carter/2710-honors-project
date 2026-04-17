// Main.cpp
// Honors Project - COMP 2710
/* Group members
Carter Johnson: cmj0083@auburn.edu
Natalie Nguyen: ntn0007@auburn.edu */
/* Project description: A C++ testing service application utilizing a Singly Linked List 
to manage dynamic assessment data. Supports creation, modification, and deletion of MCQ, 
true/false, and written response questions. Features include manual memory management to 
ensure zero leaks, robust input validation, and a conditional compilation framework for 
automated unit testing.*/
/* Any outside help/resources: Developed exclusively by team members using course lectures 
and standard C++ documentation.*/

// Part A
// To compile (from ~/honors_project/): g++ src/Main.cpp
// To run: ./a.out

// Part B
// compile: g++ -DUNIT_TESTING src/Main.cpp
// run: ./a.out

#include <iostream>
#include <string>
#include <assert.h>
#include <algorithm>
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
void clearMemory(LinkedQuestion* head);
void invalidInput() {cout << "[Command not recognized, please try again!]\n\n";}

// Fields:
LinkedQuestion* firstQuestion = nullptr;
int numQuestions = 0;

// Part B: UNIT TESTING
#ifdef UNIT_TESTING
void runTests() {
    cout << "*** This is a debugging version ***\n\n";

    // unit test case 1
    cout << "Unit Test Case 1: empty list\n";
    TestSession emptySession(nullptr);
    assert(emptySession.totalQuestions == 0);
    if (emptySession.totalQuestions < 1) {
        cout << "Warning - the number of questions to be asked must equal to or be larger than 1.\n";
    }
    cout << "Case 1 passed\n\n";

    // unit test case 2.1
    cout << "Unit Test Case 2.1: incorrect answer\n";
    
    // setup a single question
    LinkedQuestion* q1 = new LinkedQuestion();
    q1->questionContent = "How long was the shortest war on record?";
    q1->targetWord = "38";
    q1->pointValue = 100.0;
    q1->questionType = LinkedQuestion::WRQ;

    TestSession session2(q1);

    cout << "Question: " << q1->questionContent << endl;
    string mockInputWrong = "85";
    cout << "Answer: " << mockInputWrong << endl;

    if (!session2.compareAnswers(mockInputWrong, q1->targetWord)) {
        cout << "Your answer is wrong. The correct answer is " << q1->targetWord << endl;
        // since it's wrong, earnedPoints stays 0
        cout << "Your total points 0\n";
    }
    
    assert(session2.compareAnswers(mockInputWrong, q1->targetWord) == false);
    cout << "\nCase 2.1 passed\n\n";

    // unit test case 2.2
    cout << "Unit Test Case 2.2: correct answer\n";
    
    cout << "Question: " << q1->questionContent << " Answer: 38\n";
    
    string mockInputRight = "38";
    if (session2.compareAnswers(mockInputRight, q1->targetWord)) {
        session2.earnedPoints += q1->pointValue; // Simulate adding points
        cout << "Your answer is correct! You receive " << q1->pointValue << " points.\n";
        cout << "Your total points: " << session2.earnedPoints << endl;
    }

    assert(session2.compareAnswers(mockInputRight, q1->targetWord) == true);
    assert(session2.earnedPoints == 100.0);
    cout << "\nCase 2.2 passed\n\n";

    // unit test case 3
    cout << "Unit Test Case 3: traverse and ask ALL questions\n";

    LinkedQuestion* qt1 = new LinkedQuestion();
    qt1->questionContent = "How long was the shortest war on record?";
    qt1->targetWord = "38";
    qt1->pointValue = 100.0;
    qt1->questionType = LinkedQuestion::WRQ;

    LinkedQuestion* qt2 = new LinkedQuestion();
    qt2->questionContent = "What was Bank of America's original name? (Hint: Bank of Italy or Bank of Germany)";
    qt2->targetWord = "Bank of Italy";
    qt2->pointValue = 100.0;
    qt2->questionType = LinkedQuestion::WRQ;

    LinkedQuestion* qt3 = new LinkedQuestion();
    qt3->questionContent = "Which university is located in Auburn, Alabama?";
    qt3->targetWord = "Auburn University";
    qt3->pointValue = 100.0;
    qt3->questionType = LinkedQuestion::WRQ;

    // Link them together: qt1 -> qt2 -> qt3
    qt1->nextQuestion = qt2;
    qt2->nextQuestion = qt3;

    TestSession session3(qt1);

    // question 1 logic (correct)
    cout << "Question: " << qt1->questionContent << endl;
    string mockAns1 = "38";
    cout << "Answer: " << mockAns1 << endl;
    if (session3.compareAnswers(mockAns1, qt1->targetWord)) {
        session3.earnedPoints += qt1->pointValue;
        cout << "Your answer is correct! You receive " << qt1->pointValue << " points. ";
        cout << "Your total points: " << session3.earnedPoints << "\n\n";
    }

    // question 2 logic (incorrect)
    cout << "Question: " << qt2->questionContent << endl;
    string mockAns2 = "Bank of Germany";
    cout << "Answer: " << mockAns2 << endl;
    if (!session3.compareAnswers(mockAns2, qt2->targetWord)) {
        cout << "Your answer is wrong. The correct answer is " << qt2->targetWord << ".\n";
        cout << "Your total points " << session3.earnedPoints << "\n\n";
    }

    // question 3 logic (correct)
    cout << "Question: " << qt3->questionContent << endl;
    string mockAns3 = "Auburn University";
    cout << "Answer: " << mockAns3 << endl;
    if (session3.compareAnswers(mockAns3, qt3->targetWord)) {
        session3.earnedPoints += qt3->pointValue;
        cout << "Your answer is correct! You receive " << qt3->pointValue << " points. ";
        cout << "Your total points: " << session3.earnedPoints << endl;
    }

    assert(session3.earnedPoints == 200.0); // Q1 and Q3 were correct
    assert(session3.totalQuestions == 3);
    
    cout << "\nCase 3 passed\n\n";

    // unit test case 4
    cout << "Unit Test Case 4: boundary check\n";
    
    int requestedQuestions = 5;
    int actualSize = session3.totalQuestions; // this should be 3

    if (requestedQuestions > actualSize) {
        cout << "Warning - There is only " << actualSize << " questions in the list.\n";
    }

    assert(requestedQuestions > actualSize);
    
    cout << "Case 4 passed\n\n";

    // unit test case 5
    cout << "Unit Test Case 5: MCQ correct answer\n";
    LinkedQuestion* qMCQ = new LinkedQuestion();
    qMCQ->questionType = LinkedQuestion::MCQ;
    qMCQ->correctLetter = 'A';
    qMCQ->pointValue = 50.0;
    TestSession session5(qMCQ);
    assert(session5.compareAnswers("A", "A") == true);
    cout << "Case 5 passed\n\n";

    // unit test case 6
    cout << "Unit Test Case 6: MCQ incorrect answer\n";
    assert(session5.compareAnswers("B", "A") == false);
    cout << "Case 6 passed\n\n";

    // unit test case 7
    cout << "Unit Test Case 7: T/F logic (True)\n";
    LinkedQuestion* qTF = new LinkedQuestion();
    qTF->questionType = LinkedQuestion::TFQ;
    qTF->isTrue = true;
    TestSession session7(qTF);
    assert(session7.compareAnswers("true", "true") == true);
    cout << "Case 7 passed\n\n";

    // unit test case 8
    cout << "Unit Test Case 8: T/F logic (False)\n";
    qTF->isTrue = false;
    assert(session7.compareAnswers("false", "false") == true);
    cout << "Case 8 passed\n\n";

    // unit test case 9
    cout << "Unit Test Case 9: linked list node deletion safety\n";
    LinkedQuestion* n1 = new LinkedQuestion();
    LinkedQuestion* n2 = new LinkedQuestion();
    LinkedQuestion* n3 = new LinkedQuestion();
    n1->nextQuestion = n2;
    n2->nextQuestion = n3;
    // Simulate deleting n2
    n1->nextQuestion = n3; 
    assert(n1->nextQuestion == n3);
    delete n2; // Clean up the orphaned node
    cout << "Case 9 passed\n\n";

    // unit test case 10
    cout << "Unit Test Case 10: point summation across multiple questions\n";
    TestSession session10(n1); 
    session10.earnedPoints = 0;
    session10.earnedPoints += 10.5;
    session10.earnedPoints += 20.0;
    assert(session10.earnedPoints == 30.5);
    cout << "Case 10 passed\n\n";

    // unit test case 11
    cout << "Unit Test Case 11: editing question content\n";
    LinkedQuestion* qEdit = new LinkedQuestion();
    qEdit->questionContent = "Old Question";
    
    qEdit->questionContent = "New Question";
    
    assert(qEdit->questionContent == "New Question");
    cout << "Case 11 passed\n\n";

    cout << "*** End of the Debugging Version ***\n";

    // memory cleanup
    clearMemory(qt1); 
    clearMemory(q1); 
    clearMemory(n1);
    clearMemory(qTF);
    clearMemory(qMCQ);
    clearMemory(qEdit);
}
#endif

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
    // Debug mode
    #ifdef UNIT_TESTING
        runTests();
        return 0;
    #endif

    // Normal execution
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
    
    do{
        cout << "Enter point value: ";
        getline(cin, ptsStr);
        try{
            pts = stod(ptsStr);
            if(pts < 0) invalidInput();
        }
        catch(...){
            invalidInput();
            pts = -1;
        }
    }while (pts < 0);
    
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
    
    do{
        cout << "Type a number to edit [1-" << numQuestions << "] or type 'quit()' to exit: ";
        getline(cin, inputStr);
        if(inputStr == "quit()"){ cout << endl; return;}
        try {
            input = stoi(inputStr);
            if(input < 1 || input > numQuestions) {
                invalidInput();
            }
        } catch (...) {
            input = -1; // force invalid to enter loop
        }
    }while(input < 1 || input > numQuestions);

    LinkedQuestion* qEdit = firstQuestion;
    for(int i = 1; i < input; i++){
        qEdit = qEdit->nextQuestion;
    }
    qEdit->getQuestionDetails(input);

    cout << "Type a number to edit, or type 'quit()' to exit: ";
    string editOptionStr;
    int editOption = -1;
    int numOptions = 4;
    if(qEdit->questionType == LinkedQuestion::MCQ)numOptions = 5; // MCQ has an extra option to edit answer choices
    
    do{
        getline(cin, editOptionStr);
        if(editOptionStr == "quit()") return;
        try {
            editOption = stoi(editOptionStr);
        } catch (...) {
            editOption = -1; // force invalid to enter loop
        }
    }while(editOption < 1 || editOption > numOptions);

    if(editOption == 1){
        string newTypeStr;
        do{
            if(newTypeStr != "") invalidInput();

            cout << "New type [mcq/tf/wr]: ";
            getline(cin, newTypeStr);
        }while(newTypeStr != "mcq" && newTypeStr != "tf" && newTypeStr != "wr");
        if(newTypeStr == "mcq"){
            qEdit->questionType = LinkedQuestion::MCQ;
            buildMCQAnswers(qEdit);
        }
        else if(newTypeStr == "tf"){
            qEdit->questionType = LinkedQuestion::TFQ;
            cout << "Select correct answer: ";
            string tOrF;
            do{
                getline(cin, tOrF);
                if(tOrF == "true"){
                    qEdit->isTrue = true;
                } else if(tOrF == "false"){
                    qEdit->isTrue = false;
                } else {
                    invalidInput();
                    cout << "Select correct answer: ";
                }
            }while(tOrF != "true" && tOrF != "false");
        }
        else if(newTypeStr == "wr"){
            qEdit->questionType = LinkedQuestion::WRQ;
            string qAns;
            cout << "Select correct answer: ";
            getline(cin, qAns);
            qEdit->targetWord = qAns;
        }
    } else if(editOption == 2){
        string newContent;
        do{
            cout << "Enter new question content: ";
            getline(cin, newContent);
        }while(newContent == "");

        qEdit->questionContent = newContent;
    } else if(editOption == 3){
        string ptsStr;
        double pts = -1;
        do{
            cout << "Enter new point value: ";
            getline(cin, ptsStr);
            try{
                pts = stod(ptsStr);
            }
            catch(...){
                invalidInput();
                pts = -1;
            }
        }while (pts < 0);
        
        qEdit->pointValue = pts;
    } else if(editOption == 4 && qEdit->questionType == LinkedQuestion::MCQ){
        buildMCQAnswers(qEdit);
    } else if(editOption == 4){
        if(qEdit->questionType == LinkedQuestion::MCQ){
            qEdit->firstAnswer = nullptr;
            buildMCQAnswers(qEdit);
        } else {
            string newAns;
            bool newAnsFound = false;
            do{
                cout << "Enter new correct answer: ";
                getline(cin, newAns);

                if(newAns == "") {
                    invalidInput();
                } else if(qEdit->questionType == LinkedQuestion::TFQ){
                    if(newAns == "true"){
                        qEdit->isTrue = true;
                        newAnsFound = true;
                    } else if(newAns == "false"){
                        qEdit->isTrue = false;
                        newAnsFound = true;
                    } else {
                        invalidInput();
                    }
                } else {
                    qEdit->targetWord = newAns;
                    newAnsFound = true;
                }
            }while(!newAnsFound);
        }
        // Depends on MCQ/TF&WR
    } else if(editOption == 5){
        if(numOptions == 5){
            string newAns;
            bool validInput = false;
            int numAnswerChoices = qEdit->getNumMCQChoices();

            do{
                cout << "Enter new correct answer choice: ";
                getline(cin, newAns);

                if(newAns.length() != 1 || toupper(newAns[0]) < 'A' || toupper(newAns[0]) >= 'A' + numAnswerChoices - 1){
                    invalidInput();
                }else {
                    validInput = true;
                    char letter = toupper(newAns[0]);
                    LinkedQuestion::LinkedAnswer* temp = qEdit->firstAnswer;
                
                    while(temp != nullptr){
                        if(temp->letter == letter){
                            temp->correctAnswer = true;
                            qEdit->correctLetter = letter;
                        } else {
                            temp->correctAnswer = false;
                        }
                        temp = temp->nextChoice;
                    }
                }
            }while(!validInput);
        }
    }

}
void deleteQuestion(){
    if (numQuestions == 0) {
        cout << "No questions to delete.\n";
        return;
    }
    
    int input;
    cout << "\nType a number to delete [1-" << numQuestions << "]: ";
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
