#include <iostream>
#include <string>
using namespace std;

struct FullName {
    string lastName;
    string firstName;
    string middleName;
    
    bool operator==(const FullName& other) const {
        return lastName == other.lastName &&
               firstName == other.firstName &&
               middleName == other.middleName;
    }
};

ostream& operator<<(ostream& os, const FullName& name) {
    os << name.lastName << " " << name.firstName << " " << name.middleName;
    return os;
}

istream& operator>>(istream& is, FullName& name) {
    is >> name.lastName >> name.firstName >> name.middleName;
    return is;
}

class PhoneDirectory {
private:
    string* phones;
    FullName* names;
    int size;
    int count;
    int codeError;
    
public:
    PhoneDirectory(int maxSize = 10) : size(maxSize), count(0), codeError(0) {
        phones = new string[size];
        names = new FullName[size];
    }
    
    ~PhoneDirectory() {
        delete[] phones;
        delete[] names;
    }
    
    void createAssociations(string* phoneArr, FullName* nameArr, int num) {
        if (num > size) {
            delete[] phones;
            delete[] names;
            size = num;
            phones = new string[size];
            names = new FullName[size];
        }
        
        for (int i = 0; i < num; i++) {
            phones[i] = phoneArr[i];
            names[i] = nameArr[i];
        }
        count = num;
        codeError = 0;
    }
    
    void addAssociation(const string& phone, const FullName& name) {
        if (count < size) {
            phones[count] = phone;
            names[count] = name;
            count++;
            codeError = 0;
        } else {
            codeError = -2; // Масив заповнений
        }
    }
    
    FullName operator[](const string& phone) {
        for (int i = 0; i < count; i++) {
            if (phones[i] == phone) {
                codeError = 0;
                return names[i];
            }
        }
        codeError = -1; // Не знайдено
        return FullName();
    }
    
    string operator[](const FullName& name) {
        for (int i = 0; i < count; i++) {
            if (names[i] == name) {
                codeError = 0;
                return phones[i];
            }
        }
        codeError = -1; // Не знайдено
        return "";
    }
    
    void operator()() {
        cout << "Телефонний довідник (" << count << " записів):\n";
        for (int i = 0; i < count; i++) {
            cout << phones[i] << " - " << names[i] << "\n";
        }
    }
    
    int getErrorCode() const { return codeError; }
    int getCount() const { return count; }
    
    friend ostream& operator<<(ostream& os, const PhoneDirectory& dir);
    friend istream& operator>>(istream& is, PhoneDirectory& dir);
};

ostream& operator<<(ostream& os, const PhoneDirectory& dir) {
    os << "Телефонний довідник (" << dir.count << " записів):\n";
    for (int i = 0; i < dir.count; i++) {
        os << dir.phones[i] << " - " << dir.names[i] << "\n";
    }
    return os;
}

istream& operator>>(istream& is, PhoneDirectory& dir) {
    string phone;
    FullName name;
    
    cout << "Введіть номер телефону: ";
    is >> phone;
    cout << "Введіть ПІБ (прізвище ім'я по-батькові): ";
    is >> name;
    
    dir.addAssociation(phone, name);
    
    if (dir.codeError != 0) {
        cout << "Помилка додавання запису! Код помилки: " << dir.codeError << "\n";
    }
    
    return is;
}

int main() {
    PhoneDirectory dir(5);
    
    // Створення набору асоціативних сутностей
    string phones[] = {"0501111111", "0672222222", "0633333333"};
    FullName names[] = {
        {"Іванов", "Іван", "Іванович"},
        {"Петров", "Петро", "Петрович"},
        {"Сидоров", "Сидір", "Сидорович"}
    };
    dir.createAssociations(phones, names, 3);
    cout << dir;
    
    // Пошук за номером телефону через оператор []
    string searchPhone = "0672222222";
    FullName foundName = dir[searchPhone];
    if (dir.getErrorCode() == 0) {
        cout << "\nЗа номером " << searchPhone << " знайдено: " << foundName << "\n";
    } else {
        cout << "\nЗа номером " << searchPhone << " нічого не знайдено\n";
    }
    
    // Пошук за ПІБ через оператор []
    FullName searchName = {"Сидоров", "Сидір", "Сидорович"};
    string foundPhone = dir[searchName];
    if (dir.getErrorCode() == 0) {
        cout << "За ПІБ " << searchName << " знайдено номер: " << foundPhone << "\n";
    } else {
        cout << "За ПІБ " << searchName << " нічого не знайдено\n";
    }
    
    // Альтернативне звернення через оператор ()
    cout << "\nВиведення через оператор():\n";
    dir();
    
    // Додавання нового запису через оператор >>
    cin >> dir;
    cout << "\nОновлений довідник:\n";
    cout << dir;
    cout << "\nСпроба додати запис у заповнений довідник:\n";
    dir.addAssociation("0999999999", {"Тест", "Тестович", "Тестович"});
    if (dir.getErrorCode() == -2) {
        cout << "Помилка: довідник заповнений!\n";
    }
    
    return 0;
}
