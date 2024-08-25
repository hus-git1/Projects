#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<vector>
using namespace std;

class Food;
class Person;
class Patient;
class Admin;
class Doctor;

int sug_range[2] = { 70,100 };
int BPS_range[2] = { 90,120 };
int BPD_range[2] = { 60,80 };
int chol_range[2] = { 50,100 };
int pulse_range[2] = { 60,100 };
int* Hranges[5] = { sug_range,BPS_range,BPD_range,chol_range,pulse_range };

class Food {
    string name;
    int calories;
    double percent_fat;
    double percent_carbs;
    double percent_protein;
    double percent_fiber;
public:
    Food() {}
    string Getname()const { return name; }
    int Getcalories()const { return calories; }
    double Getpercent_fat()const { return percent_fat; }
    double Getpercent_carbs()const { return percent_carbs; }
    double Getpercent_protein()const { return percent_protein; }
    double Getpercent_fiber()const { return percent_fiber; }
};

class Person {
protected:
    string name;
    int age = 0;
    string gender;
public:
    Person() {
    }
    Person(string name, int age, string gender) {
        this->name = name;
        this->age = age;
        this->gender = gender;
    }
    string getname()const { return name; }
    int getage()const { return age; }
    string getgender()const { return gender; }
    virtual void Input() = 0;
    friend istream& operator>>(istream& input, Person& P);
    friend ostream& operator<<(ostream& output, Person& P);
};

class Doctor : public Person {
    string specialization;
    vector<Patient> patients;
public:
    Doctor(){}
    Doctor(string name, int age, string gender, string specialization);
    void Input() {
        cin >> *this;
        cout << "Specialization: " << endl; cin >> specialization;
    }
    string getspecialization()const { return specialization; }
    void SetByExercise(int minutes, Patient& P);
    void SetByFood(string food_name, Patient& P);
    void operator+(Patient& P);
    friend istream& operator>>(istream& input, Person& P);
    friend ostream& operator<<(ostream& output, Person& P);
};

class Patient :public Person {
protected:
    int sugar = 0;
    int sys_BP = 0;
    int dias_BP = 0;
    int chol = 0;
    int pulse = 0;
public:
    Doctor doc;
    Patient() {}
    Patient(string name, int age, string gender, int sugar, int sys_BP, int dias_BP, int chol, int pulse) :Person(name, age, gender) {
        this->sugar = sugar;
        this->sys_BP = sys_BP;
        this->dias_BP = dias_BP;
        this->chol = chol;
        this->pulse = pulse;
    }
    void Input() {
        cin >> *this;
        InputSugar();
        InputBP();
        InputChol();
        InputPulse();
    }
    void InputSugar() {
        string unit;
        cout << "Please enter your sugar level with the appropiate units: " << endl;
        cin >> sugar >> unit;
        if (unit == "mmol/L") {
            sugar = sugar * 18;
        }
    }
    void InputChol() {
        cout << "Please enter your cholesterol level (units not required): " << endl;
        cin >> chol;
    }
    void InputPulse() {
        cout << "Please enter your pulse rate (units not required): " << endl;
        cin >> pulse;
    }
    void InputBP() {
        cout << "Please enter your blood pressure in the form \"X Y\", where X is the systolic(higher) reading and Y is the diastolic(lower) reading: " << endl;
        cin >> sys_BP >> dias_BP;
    }
    void display() {
        cout << *this;
        cout << "Sugar: " << sugar << endl;
        cout << "BP: " << sys_BP << "/" << dias_BP << endl;
        cout << "Pulse: " << pulse << endl;
        cout << "Cholesterol: " << chol << endl;
    }
    int Getsugar()const { return sugar; }
    int Getsys_BP()const { return sys_BP; }
    int Getdias_BP()const { return dias_BP; }
    int Getchol()const { return chol; }
    int Getpulse()const { return pulse; }
    Patient& setsugar(double sugar) {
        this->sugar = sugar;
        return *this;
    }
    Patient& setsys_BP(double sys_BP) {
        this->sys_BP = sys_BP;
        return *this;
    }
    Patient& setdias_BP(double dias_BP) {
        this->dias_BP = dias_BP;
        return *this;
    }
    Patient& setchol(double chol) {
        this->chol = chol;
        return *this;
    }
    Patient& setpulse(double pulse) {
        this->pulse = pulse;
        return *this;
    }
    using func_ptr = int (Patient::*)()const;
    func_ptr Get[5] = {
        &Patient::Getsugar,
        &Patient::Getsys_BP,
        &Patient::Getdias_BP,
        &Patient::Getchol,
        &Patient::Getpulse
    };
    void AssignDoctor(int health_levels[], Doctor docs[]) {
        int diff[5], max_diff = 0;//stores difference between patient's value for each health factor and borderline value for that factor
        if (health_levels[0] == -1) {
            diff[0] = sug_range[0] - sugar;
        }
        else if (health_levels[0] == 1) {
            diff[0] = sugar - sug_range[1];
        }
        else diff[0] = 0;
        if (health_levels[1] == -1) {
            diff[1] = BPS_range[0] - sys_BP;
        }
        else if (health_levels[1] == 1) {
            diff[1] = sys_BP - BPS_range[1];
        }
        else diff[1] = 0;
        if (health_levels[2] == -1) {
            diff[2] = BPD_range[0] - dias_BP;
        }
        else if (health_levels[2] == 1) {
            diff[2] = dias_BP - BPD_range[1];
        }
        else diff[2] = 0;
        if (health_levels[3] == -1) {
            diff[3] = chol_range[0] - chol;
        }
        else if (health_levels[3] == 1) {
            diff[3] = chol - chol_range[1];
        }
        else diff[3] = 0;
        if (health_levels[4] == -1) {
            diff[4] = pulse_range[0] - pulse;
        }
        else if (health_levels[4] == 1) {
            diff[4] = pulse - pulse_range[1];
        }
        else diff[4] = 0;
        for (int i = 0; i < 5; ++i) {
            if (diff[i] > max_diff) {
                max_diff = diff[i];
            }
        }
        if (max_diff == diff[0]) {
            for (int i = 0; i < 4; ++i) {
                if (docs[i].getspecialization() == "Sugar level") {
                    docs[i] + *this;
                    doc = docs[i];
                }
            }
        }
        else if (max_diff == diff[1] || max_diff == diff[2]) {
            for (int i = 0; i < 4; ++i) {
                if (docs[i].getspecialization() == "Blood pressure") {
                    docs[i] + *this;
                    doc = docs[i];
                }
            }
        }
        else if (max_diff == diff[3]) {
            for (int i = 0; i < 4; ++i) {
                if (docs[i].getspecialization() == "Cholesterol level") {
                    docs[i] + *this;
                    doc = docs[i];
                }
            }
        }
        else {
            for (int i = 0; i < 4; ++i) {
                if (docs[i].getspecialization() == "Pulse rate") {
                    docs[i] + *this;
                    doc = docs[i];
                }
            }
        }
        cout << "You have been assigned to " << doc.getname() << " , specialization: " << doc.getspecialization() << endl;
    }
    virtual void Exercise() {
        int minutes;
        cout << "How many minutes did you exercise per day? " << endl;
        cin >> minutes;
        doc.SetByExercise(minutes, *this);
    }
    virtual void Eat() {
        char choice = 'Y';
        cout << "Please enter any notable item you have consumed since your last visit: ";
        while (choice == 'Y' || choice == 'y') {
            string food;
            cin >> food;
            doc.SetByFood(food, *this);
            cout << "Continue?: " << endl;
            cin >> choice;
            if (choice == 'Y' || choice == 'y') {
                cout << "Enter next item: " << endl;
            }
        }
    }
    friend istream& operator>>(istream& input, Person& P);
    friend ostream& operator<<(ostream& output, Person& P);
    friend class Admin;
};

