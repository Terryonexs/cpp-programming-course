#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <numeric>
#include <cmath>

// ===================== Задача 1: Паралельні математичні обчислення =====================

// Функції для потоків
void factorial(int n) {
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    std::cout << "Factorial(" << n << ") = " << result << std::endl;
}

void sumOfSquares(int limit) {
    long long sum = 0;
    for (int i = 1; i <= limit; ++i) sum += i * i;
    std::cout << "Sum of squares (1-" << limit << ") = " << sum << std::endl;
}

void fibonacci(int n) {
    std::vector<unsigned long long> fib(n);
    fib[0] = 0; fib[1] = 1;
    for (int i = 2; i < n; ++i) fib[i] = fib[i - 1] + fib[i - 2];
    std::cout << "Fibonacci up to " << n << " elements: ";
    for (auto val : fib) std::cout << val << " ";
    std::cout << std::endl;
}

void primesUpTo(int limit) {
    std::vector<int> primes;
    for (int i = 2; i <= limit; ++i) {
        bool isPrime = true;
        for (int j = 2; j * j <= i; ++j)
            if (i % j == 0) { isPrime = false; break; }
        if (isPrime) primes.push_back(i);
    }
    std::cout << "Primes up to " << limit << ": ";
    for (int p : primes) std::cout << p << " ";
    std::cout << std::endl;
}

// ===================== Задача 2: Система обробки замовлень =====================

class Order {
private:
    int id;
    std::vector<std::string> items;
    double totalPrice;
    std::string status;
public:
    Order(int orderId, const std::vector<std::string>& orderItems)
        : id(orderId), items(orderItems), totalPrice(0.0), status("NEW") {
    }

    void setPrice(double price) { totalPrice = price; }
    void setStatus(const std::string& st) { status = st; }

    int getId() const { return id; }
    double getPrice() const { return totalPrice; }
    const std::string& getStatus() const { return status; }
    const std::vector<std::string>& getItems() const { return items; }
};

class OrderProcessor {
public:
    std::vector<Order> validateOrders(const std::vector<Order>& rawOrders) {
        std::vector<Order> valid;
        for (auto& o : rawOrders)
            if (!o.getItems().empty()) valid.push_back(o);
        std::cout << "Orders validated: " << valid.size() << std::endl;
        return valid;
    }

    std::vector<Order> calculatePricing(std::vector<Order>& orders) {
        for (auto& o : orders) o.setPrice(o.getItems().size() * 10.0);
        std::cout << "Prices calculated." << std::endl;
        return orders;
    }

    std::vector<Order> checkInventory(std::vector<Order>& orders) {
        for (auto& o : orders) o.setStatus("IN_STOCK");
        std::cout << "Inventory checked." << std::endl;
        return orders;
    }

    void generateInvoices(const std::vector<Order>& orders) {
        for (auto& o : orders)
            std::cout << "Invoice for Order " << o.getId()
            << ": $" << o.getPrice()
            << " Status: " << o.getStatus() << std::endl;
        std::cout << "Invoices generated." << std::endl;
    }
};

// ===================== Головна функція =====================

int main() {
    std::cout << "=== Task 1: Parallel Math Functions ===\n";

    // Використовуємо стиль твого example11_thread_pool
    std::vector<std::thread> mathThreads;
    mathThreads.emplace_back(factorial, 15);
    mathThreads.emplace_back(sumOfSquares, 1000);
    mathThreads.emplace_back(fibonacci, 30);
    mathThreads.emplace_back(primesUpTo, 100);

    for (auto& t : mathThreads) t.join(); // Чекаємо завершення всіх потоків

    std::cout << "\n=== Task 2: Order Processing System ===\n";
    OrderProcessor processor;
    std::vector<Order> rawOrders = {
        Order(1, {"item1","item2"}),
        Order(2, {"item3"}),
        Order(3, {})
    };

    std::vector<Order> validatedOrders;
    std::vector<Order> pricedOrders;
    std::vector<Order> finalOrders;

    // Потоки виконуються послідовно
    std::thread t1([&]() { validatedOrders = processor.validateOrders(rawOrders); });
    t1.join();

    std::thread t2([&]() { pricedOrders = processor.calculatePricing(validatedOrders); });
    t2.join();

    std::thread t3([&]() { finalOrders = processor.checkInventory(pricedOrders); });
    t3.join();

    std::thread t4([&]() { processor.generateInvoices(finalOrders); });
    t4.join();

    return 0;
}
