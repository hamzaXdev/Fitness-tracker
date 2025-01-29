#include <iostream>
#include <string> //Hamza,Shayyan,Zaid(BSSE)
#include <sstream>

using namespace std;

// Node structure for a linked list to store user data
struct UserNode {
    string username;
    string password;
    int age;
    float weight; // in kg
    float height; // in cm
    float dailyCalories;
    float bulkCalories;
    float maintainCalories;
    float mildWeightLossCalories;
    float weightLossCalories;
    UserNode* next;

    UserNode(string uname, string pword, int a, float w, float h, float dc, float bc, float mc, float mwlc, float wlc)
        : username(uname), password(pword), age(a), weight(w), height(h),
          dailyCalories(dc), bulkCalories(bc), maintainCalories(mc),
          mildWeightLossCalories(mwlc), weightLossCalories(wlc), next(nullptr) {}
};

// Node structure for a stack to store activity records
struct ActivityNode {
    float caloriesBurned;
    float caloriesEaten;
    float remainingCalories;
    ActivityNode* next;

    ActivityNode(float cb, float ce, float rc)
        : caloriesBurned(cb), caloriesEaten(ce), remainingCalories(rc), next(nullptr) {}
};

// Base class for User
class User {
protected:
    string username;
    string password;
    int age;
    float weight;
    float height;
    float dailyCalories;
    float bulkCalories;
    float maintainCalories;
    float mildWeightLossCalories;
    float weightLossCalories;
    bool loggedIn;
    UserNode* userListHead;  // Head of the linked list storing user records
    ActivityNode* activityStackTop;  // Top of the stack storing activity records

public:
    User() : loggedIn(false), userListHead(nullptr), activityStackTop(nullptr) {}

    virtual void registerUser() = 0;

    void loginUser() {
        if (!userListHead) {
            cout << "No registered users found." << endl;
            return;
        }

        string enteredUsername, enteredPassword;
        cout << "Enter username: ";
        cin >> enteredUsername;
        cout << "Enter password: ";
        cin >> enteredPassword;

        UserNode* temp = userListHead;
        while (temp) {
            if (temp->username == enteredUsername && temp->password == enteredPassword) {
                username = temp->username;
                password = temp->password;
                age = temp->age;
                weight = temp->weight;
                height = temp->height;
                dailyCalories = temp->dailyCalories;
                bulkCalories = temp->bulkCalories;
                maintainCalories = temp->maintainCalories;
                mildWeightLossCalories = temp->mildWeightLossCalories;
                weightLossCalories = temp->weightLossCalories;
                loggedIn = true;
                cout << "Login successful!" << endl;
                return;
            }
            temp = temp->next;
        }

        cout << "Invalid username or password!" << endl;
    }

    void recordActivity() {
        if (!loggedIn) {
            cout << "Please log in first." << endl;
            return;
        }

        float caloriesBurned, caloriesEaten, remainingCalories;
        do {
            cout << "Enter calories burned: ";
            cin >> caloriesBurned;
            cout << "Enter calories eaten: ";
            cin >> caloriesEaten;

            cout << "Choose your option:\n";
            cout << "1. Bulk\n";
            cout << "2. Maintain\n";
            cout << "3. Mild Weight Loss\n";
            cout << "4. Weight Loss\n";
            cout << "Enter choice (1-4): ";
            int option;
            cin >> option;

            switch (option) {
                case 1:
                    remainingCalories = bulkCalories - (caloriesEaten - caloriesBurned);
                    break;
                case 2:
                    remainingCalories = maintainCalories - (caloriesEaten - caloriesBurned);
                    break;
                case 3:
                    remainingCalories = mildWeightLossCalories - (caloriesEaten - caloriesBurned);
                    break;
                case 4:
                    remainingCalories = weightLossCalories - (caloriesEaten - caloriesBurned);
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
                    continue;
            }

            if (remainingCalories == 0) {
                cout << "You are good to go! You have consumed exactly the required calories." << endl;
            } else if (remainingCalories > 0) {
                cout << "You have consumed fewer calories than needed. You can still consume " << remainingCalories << " extra calories." << endl;
            } else {
                cout << "You have consumed more calories than needed. You have exceeded the required calories by " << -remainingCalories << " calories." << endl;
            }

            // Push the activity record onto the stack
            ActivityNode* newActivity = new ActivityNode(caloriesBurned, caloriesEaten, remainingCalories);
            newActivity->next = activityStackTop;
            activityStackTop = newActivity;

            break;
        } while (true);
    }

