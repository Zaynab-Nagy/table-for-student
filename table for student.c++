#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // for std::transform
#include <cctype>    // for std::isalnum

using namespace std;

class StudentData {
public:
    string name;
    string gmail;
    int level;
    int numSubjects;
    vector<string> subjects;
    vector<int> studyDurations;
    int breakDuration;
    string studyTime;
    vector<string> dailyActivities;
};

// Define a struct to represent each activity (study or break)
struct Activity {
    string name;
    string timeSlot;
};

// Function to display the activity table
void displayActivityTable(const vector<Activity>& activities) {
    cout << "----------------------------------------------\n";
    cout << "|  Activity            |       Time Slot       |\n";
    cout << "----------------------------------------------\n";
    for (const Activity& act : activities) {
        cout << "|  " << act.name;
        for (int i = 0; i < 20 - act.name.length(); ++i) {
            cout << " ";
        }
        cout << "|       " << act.timeSlot;
        for (int i = 0; i < 20 - act.timeSlot.length(); ++i) {
            cout << " ";
        }
        cout << "|\n";
    }
    cout << "----------------------------------------------\n";
}

// Function to generate study times for each subject
vector<Activity> generateStudyTimes(const vector<string>& subjects, const vector<int>& studyDurations, int breakDuration, string studyTime, const vector<string>& dailyActivities) {
    vector<Activity> activities;
    int currentTime = (studyTime == "morning" || studyTime == "m" || studyTime == "mo") ? 7 * 60 : 19 * 60; // Start from 7:00 AM or 7:00 PM in minutes

    for (size_t i = 0; i < subjects.size(); ++i) {
        // Convert current time to 12-hour format
        int hours = currentTime / 60;
        int minutes = currentTime % 60;
        string timeSuffix = (hours < 12) ? "AM" : "PM";
        if (hours > 12) hours -= 12;

        // Add study activity
        string startHour = (hours == 0) ? "12" : ((hours < 10) ? "0" + to_string(hours) : to_string(hours));
        string endHour = (((hours + (studyDurations[i] / 60)) % 12) == 0) ? "12" : (((hours + (studyDurations[i] / 60)) % 12) < 10 ? "0" + to_string((hours + (studyDurations[i] / 60)) % 12) : to_string((hours + (studyDurations[i] / 60)) % 12));
        string startMin = (minutes < 10) ? "0" + to_string(minutes) : to_string(minutes);
        string endMin = (((minutes + studyDurations[i] % 60)) < 10) ? "0" + to_string(minutes + (studyDurations[i] % 60)) : to_string(minutes + (studyDurations[i] % 60));

        activities.push_back({subjects[i] + " Study", startHour + ":" + startMin + " " + timeSuffix + " - " + endHour + ":" + endMin + " " + timeSuffix});

        // Add break activity
        currentTime += studyDurations[i];
        int breakStartHour = (currentTime / 60) % 12;
        int breakStartMin = currentTime % 60;
        string breakStartSuffix = (currentTime < 720) ? "AM" : "PM";
        string breakEndHour = (((currentTime + breakDuration) / 60) % 12) == 0 ? "12" : to_string(((currentTime + breakDuration) / 60) % 12);
        string breakEndMin = ((currentTime + breakDuration) % 60) < 10 ? "0" + to_string((currentTime + breakDuration) % 60) : to_string((currentTime + breakDuration) % 60);
        string breakEndSuffix = ((currentTime + breakDuration) < 720) ? "AM" : "PM";

        activities.push_back({"Break", 
                               (breakStartHour == 0 ? "12" : to_string(breakStartHour)) + ":" + (breakStartMin < 10 ? "0" : "") + to_string(breakStartMin) + " " + breakStartSuffix +
                               " - " +
                               (breakEndHour == "0" ? "12" : breakEndHour) + ":" + breakEndMin + " " + breakEndSuffix});

        currentTime += breakDuration;
    }

    // Add daily activities
    for (const string& activity : dailyActivities) {
        activities.push_back({activity, "All day"});
    }

    return activities;
}

