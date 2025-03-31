#include <iostream>
#include <cmath>
#include <stdexcept>

using namespace std;

enum State {
    OK, NULL_POINTER_ERROR, DIVISION_BY_ZERO_ERROR, INVALID_INDEX_ERROR};

class Vector3F {
private:
    float x, y, z;
    State state;
    static int count; // Лічильник об'єктів

public:
    // Конструктори
    Vector3F() : x(0.0f), y(0.0f), z(0.0f), state(OK) { count++; }
    
    Vector3F(float value) : x(value), y(value), z(value), state(OK) { count++; }
    
    Vector3F(const float* arr) : state(OK) {
        if (arr == nullptr) {
            x = y = z = 0.0f;
            state = NULL_POINTER_ERROR;
        } else {
            x = arr[0];
            y = arr[1];
            z = arr[2];
        }
        count++;
    }
    
    Vector3F(float x, float y, float z) : x(x), y(y), z(z), state(OK) { count++; }

    // Деструктор
    ~Vector3F() {
        cout << "Деструктор Vector3F. Стан: ";
        printState();
        count--;
    }

    // Допоміжні функції
    void printState() const {
        switch (state) {
            case OK: cout << "OK"; break;
            case NULL_POINTER_ERROR: cout << "NULL_POINTER_ERROR"; break;
            case DIVISION_BY_ZERO_ERROR: cout << "DIVISION_BY_ZERO_ERROR"; break;
            case INVALID_INDEX_ERROR: cout << "INVALID_INDEX_ERROR"; break;
        }
        cout << ", Значення: (" << x << ", " << y << ", " << z << ")" << endl;
    }

    // Геттери
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
    State getState() const { return state; }
    static int getCount() { return count; }

    // Унарні оператори
    Vector3F operator+() const { return *this; } // Унарний плюс
    
    Vector3F operator-() const { return Vector3F(-x, -y, -z); } // Унарний мінус
    
    bool operator!() const { return x != 0.0f || y != 0.0f || z != 0.0f; } // Логічне заперечення
    
    Vector3F operator~() const { return Vector3F(-y, x, z); } // Перпендикулярний вектор
    
    // Префіксні оператори
    Vector3F& operator++() {
        x += 1.0f; y += 1.0f; z += 1.0f;
        return *this;
    }
    
    Vector3F& operator--() {
        x -= 1.0f; y -= 1.0f; z -= 1.0f;
        return *this;
    }
    
    // Постфіксні оператори
    Vector3F operator++(int) {
        Vector3F temp = *this;
        ++(*this);
        return temp;
    }
    
    Vector3F operator--(int) {
        Vector3F temp = *this;
        --(*this);
        return temp;
    }