class SugarPatient : public Patient {
    bool IsHigh_Sugar() {
        bool ishigh = false;
        if (sugar > sug_range[1]) {
            ishigh = true;
        }
        return ishigh;
    }
    bool IsLow_Sugar() {
        bool islow = false;
        if (sugar < sug_range[0]) {
            islow = true;
        }
        return islow;
    }
public:
    SugarPatient() {}
    SugarPatient(string name, int age, string gender, int sugar, int chol, int pulse, int dias_BP, int sys_BP, Doctor doc) :Patient(name, age, gender, sugar, chol, pulse, dias_BP, sys_BP) {
        this->doc = doc;
    }
    void Exercise() {
        Patient::Exercise();
    }
    void Eat() {
        Patient::Eat();
    }
};

class BPPatient : public Patient {
    bool IsHigh_BP() {
        bool ishigh = false;
        if (sys_BP > BPS_range[1] || dias_BP > BPD_range[1]) {
            ishigh = true;
        }
        return ishigh;
    }
    bool IsLow_BP() {
        bool islow = false;
        if (dias_BP < BPD_range[0] && sys_BP < BPS_range[0]) {
            islow = true;
        }
        return islow;
    }
public:
    BPPatient() {}
    BPPatient(string name, int age, string gender, int sugar, int chol, int pulse, int dias_BP, int sys_BP, Doctor doc) :Patient(name, age, gender, sugar, chol, pulse, dias_BP, sys_BP) {
        this->doc = doc;
    }
    void Exercise() {
        Patient::Exercise();
    }
    void Eat() {
        Patient::Eat();
    }
};

class CholPatient : public Patient {
    bool IsHigh_Chol() {
        bool ishigh = false;
        if (chol > chol_range[1]) {
            ishigh = true;
        }
        return ishigh;
    }
    bool IsLow_Chol() {
        bool islow = false;
        if (chol < chol_range[0]) {
            islow = true;
        }
        return islow;
    }
public:
    CholPatient() {}
    CholPatient(string name, int age, string gender, int sugar, int chol, int pulse, int dias_BP, int sys_BP, Doctor doc) :Patient(name, age, gender, sugar, chol, pulse, dias_BP, sys_BP) {
        this->doc = doc;
    }
    void Exercise() {
        Patient::Exercise();
    }
    void Eat() {
        Patient::Eat();
    }
};

class PulsePatient : public Patient {
    bool IsHigh_Pulse() {
        bool ishigh = false;
        if (pulse > pulse_range[1]) {
            ishigh = true;
        }
        return ishigh;
    }
    bool IsLow_Sugar() {
        bool islow = false;
        if (pulse < pulse_range[0]) {
            islow = true;
        }
        return islow;
    }
public:
    PulsePatient() {}
    PulsePatient(string name, int age, string gender, int sugar, int chol, int pulse, int dias_BP, int sys_BP, Doctor doc) :Patient(name, age, gender, sugar, chol, pulse, dias_BP, sys_BP) {
        this->doc = doc;
    }
    void Exercise() {
        Patient::Exercise();
    }
    void Eat() {
        Patient::Eat();
    }
};