    void undoLastActivity() {
        if (!loggedIn) {
            cout << "Please log in first." << endl;
            return;
        }

        if (!activityStackTop) {
            cout << "No activity to undo!" << endl;
            return;
        }

        ActivityNode* temp = activityStackTop;
        activityStackTop = activityStackTop->next;
        delete temp;
        cout << "Last activity undone successfully!" << endl;
    }

    void viewActivity() {
        if (!loggedIn) {
            cout << "Please log in first." << endl;
            return;
        }

        if (!activityStackTop) {
            cout << "No activity recorded yet." << endl;
            return;
        }

        ActivityNode* temp = activityStackTop;
        while (temp) {
            cout << "Calories Burned: " << temp->caloriesBurned
                 << ", Calories Eaten: " << temp->caloriesEaten
                 << ", Remaining Calories: " << temp->remainingCalories << endl;
            temp = temp->next;
        }
    }

    void viewProfile() {
        if (!loggedIn) {
            cout << "Please log in first." << endl;
            return;
        }

        cout << "Username: " << username << endl;
        cout << "Age: " << age << endl;
        cout << "Weight: " << weight << " kg" << endl;
        cout << "Height: " << height << " cm" << endl;
        cout << "Daily Calories to Maintain Weight: " << maintainCalories << endl;
        cout << "Daily Calories to Bulk: " << bulkCalories << endl;
        cout << "Daily Calories for Mild Weight Loss: " << mildWeightLossCalories << endl;
        cout << "Daily Calories for Weight Loss: " << weightLossCalories << endl;
    }

    void logout() {
        if (loggedIn) {
            loggedIn = false;
            cout << "Logged out successfully!" << endl;
        } else {
            cout << "You are not logged in." << endl;
        }
    }

    void addUserToList(string uname, string pword, int a, float w, float h, float dc, float bc, float mc, float mwlc, float wlc) {
        UserNode* newUser = new UserNode(uname, pword, a, w, h, dc, bc, mc, mwlc, wlc);
        newUser->next = userListHead;
        userListHead = newUser;
    }

    ~User() {
        while (userListHead) {
            UserNode* temp = userListHead;
            userListHead = userListHead->next;
            delete temp;
        }
        while (activityStackTop) {
            ActivityNode* temp = activityStackTop;
            activityStackTop = activityStackTop->next;
            delete temp;
        }
    }
};

class UserActivityTracker : public User {
public:
    void registerUser() {
        do {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            cout << "Enter age: ";
            cin >> age;
            cout << "Enter weight (kg): ";
            cin >> weight;
            cout << "Enter height (cm): ";
            cin >> height;

            int activityLevel;
            cout << "Select your activity level:\n";
            cout << "1. Sedentary (little or no exercise)\n";
            cout << "2. Light (exercise 1 to 3 times/week)\n";
            cout << "3. Moderate (exercise 4 to 5 times/week)\n";
            cout << "4. Active (daily exercise or intense exercise 3 to 4 times/week)\n";
            cout << "5. Very Active (intense exercise 6 to 7 times per week)\n";
            cout << "Enter choice (1-5): ";
            cin >> activityLevel;

            float bmr = 10 * weight + 6.25 * height - 5 * age;
            switch (activityLevel) {
                case 1:
                    dailyCalories = bmr * 1.2;
                    break;
                case 2:
                    dailyCalories = bmr * 1.375;
                    break;
                case 3:
                    dailyCalories = bmr * 1.55;
                    break;
                case 4:
                    dailyCalories = bmr * 1.725;
                    break;
                case 5:
                    dailyCalories = bmr * 1.9;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
                    continue;
            }

            bulkCalories = dailyCalories + 250;
            maintainCalories = dailyCalories;
            mildWeightLossCalories = dailyCalories - 250;
            weightLossCalories = dailyCalories - 500;

            addUserToList(username, password, age, weight, height, dailyCalories, bulkCalories, maintainCalories, mildWeightLossCalories, weightLossCalories);
            cout << "Registration successful!" << endl;
            break;
        } while (true);
    }
};

int main() {
    UserActivityTracker user;

    do {
        int choice;
        cout << "\n*****************************" << endl;
        cout << "Fitness Tracker Menu:" << endl;
        cout << "*****************************" << endl;
        cout << "1. Register User\n"
             << "2. Login User\n"
             << "3. Record Activity\n"
             << "4. Undo Last Activity\n"
             << "5. View Activity\n"
             << "6. View Profile\n"
             << "7. Logout\n"
             << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                user.registerUser();
                break;
            case 2:
                user.loginUser();
                break;
            case 3:
                user.recordActivity();
                break;
            case 4:
                user.undoLastActivity();
                break;
            case 5:
                user.viewActivity();
                break;
            case 6:
                user.viewProfile();
                break;
            case 7:
                user.logout();
                break;
            case 8:
                cout << "Exiting program. Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (true);
}
