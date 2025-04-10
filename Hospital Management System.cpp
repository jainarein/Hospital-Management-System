#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

struct Doctor {
    string name;
    string specialty;
    int fee;
    vector<string> timeSlots;
    map<string, bool> slotBooked;
};

struct Appointment {
    string patientName;
    int age;
    string disease;
    string doctorName;
    string timeSlot;
    int consultationFee;
    int testCost;
};

struct Feedback {
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

bool doctorLogin() {
    string username, password;
    map<string, string> credentials = {
        {"drsharma", "pass123"},
        {"drkapoor", "pass456"},
        {"drbansal", "pass789"}
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

void displayAvailableSlots(const Doctor& doc) {
    cout << "\nAvailable slots for " << doc.name << " (" << doc.specialty << "):\n";
    int idx = 1;
    for (const string& slot : doc.timeSlots) {
        cout << idx << ". " << slot;
        if (doc.slotBooked.at(slot)) cout << " [BOOKED]";
        cout << endl;
        idx++;
    }
}

void showDoctorAvailability(const map<string, Doctor>& doctors) {
    cout << "\n📋 Doctor Availability Today:\n";
    for (auto& pair : doctors) {
        const Doctor& doc = pair.second;
        int free = 0, total = doc.timeSlots.size();
        for (const string& slot : doc.timeSlots) {
            if (!doc.slotBooked.at(slot)) free++;
        }
        cout << "- " << doc.name << " (" << doc.specialty << "): " << free << "/" << total << " slots available\n";
    }
}

void handleCheckUp(map<string, Doctor>& doctors) {
    string patientName, disease;
    int age;

    cout << "\nEnter patient name: ";
    cin.ignore();
    getline(cin, patientName);
    cout << "Enter age: ";
    cin >> age;
    cout << "Enter disease (e.g., Fever, Diabetes, Skin): ";
    cin >> disease;

    string docKey = disease;
    if (doctors.find(docKey) == doctors.end()) {
        cout << "No doctor available for this disease.\n";
        return;
    }

    Doctor& doc = doctors[docKey];

    bool hasFreeSlot = false;
    for (const string& slot : doc.timeSlots) {
        if (!doc.slotBooked[slot]) {
            hasFreeSlot = true;
            break;
        }
    }
    if (!hasFreeSlot) {
        cout << "\n❌ Sorry! Doctor " << doc.name << " (" << doc.specialty << ") is fully booked today.\n";
        return;
    }

    displayAvailableSlots(doc);

    int choice;
    cout << "Select a time slot: ";
    cin >> choice;

    if (choice < 1 || choice > (int)doc.timeSlots.size()) {
        cout << "❌ Invalid slot number.\n";
        return;
    }

    string selectedSlot = doc.timeSlots[choice - 1];

    if (doc.slotBooked.find(selectedSlot) != doc.slotBooked.end() && doc.slotBooked[selectedSlot]) {
        cout << "❌ That time slot is already booked.\n";
        return;
    }

    doc.slotBooked[selectedSlot] = true;

    vector<pair<string, int>> tests;
    map<string, vector<pair<string, int>>> diseaseTests = {
        {"Fever", { {"Blood Test", 200}, {"Temperature Check", 100} }},
        {"Diabetes", { {"Blood Sugar", 250}, {"HbA1c", 300} }},
        {"Skin", { {"Allergy Test", 400}, {"Skin Scraping", 350} }}
    };

    int totalTestCost = 0;
    cout << "\nRecommended tests for " << disease << ":\n";
    if (diseaseTests.find(disease) != diseaseTests.end()) {
        auto options = diseaseTests[disease];
        for (int i = 0; i < options.size(); ++i) {
            cout << i + 1 << ". " << options[i].first << " - ₹" << options[i].second << endl;
        }
        cout << "Enter number of tests you want to do: ";
        int num;
        cin >> num;
        for (int i = 0; i < num; ++i) {
            cout << "Select test #" << (i + 1) << ": ";
            int t;
            cin >> t;
            if (t >= 1 && t <= options.size()) {
                tests.push_back(options[t - 1]);
                totalTestCost += options[t - 1].second;
            }
        }
    }

    int totalBill = doc.fee + totalTestCost;
    char hasInsurance;
    int insuranceCoverage = 0;
    int amountPayable;

    cout << "\n🛡 Do you have medical insurance? (y/n): ";
    cin >> hasInsurance;

    if (hasInsurance == 'y' || hasInsurance == 'Y') {
        cout << "Enter insurance coverage amount (₹): ";
        cin >> insuranceCoverage;
        if (insuranceCoverage > totalBill) insuranceCoverage = totalBill;
    }

    amountPayable = totalBill - insuranceCoverage;

    cout << "\n💳 =======================\n";
    cout << "        Final Billing      \n";
    cout << "===========================\n";
    cout << "👤 Patient Name     : " << patientName << endl;
    cout << "🧑‍⚕ Doctor Assigned : " << doc.name << " (" << doc.specialty << ")\n";
    cout << "🕒 Time Slot        : " << selectedSlot << endl;
    cout << "💰 Consultation Fee : ₹" << doc.fee << endl;
    cout << "🧪 Test Charges     : ₹" << totalTestCost << endl;
    cout << "💳 Total Bill       : ₹" << totalBill << endl;

    if (insuranceCoverage > 0) {
        cout << "🛡 Insurance Applied : ₹" << insuranceCoverage << endl;
        cout << "💸 Final Amount to Pay: ₹" << amountPayable << endl;
    } else {
        cout << "💸 Final Amount to Pay: ₹" << totalBill << endl;
    }

    cout << "===========================\n";

    string paymentMethod;
    cout << "\n💳 Choose a payment method (Cash / Card / UPI): ";
    cin >> ws;
    getline(cin, paymentMethod);
    cout << "✅ Payment of ₹" << amountPayable << " received via " << paymentMethod << ". Thank you!\n";

    Appointment appt;
    appt.patientName = patientName;
    appt.age = age;
    appt.disease = disease;
    appt.doctorName = doc.name;
    appt.timeSlot = selectedSlot;
    appt.consultationFee = doc.fee;
    appt.testCost = totalTestCost;
    allAppointments.push_back(appt);

    Feedback fb;
    fb.patientName = patientName;
    fb.doctorName = doc.name;
    cout << "\n📝 Please rate your experience with Dr. " << doc.name << " (1 to 5): ";
    cin >> fb.rating;
    cin.ignore();
    cout << "💬 Any comments? ";
    getline(cin, fb.comment);
    allFeedbacks.push_back(fb);
}

void handleEmergency(map<string, Doctor>& doctors) {
    cout << "\n🚨 Emergency detected! Assigning available doctor...\n";
    for (auto& pair : doctors) {
        Doctor& doc = pair.second;
        for (string slot : doc.timeSlots) {
            if (!doc.slotBooked[slot]) {
                doc.slotBooked[slot] = true;
                cout << "👨‍⚕ Assigned Doctor: " << doc.name << " (" << doc.specialty << ") at " << slot << endl;
                return;
            }
        }
    }
    cout << "❌ No doctors available right now.\n";
}

void displayAllAppointments() {
    if (allAppointments.empty()) {
        cout << "\n📭 No appointments have been booked yet.\n";
        return;
    }

    cout << "\n📋 ========================\n";
    cout << "   ALL PATIENT APPOINTMENTS\n";
    cout << "============================\n";

    for (int i = 0; i < allAppointments.size(); ++i) {
        Appointment appt = allAppointments[i];
        cout << "\n🧾 Patient #" << i + 1 << ":\n";
        cout << "👤 Name           : " << appt.patientName << endl;
        cout << "🎂 Age            : " << appt.age << endl;
        cout << "🦠 Disease        : " << appt.disease << endl;
        cout << "🧑‍⚕ Doctor        : " << appt.doctorName << endl;
        cout << "🕒 Time Slot      : " << appt.timeSlot << endl;
        cout << "💰 Consultation   : ₹" << appt.consultationFee << endl;
        cout << "🧪 Test Charges   : ₹" << appt.testCost << endl;
        cout << "💳 Total Bill     : ₹" << (appt.consultationFee + appt.testCost) << endl;
        cout << "-----------------------------";
    }
    cout << "\n✅ End of Appointments List.\n";
}

void displayFeedbacks() {
    if (allFeedbacks.empty()) {
        cout << "\n📭 No feedbacks submitted yet.\n";
        return;
    }

    cout << "\n🗣 All Patient Feedbacks:\n=========================\n";
    for (const Feedback& fb : allFeedbacks) {
        cout << "👤 Patient      : " << fb.patientName << endl;
        cout << "🧑‍⚕ Doctor       : " << fb.doctorName << endl;
        cout << "⭐ Rating       : " << fb.rating << "/5\n";
        cout << "💬 Comment      : " << fb.comment << endl;
        cout << "-----------------------------\n";
    }
}

int main() {
    map<string, Doctor> doctors = {
        {"Fever", {"Dr. Sharma", "General Physician", 300, {"10:00 AM", "10:30 AM", "11:00 AM", "11:30 AM"}}},
        {"Diabetes", {"Dr. Kapoor", "Endocrinologist", 450, {"1:00 PM", "1:30 PM", "2:00 PM", "2:30 PM"}}},
        {"Skin", {"Dr. Bansal", "Dermatologist", 400, {"3:00 PM", "3:30 PM", "4:00 PM"}}}
    };

    for (auto& pair : doctors) {
        for (const string& slot : pair.second.timeSlots) {
            pair.second.slotBooked[slot] = false;
        }
    }

    while (true) {
        cout << "\n👥 Choose User Type:\n";
        cout << "1. Patient\n";
        cout << "2. Doctor (Login Required)\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        int userType;
        cin >> userType;

        if (userType == 1) {
            while (true) {
                cout << "\n🏥 Patient Menu:\n";
                cout << "1. Emergency Admission\n";
                cout << "2. Book a Check-up\n";
                cout << "3. View Doctor Availability\n";
                cout << "4. Exit to Main Menu\n";
                cout << "Enter your choice: ";
                int choice;
                cin >> choice;

                switch (choice) {
                    case 1: handleEmergency(doctors); break;
                    case 2: handleCheckUp(doctors); break;
                    case 3: showDoctorAvailability(doctors); break;
                    case 4: goto main_menu;
                    default: cout << "❌ Invalid option. Try again.\n";
                }
            }
        } else if (userType == 2) {
            if (doctorLogin()) {
                while (true) {
                    cout << "\n🩺 Doctor Panel:\n";
                    cout << "1. View All Appointments\n";
                    cout << "2. View Feedbacks\n";
                    cout << "3. Logout\n";
                    cout << "Enter your choice: ";
                    int choice;
                    cin >> choice;

                    switch (choice) {
                        case 1: displayAllAppointments(); break;
                        case 2: displayFeedbacks(); break;
                        case 3: goto main_menu;
                        default: cout << "❌ Invalid option. Try again.\n";
                    }
                }
            }
        } else if (userType == 3) {
            cout << "🙏 Thank you for visiting Jaypee Hospital.\n";
            break;
        } else {
            cout << "❌ Invalid user type.\n";
        }

        main_menu: continue;
    }

    return 0;
}
