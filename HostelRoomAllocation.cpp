#include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <iterator>
#include <fstream>
#include <sstream>

using namespace std;

class Student
{
public :
  string rollNo, roomNo, name, gender, department, year, address, password;
  bool isLoggedIn;
};

string studentIndex[8] = {"ROLLNO", "NAME", "GENDER", "DEPARTMENT", "YEAR", "ADDRESS", "PASSWORD", "ROOMNO"};
string hostelIndex[4] = {"ROOMNO", "GENDER", "YEAR", "AVAILABLE_SLOTS"};
/*
Since reading the file each time and updating can be difficult, all the data has been
loaded in memory.
*/
std::unordered_map< std::string, Student> overAllStudentDatabase;
std::unordered_map< string, set< pair <string, int> > > maleYearWiseAvailableRooms;
std::unordered_map< string, set< pair <string, int> > > femaleYearWiseAvailableRooms;
string FIRST_YEAR = "I";
string SECOND_YEAR = "II";
string THIRD_YEAR = "III";
string FOURTH_YEAR = "IV";
string MALE = "M";
string FEMALE = "F";
string genderArr[] = {MALE, FEMALE};
string yearArr[] = {FIRST_YEAR, SECOND_YEAR, THIRD_YEAR, FOURTH_YEAR};
string NOT_ALLOCATED = "NA";
string delimiter = "#";

void loadStudentData() {
    ifstream fin;
    string data;
    fin.open("student.txt");
    while(fin) {
        getline(fin, data);
        if (data == "") {
            break;
        }
        int position = 0;
        int iteration = 0;
        string rollno;
        string field;
        Student student;
        while (position != -1) {
            position = data.find(delimiter);
            field = data.substr(0, position);
            if (iteration == 0) {
                rollno = field;
                student.rollNo = field;
            } else if (iteration == 1) {
                student.name = field;
            } else if (iteration == 2) {
                student.gender = field;
            } else if (iteration == 3) {
                student.department = field;
            } else if (iteration == 4) {
                student.year = field;
            } else if (iteration == 5) {
                student.address = field;
            } else if (iteration == 6) {
                student.password = field;
            } else {
                student.roomNo = field;
            }
            data = data.substr(position+1);
            iteration++;
        }
        overAllStudentDatabase.insert(make_pair(rollno, student));
    }
    cout << "Student data loaded successfully!" << endl;
}

set< pair <string, int> > getAvailableRooms(string gender, string year) {
    if (gender == MALE) {
        return maleYearWiseAvailableRooms[year];
    } else {
        return femaleYearWiseAvailableRooms[year];
    }
}

void setAvailableRooms(string gender, string year, set< pair <string, int> > availableRooms) {
    if (gender == MALE) {
        maleYearWiseAvailableRooms[year] = availableRooms;
    } else {
        femaleYearWiseAvailableRooms[year] = availableRooms;
    }
}

int getIntValue(string data) {
    stringstream sstream(data);
    int intValue;
    sstream >> intValue;
    return intValue;
}

string getStringValue(int data) {
    stringstream sstream;
    sstream << data;
    return sstream.str();
}

string getDataInUpperCase(string data) {
    for(unsigned int i = 0; i < data.size(); i++) {
        data.at(i) = toupper(data.at(i));
    }
    return data;
}

bool isElementPresentInArray(string dataArr[], int arrSize, string option) {
    for(int i = 0; i < arrSize; i++) {
        if (dataArr[i].compare(option) == 0) {
            return true;
        }
    }
    return false;
}

void loadAvailableRoomData() {
    ifstream fin;
    string data;
    fin.open("availableRooms.txt");
    while(fin) {
        getline(fin, data);
        if (data == "") {
            break;
        }
        int position = 0;
        int iteration = 0;
        string field;
        std::unordered_map<std::string, std::string> roomData;
        while (position != -1) {
            position = data.find(delimiter);
            field = data.substr(0, position);
            roomData.insert(make_pair(hostelIndex[iteration], field));
            data = data.substr(position+1);
            iteration++;
        }
        set<string> availableRooms;
        int availableSlots = getIntValue(roomData["AVAILABLE_SLOTS"]);
        if (roomData["GENDER"] == MALE) {
            maleYearWiseAvailableRooms[roomData["YEAR"]].insert(make_pair(roomData["ROOMNO"], availableSlots));
        } else {
            femaleYearWiseAvailableRooms[roomData["YEAR"]].insert(make_pair(roomData["ROOMNO"], availableSlots));
        }
    }
    cout << "Hostel data loaded successfully!" << endl;
}