    // Оператори присвоєння
    Vector3F& operator=(const Vector3F& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
            state = other.state;
        }
        return *this;
    }
    
    Vector3F& operator+=(const Vector3F& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    Vector3F& operator-=(const Vector3F& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    Vector3F& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    Vector3F& operator/=(float scalar) {
        if (scalar == 0.0f) {
            state = DIVISION_BY_ZERO_ERROR;
            return *this;
        }
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    
    // Векторний добуток
    Vector3F& operator&=(const Vector3F& other) {
        float newX = y * other.z - z * other.y;
        float newY = z * other.x - x * other.z;
        float newZ = x * other.y - y * other.x;
        x = newX; y = newY; z = newZ;
        return *this;
    }
    
    // Остача від ділення цілої частини
    Vector3F& operator%=(int divisor) {
        if (divisor == 0) {
            state = DIVISION_BY_ZERO_ERROR;
            return *this;
        }
        x = fmod(floor(x), divisor);
        y = fmod(floor(y), divisor);
        z = fmod(floor(z), divisor);
        return *this;
    }

    // Бінарні арифметичні оператори
    Vector3F operator+(const Vector3F& other) const {
        return Vector3F(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3F operator-(const Vector3F& other) const {
        return Vector3F(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3F operator*(float scalar) const {
        return Vector3F(x * scalar, y * scalar, z * scalar);
    }
    
    Vector3F operator/(float scalar) const {
        if (scalar == 0.0f) {
            Vector3F result = *this;
            result.state = DIVISION_BY_ZERO_ERROR;
            return result;
        }
        return Vector3F(x / scalar, y / scalar, z / scalar);
    }
    
    // Векторний добуток
    Vector3F operator&(const Vector3F& other) const {
        return Vector3F(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    // Остача від ділення цілої частини
    Vector3F operator%(int divisor) const {
        if (divisor == 0) {
            Vector3F result = *this;
            result.state = DIVISION_BY_ZERO_ERROR;
            return result;
        }
        return Vector3F(
            fmod(floor(x), divisor),
            fmod(floor(y), divisor),
            fmod(floor(z), divisor)
        );
    }

    // Оператори порівняння
    bool operator==(const Vector3F& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator!=(const Vector3F& other) const {
        return !(*this == other);
    }
    
    bool operator>(const Vector3F& other) const {
        return x > other.x && y > other.y && z > other.z;
    }
    
    bool operator>=(const Vector3F& other) const {
        return x >= other.x && y >= other.y && z >= other.z;
    }
    
    bool operator<(const Vector3F& other) const {
        return x < other.x && y < other.y && z < other.z;
    }
    
    bool operator<=(const Vector3F& other) const {
        return x <= other.x && y <= other.y && z <= other.z;
    }

    // Оператор індексації
    float& operator[](int index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:
                state = INVALID_INDEX_ERROR;
                return z;
        }
    }
    
    const float& operator[](int index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:
                const_cast<Vector3F*>(this)->state = INVALID_INDEX_ERROR;
                return z;
        }
    }

    // Оператор виклику функції
    void operator()() const {
        cout << "Виклик вектора: (" << x << ", " << y << ", " << z << ")" << endl;
    }

    // Дружні оператори введення/виведення
    friend ostream& operator<<(ostream& os, const Vector3F& vec);
    friend istream& operator>>(istream& is, Vector3F& vec);
    
    // Перевантаження new та delete
    void* operator new(size_t size) {
        cout << "Викликано перевантажений new для Vector3F" << endl;
        void* p = malloc(size);
        if (!p) throw bad_alloc();
        return p;
    }
    
    void operator delete(void* p) {
        cout << "Викликано перевантажений delete для Vector3F" << endl;
        free(p);
    }
};

// Ініціалізація статичного члена
int Vector3F::count = 0;

// Дружні оператори введення/виведення
ostream& operator<<(ostream& os, const Vector3F& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

istream& operator>>(istream& is, Vector3F& vec) {
    is >> vec.x >> vec.y >> vec.z;
    return is;
}

// Функція для тестування класу Vector3F
void testVector3F() {
    cout << "=== Тестування класу Vector3F ===" << endl;
    cout << "Початкова кількість об'єктів: " << Vector3F::getCount() << endl;

    {
        cout << "\n--- Тестування конструкторів ---" << endl;
        Vector3F v1;
        cout << "v1 (конструктор без параметрів): " << v1 << endl;
        
        Vector3F v2(5.0f);
        cout << "v2 (конструктор з одним параметром): " << v2 << endl;
        
        float arr[] = {1.0f, 2.0f, 3.0f};
        Vector3F v3(arr);
        cout << "v3 (конструктор з масивом): " << v3 << endl;
        
        Vector3F v4(nullptr);
        cout << "v4 (конструктор з nullptr, стан): ";
        v4.printState();
        
        cout << "Поточна кількість об'єктів: " << Vector3F::getCount() << endl;
    }

    cout << "\nКількість об'єктів після виходу з області видимості: " << Vector3F::getCount() << endl;

    {
        cout << "\n--- Тестування унарних операторів ---" << endl;
        Vector3F v(1.0f, 2.0f, 3.0f);
        
        cout << "Початковий вектор: " << v << endl;
        cout << "Унарний плюс: " << +v << endl;
        cout << "Унарний мінус: " << -v << endl;
        cout << "Логічне заперечення: " << !v << endl;
        cout << "Перпендикулярний вектор: " << ~v << endl;
        
        cout << "Префіксний ++: " << ++v << endl;
        cout << "Постфіксний ++: " << v++ << " (після: " << v << ")" << endl;
        cout << "Префіксний --: " << --v << endl;
        cout << "Постфіксний --: " << v-- << " (після: " << v << ")" << endl;
    }

    {
        cout << "\n--- Тестування бінарних операторів ---" << endl;
        Vector3F v1(1.0f, 2.0f, 3.0f);
        Vector3F v2(4.0f, 5.0f, 6.0f);
        
        cout << "v1: " << v1 << endl;
        cout << "v2: " << v2 << endl;
        
        cout << "Додавання: " << v1 + v2 << endl;
        cout << "Віднімання: " << v1 - v2 << endl;
        cout << "Множення на скаляр (2.0): " << v1 * 2.0f << endl;
        cout << "Ділення на скаляр (2.0): " << v1 / 2.0f << endl;
        
        Vector3F v3 = v1 / 0.0f;
        cout << "Ділення на 0 (стан): ";
        v3.printState();
        
        cout << "Векторний добуток: " << (v1 & v2) << endl;
        cout << "Остача від ділення (ціла частина на 2): " << (v1 % 2) << endl;
    }

    {
        cout << "\n--- Тестування операторів присвоєння ---" << endl;
        Vector3F v1(1.0f, 2.0f, 3.0f);
        Vector3F v2;
        
        v2 = v1;
        cout << "Після присвоєння: " << v2 << endl;
        
        v2 += Vector3F(1.0f, 1.0f, 1.0f);
        cout << "Після +=: " << v2 << endl;
        
        v2 -= Vector3F(1.0f, 1.0f, 1.0f);
        cout << "Після -=: " << v2 << endl;
        
        v2 *= 2.0f;
        cout << "Після *=: " << v2 << endl;
        
        v2 /= 2.0f;
        cout << "Після /=: " << v2 << endl;
        
        Vector3F v3(0.0f, 1.0f, 0.0f);
        Vector3F v4(0.0f, 0.0f, 1.0f);
        v3 &= v4;
        cout << "Векторний добуток (&=): " << v3 << endl;
        
        Vector3F v5(5.5f, 6.6f, 7.7f);
        v5 %= 3;
        cout << "Остача від ділення (%=): " << v5 << endl;
    }

    {
        cout << "\n--- Тестування операторів порівняння ---" << endl;
        Vector3F v1(1.0f, 2.0f, 3.0f);
        Vector3F v2(1.0f, 2.0f, 3.0f);
        Vector3F v3(4.0f, 5.0f, 6.0f);
        
        cout << "v1: " << v1 << endl;
        cout << "v2: " << v2 << endl;
        cout << "v3: " << v3 << endl;
        
        cout << "v1 == v2: " << (v1 == v2) << endl;
        cout << "v1 != v2: " << (v1 != v2) << endl;
        cout << "v1 > v3: " << (v1 > v3) << endl;
        cout << "v1 >= v2: " << (v1 >= v2) << endl;
        cout << "v1 < v3: " << (v1 < v3) << endl;
        cout << "v1 <= v2: " << (v1 <= v2) << endl;
    }

    {
        cout << "\n--- Тестування оператора індексації ---" << endl;
        Vector3F v(1.0f, 2.0f, 3.0f);
        
        cout << "Початковий вектор: " << v << endl;
        cout << "v[0]: " << v[0] << endl;
        cout << "v[1]: " << v[1] << endl;
        cout << "v[2]: " << v[2] << endl;
        
        v[1] = 5.0f;
        cout << "Після v[1] = 5.0f: " << v << endl;
        
        cout << "Спроба доступу до v[3]: " << v[3] << endl;
        cout << "Стан після невірного індексу: ";
        v.printState();
    }

    {
        cout << "\n--- Тестування оператора виклику функції ---" << endl;
        Vector3F v(1.0f, 2.0f, 3.0f);
        v();
    }

    {
        cout << "\n--- Тестування операторів new та delete ---" << endl;
        Vector3F* pv = new Vector3F(1.0f, 2.0f, 3.0f);
        cout << "Створений вектор: " << *pv << endl;
        delete pv;
    }

    cout << "\nФінальна кількість об'єктів: " << Vector3F::getCount() << endl;
    cout << "=== Тестування завершено ===" << endl;
}

int main() {
    testVector3F();
    return 0;
}
