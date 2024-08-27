#include <iostream>
#include <vector>
#include <string>
#include <limits>

class Exercise {
protected:
    std::string type;
    int duration; // minutes
    double caloriesBurned;

public:
    Exercise(std::string t, int d) : type(t), duration(d), caloriesBurned(0) {}
    virtual void calculateCalories() = 0; // pure virtual function
    double getCaloriesBurned() const { return caloriesBurned; }
    std::string getType() const { return type; }
    int getDuration() const { return duration; }
    virtual ~Exercise() {} // virtual destructor
};

class Cardio : public Exercise {
public:
    Cardio(int duration) : Exercise("Cardio", duration) {}
    void calculateCalories() override {
        caloriesBurned = duration * 8.0; // example calculation
    }
};

class Strength : public Exercise {
public:
    Strength(int duration) : Exercise("Strength", duration) {}
    void calculateCalories() override {
        caloriesBurned = duration * 5.0; // example calculation
    }
};

class User {
private:
    std::string name;
    int age;
    double weight; // in kg
    double height; // in cm
    std::vector<Exercise*> exercises; // vector of pointers

public:
    User(std::string n, int a, double w, double h) : name(n), age(a), weight(w), height(h) {}
    ~User() {
        for (auto& exercise : exercises) {
            delete exercise; // free dynamically allocated memory
        }
    }
    void addExercise(Exercise* exercise) {
        exercise->calculateCalories();
        exercises.push_back(exercise);
    }
    double totalCaloriesBurned() const {
        double total = 0;
        for (const auto& exercise : exercises) {
            total += exercise->getCaloriesBurned();
        }
        return total;
    }
    void displayProgress() const {
        std::cout << "User: " << name << std::endl;
        std::cout << "Total Calories Burned: " << totalCaloriesBurned() << std::endl;
        for (const auto& exercise : exercises) {
            std::cout << "Exercise: " << exercise->getType() << ", Duration: " << exercise->getDuration() 
                      << " minutes, Calories Burned: " << exercise->getCaloriesBurned() << std::endl;
        }
    }
};

int main() {
    std::string name;
    int age;
    double weight, height;

    std::cout << "Enter your name: ";
    std::getline(std::cin, name);

    std::cout << "Enter your age: ";
    std::cin >> age;

    std::cout << "Enter your weight (in kg): ";
    std::cin >> weight;

    std::cout << "Enter your height (in cm): ";
    std::cin >> height;

    User user(name, age, weight, height);

    char choice;
    do {
        int type, duration;

        std::cout << "Log an exercise (1 for Cardio, 2 for Strength): ";
        std::cin >> type;

        std::cout << "Enter duration (in minutes): ";
        std::cin >> duration;

        if (type == 1) {
            user.addExercise(new Cardio(duration));
        } else if (type == 2) {
            user.addExercise(new Strength(duration));
        }

        std::cout << "Do you want to log another exercise? (y/n): ";
        std::cin >> choice;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    } while (choice == 'y' || choice == 'Y');

    user.displayProgress();

    return 0;
}