class Admin{
    string * patients;
    int num_patients = 0;
public:
    Admin() {
        FetchPatients();
    }
    void FetchPatients() {
        ifstream File("E:\\all_patients.txt");
        string text;
        while (getline(File, text)) {
            num_patients++;
            string* patients2 = new string[num_patients];
            for (int i = 0; i < num_patients - 1; ++i) {
                patients2[i] = patients[i];
            }
            patients2[num_patients - 1].resize(text.size());
            patients2[num_patients - 1] = text;
            patients = new string[num_patients];
            for (int i = 0; i < num_patients; ++i) {
                patients[i] = patients2[i];
            }
            delete[] patients2;
        }
        File.close();
    }
    void Display() {
        for (int i = 0; i < num_patients; ++i) {
            cout << patients[i] << endl;
        }
    }
    Patient NewPatientProfile() {
        Patient P;
        string units;
        cout << "Enter your name: " << endl; cin >> P.name;
        cout << "Enter your age: " << endl; cin >> P.age;
        cout << "Please specify your gender: " << endl; cin >> P.gender;
        P.InputSugar();
        P.InputBP();
        P.InputChol();
        P.InputPulse();
        return P;
    }
    Patient ExistingPatientProfile(Doctor docs[]) {
        Patient P;
        int line = 0;
        string text;
        cout << "Enter your name: " << endl; cin >> P.name;
        for (int i = 0; i < num_patients; ++i) {
            try {
                if (patients[i] != P.name && i == num_patients - 1) {
                    throw 1;
                }
            }
            catch (int num) {
                cout << "File not found. Please try again." << endl;
                exit(0);
            }
            if (patients[i] == P.name) {
                cout << "Welcome back " << P.name << endl;
            }
        }
        string filename = "E:\\" + P.name + ".txt";
        ifstream File(filename);
        while (getline(File, text)) {
            line++;
            if (line == 2) {
                for (int i = 5; text[i] != '\0'; ++i) {
                    P.age += ((int)text[i] - '0') * pow(10, text.size() - i - 1);
                }
            }
            else if (line == 3) {
                P.gender.resize(text.size() - 8);
                for (int i = 8; text[i] != '\0'; ++i) {
                    P.gender[i - 8] = text[i];
                }
            }
            else if (line == 4) {
                for (int i = 7; text[i] != ' '; ++i) {
                    P.sugar += ((int)text[i] - '0') * pow(10, text.size() - i - 7);
                }
            }
            else if (line == 5) {
                int index1 = 0, index2 = 0;
                bool flag = false;
                for (int i = text.size() - 7.00; text[i] != ' '; --i) {
                    char prev_ch = text[i + 1];
                    if (text[i] == '/') {
                        continue;
                    }
                    if (prev_ch == '/') {
                        flag = true;
                    }
                    if (flag) {
                        P.sys_BP += ((int)text[i] - '0') * pow(10, index1);
                        index1++;
                    }
                    else {
                        P.dias_BP += ((int)text[i] - '0') * pow(10, index2);
                        index2++;
                    }

                }
            }
            else if (line == 6) {
                for (int i = 13; text[i] != ' '; ++i) {
                    P.chol += ((int)text[i] - '0') * pow(10, text.size() - i - 7);
                }
            }
            else if (line == 7) {
                for (int i = 7; text[i] != ' '; ++i) {
                    P.pulse += ((int)text[i] - '0') * pow(10, text.size() - i - 5);
                }
            }
            else if (line == 8) {
                string doc_name;
                doc_name.resize(text.size() - 8);
                for (int i = 8; text[i] != '\0'; ++i) {
                    doc_name[i - 8] = text[i];
                }
                for (int i = 0; i < 4; ++i) {
                    if (doc_name == docs[i].getname()) {
                        P.doc = docs[i];
                    }
                }
            }
        }
        File.close();
        return P;
    }
    template <typename T>
    int Compare(T value, T min, T max) {
        int level;
        if (value < min) {
            level = -1;
        }
        else if (value > max) {
            level = 1;
        }
        else level = 0;
        return level;
    }
    int CombineFactors(int factor1, int factor2) {
        int value=0;
        switch (factor1) {
        case -1:
            switch (factor2) {
            case -1:
                value = -4;
                break;
            case 0:
                value = -3;
                break;
            case 1:
                value = -2;
                break;
            }
            break;
        case 0:
            switch (factor2) {
            case -1:
                value = -1;
                break;
            case 0:
                value = 0;
                break;
            case 1:
                value = 1;
                break;
            }
            break;
        case 1:
            switch (factor2) {
            case -1:
                value = 2;
                break;
            case 0:
                value = 3;
                break;
            case 1:
                value = 4;
                break;
            }
            break;
        }
        return value;
    }
    int CombineFactors(int factor1, int factor2, int factor3) {
        int value=0;
        switch (factor1) {
        case -1:
            if (factor2 == -1 && factor3 == -1) {
                value = -4;
            }
            else if ((factor2 == -1 && factor3 == 0) || (factor2 == 0 && factor3 <= 0)) {
                value = -3;
            }
            else {
                value = -2;
            }
            break;
        case 0:
            if (factor2 == -1 && factor3 == -1) {
                value = -1;
            }
            else if ((factor2 == -1 && factor3 == 0) || (factor2 == 0 && factor3 <= 0)) {
                value = 0;
            }
            else {
                value = 1;
            }
            break;
        case 1:
            if (factor2 == -1 && factor3 == -1) {
                value = 2;
            }
            else if ((factor2 == -1 && factor3 == 0) || (factor2 == 0 && factor3 <= 0)) {
                value = 3;
            }
            else {
                value = 4;
            }
            break;
        }
        return value;
    }
    void StoreData(Patient P, int health_levels[], int combined[]) {
        string filename = "E:\\" + P.name + ".txt";
        string factors[5] = { "sugar", "systolic blood pressure","diastolic blood pressure","cholesterol","pulse rate" };
        ofstream File(filename);
        if (!File.is_open()) {
            cout << "Trouble creating file. Please try again." << endl;
        }
        File << "Name: " << P.name << endl;
        File << "Age: " << P.age << endl;
        File << "Gender: " << P.gender << endl;
        File << "Sugar: " << P.sugar << " mg/dL" << endl;
        File << "Blood Pressure: " << P.sys_BP << "/" << P.dias_BP << " mm Hg" << endl;
        File << "Cholesterol: " << P.chol << " mg/dL" << endl;
        File << "Pulse: " << P.pulse << " BPM" << endl;
        File << "Doctor: " << P.doc.getname() << endl;
        File << endl;
        for (int i = 0; i < 5; ++i) {
            if (health_levels[i] == -1) {
                File << "Low " << factors[i] << endl;
            }
            else if (health_levels[i] == 1) {
                File << "High " << factors[i] << endl;
            }
        }
        File << endl;
        switch (combined[0]) {
        case -4://low sugar, low BP
            File << "- \tYour low sugar level might be the main cause of your low blood pressure. The main complications that this can cause are : dizziness, weakness, and fainting." << endl;
            break;
        case -3://low sugar
            File << "-\tYou have hypoglycemia. This can result in: dizziness, sweating, anxiety, pale skin and hunger pangs." << endl;
            break;
        case -2://low sugar, high BP
            File << "-\tLow sugar and high blood pressure is a very alarming situation that may result in hypertension. If systolic blood pressure is higher than 160 mm Hg, consult your doctor immediately." << endl;
            break;
        case -1://low BP
            File << "-\tYou are experiencing low blood pressure, also known as hypotension. Side effects of this include dizziness, fainting, etc. Consult your doctor immediately in case you experience these symptoms for a prolonged duration." << endl;
            break;
        case 0:
            ;
            break;
        case 1://high BP
            File << "-\tYou are experiencing high blood pressure, also known as hypertension. Side effects of this include heart damage and brain damage. It is recommended to consult a doctor immediately." << endl;
            break;
        case 2://high sugar, low BP
            File << "-\tHigh sugar with low blood pressure indicates diabetes. The nerve sensors that monitor blood pressure do not work properly under high sugar, causing drastic drops in blood pressure.\n\tFurther symptoms of diabetes include extreme thirst and hunger, blurry vision and dry skin." << endl;
            break;
        case 3://high sugar
            File << "-\tYou have hyperglycemia. This can result in: heart and kidney diseases, stroke, and vision and nerve problems. This can, of course, result in diabetes." << endl;
            break;
        case 4://high sugar, high BP
            File << "-\tHigh sugar and high blood pressure indicate risk of heart and kidney failure. Vision and nervous system can be compromised and major chances of stroke are there. Visit the doctor ASAP." << endl;
            break;
        }
        switch (combined[1]) {
        case -4://low sugar, low chol
            File << "-\tSigns of low sugar level (hypoglycaemia) include sweating, dizziness, pounding heartbeat, and hunger. This coupled with a low cholesterol level increases risk for diseases such as cancer and hemorrhagic stroke. Consult your doctor immediately in case you are experiencing any symptoms." << endl;
            break;
        case -3://low sugar
            if (combined[0] == -3 || combined[1] == -3) {
                ;
            }
            else {
                File << "-\tYou have hypoglycemia. This can result in: dizziness, sweating, anxiety, pale skin and hunger pangs." << endl;
            }
            break;
        case -2://low sugar, high chol
            File << "-\tHigh cholesterol is normally caused by higher sugar consumption, so your case is unusual. However, look out for symptoms such as anxiety, pain in the arms and in the chest- your higher cholesterol levels might lead to a heart attack." << endl;
            break;
        case -1://low chol
            File << "-\tLow levels of cholesterol are generally considered beneficial, but in some cases might lead to health problems. These problems include cancer, hemorrhagic stroke and depression. Consult your doctor if you are experiencing symptoms of any of these diseases." << endl;
            break;
        case 0:
            ;
            break;
        case 1://high chol
            File << "-\tHigh cholesterol can be a result of various dangers, including coronary heart disease,stroke, or heart attack. Some general symptoms of these diseases include anxiety, chest pain, and dizziness. If you have any of these symptoms, consult your doctor immediately." << endl;
            break;
        case 2://high sugar, low chol
            File << "-\tLow cholesterol is normally caused by lower sugar consumption, so your case is unusual. However, look out for symptoms such as extreme thirst and hunger, unintended weight loss, and dry skin- you might be showing early signs of diabetes." << endl;
            break;
        case 3://high sugar
            if (combined[0] == 3 || combined[1] == 3) {
                ;
            }
            else {
                File << "-\tYou have hyperglycemia. This can result in: heart and kidney diseases, stroke, and vision and nerve problems. This can, of course, result in diabetes." << endl;
            }
            break;
        case 4://high sugar, high chol
            File << "-\tHigh sugar can directly cause high cholesterol, and vice versa. These two factors increase the risk of diabetes and heart attack. Consult your doctor if you are experiencing symptoms of these diseases." << endl;
            break;
        }
        switch (combined[2]) {
        case -4://low sugar, low pulse
            File << "-\tAlthough there is no such association between the two, low sugar level (hypoglycaemia) and low pulse rate (bradychardia) combined can create fatal heart rhythms." << endl;
            break;
        case -3://low sugar
            if (combined[0] == -3) {
                ;
            }
            else {
                File << "-\tYou have hypoglycemia.\n\tThis can result in: dizziness, sweating, anxiety, pale skin and hunger pangs." << endl;
            }
            break;
        case -2://low sugar, high pulse
            File << "-\tYour insufficient sugar levels might be causing rapid heart palpitations which is leading to a greater pulse." << endl;
            break;
        case -1://low pulse
            File << "-\tLow pulse rate is an indication of Bradychardia. Bradychardia can cause chest pain, confusion, and dizziness.Bradychardia gets progressively worse over the years, so it is advisable to take precautions immediately." << endl;
            break;
        case 0:
            ;
            break;
        case 1://high pulse
            File << "-\tHigh pulse rate is an indication of Tachychardia. Tachychardia is not always a cause of concern, as heart rate tends to spike after physical activity like exercise. Signs of tachycardia include a pounding heartbeat, chest pain, shortness of breath, etc. If you experience symptoms like fainting and weakness, seek immediate medical help." << endl;
            break;
        case 2://high sugar, low pulse
            File << "-\tLow pulse is closely associated with low blood pressure, so coupled with high sugar, this can indicate diabetes. The nerve sensors that monitor blood pressure do not work properly under high sugar, causing drastic drops in blood pressure. Further symptoms of diabetes include extreme thirst and hunger, blurry vision and dry skin." << endl;
            break;
        case 3://high sugar
            if (combined[0] == 3) {
                ;
            }
            else {
                File << "-\tYou have hyperglycemia.\nThis can result in: heart and kidney diseases, stroke, and vision and nerve problems. This can, of course, result in diabetes." << endl;
            }
            break;
        case 4://high sugar, high pulse
            File << "\tHigh pulse is a side-effect of high sugar levels. If left untreated, these two conditions can lead to severe diseases including cardiovascular and kidney diseases. If you have chest pain, weakness or fatigue, please consult your doctor immediately." << endl;
            break;
        }
        switch (combined[3]) {
        case -4://low BP,low chol
            File << "-\tIn general, lower levels of cholesterol and BP are beneficial. However, it is necessary to maintain them at a stable level, otherwise worrying symptoms such as dizziness, confusion, and even stroke, might appear." << endl;
            break;
        case -3://low BP
            if (combined[0] == -1) {
                ;
            }
            else {
                File << "-\tYou are experiencing low blood pressure, also known as hypotension. Side effects of this include dizziness, fainting, etc. Consult your doctor immediately in case you experience these symptoms for a prolonged duration." << endl;
            }
            break;
        case -2://low BP, high chol
            File << "-\tEven though high cholesterol and low blood pressure might not be interconnected, they should both individually be tended to. High cholesterol can lead to several types of cardiovascular diseases, while low blood pressure can lead to dizziness, fainting, etc." << endl;
            break;
        case -1://low chol
            if (combined[1] == -1) {
                ;
            }
            else {
                File << "-\tLow levels of cholesterol are generally considered beneficial, but in some cases might lead to health problems. These problems include cancer, hemorrhagic stroke and depression. Consult your doctor if you are experiencing symptoms of any of these diseases." << endl;
            }
            break;
        case 0:
            ;
            break;
        case 1://high chol
            if (combined[1] == 1) {
                ;
            }
            else {
                File << "-\tHigh cholesterol can be a result of various dangers, including coronary heart disease, stroke, or heart attack. Some general symptoms of these diseases include anxiety, chest pain, and dizziness. If you have any of these symptoms, consult your doctor immediately." << endl;
            }
            break;
        case 2://high BP, low chol
            File << "-\tAlthough low cholesterol is generally considered a good sign, your elevated blood pressure suggests otherwise. You might have a condition known as metabolic syndrome, which puts you at risk of heart diseases. Other indications of metabolic syndrome include high glucose and triglyceride levels. Please get yourself tested for these and consult a doctor in case they are also elevated." << endl;
            break;
        case 3://high BP
            if (combined[0] == 1) {
                File << "-\tYou are experiencing high blood pressure, also known as hypertension. Side effects of this include heart damage and brain damage. It is recommended to consult a doctor immediately." << endl;
            }
            break;
        case 4://high BP, high pulse
            File << "-\tHigh blood pressure is common in a person with high cholesterol levels, and vice versa. Both of these are risk factors for heart diseases, so it is suggested to consult your doctor immediately." << endl;
            break;
        }
        switch (combined[4]) {
        case -4://low BP, low pulse
            File << "-\tLow blood pressure might be a result of low heart rate, or bradychardia. Bradychardia can cause chest pain, confusion, and dizziness. Bradychardia gets progressively worse over the years, so it is advisable to take precautions immediately." << endl;
            break;
        case -3://low BP
            if (combined[0] == -1 || combined[3] == -3) {
                ;
            }
            else {
                File << "-\tYou are experiencing low blood pressure, also known as hypotension. Side effects of this include dizziness, fainting, etc. Consult your doctor immediately in case you experience these symptoms for a prolonged duration." << endl;
            }
            break;
        case -2://low BP, high pulse
            File << "-\tLow blood pressure (hypotension) coupled with high pulse rate can be a sign of pregnancy, nutrient deficiency, severe infection, allergic reaction, or dehydration." << endl;
            break;
        case -1://low pulse
            if (combined[2] == -1) {
                ;
            }
            else {
                File << "\tLow pulse rate is an indication of Bradychardia. Bradychardia can cause chest pain, confusion, and dizziness. Bradychardia gets progressively worse over the years, so it is advisable to take precautions immediately." << endl;
            }
            break;
        case 0:
            ;
            break;
        case 1://high pulse
            if (combined[2] == 1) {
                ;
            }
            else {
                File << "-\tHigh pulse rate is an indication of Tachychardia. Tachychardia is not always a cause of concern, as heart rate tends to spike after physical activity like exercise. Signs of tachycardia include a pounding heartbeat, chest pain, shortness of breath, etc. If you experience symptoms like fainting and weakness, seek immediate medical help." << endl;
            }
            break;
        case 2://high BP, low pulse
            File << "-\tHigh blood pressure (hypertension) but low pulse can be due to use of anti-hypertensive medication or due to a thickened heart tissue. If you experience any of the following symptoms, please head immediately to an emergency room: Chest or back pain, shortness of breath, or difficulty speaking" << endl;
            break;
        case 3://high BP
            if (combined[0] == 1 || combined[3] == 3) {
                ;
            }
            else {
                File << "-\tYou are experiencing high blood pressure, also known as hypertension. Side effects of this include heart damage and brain damage. It is recommended to consult a doctor immediately." << endl;
            }
            break;
        case 4://high BP,high pulse
            File << "-\tHigh pulse rate is closely associated with high blood pressure and is indicative of risk for cardiovascular disease. Symptoms of cardiovascular disease include chest discomfort, pain in the neck, upper belly area, or back, weakness in the arms, etc. Cardiovascular disease tends to decrease life expectancy by 10 per cent therefore it is advisable to seek immediate help." << endl;
        }
        switch (combined[5]) {
        case -4://low chol, low pulse
            File << "-\tLower pulse and cholesterol generally correspond to a lesser risk of heart diseases. However, it is still advised to maintain pulse and cholesterol at a higher level in order to avoid symptoms of bradychardia (low pulse rate) and symptoms associated with lower cholesterol." << endl;
            break;
        case -3://low chol
            if (combined[1] == -1 || combined[3] == -1) {
                ;
            }
            else {
                File << "-\tLow levels of cholesterol are generally considered beneficial, but in some cases might lead to health problems. These problems include cancer, hemorrhagic stroke and depression. Consult your doctor if you are experiencing symptoms of any of these diseases." << endl;
            }
            break;
        case -2://low chol,high pulse
            File << "-\tAlthough low cholesterol is generally a good sign, coupled with high heart rate it can be a sign of metabolic syndrome. This syndrome increases the risk of heart disease. Other signs of it include high glucose and triglyceride levels, so it is recommended to get these levels checked." << endl;
            break;
        case -1://low pulse
            if (combined[2] == -1 || combined[4] == -1) {
                ;
            }
            else {
                File << "-\tLow pulse rate is an indication of Bradychardia. Bradychardia can cause chest pain, confusion, and dizziness. Bradychardia gets progressively worse over the years, so it is advisable to take precautions immediately." << endl;
            }
            break;
        case 0:
            ;
            break;
        case 1://high pulse
            if (combined[2] == 1 || combined[4] == 1) {
                ;
            }
            else {
                File << "-\tHigh pulse rate is an indication of Tachychardia. Tachychardia is not always a cause of concern, as heart rate tends to spike after physical activity like exercise. Signs of tachycardia include a pounding heartbeat, chest pain, shortness of breath, etc. If you experience symptoms like fainting and weakness, seek immediate medical help." << endl;
            }
            break;
        case 2://high chol, low pulse
            File << "-\tGenerally, low pulse is associated with low cholesterol, so it is possible that a separate underlying disease is causing the elevated cholesterol level. In such case, consult a doctor immediately. It is also suggested to check for symptoms of bradychardia like chest pain, confusion and dizziness." << endl;
            break;
        case 3://high chol
            if (combined[1] == 1 || combined[3] == 1) {
                ;
            }
            else {
                File << "-\tHigh cholesterol can be a result of various dangers, including coronary heart disease, stroke, or heart attack. Some general symptoms of these diseases include anxiety, chest pain, and dizziness. If you have any of these symptoms, consult your doctor immediately." << endl;
            }
            break;
        case 4://high chol, high pulse
            File << "-\tHigh pulse rate is often caused by high cholesterol levels and is a signal for heart diseases. In case you experience symptoms of cardiovascular disease such as chest pain, short-breathedness and fatigue, etc. consult your doctor immediately." << endl;
            break;
        }
        File << endl;
        File << "SOLUTIONS" << endl;
        File << endl;
        switch (health_levels[0]) {
        case -1:
            File << "TO INCREASE SUGAR LEVEL...\n-\tConsume foods and drinks with higher levels of sugar\n\tExamples of this include orange juice, cake soda,etc.\n-\tHave an extra snack after exercise\n-\tGet a glucagon shot" << endl;
        case 0:
            ;
        case 1:
            File << "TO DECREASE SUGAR LEVEL...\n-\tExercise regularly\n-\tDecrease your intake of foods that are rich in carbs like soft drinks, desserts, etc.\n-\tEat more fiber, i.e. vegetables and fruits\n-\tStay hydrated\n-\tManage your stress levels" << endl;
        }
        switch (health_levels[1]) {
        case -1:
            if (health_levels[2] == -1) {
                File << "TO INCREASE BLOOD PRESSURE...\n-\tIncrease salt intake\n-\tStay hydrated\n-\tLimit high carbohydrate foods like potatoes and rice\n-\tExercise regularly" << endl;
            }
            break;
        case 0:
            if (health_levels[2] == 1) {
                File << "TO DECREASE BLOOD PRESSURE...\n-\tMaintain a healthy weight\n-\tExercise regularly\n-\tLimit salt intake in your diet\n-\tQuit smoking\n-\tGet a good night\'s sleep" << endl;
            }
            break;
        case 1:
            File << "TO DECREASE BLOOD PRESSURE...\n-\tMaintain a healthy weight\n-\tExercise regularly\n-\tLimit salt intake in your diet\n-\tQuit smoking\n-\tGet a good night\'s sleep" << endl;
            break;
        }
        switch (health_levels[3]) {
        case -1:
            File << "TO INCREASE CHOLESTEROL LEVEL...\n-\tEat food containing more fat\n-\tIn general, it is not advisable to increase your cholesterol levels." << endl;
            break;
        case 0:
            ;
            break;
        case 1:
            File << "TO DECREASE CHOLESTEROL LEVEL...\n-\tFollow a heart healthy diet, e.g. the Mediterranean Diet\n-\tAvoid smoking\n-\tExercise regularly\n-\tMaintain a healthy weight." << endl;
            break;
        }
        switch (health_levels[4]) {
        case -1:
            File << "TO INCREASE PULSE RATE...\n-\tExercise regularly\n-\tMaintain a healthy weight\n-\tKeep blood pressure and cholesterol under control\n-\tAvoid smoking" << endl;
            break;
        case 0:
            ;
            break;
        case 1:
            File << "TO DECREASE PULSE RATE...\n-\tExercise regularly\n-\tStay hydrated\n-\tManage your stress levels\n-\tLimit intake of stimulants such as adderall\n-\tSpend more time outdoors" << endl;
        }
        File.close();
    }

