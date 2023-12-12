#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <regex>

using namespace std;

class Hostel;
class Person
{
public:
    string name;
    Person(string n) : name(n) {}
};

class Student : public Person
{
public:
    int rollNumber;
    string course;

    Student(string n, int r, string c) : Person(n), rollNumber(r), course(c) {}
};

class Room
{
public:
    static const int MAX_STUDENTS = 2;

    int roomNumber;
    vector<Student> students;

    Room(int number) : roomNumber(number) {}

    void addStudent(Student student)
    {
        if (students.size() < MAX_STUDENTS)
        {
            students.push_back(student);
            cout << student.name << " added to Room " << roomNumber << " successfully.\n";
        }
        else
        {
            cout << "Room " << roomNumber << " is full. Cannot add more students.\n";
        }
    }

    void removeStudent(int roomNumber, int rollNumber)
    {
        for (auto it = students.begin(); it != students.end(); ++it)
        {
            if (it->rollNumber == rollNumber)
            {
                cout << it->name << " removed from Room " << roomNumber << " successfully.\n";
                students.erase(it);
                return;
            }
        }
        cout << "Student with Roll Number " << rollNumber << " not found in Room " << roomNumber << ".\n";
    }
    /*
        void displayRoomDetails()
        {

            cout << "+-------+----------------------+--------------+-----------+" << endl;
            cout << "| Room  | Name                 | Roll Number  | Course    |" << endl;
            cout << "+-------+----------------------+--------------+-----------+" << endl;

            for (const auto &student : students)
            {
                cout << "| " << setw(6) << roomNumber;
                cout << " | " << setw(12) << student.name;
                cout << " | " << setw(13) << student.rollNumber;
                cout << " | " << setw(10) << student.course << " |" << endl;
            }

            cout << "+-------+----------------------+--------------+-----------+" << endl;
        }
    */
    int getNumStudents() const
    {
        return students.size();
    }
};

class Hostel
{
public:
    static const int TOTAL_ROOMS = 10; // Total number of rooms
    vector<Room> rooms;

public:
    void addRoom(int roomNumber)
    {
        if (rooms.size() < TOTAL_ROOMS && roomNumber >= 101 && roomNumber <= 110)
        {
            rooms.push_back(Room(roomNumber));
            cout << "Room " << roomNumber << " added successfully.\n";
        }
        else
        {
            cout << "Error: Invalid room number or maximum limit of rooms reached.\n";
        }
    }

    void addStudentToRoom(int roomNumber, const Student &student)
    {
        bool roomFound = false;
        for (auto &room : rooms)
        {
            if (room.roomNumber == roomNumber)
            {
                roomFound = true;
                // Check if room is full
                if (room.getNumStudents() >= Room::MAX_STUDENTS)
                {
                    cout << "Room " << roomNumber << " is full. Cannot add more students.\n";
                    return;
                }

                room.addStudent(student);

                return;
            }
        }

        if (!roomFound)
        {
            cout << "Room " << roomNumber << " not found.\n";
        }
    }

    void removeStudentFromRoom(int roomNumber, int rollnumber)
    {
        for (auto &room : rooms)
        {
            room.removeStudent(roomNumber, rollnumber);
        }
    }

    void displayRoomDetails(int roomNumber) const
    {
        for (const auto &room : rooms)
        {
            if (room.roomNumber == roomNumber)
            {
                cout << "Room Number: " << room.roomNumber << "\n";

                cout << setw(15) << "Name" << setw(15) << "Roll Number" << setw(15) << "Course\n";

                if (!room.students.empty())
                {
                
                    const auto &student = room.students[0];
                    cout << setw(15) << student.name << setw(15) << student.rollNumber << setw(15) << student.course << "\n";
                }
                else
                {
                    cout << "No students in this room.\n";
                }

                cout << "\n";
                return;
            }
        }
        cout << "Room " << roomNumber << " not found.\n";
    }

