#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>
using namespace std;

// Define a simplistic character struct
struct Character {
    string name;
    unordered_set<string> traits;  // Each character has a set of traits
};

// Function to calculate heuristic: information gain
double calculateHeuristic(const vector<Character>& characters, const string& trait) {
    int countWithTrait = 0;
    for (const auto& character : characters) {
        if (character.traits.count(trait)) {
            countWithTrait++;  // Count how many characters have this trait
        }
    }
    if (countWithTrait == 0 || countWithTrait == characters.size()) {
        return 0.0; // No information gain if no characters or all characters have this trait
    }
    
    // Calculate probability
    double p = static_cast<double>(countWithTrait) / characters.size();
    
    // Calculate heuristic value (entropy)
    double heuristicValue = -p * log2(p) - (1 - p) * log2(1 - p);
    
    // Print the heuristic calculation
    cout << "Heuristic for trait '" << trait << "': " 
         << heuristicValue << " (Count with trait: " << countWithTrait 
         << ", Total characters: " << characters.size() << ")\n";
    
    return heuristicValue;  // Return the calculated heuristic value
}

// Function to select the best question based on heuristic
string selectBestQuestion(const vector<Character>& characters, const unordered_set<string>& askedTraits) {
    unordered_map<string, double> heuristicValues;  // Store heuristic values for each trait
    for (const auto& character : characters) {
        for (const auto& trait : character.traits) {
            if (askedTraits.count(trait) == 0) { // Only consider unasked traits
                heuristicValues[trait] = calculateHeuristic(characters, trait);
            }
        }
    }
    
    // Find the trait with the highest heuristic value
    string bestTrait;
    double bestValue = -1.0;
    for (const auto& [trait, value] : heuristicValues) {
        if (value > bestValue) {
            bestValue = value;  // Update best value
            bestTrait = trait;  // Update best trait
        }
    }
    
    // Print the best trait selected
    cout << "\nBest trait to ask: " << bestTrait << " with heuristic value: " << bestValue << "\n";
    
    return bestTrait;  // Return the best trait to ask
}

int main() {
    // Example dataset of characters
    vector<Character> characters = {
        {"Thor", {"God of Thunder", "Hammer", "Asgardian"}},
        {"Harry Potter", {"Wizard", "Scar", "Hogwarts"}},
        {"Hulk", {"Bruce Banner", "Super Strength", "Avenger"}}
    };

    unordered_set<string> askedTraits;  // Set to keep track of asked traits

    cout << "\nWelcome to the Akinator-style game!\n";
    cout << "\nThink of a character from the following options:\n";
    for (const auto& character : characters) {
        cout << "- " << character.name << "\n";  // Show characters to the user
    }
    cout<<endl;
    // Game loop to ask questions until one character remains
    while (characters.size() > 1) {
        string question = selectBestQuestion(characters, askedTraits);  // Get the best question to ask
        if (question.empty()) {
            break;  // Exit if there are no traits left to ask
        }

        cout << endl <<"Ask about trait: " << question << endl;
        cout << "\nPlease respond with 'yes' or 'no': ";
        string response;
        getline(cin, response);  // Get user response

        // Update character list based on response
        if (response == "yes") {
            characters.erase(remove_if(characters.begin(), characters.end(),
                [&question](const Character& c) { return c.traits.count(question) == 0; }), characters.end());
            cout << "\nYou answered 'yes'. Narrowing down the characters...\n";
        } else {
            characters.erase(remove_if(characters.begin(), characters.end(),
                [&question](const Character& c) { return c.traits.count(question) != 0; }), characters.end());
            cout << "\nYou answered 'no'. Narrowing down the characters...\n";
        }
        
        askedTraits.insert(question);  // Add the trait to asked traits
        
        // Print remaining characters
        cout << "Remaining characters: ";
        for (const auto& character : characters) {
            cout << character.name << " ";
        }
        cout << "\n";
    }

    // Final guess
    if (characters.size() == 1) {
        cout << "\nCharacter guessed: " << characters.front().name << endl;
    } else {
        cout << "\nUnable to guess the character. Try again!\n";
    }

    return 0;
}