// Method to display details of a student
Student getStudentDetails() {
    cout << "Enter student's rollno";
    string rollno;
    cin >> rollno;
    return overAllStudentDatabase[rollno];
}

void displayStudentDetails(string rollNo) {
    if (overAllStudentDatabase.find(rollNo) != overAllStudentDatabase.end()) {
        Student studentRecord = overAllStudentDatabase[rollNo];
        cout << "ROLLNO\t - \t" << studentRecord.rollNo << endl;
        cout << "NAME\t - \t" << studentRecord.name << endl;
        cout << "GENDER\t - \t" << studentRecord.gender << endl;
        cout << "DEPT\t - \t" << studentRecord.department << endl;
        cout << "YEAR\t - \t" << studentRecord.year << endl;
        cout << "ADDRESS\t - \t" << studentRecord.address << endl;
        cout << "ROOMNO\t - \t" << studentRecord.roomNo << endl;
    }
}

// Method used to allocate rooms to student based on GENDER and YEAR.
string allocateRoom(Student studentDetails) {
    string gender = studentDetails.gender;
    string year = studentDetails.year;
    set< pair <string, int> > availableRooms = getAvailableRooms(gender, year);
    if (!availableRooms.empty()) {
        cout << "Available rooms are : " << endl;
        set< pair <string,int> >::iterator it;
        for (it = availableRooms.begin(); it != availableRooms.end(); ++it) {
            cout << it->first  << endl;
        }
        cout << "Enter your option" << endl;
        string option;
        cin >> option;
        for (it = availableRooms.begin(); it != availableRooms.end(); ++it) {
            if (it->first == option) {
                return option;
            }
        }
        cout << option << " is not available!" << endl;
    } else {
        cout << "No rooms are available" << endl;
    }
    return NOT_ALLOCATED;
}

void showMainMenu() {
    cout << "Please select an action to continue" << endl;
    cout << "1 - REGISTER" << endl;
    cout << "2 - LOGIN" << endl;
    cout << "3 - LOGOUT" << endl;
    cout << "4 - FIND HOSTEL ROOM" << endl;
    cout << "5 - EDIT HOSTEL ROOM" << endl;
    cout << "6 - FIND MY DETAILS" << endl;
    cout << "7 - EXIT" << endl;
}

string getUserDetailsRow(Student student) {
    string studentData = "";
    studentData += student.rollNo + delimiter;
    studentData += student.name + delimiter;
    studentData += student.gender + delimiter;
    studentData += student.department + delimiter;
    studentData += student.year + delimiter;
    studentData += student.address + delimiter;
    studentData += student.password + delimiter;
    studentData += student.roomNo + "\n";
    return studentData;
}

string getRoomDetailsRow(string gender, string year, string roomNo, string availableSlots) {
    string roomData = "";
    roomData += roomNo + delimiter;
    roomData += gender + delimiter;
    roomData += year + delimiter;
    roomData += availableSlots + "\n";
    return roomData;
}

void writeChangetoFiles() {
    ofstream fout;
    fout.open("student.txt");
    for (unordered_map<string, Student>::iterator it=overAllStudentDatabase.begin(); it!=overAllStudentDatabase.end(); ++it) {
        fout << getUserDetailsRow(it->second);
    }
    fout.close();
    fout.open("availableRooms.txt");
    set< pair <string,int> >::iterator pairData;
    for (unordered_map< string, set< pair <string, int> > >::iterator it=maleYearWiseAvailableRooms.begin(); it!=maleYearWiseAvailableRooms.end(); ++it) {
        for (pairData = it->second.begin(); pairData != it->second.end(); ++pairData) {
            fout << getRoomDetailsRow(MALE, it->first, pairData->first, getStringValue(pairData->second));
        }
    }
    for (unordered_map< string, set< pair <string, int> > >::iterator it=femaleYearWiseAvailableRooms.begin(); it!=femaleYearWiseAvailableRooms.end(); ++it) {
        for (pairData = it->second.begin(); pairData != it->second.end(); ++pairData) {
            fout << getRoomDetailsRow(FEMALE, it->first, pairData->first, getStringValue(pairData->second));
        }
    }
    fout.close();
}