    void displayHostelDetails() const
    {
        cout << "Hostel Details:\n";
        cout << "===================================================\n";
        cout << "| Room  | Name                | Roll Number        |\n";
        cout << "---------------------------------------------------\n";

        for (const auto &room : rooms)
        {
            if (!room.students.empty())
            {
                const auto &student = room.students[0];
                cout << "| " << setw(4) << room.roomNumber << " | " << setw(10) << student.name << " | " << setw(15) << student.rollNumber << " |\n";
            }
            else
            {
                cout << "| " << setw(4) << room.roomNumber << " | " << setw(15) << "No student"
                     << " | " << setw(15) << "-"
                     << " |\n";
            }
        }
        cout << "===================================================\n";
    }

    void saveHostelData(const string &filename) const
    {
        ofstream file(filename);
        if (file.is_open())
        {
            for (const auto &room : rooms)
            {
                for (const auto &student : room.students)
                {
                    file << room.roomNumber << "," << student.name << "," << student.rollNumber << "," << student.course << "\n";
                }
            }
            cout << "Hostel data saved to " << filename << " successfully.\n";
        }
        else
        {
            cout << "Unable to open file: " << filename << "\n";
        }
    }

    void loadHostelData(const string &filename)
    {
        ifstream file(filename);
        if (file.is_open())
        {
            int roomNumber;
            string name, course;
            int rollNumber;

            while (file >> roomNumber >> name >> rollNumber >> course)
            {
                addStudentToRoom(roomNumber, Student(name, rollNumber, course));
            }

            cout << "Hostel data loaded from " << filename << " successfully.\n";
        }
    }

    int getAvailableSeats(int roomNumber) const
    {
        for (const auto &room : rooms)
        {
            if (room.roomNumber == roomNumber && room.getNumStudents() < Room::MAX_STUDENTS)
            {
                return Room::MAX_STUDENTS - room.getNumStudents();
            }
        }
        return 0;
    }

    void displayAvailableRooms() const
    {
        cout << "Available Rooms:\n";
        cout << "---------------------------------------\n";
        cout << "| Room |  Seats   | Availability       |\n";
        cout << "---------------------------------------\n";

        for (int roomNumber = 101; roomNumber <= 110; ++roomNumber)
        {
            // Check if room exists before checking availability
            bool roomExists = false;
            for (const auto &room : rooms)
            {
                if (room.roomNumber == roomNumber)
                {
                    roomExists = true;
                    break;
                }
            }

            if (!roomExists)
            {
                cout << "| " << roomNumber << " |     ---    | Not Added Yet |\n";
            }
            else
            {
                int availableSeats = getAvailableSeats(roomNumber);
                cout << "| " << roomNumber << " |  " << availableSeats << "          | ";
                if (availableSeats > 0)
                {
                    cout << "Available";
                }
                else
                {
                    cout << "Full";
                }
                cout << " |\n";
            }
        }
    }

    int getNumRooms() const
    {
        return TOTAL_ROOMS;
    }
    bool validateStudentName(const std::string &name)
    {
        const regex pattern(R"([a-zA-Z\s]+)");
        if (regex_match(name, pattern))
        {
            return true;
        }
        else
        {
            cout << "\nInvalid student name: '" << name << "'. Names should only contain letters and spaces.\n";
            return false;
        }
    }
    int getNumRooms()
    {
        return rooms.size();
    }
};

