//#define _CRT_SECURE_NO_WARNINGS  // Додаємо, щоб уникнути помилки C4996
#include <iostream>
#include <cstdio>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

// Клас FileManager
class FileManager {
private:
    std::FILE* file_;

public:
    FileManager(const char* filename, const char* mode) {
        file_ = std::fopen(filename, mode); // fopen безпечний завдяки #define
        if (!file_) {
            throw std::runtime_error("Не вдалося відкрити файл");
        }
    }

    ~FileManager() {
        if (file_) {
            std::fclose(file_);
        }
    }

    void write(const std::string& text) {
        if (file_) {
            std::fprintf(file_, "%s\n", text.c_str());
        }
    }
};

// Клас Sensor
class Sensor {
private:
    std::shared_ptr<std::vector<int>> data;

public:
    Sensor(std::shared_ptr<std::vector<int>> sharedData) : data(sharedData) {}

    void addReading(int value) {
        data->push_back(value);
    }

    void printReadings() const {
        for (int value : *data) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }
};

// Тестова програма
int main() {
    try {
        // Використання FileManager
        FileManager fileManager("output.txt", "w");
        fileManager.write("Hello from FileManager!");
        fileManager.write("Another line of text.");

        // Використання Sensor
        auto sharedData = std::make_shared<std::vector<int>>();
        Sensor sensor1(sharedData);
        Sensor sensor2(sharedData);

        sensor1.addReading(10);
        sensor2.addReading(20);
        sensor1.addReading(30);

        std::cout << "Sensor readings: ";
        sensor1.printReadings();
    }
    catch (const std::exception& e) {
        std::cerr << "Помилка: " << e.what() << "\n";
    }

    return 0;
}
