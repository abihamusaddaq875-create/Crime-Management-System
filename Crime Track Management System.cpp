#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

// ================= OFFICER CLASS =================
class Officer {
private:
    string name;
    int badgeID;

public:
    Officer(string n = "", int b = 0) {
        name = n;
        badgeID = b;
    }

    void input() {
        // Clear any leftover newline from previous input
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Officer Name: ";
        getline(cin, name);

        cout << "Enter Badge ID: ";
        cin >> badgeID;
    }

    string toString() const {
        return name + " (Badge: " + to_string(badgeID) + ")";
    }
};

// ================= EVIDENCE CLASS =================
class Evidence {
private:
    vector<string> items;

public:
    void input() {
        items.clear();

        int n;
        cout << "How many evidence items? ";
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (int i = 0; i < n; i++) {
            string e;
            cout << "Enter evidence " << i + 1 << ": ";
            getline(cin, e);
            items.push_back(e);
        }
    }

    string toString() const {
        string result = "";
        for (size_t i = 0; i < items.size(); i++) {
            result += items[i];
            if (i != items.size() - 1)
                result += ", ";
        }
        return result;
    }
};

// ================= CRIME RECORD =================
class CrimeRecord {
private:
    int caseID;
    string crimeType;
    string criminalName;
    int age;
    string date;
    string status;

    Officer officer;
    Evidence evidence;

public:
    CrimeRecord() {
        caseID = 0;
        age = 0;
        status = "Open";
    }

    int getID() const {
        return caseID;
    }

    string getStatus() const {
        return status;
    }

    void setStatus(string s) {
        status = s;
    }

    // INPUT
    void input() {
        cout << "\nEnter Case ID: ";
        cin >> caseID;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Crime Type: ";
        getline(cin, crimeType);

        cout << "Enter Criminal Name: ";
        getline(cin, criminalName);

        cout << "Enter Age of Suspect: ";
        cin >> age;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Date of Crime (DD/MM/YYYY): ";
        getline(cin, date);

        cout << "Enter Status (Open/Closed): ";
        getline(cin, status);

        cout << "\n--- Officer Details ---\n";
        officer.input();

        cout << "\n--- Evidence Details ---\n";
        evidence.input();
    }

    // DISPLAY
    void display() const {
        cout << "\n===================================";
        cout << "\nCase ID      : " << caseID;
        cout << "\nCrime Type   : " << crimeType;
        cout << "\nCriminal     : " << criminalName;
        cout << "\nAge          : " << age;
        cout << "\nDate         : " << date;
        cout << "\nStatus       : " << status;
        cout << "\nOfficer      : " << officer.toString();
        cout << "\nEvidence     : " << evidence.toString();
        cout << "\n===================================\n";
    }

    // SAVE TO FILE
    string toFile() const {
        return to_string(caseID) + "|" +
            crimeType + "|" +
            criminalName + "|" +
            to_string(age) + "|" +
            date + "|" +
            status + "|" +
            officer.toString() + "|" +
            evidence.toString();
    }

    // LOAD FROM FILE
    void fromFile(string line) {
        stringstream ss(line);
        string temp;

        getline(ss, temp, '|');
        caseID = stoi(temp);

        getline(ss, crimeType, '|');
        getline(ss, criminalName, '|');

        getline(ss, temp, '|');
        age = stoi(temp);

        getline(ss, date, '|');
        getline(ss, status, '|');
    }
};

// ================= CRIME SYSTEM =================
class CrimeSystem {
private:
    vector<CrimeRecord> records;
    string file = "crime_data.txt";

public:
    // LOAD DATA
    void load() {
        records.clear();

        ifstream in(file);
        string line;

        while (getline(in, line)) {
            if (line.empty())
                continue;

            CrimeRecord record;
            record.fromFile(line);
            records.push_back(record);
        }

        in.close();
    }

    // SAVE DATA
    void save() {
        ofstream out(file);

        for (size_t i = 0; i < records.size(); i++) {
            out << records[i].toFile() << endl;
        }

        out.close();
    }

    // ADD CASE
    void add() {
        CrimeRecord record;
        record.input();
        records.push_back(record);
        save();

        cout << "\nRecord Added Successfully!\n";
    }

    // VIEW ALL CASES
    void viewAll() {
        if (records.empty()) {
            cout << "\nNo records found.\n";
            return;
        }

        for (size_t i = 0; i < records.size(); i++) {
            records[i].display();
        }
    }

    // SEARCH CASE
    void search() {
        int id;
        cout << "\nEnter Case ID to Search: ";
        cin >> id;

        for (size_t i = 0; i < records.size(); i++) {
            if (records[i].getID() == id) {
                records[i].display();
                return;
            }
        }

        cout << "\nRecord Not Found.\n";
    }

    // DELETE CASE
    void remove() {
        int id;
        cout << "\nEnter Case ID to Delete: ";
        cin >> id;

        for (auto it = records.begin(); it != records.end(); ++it) {
            if (it->getID() == id) {
                records.erase(it);
                save();
                cout << "\nRecord Deleted Successfully!\n";
                return;
            }
        }

        cout << "\nRecord Not Found.\n";
    }

    // UPDATE STATUS
    void update() {
        int id;
        cout << "\nEnter Case ID to Update: ";
        cin >> id;

        for (size_t i = 0; i < records.size(); i++) {
            if (records[i].getID() == id) {
                string newStatus;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Enter New Status (Open/Closed): ";
                getline(cin, newStatus);

                records[i].setStatus(newStatus);
                save();

                cout << "\nStatus Updated Successfully!\n";
                return;
            }
        }

        cout << "\nRecord Not Found.\n";
    }

    // STATISTICS
    void stats() {
        int total = records.size();
        int open = 0;
        int closed = 0;

        for (size_t i = 0; i < records.size(); i++) {
            if (records[i].getStatus() == "Open")
                open++;
            else
                closed++;
        }

        cout << "\n========== STATISTICS DASHBOARD ==========\n";
        cout << "Total Cases   : " << total << endl;
        cout << "Open Cases    : " << open << endl;
        cout << "Closed Cases  : " << closed << endl;
        cout << "=========================================\n";
    }

    // MENU
    void menu() {
        int choice;

        do {
            cout << "\n\n========== CRIME MANAGEMENT SYSTEM ==========\n";
            cout << "1. Add New Case\n";
            cout << "2. View All Cases\n";
            cout << "3. Search Case by ID\n";
            cout << "4. Delete Case\n";
            cout << "5. Update Case Status\n";
            cout << "6. View Statistics Dashboard\n";
            cout << "7. Exit\n";
            cout << "Enter Your Choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                add();
                break;
            case 2:
                viewAll();
                break;
            case 3:
                search();
                break;
            case 4:
                remove();
                break;
            case 5:
                update();
                break;
            case 6:
                stats();
                break;
            case 7:
                cout << "\nExiting Program...\n";
                break;
            default:
                cout << "\nInvalid Choice! Please Try Again.\n";
            }

        } while (choice != 7);
    }
};

// ================= MAIN FUNCTION =================
int main() {
    CrimeSystem system;
    system.load();
    system.menu();
    return 0;
}