void registerStudent() {
    string input;
    string studentData = "";
    Student newStudent;
    string rollNo;
    int iteration;
    for (iteration = 0 ; iteration < 7;) {
        cout << "Enter your " + studentIndex[iteration] << " : " << endl;
        getline(cin, input);
        if (input == "") {
            cout << "Please check the data you entered!" << endl;
            continue;
        }
        if (iteration == 2) {
            input = getDataInUpperCase(input);
            if (!isElementPresentInArray(genderArr, 2, input)) {
                cout << "Gender can be only M / F" << endl;
                continue;
            }
        }
        if (iteration == 4) {
            input = getDataInUpperCase(input);
            if (!isElementPresentInArray(yearArr, 4, input)) {
                cout << "Year of study can only be I / II / III / IV" << endl;
                continue;
            }
        }
        if (iteration == 0 && overAllStudentDatabase.find(input) != overAllStudentDatabase.end()) {
            cout << "User already exists!" << endl;
            return;
        }
        if (iteration > 0) {
            studentData = studentData + delimiter;
        }
        studentData = studentData + input;
        if (iteration == 0) {
            rollNo = input;
            newStudent.rollNo = input;
        } else if (iteration == 1) {
            newStudent.name = input;
        } else if (iteration == 2) {
            newStudent.gender = input;
        } else if (iteration == 3) {
            newStudent.department = input;
        } else if (iteration == 4) {
            newStudent.year = input;
        } else if (iteration == 5) {
            newStudent.address = input;
        } else if (iteration == 6) {
            newStudent.password = input;
        }
        iteration++;
    }
    newStudent.roomNo = NOT_ALLOCATED;
    studentData = studentData + delimiter + NOT_ALLOCATED;
    overAllStudentDatabase[rollNo] = newStudent;
    cout << "You have entered.." << endl;
    displayStudentDetails(rollNo);
    writeChangetoFiles();
    cout << "Registration successful! Please login to continue." << endl;
}

bool login(string rollNo) {
    cout << "Enter password for " + rollNo + " : " << endl;
    string password;
    cin >> password;
    if (overAllStudentDatabase[rollNo].password.compare(password) == 0) {
        overAllStudentDatabase[rollNo].isLoggedIn = true;
    } else {
        cout << "User name / password is incorrect! Please try later!" << endl;
        return false;
    }
    cout << "Login successful!" << endl;
    return true;
}

void logout(string rollNo) {
    overAllStudentDatabase[rollNo].isLoggedIn = false;
    cout << "User successfully logged out!" << endl;
}

bool isUserLoggedIn(string rollNo) {
    if (rollNo != "" && overAllStudentDatabase[rollNo].isLoggedIn) {
        return true;
    }
    return false;
}

void modifyAvailableRoomDetails(string gender, string year, string roomNo) {
    // Once a room is selected, it should be removed from available list
    set< pair <string,int> > availableRooms = getAvailableRooms(gender, year);
    set< pair <string,int> >::iterator it;
    for (it = availableRooms.begin(); it != availableRooms.end(); ++it) {
        if (it->first == roomNo) {
            int availableSlots = it->second;
            if (availableSlots == 1) {
                /*
                    Only one vacant slot was available. Since it is
                    allocated for particular student, remove the roomno
                    from available list.
                */
                availableRooms.erase(make_pair(it->first, it->second));
            } else {
                /*
                    Roomno is allocated for student. So reduce the number
                    of available slots for the particular room.
                */
                availableSlots--;
                availableRooms.erase(it);
                availableRooms.insert(make_pair(it->first, availableSlots));
            }
            break;
        }
    }
    setAvailableRooms(gender, year, availableRooms);
}