int main()
{
    Hostel hostel;

    hostel.loadHostelData("hostel_data.txt");

    while (true)
    {
        cout << "\n\n";
        cout << "***************************************\n";
        cout << "         Hostel Management System       \n\n";
        cout << "***************************************\n\n";

        // Display menu options
        cout << "1. Display Total Number of Rooms\n";
        cout << "2. Display Available Rooms\n";
        cout << "3. Add Room\n";
        cout << "4. Add Student to Room\n";
        cout << "5. Remove Student from Room\n";
        cout << "6. Display Room Details\n";
        cout << "7. Display Hostel Details\n";
        cout << "8. Save Hostel Data\n";
        cout << "9. Exit\n";
        cout << "Choose an option: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << endl;
            cout << "Total Number of Rooms: " << hostel.getNumRooms() << endl;
            cout << "Rooms are numbered from  101 to 110. " << endl;
            break;

        case 2:
            hostel.displayAvailableRooms();
            break;

        case 3:
        {
            int roomNumber;
            cout << "Enter Room Number (between 101 and 110): ";
            cin >> roomNumber;
            bool roomExists = false;
            for (const auto &room : hostel.rooms)
            {
                if (roomNumber < 101 || roomNumber > 110)
                {
                    cout << "Invalid room number. Room number should be between 101 and 110.\n";
                    break;
                }
            }
            for (const auto &room : hostel.rooms)
            {
                if (room.roomNumber == roomNumber)
                {
                    roomExists = true;
                    break;
                }
            }

            if (roomExists)
            {
                cout << "Room " << roomNumber << " is already added !!!.\n";
                break;
            }
            hostel.addRoom(roomNumber);
            break;
        }

        case 4:
        {
            if (hostel.rooms.empty())
            {
                cout << "No rooms exist in the hostel. Please add a room first.\n";
                cout << "Would you like to add a room now? (y/n): ";
                char choice;
                cin >> choice;
                if (choice == 'y')
                {
                    cout << "Choose option 3";
                }
                break;
            }
            int roomNumber;
            cout << "Enter Room Number: ";
            cin >> roomNumber;

            bool roomExists = false;
            for (const auto &room : hostel.rooms)
            {
                if (room.roomNumber == roomNumber)
                {
                    roomExists = true;
                    break;
                }
            }
            if (!roomExists)
            {
                cout << "Room " << roomNumber << " you need to add first!!!\n";
                break;
            }

            string name, course;
            int rollNumber;

            cout << "Please enter validate student name : ";
            cin.ignore();
            getline(cin, name);

            bool a = hostel.validateStudentName(name);
            if (!a)
            {
                cout << "Enter Student Name: ";
                cin.ignore();
                getline(cin, name);
            }

            cout << "Enter Roll number: ";
            cin >> rollNumber;
            cout << "Enter Course: ";
            cin.ignore();
            getline(cin, course);

            hostel.addStudentToRoom(roomNumber, Student(name, rollNumber, course));
            break;
        }

        case 5:
        {
            if (hostel.rooms.empty())
            {
                cout << "\nNo rooms exist in the hostel to remove the student";
            }

            int roomnumber, rollnumber;
            cout << "Enter Room number :";
            cin >> roomnumber;
            if (roomnumber < 101 || roomnumber > 110)
            {
                cout << "Invalid room number. Room number should be between 101 and 110.\n";
                break;
            }
            cout << "Enter Roll number :";
            cin >> rollnumber;

            hostel.removeStudentFromRoom(roomnumber, rollnumber);
            break;
        }

        case 6:
        {
            if (hostel.rooms.empty())
            {
                cout << "No rooms exist in the hostel. Please add a room first.\n";
                cout << "Would you like to add a room now? (y/n): ";
                char choice;
                cin >> choice;
                if (choice == 'y')
                {
                    cout << "Choose option 3";
                }
                break;
            }
            int roomNumber;
            cout << "Enter Room Number: ";
            cin >> roomNumber;
            hostel.displayRoomDetails(roomNumber);
            break;
        }

        case 7:
            hostel.displayHostelDetails();
            break;

        case 8:
            hostel.saveHostelData("hostel_data.txt");
            break;

        case 9:
            cout << "Exiting the Hostel Management System.\n";
            return 0;

        default:
            cout << "Invalid option. Please try again.\n";

            return 0;
        }
    }
}
