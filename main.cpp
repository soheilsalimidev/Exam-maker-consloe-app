#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <ctime>

using namespace std;

struct person {
    string id;
    string userName;
    string role;
};

struct questionsStr {
    string question;
    string op1;
    string op2;
    string op3;
    string op4;
    int correctAns{};
};

person User;


int Now();

void searchData(const string &userName, const string &password);

void addNewUser(const string &role);

void adminActions();

void studentAction();

void ProfessorAction();

void takeExam();

double calPerson(int correct, int wrong, int q);

void examResult();

void newExam();

void login();

questionsStr splitQuestion(const string &str);

void examList(const string &role = "pro");

int main() {
    login();
    system("pause");
    return 0;
}

void login() {
    string username, password;
    cout << "inter your username: \n";
    cin >> username;
    cout << "inter your password: \n";
    cin >> password;
    system("cls");
    if (username == "admin" & password == "admin") adminActions();
    else searchData(username, password);
}

void searchData(const string &userName, const string &password) {
    string lines;
    ifstream DataBase("DataBaseUsers.txt");
    if (DataBase.is_open()) {
        bool userFind = false;
        while (getline(DataBase, lines)) {
            stringstream ss(lines);
            string word;
            bool isPass = false, isUser = false;
            int i = 0;
            while (ss >> word) {
                if (i == 0) {
                    if (userName == word) isUser = true;
                } else if (i == 1) {
                    if (password == word) isPass = true;
                } else break;
                i++;
            }
            if (isUser && isPass) {
                i = 0;
                stringstream s(lines);
                while (s >> word) {
                    if (i == 0) User.userName = word;
                    else if (i == 2) User.id = word;
                    else if (i == 3) User.role = word;
                    i++;
                }
                userFind = true;
                if (User.role == "stu") studentAction();
                else ProfessorAction();
                break;
            }
        }
        if (!userFind) {
            string again;
            cout << "User not found!\n" << "Do you want to try again?";
            cin >> again;
            if (again == "y" || again == "yes") {
                system("cls");
                login();
            }
        }
        DataBase.close();
    } else cout << "Unable to open file";
}

void adminActions() {
    cout << "========= [Welcome admin...] =========\n\n";
    cout << "type 1 if you want to add new Professor \ntype 2 if you want to add new student \ntype 3 to exit\n";
    int command;
    cin >> command;
    if (command == 1) addNewUser("pro");
    else if (command == 2)addNewUser("stu");
    else {
        cout << "Have Nice Day :) \n";
        system("pause");
        exit(0);
    }
}

void studentAction() {
    cout << "========= [Welcome student " << User.userName << "] =========\n\n";
    cout << "type 1 if you want to see list of exams \ntype 2 if you want to take exam \ntype 3 to exit\n";
    int command;
    cin >> command;
    if (command == 1) examList("stu");
    else if (command == 2) takeExam();
    else {
        cout << "Have Nice Day :) \n";
        system("pause");
        exit(0);
    }
}

void ProfessorAction() {
    cout << "========= [Welcome Professor " + User.userName + "] =========\n\n";
    cout << "type 1 if you want to add new exam"
            "\ntype 2 if you want to exam lists"
            "\ntype 3 if you want to see exam results"
            "\ntype 4 to exit\n";
    int command;
    cin >> command;
    if (command == 1) newExam();
    else if (command == 2)examList();
    else if (command == 3) examResult();
    else {
        cout << "Have Nice Day :) \n";
        system("pause");
        exit(0);
    }
}

void examList(const string &role) {
    string lines;
    ifstream ExamDataBase("exams.txt");
    system("cls");
    cout << "-------------------------------------------------------------------------------\n";
    if (ExamDataBase.is_open()) {
        while (getline(ExamDataBase, lines)) {
            string q;
            ifstream QDataBase(("QDataBase" + lines + ".txt"));
            string qNum, time, startDate, EndDate;
            if (QDataBase.is_open()) {
                while (getline(QDataBase, q)) {
                    stringstream ss(q);
                    string word;
                    int i = 0;
                    while (ss >> word) {
                        if (i == 0) qNum = word;
                        else if (i == 1) time = word;
                        else if (i == 2) startDate = word;
                        else EndDate = word;
                        i++;
                    }
                    break;
                }
                cout << "    exam number " << stoi(lines) << " with " << qNum << " question" << " and " << time
                     << " min time start at " << startDate << " and edn at " << EndDate << "\n";
            }
        }
        ExamDataBase.close();
    }
    cout << "-------------------------------------------------------------------------------\n";

    if (role == "pro") ProfessorAction();
    else studentAction();
}

void newExam() {
    string lines;
    ifstream ExamDataBase("exams.txt");
    string ExamId = "0";
    if (ExamDataBase.is_open()) {
        while (getline(ExamDataBase, lines))ExamId = lines;
        ExamDataBase.close();
    }
    int questionsNu, time;
    string startDate, endDate;
    cout << "How many questions ?";
    cin >> questionsNu;
    cout << "when this exam must start ?";
    cin >> startDate;
    cout << "when this exam must end ?";
    cin >> endDate;
    cout << "How long does it take ?";
    cin >> time;

    ofstream QDataBase(("QDataBase" + to_string(stoi(ExamId) + 1) + ".txt"), ios_base::app | ios_base::out);

    system("cls");
    if (QDataBase.is_open()) {
        QDataBase << questionsNu << " " << time << " " << startDate << " " << endDate << "\n";
        for (int i = 0; i < questionsNu; ++i) {
            system("cls");
            string q, op1, op2, op3, op4;
            int c;
            cout << "what is the question " << (i + 1) << "\n";
            cin.ignore();
            getline(cin, q);
            cout << "what is option 1 ?";
            getline(cin, op1);
            cout << "what is option 2 ?";
            getline(cin, op2);
            cout << "what is option 3 ?";
            getline(cin, op3);
            cout << "what is option 4 ?";
            getline(cin, op4);
            cout << "what is correct ans?";
            cin >> c;
            QDataBase << q << "," << op1 << "," << op2 << "," << op3 << "," << op4 << "," << c << "\n";
        }
    } else cout << "Unable to open file";
    QDataBase.close();

    ofstream DataBase("exams.txt", ios_base::app | ios_base::out);
    if (DataBase.is_open())
        DataBase << (stoi(ExamId) + 1) << "\n";
    else cout << "Unable to open file";
    DataBase.close();

    system("cls");
    cout << "Exam Added successfully\n";
    ProfessorAction();

}