int main() {
    loadStudentData();
    loadAvailableRoomData();
    Student currStudent;
    cout << "WELCOME TO STUDENT PORTAL" << endl;
    int choice;
    string rollNo;
    while(true) {
        showMainMenu();
        cout << "Enter your choice" << endl;
        cin >> choice;
        switch(choice) {
        case 1:
            rollNo = "";
            registerStudent();
            break;
        case 2:
            cout << "Enter your roll-no :" << endl;
            cin >> rollNo;
            if (login(rollNo)) {
                currStudent = overAllStudentDatabase[rollNo];
            }
            break;
        case 3:
            if (isUserLoggedIn(rollNo)) {
                logout(rollNo);
            } else {
                cout << "You need to login first to perform this action" << endl;
            }
            break;
        case 4:
            if (isUserLoggedIn(rollNo)) {
                if (currStudent.roomNo.compare(NOT_ALLOCATED) != 0) {
                    cout << "Room no " << currStudent.roomNo << " is already allocated for you!" << endl;
                    break;
                }
                string roomNo = allocateRoom(currStudent);
                if (roomNo.compare(NOT_ALLOCATED) == 0) {
                    cout << "Please contact executive warden!";
                } else {
                    cout << "Allocated  room no - " << roomNo << endl;
                    // Update roomNo for student
                    currStudent.roomNo = roomNo;
                    overAllStudentDatabase[currStudent.rollNo]= currStudent;
                    modifyAvailableRoomDetails(currStudent.gender, currStudent.year, roomNo);
                    writeChangetoFiles();
                }
            } else {
                cout << "You need to login first to perform this action" << endl;
            }
            break;
        case 5:
            if (isUserLoggedIn(rollNo)) {
                if (currStudent.roomNo.compare(NOT_ALLOCATED) == 0) {
                    cout << "No room is allocated to you! Please allocate a room before you try to edit" << endl;
                    break;
                } else {
                    string existingRoomNo = currStudent.roomNo;
                    string option;
                    cout << "Do you wish to update your room? Type yes / no" << endl;
                    bool exit = false;
                    while(true) {
                        cin >> option;
                        option = getDataInUpperCase(option);
                        if (option.compare("NO") == 0) {
                            exit = true;
                            break;
                        } else if (option.compare("YES") != 0) {
                            cout << "Please type yes / no" << endl;
                            continue;
                        } else {
                            break;
                        }
                    }
                    if (exit) {
                        break;
                    }
                    string roomNo = allocateRoom(currStudent);
                    if (roomNo.compare(NOT_ALLOCATED) == 0) {
                        cout << "Requested room no is not available!" << endl;
                        break;
                    } else if (roomNo.compare(existingRoomNo) == 0) {
                        cout << "Requested room no is same as existing room no!" << endl;
                        break;
                    } else {
                        cout << "Updated  room no - " << roomNo << endl;
                        // Update roomNo for student
                        currStudent.roomNo = roomNo;
                        overAllStudentDatabase[currStudent.rollNo]= currStudent;
                        modifyAvailableRoomDetails(currStudent.gender, currStudent.year, roomNo);
                        /*
                            Since a room no is freed, it should be added back to available rooms
                        */
                        bool isExistingRoomNoPresent = false;
                        set< pair <string,int> > availableRooms = getAvailableRooms(currStudent.gender, currStudent.year);
                        set< pair <string,int> >::iterator it;
                        for (it = availableRooms.begin(); it != availableRooms.end(); ++it) {
                            if (it->first == existingRoomNo) {
                                /*
                                    There is already spaces left in the existing room.
                                    So simply increase available space by 1.
                                */
                                int availableSlots = it->second;
                                availableSlots++;
                                availableRooms.erase(it);
                                availableRooms.insert(make_pair(it->first, availableSlots));
                                isExistingRoomNoPresent = true;
                                break;
                            }
                        }
                        cout << "Status - " << isExistingRoomNoPresent << "\t" << existingRoomNo << endl;
                        if (!isExistingRoomNoPresent) {
                            /*
                                Existing room is not present in the available database at all.
                                So add a new row with available spaces as 1.
                            */
                            cout << "Inserting new row " << existingRoomNo << endl;
                            availableRooms.insert(make_pair(existingRoomNo, 1));
                        }
                        setAvailableRooms(currStudent.gender, currStudent.year, availableRooms);
                        writeChangetoFiles();
                    }
                }
            } else {
                cout << "You need to login first to perform this action" << endl;
            }
            break;
        case 6:
            if (isUserLoggedIn(rollNo)) {
                displayStudentDetails(rollNo);
            } else {
                cout << "You need to login first to perform this action" << endl;
            }
            break;
        case 7:
            return 0;
        }
    }
	return 0;
}