    void operator +(Patient P) {
        ofstream File("E:\\all_patients.txt",ios::app);
        File << P.name << endl;
        File.close();
    }
};

Doctor::Doctor(string name, int age, string gender, string specialization) :Person(name, age, gender) {
    this->specialization = specialization;
    ifstream File("E:\\" + name + ".bin", ios::binary);
    Patient pat;
    while (File.read(reinterpret_cast<char*>(&pat), sizeof(Patient))) {
        patients.push_back(pat);
    }
}

void Doctor::SetByExercise(int minutes, Patient& P) {
    P.setsugar(P.Getsugar() - floor(minutes / 30)).setsys_BP(P.Getsys_BP() - floor(5 * (minutes / 30))).setdias_BP(P.Getdias_BP() - floor(5 * (minutes / 30))).setchol(P.Getchol() - floor(minutes / 30));
}
void Doctor::SetByFood(string food_name, Patient& P) {
    ifstream F("E:\\foods.bin", ios::binary);
    Food food;
    while (F.read(reinterpret_cast<char*>(&food), sizeof(Food))) {
        if (food.Getname() == food_name) {
            P.setsugar(P.Getsugar() + floor(food.Getcalories() / 100)).setsugar(P.Getsugar() + floor(35 * (food.Getpercent_carbs() / 15))).setsugar(P.Getsugar() + floor(0.01 * (food.Getpercent_protein() / 50))).setsugar(P.Getsugar() + floor(0.01 * (food.Getpercent_fat() / 12.5))).setsugar(P.Getsugar() - floor(35 * (food.Getpercent_fiber() / 10)));
            P.setsys_BP(P.Getsys_BP() + floor(food.Getcalories() / 250)).setsys_BP(P.Getsys_BP() + floor(1.5 * (food.Getpercent_carbs() / 75))).setsys_BP(P.Getsys_BP() - floor(1.5 * (food.Getpercent_protein() / 10))).setsys_BP(P.Getsys_BP() - floor(2.4 * (food.Getpercent_fat() / 5))).setsys_BP(P.Getsys_BP() - floor(2.5 * (food.Getpercent_fiber() / 48)));
            P.setdias_BP(P.Getdias_BP() + floor(food.Getcalories() / 250)).setdias_BP(P.Getdias_BP() + floor(1.5 * (food.Getpercent_carbs() / 75))).setdias_BP(P.Getdias_BP() - floor(1.5 * (food.Getpercent_protein() / 10))).setdias_BP(P.Getdias_BP() - floor(2.4 * (food.Getpercent_fat() / 5))).setdias_BP(P.Getdias_BP() - floor(2.5 * (food.Getpercent_fiber() / 48)));
            P.setchol(P.Getchol() + floor(food.Getcalories() / 150)).setchol(P.Getchol() + floor(food.Getpercent_carbs() / 12.5)).setchol(P.Getchol() - floor(0.04 * (food.Getpercent_protein() / 50))).setchol(P.Getchol() + floor(food.Getpercent_fat() / 12.5)).setchol(P.Getchol() - floor(0.05 * (food.Getpercent_fiber() / 7.5)));
            P.setpulse(P.Getpulse() + floor(food.Getcalories() / 75)).setpulse(P.Getpulse() + floor(food.Getpercent_carbs() / 37.5)).setpulse(P.Getpulse() - floor(food.Getpercent_protein() / 10)).setpulse(P.Getpulse() + floor(food.Getpercent_fat() / 12.5)).setpulse(P.Getpulse() - floor(food.Getpercent_fiber() / 10));
        }
    }
}