// Function to get study preferences from the student
void getStudyPreferences(string& studyTime, vector<string>& dailyActivities) {
    cout << "Do you prefer to study in the morning or evening? (Enter 'morning' or 'evening'): ";
    string preference;
    cin >> preference;

    cin.ignore(); // Clear the newline character from the input buffer

    // Validate the input and set studyTime accordingly
    if (preference == "evening" || preference == "e" || preference == "eve") 
        studyTime = "evening";
    else
        studyTime = "morning";

    cout << "Enter your favorite daily activities separated by commas (e.g., sports, reading, gaming): ";
    string activitiesInput;
    getline(cin, activitiesInput);

    // Remove spaces from the input
    activitiesInput.erase(remove(activitiesInput.begin(), activitiesInput.end(), ' '), activitiesInput.end());

    // Split the activities by commas and store them in the vector
    size_t pos = 0;
    while ((pos = activitiesInput.find(',')) != string::npos) {
        string activity = activitiesInput.substr(0, pos);
        dailyActivities.push_back(activity);
        activitiesInput.erase(0, pos + 1);
    }
    // Add the last activity
    dailyActivities.push_back(activitiesInput);
}

// Function to sanitize Gmail input
string sanitizeGmail(const string& input) {
    string sanitized;
    for (char c : input) {
        if (isalnum(c) || c == '@' || c == '.' || c == '_') {
            if (isalpha(c)) {
                sanitized += tolower(c);
            } else {
                sanitized += c;
            }
        }
    }
    return sanitized;
}

int main() {
    vector<Activity> activities;
    StudentData student;

    // Input student data
    cout << "Enter your name: ";
    getline(cin, student.name);

    cout << "Enter your Gmail: ";
    getline(cin, student.gmail);
    // Sanitize Gmail input
    student.gmail = sanitizeGmail(student.gmail);
    // Validate Gmail input
    if (student.gmail.find('@') == string::npos || student.gmail.find('.') == string::npos) {
        cout << "Invalid Gmail format. Please enter a valid email address.\n";
        return 1; // Exit the program with an error code
    }

    cout << "Enter your level: ";
    cin >> student.level;
    // Validate level input
    if (student.level <= 0) {
        cout << "Invalid level. Please enter a positive integer.\n";
        return 1; // Exit the program with an error code
    }

    // Get study preferences from the student
    getStudyPreferences(student.studyTime, student.dailyActivities);

    cout << "Enter the number of subjects you want to study: ";
    cin >> student.numSubjects;
    // Validate number of subjects input
    if (student.numSubjects <= 0) {
        cout << "Invalid number of subjects. Please enter a positive integer.\n";
        return 1; // Exit the program with an error code
    }
    cin.ignore(); // Clear the newline character from the input buffer

    // Input subjects and study durations
    for (int i = 0; i < student.numSubjects; ++i) {
        string subject;
        int studyDuration;

        cout << "Enter subject " << i + 1 << ": ";
        getline(cin, subject);
        // Remove spaces from the subject
        subject.erase(remove(subject.begin(), subject.end(), ' '), subject.end());
        student.subjects.push_back(subject);

        cout << "Enter study duration for " << subject << " (in minutes): ";
        cin >> studyDuration;
        // Validate study duration input
        if (studyDuration <= 0) {
            cout << "Invalid study duration. Please enter a positive integer.\n";
            return 1; // Exit the program with an error code
        }
        student.studyDurations.push_back(studyDuration);

        cin.ignore(); // Clear the newline character from the input buffer
    }

    // Input break duration
    cout << "Enter break duration (in minutes) between each subject: ";
    cin >> student.breakDuration;
    // Validate break duration input
    if (student.breakDuration <= 0) {
        cout << "Invalid break duration. Please enter a positive integer.\n";
        return 1; // Exit the program with an error code
    }

    // Generate study times for each subject
    activities = generateStudyTimes(student.subjects, student.studyDurations, student.breakDuration, student.studyTime, student.dailyActivities);

    // Display activity table
    displayActivityTable(activities);

    return 0;
}
