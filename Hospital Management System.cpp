#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

// Base class
class Person {
public:
    string name;
    int age;

    Person() {}
    Person(string n, int a) : name(n), age(a) {}

    virtual void displayInfo() {
        cout << "Name: " << name << ", Age: " << age << endl;
    }
};

// Doctor inherits from Person
class Doctor : public Person {
public:
    string specialty;
    int fee;
    vector<string> timeSlots;
    map<string, bool> slotBooked;

    Doctor() {}
    Doctor(string n, string s, int f, vector<string> t) {
        name = n;
        specialty = s;
        fee = f;
        timeSlots = t;
        for (auto slot : t)
            slotBooked[slot] = false;
    }

    void displayInfo() override {
        cout << "Dr. " << name << " (" << specialty << "), Fee: ₹" << fee << endl;
    }

    void displaySlots() {
        cout << "\nAvailable slots for Dr. " << name << ":\n";
        int idx = 1;
        for (const string& slot : timeSlots) {
            cout << idx++ << ". " << slot;
            if (slotBooked[slot]) cout << " [BOOKED]";
            cout << endl;
        }
    }
};

// Appointment (Overloading Constructor)
class Appointment {
public:
    string patientName;
    int age;
    string disease;
    string doctorName;
    string timeSlot;
    int consultationFee;
    int testCost;

    Appointment() {}

    // Overloaded Constructor
    Appointment(string pname, int a, string d, string doc, string ts, int cf, int tc) {
        patientName = pname;
        age = a;
        disease = d;
        doctorName = doc;
        timeSlot = ts;
        consultationFee = cf;
        testCost = tc;
    }
};

class Feedback {
public:
    string patientName;
    string doctorName;
    int rating;
    string comment;
};

vector<Appointment> allAppointments;
vector<Feedback> allFeedbacks;

string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// ✅ Login System
bool doctorLogin() {
    string username, password;
    map<string, string> credentials = {
        {"aryan", "tripathi"},
        {"arein", "jain"},
        {"prateek", "sir"},
        {"jasmin","mam"},
        {"neetu","mam"}
    };

    cout << "\n👨‍⚕ Doctor Login\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    username = toLower(username);
    if (credentials.find(username) != credentials.end() && credentials[username] == password) {
        cout << "✅ Login successful. Welcome, Dr. " << username << "!\n";
        return true;
    } else {
        cout << "❌ Invalid username or password.\n";
        return false;
    }
}