istream& operator>>(istream& input, Person& P) {
    cout << "Please enter your name: " << endl;
    input >> P.name;
    cout << "Please enter your age: " << endl;
    input >> P.age;
    cout << "Please specify your gender: " << endl;
    input >> P.gender;
    return input;
}

ostream& operator<<(ostream& output, Person& P) {
    output << "Name: " << P.name << endl;
    output << "Age: " << P.age << endl;
    output << "Gender: " << P.gender << endl;
    return output;
}

void Doctor::operator+(Patient& P) {
    string filename = "E:\\" + name + ".bin";
    ofstream File(filename, ios::binary);
    patients.push_back(P);
    for (int i = 0; i < patients.size(); ++i) {
        File.write(reinterpret_cast<char*>(&patients[i]), sizeof(Patient));
    }
}

int main() {
    Admin A;
    Patient P;
    Patient* P2;
    Doctor d1("Doctor 1", 35, "Male", "Sugar level");
    Doctor d2("Doctor 2", 44, "Female", "Blood pressure");
    Doctor d3("Doctor 3", 38, "Female", "Pulse rate");
    Doctor d4("Doctor 4", 34, "Female", "Cholesterol level");
    Doctor docs[4] = { d1,d2,d3,d4 };
    char ch;
    int health_levels[5], combined[6];
    //combined[0]=sugar BP,[1]=sugar chol,[2]=sugar pulse,[3]=BP chol,[4]=BP pulse,[5]=pulse chol
    cout << "New or existing patient\? Press \'N\' for new, any other key for existing: "; cin >> ch;
    if (ch == 'N' || ch == 'n') {
        P = A.NewPatientProfile();
        A + P;
    }
    else {
        P = A.ExistingPatientProfile(docs);
        if (P.doc.getspecialization() == "Sugar level") {
            SugarPatient sug(P.getname(), P.getage(), P.getgender(), P.Getsugar(), P.Getchol(), P.Getpulse(), P.Getdias_BP(), P.Getsys_BP(), P.doc);
            P2 = &sug;
            P2->Eat();
            P2->Exercise();
        }
        else if (P.doc.getspecialization() == "Blood pressure") {
            BPPatient bp(P.getname(), P.getage(), P.getgender(), P.Getsugar(), P.Getchol(), P.Getpulse(), P.Getdias_BP(), P.Getsys_BP(), P.doc);
            P2 = &bp;
            P2->Eat();
            P2->Exercise();
        }
        else if (P.doc.getspecialization() == "Pulse rate") {
            PulsePatient pul(P.getname(), P.getage(), P.getgender(), P.Getsugar(), P.Getchol(), P.Getpulse(), P.Getdias_BP(), P.Getsys_BP(), P.doc);
            P2 = &pul;
            P2->Eat();
            P2->Exercise();
        }
        else if (P.doc.getspecialization() == "Cholesterol level") {
            CholPatient chol(P.getname(), P.getage(), P.getgender(), P.Getsugar(), P.Getchol(), P.Getpulse(), P.Getdias_BP(), P.Getsys_BP(), P.doc);
            P2 = &chol;
            P2->Eat();
            P2->Exercise();
        }
    }
    for (int i = 0; i < 5; ++i) {
        int value = (P.*(P.Get[i]))();
        health_levels[i] = A.Compare<int>(value, Hranges[i][0], Hranges[i][1]);
    }
    P.AssignDoctor(health_levels, docs);
    combined[0] = A.CombineFactors(health_levels[0], health_levels[1], health_levels[2]);
    combined[1] = A.CombineFactors(health_levels[0], health_levels[3]);
    combined[2] = A.CombineFactors(health_levels[0], health_levels[4]);
    combined[3] = A.CombineFactors(health_levels[3], health_levels[1], health_levels[2]);
    combined[4] = A.CombineFactors(health_levels[4], health_levels[1], health_levels[2]);
    combined[5] = A.CombineFactors(health_levels[3], health_levels[4]);
    A.StoreData(P, health_levels, combined);
}
