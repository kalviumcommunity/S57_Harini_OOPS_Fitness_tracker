#include <iostream>
#include <vector>
#include <string>
#include <limits>

// Abstract base class definition (Exercise).
class Exercise {
protected:
    std::string type;
    int duration; // minutes
    double caloriesBurned;

// Pure virtual function (calculateCalories()).
public:
    // constructor
    Exercise(std::string t, int d) : type(t), duration(d), caloriesBurned(0) {}
    virtual void calculateCalories() = 0; // pure virtual function
    double getCaloriesBurned() const { return caloriesBurned; }
    std::string getType() const { return type; }
    int getDuration() const { return duration; }
    virtual ~Exercise() {} // virtual destructor
};

class Cardio : public Exercise {
public:
    // Derived class Cardio implements the abstract function.
    Cardio(int duration) : Exercise("Cardio", duration) {}
    void calculateCalories() override {
        caloriesBurned = duration * 8.0; 
    }
};

class Strength : public Exercise {
public:
    // Derived class Strength implements the abstract function.
    Strength(int duration) : Exercise("Strength", duration) {}
    void calculateCalories() override {
        caloriesBurned = duration * 5.0;
    }
};

// Encapsulation in the User class:
class User {
protected:  // Changed from private to protected for inheritance access.
    std::string name;
    int age;
    double weight; // in kg
    double height; // in cm
    std::vector<Exercise*> exercises; // Stores exercises of the user.

    static int totalUsers; // Static variable to count the total number of users.

public:
    // Parameterized constructor.
    User(std::string n, int a, double w, double h) : name(n), age(a), weight(w), height(h) {
        ++totalUsers; // Increment the static variable when a new user is created.
    }

    // Using 'this' pointer to return the current object reference.
    User& setName(const std::string& n) {
        this->name = n;
        return *this;
    }

    User& setAge(int a) {
        this->age = a;
        return *this;
    }

    User& setWeight(double w) {
        this->weight = w;
        return *this;
    }

    User& setHeight(double h) {
        this->height = h;
        return *this;
    }

    void addExercise(Exercise* exercise) {
        exercise->calculateCalories();
        exercises.push_back(exercise);
    }

    // Encapsulation of totalUsers variable.
    virtual double totalCaloriesBurned() const {
        double total = 0;
        for (const auto& exercise : exercises) {
            total += exercise->getCaloriesBurned();
        }
        return total;
    }

    virtual void displayProgress() const {
        std::cout << "User: " << name << std::endl;
        std::cout << "Total Calories Burned: " << totalCaloriesBurned() << std::endl;
        for (const auto& exercise : exercises) {
            std::cout << "Exercise: " << exercise->getType() << ", Duration: " << exercise->getDuration()
                      << " minutes, Calories Burned: " << exercise->getCaloriesBurned() << std::endl;
        }
    }

    // Static member function to get the total number of users.
    static int getTotalUsers() {
        return totalUsers;
    }

    virtual ~User() {
        for (auto& exercise : exercises) {
            delete exercise; // Free dynamically allocated memory for exercises.
        }
        --totalUsers;      // Decrement the static variable when a user is deleted.
    }
};

// Initialize the static variable outside the class.
int User::totalUsers = 0;

// Derived class for Premium Users
class PremiumUser : public User {
public:
    // Constructor for premium user.
    PremiumUser(std::string n, int a, double w, double h) : User(n, a, w, h) {}

    // Override totalCaloriesBurned() to give 1.5x benefit for premium users.
    double totalCaloriesBurned() const override {
        return User::totalCaloriesBurned() * 1.5;
    }

    // Override displayProgress to show a message for premium users.
    void displayProgress() const override {
        std::cout << "Premium User: " << name << " (1.5x calorie burn benefit)\n";
        User::displayProgress();
    }
};

int main() {
    int numUsers;

    std::cout << "Enter the number of users: ";
    std::cin >> numUsers;

    // Allocate an array of pointers to User objects.
    User** users = new User*[numUsers];

    // Initialize each user using the parameterized constructor.
    for (int i = 0; i < numUsers; ++i) {
        std::string name;
        int age;
        double weight, height;

        std::cout << "\nEnter details for User " << i + 1 << std::endl;
        std::cout << "Enter your name: ";
        std::cin >> name;
        std::cout << "Enter your age: ";
        std::cin >> age;
        std::cout << "Enter your weight (in kg): ";
        std::cin >> weight;
        std::cout << "Enter your height (in cm): ";
        std::cin >> height;

        char userType;
        std::cout << "Is this user premium? (y/n): ";
        std::cin >> userType;

        if (userType == 'y' || userType == 'Y') {
            users[i] = new PremiumUser(name, age, weight, height); // PremiumUser
        } else {
            users[i] = new User(name, age, weight, height); // RegularUser
        }

        char choice;
        do {
            int type, duration;

            std::cout << "Log an exercise (1 for Cardio, 2 for Strength): ";
            std::cin >> type;

            std::cout << "Enter duration (in minutes): ";
            std::cin >> duration;

            if (type == 1) {
                users[i]->addExercise(new Cardio(duration));
            } else if (type == 2) {
                users[i]->addExercise(new Strength(duration));
            }

            std::cout << "Do you want to log another exercise? (y/n): ";
            std::cin >> choice;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        } while (choice == 'y' || choice == 'Y');
    }

    // Display progress for all users.
    for (int i = 0; i < numUsers; ++i) {
        std::cout << "\nProgress for User " << i + 1 << std::endl;
        users[i]->displayProgress();
        delete users[i]; // Free memory for each User object.
    }

    // Static function call to get the total number of users.
    std::cout << "\nTotal number of users at the end: " << User::getTotalUsers() << std::endl;

    delete[] users; // Free memory for the array of pointers.

    return 0;
}