void examResult() {
    int examId;
    cout << "which Exam do you want to see the result ? \n";
    cin >> examId;
    string lines;
    ifstream ExamDataBase(("AnsDataBaseExam" + to_string(examId) + ".txt"));
    system("cls");
    cout << "-------------------------------------------------------------------------------\n";
    if (ExamDataBase.is_open())
        while (getline(ExamDataBase, lines)) cout << lines << "%\n";
    cout << "-------------------------------------------------------------------------------\n";
    ProfessorAction();
}

void takeExam() {
    int examId;
    cout << "which Exam do you want to take ? \n";
    cin >> examId;
    string lines;
    ifstream ExamDataBase(("QDataBase" + to_string(examId) + ".txt"));
    system("cls");
    string qNum, time, startDate, EndDate;

    int ansCorrect = 0;
    int ansWrong = 0;

    while (getline(ExamDataBase, lines)) {
        stringstream ss(lines);
        string word;
        int i1 = 0;
        while (ss >> word) {
            if (i1 == 0) qNum = word;
            else if (i1 == 1) time = word;
            else if (i1 == 2) startDate = word;
            else EndDate = word;
            i1++;
        }
        break;
    }

    startDate.erase(remove(startDate.begin(), startDate.end(), '/'), startDate.end());
    EndDate.erase(remove(EndDate.begin(), EndDate.end(), '/'), EndDate.end());

    if (stoi(startDate) > Now())
        cout << "Exam Time hasn't come yet! :) \n";
    else if (stoi(EndDate) <= Now())
        cout << "Exam Time has finished sorry :( \n";
    else {

        chrono::steady_clock::time_point tend = std::chrono::steady_clock::now()
                                                + std::chrono::minutes(stoi(time));
        chrono::steady_clock::time_point warning = std::chrono::steady_clock::now()
                                                   + std::chrono::minutes(stoi(time) - 1);
        while (chrono::steady_clock::now() < tend) {
            if (chrono::steady_clock::now() > warning)
                cout << "time running out! you have one minute left!";
            if (ExamDataBase.is_open()) {
                int i = 1;
                while (getline(ExamDataBase, lines)) {
                        system("cls");
                        int qAns;
                        cout << i << "/" << qNum << "\n";
                        questionsStr qus = splitQuestion(lines);
                        cout << qus.question << "\n1. " << qus.op1 << "\n2." << qus.op2 <<
                             "\n3. " << qus.op3 << "\n4. " << qus.op4 << "\n";
                        cin >> qAns;
                        if (qAns == qus.correctAns)
                            ansCorrect++;
                        else ansWrong++;
                    i++;
                }
                break;
            }
        }

        system("cls");

        ofstream DataBase(("AnsDataBaseExam" + to_string(examId) + ".txt"), ios_base::app | ios_base::out);
        if (DataBase.is_open())
            DataBase << User.userName << " has " << ansCorrect << " right answer and " << ansWrong
                     << " wrong answer and user grade is "
                     << calPerson(ansCorrect, ansWrong, stoi(qNum)) << "\n";
        else cout << "Unable to open file";
        cout << "-------------------------------------------------------------------------------\n";
        cout << "you have " << ansCorrect << " right answer and " << ansWrong << " wrong answer and your grade is "
             << calPerson(ansCorrect, ansWrong, stoi(qNum)) << "%\nGood Luck :)\n";
        cout << "-------------------------------------------------------------------------------\n";
    }

    studentAction();
}

void addNewUser(const string &role) {
    string username, password, Id;
    cout << "inter user username: \n";
    cin >> username;
    cout << "inter user password: \n";
    cin >> password;
    cout << "inter user Id: \n";
    cin >> Id;

    ofstream DataBase("DataBaseUsers.txt", ios_base::app | ios_base::out);
    if (DataBase.is_open()) {
        DataBase << username + " " + password + " " + Id + " " + role + " \n";
        DataBase.close();
    } else cout << "Unable to open file";
    system("cls");
    cout << "user added \n";
    adminActions();
}

questionsStr splitQuestion(const string &str) {
    stringstream ss(str);
    questionsStr qus;

    int i = 0;
    while (ss.good()) {
        string line;
        getline(ss, line, ',');
        switch (i) {
            case 0:
                qus.question = line;
                break;
            case 1:
                qus.op1 = line;
                break;
            case 2:
                qus.op2 = line;
                break;
            case 3:
                qus.op3 = line;
                break;
            case 4:
                qus.op4 = line;
                break;
            case 5:
                qus.correctAns = stoi(line);
                break;
            default:
                break;
        }
        i++;
    }
    return qus;
}

double calPerson(int correct, int wrong, int q) {
    return ((((double) correct * 3) - (double) wrong) / ((double) q * 3)) * 100;
}

int Now() {
    time_t now = time(0);
    struct tm tstruct{};
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);
    return stoi(buf);
}