// Patient Checkup
void handleCheckUp(map<string, Doctor>& doctors) {
    string patientName, disease;
    int age;

    cout << "\nEnter patient name: ";
    cin.ignore();
    getline(cin, patientName);
    cout << "Enter age: ";
    cin >> age;
    cout << "Enter disease (Fever / Diabetes / Skin): ";
    cin >> disease;

    if (doctors.find(disease) == doctors.end()) {
        cout << "❌ No doctor available for this disease.\n";
        return;
    }

    Doctor& doc = doctors[disease];
    doc.displaySlots();

    int choice;
    cout << "Choose a slot: ";
    cin >> choice;

    if (choice < 1 || choice > doc.timeSlots.size()) {
        cout << "❌ Invalid slot.\n";
        return;
    }

    string selectedSlot = doc.timeSlots[choice - 1];
    if (doc.slotBooked[selectedSlot]) {
        cout << "❌ Slot already booked.\n";
        return;
    }

    doc.slotBooked[selectedSlot] = true;

    // Tests by disease
    map<string, vector<pair<string, int>>> diseaseTests = {
        {"Fever", {{"Blood Test", 800}, {"RBC Count", 300}}},
        {"Diabetes", {{"Blood Sugar", 100}, {"HbA1c", 300}}},
        {"Skin", {{"Allergy Test", 400}, {"Skin Scraping", 350}}}
    };

    vector<pair<string, int>> selectedTests;
    int totalTestCost = 0;
    if (diseaseTests.find(disease) != diseaseTests.end()) {
        auto tests = diseaseTests[disease];
        cout << "\nRecommended tests:\n";
        for (int i = 0; i < tests.size(); i++) {
            cout << i + 1 << ". " << tests[i].first << " - ₹" << tests[i].second << endl;
        }
        cout << "How many tests to do? ";
        int num;
        cin >> num;
        for (int i = 0; i < num; i++) {
            int t;
            cout << "Select test #" << (i + 1) << ": ";
            cin >> t;
            if (t >= 1 && t <= tests.size()) {
                selectedTests.push_back(tests[t - 1]);
                totalTestCost += tests[t - 1].second;
            }
        }
    }

    int totalBill = doc.fee + totalTestCost;
    int insurance = 0;
    char ch;
    cout << "Have insurance? (y/n): ";
    cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        cout << "Insurance coverage amount: ";
        cin >> insurance;
        if (insurance > totalBill) insurance = totalBill;
    }

    int finalAmount = totalBill - insurance;

    cout << "\n💳 BILL SUMMARY\n";
    cout << "Consultation Fee: ₹" << doc.fee << endl;
    cout << "Test Charges: ₹" << totalTestCost << endl;
    cout << "Total: ₹" << totalBill << endl;
    cout << "Insurance: ₹" << insurance << endl;
    cout << "Payable: ₹" << finalAmount << endl;

    string paymentMode;
    cout << "Payment method (Cash / UPI / Card): ";
    cin >> paymentMode;
    cout << "✅ Payment received via " << paymentMode << ". Thank you!\n";

    Appointment appt(patientName, age, disease, doc.name, selectedSlot, doc.fee, totalTestCost);
    allAppointments.push_back(appt);

    Feedback fb;
    fb.patientName = patientName;
    fb.doctorName = doc.name;
    cout << "\nRate Dr. " << doc.name << " (1-5): ";
    cin >> fb.rating;
    cin.ignore();
    cout << "Comment: ";
    getline(cin, fb.comment);
    allFeedbacks.push_back(fb);
}

void displayAppointments() {
    cout << "\n📋 All Appointments:\n";
    for (int i = 0; i < allAppointments.size(); i++) {
        Appointment& a = allAppointments[i];
        cout << i + 1 << ". " << a.patientName << " | Age: " << a.age << " | " << a.disease
             << " | Dr. " << a.doctorName << " | " << a.timeSlot
             << " | Fee: ₹" << a.consultationFee << " | Tests: ₹" << a.testCost << endl;
    }
}

void displayFeedbacks() {
    cout << "\n🗣 Feedbacks:\n";
    for (auto& fb : allFeedbacks) {
        cout << "Patient: " << fb.patientName << " | Dr. " << fb.doctorName
             << " | ⭐ " << fb.rating << "/5 | \"" << fb.comment << "\"\n";
    }
}

int main() {
    map<string, Doctor> doctors = {
        {"Fever", Doctor("Sharma", "General Physician", 300, {"10:00", "10:30", "11:00"})},
        {"Diabetes", Doctor("Kapoor", "Endocrinologist", 450, {"1:00", "1:30", "2:00"})},
        {"Skin", Doctor("Bansal", "Dermatologist", 400, {"3:00", "3:30", "4:00"})}
    };

    while (true) {
        cout << "\n👥 User Type:\n1. Patient\n2. Doctor Login\n3. Exit\nChoice: ";
        int type;
        cin >> type;
        if (type == 1) {
            while (true) {
                cout << "\n🏥 Patient Menu:\n1. Book Checkup\n2. Back\nChoice: ";
                int c;
                cin >> c;
                if (c == 1)
                    handleCheckUp(doctors);
                else
                    break;
            }
        } else if (type == 2) {
            if (doctorLogin()) {
                while (true) {
                    cout << "\n🩺 Doctor Menu:\n1. View Appointments\n2. View Feedbacks\n3. Logout\nChoice: ";
                    int d;
                    cin >> d;
                    if (d == 1)
                        displayAppointments();
                    else if (d == 2)
                        displayFeedbacks();
                    else
                        break;
                }
            }
        } else {
            cout << "\n🙏 Thank you for visiting!\n";
            break;
        }
    }

    return 0;
}
