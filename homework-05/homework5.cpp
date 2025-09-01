#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include <atomic>

// ============================================================================
// 1. НЕБЕЗПЕЧНА ВЕРСІЯ
// ============================================================================

class UnsafeBankAccount {
private:
    double balance = 1000.0; // Початковий баланс

public:
    void deposit(double amount) {
        balance += amount; // Небезпечна операція без блокування
    }

    bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }

    double get_balance() const {
        return balance; // Читання без синхронізації
    }
};

class UnsafeBankStatistics {
private:
    int transaction_count = 0;
    double total_amount = 0.0;

public:
    void record_transaction(double amount) {
        transaction_count++; // Без синхронізації
        total_amount += amount;
    }

    int get_total_transactions() const { return transaction_count; }
    double get_total_amount() const { return total_amount; }
};

// Функція для роботи касира (НЕБЕЗПЕЧНА)
void unsafe_cashier_work(UnsafeBankAccount& account, UnsafeBankStatistics& stats, int operations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> op_type(0, 1);
    std::uniform_real_distribution<> deposit_amount(50, 500);
    std::uniform_real_distribution<> withdraw_amount(10, 200);

    for (int i = 0; i < operations; ++i) {
        if (op_type(gen) == 0) {
            double amount = deposit_amount(gen);
            account.deposit(amount);
            stats.record_transaction(amount);
        }
        else {
            double amount = withdraw_amount(gen);
            if (account.withdraw(amount)) {
                stats.record_transaction(amount);
            }
        }
    }
}

void demo_unsafe_version() {
    std::cout << "=== UNSAFE VERSION DEMO ===\n";

    UnsafeBankAccount account;
    UnsafeBankStatistics stats;

    std::cout << "Initial balance: " << account.get_balance() << "\n";

    std::vector<std::thread> cashiers;
    for (int i = 0; i < 5; ++i) {
        cashiers.emplace_back(unsafe_cashier_work, std::ref(account), std::ref(stats), 100);
    }

    for (auto& t : cashiers) t.join();

    std::cout << "Final balance: " << account.get_balance() << "\n";
    std::cout << "Total transactions: " << stats.get_total_transactions() << "\n";
    std::cout << "Total amount: " << stats.get_total_amount() << "\n\n";
}

// ============================================================================
// 2. БЕЗПЕЧНА ВЕРСІЯ
// ============================================================================

class SafeBankAccount {
private:
    double balance = 1000.0;
    mutable std::mutex balance_mutex; // Захист балансу

public:
    void deposit(double amount) {
        std::lock_guard<std::mutex> lock(balance_mutex); // Автоматичне блокування
        balance += amount;
    }

    bool withdraw(double amount) {
        std::lock_guard<std::mutex> lock(balance_mutex);
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }

    double get_balance() const {
        std::lock_guard<std::mutex> lock(balance_mutex);
        return balance;
    }
};

class SafeBankStatistics {
private:
    std::atomic<int> transaction_count{ 0 };
    double total_amount = 0.0;
    mutable std::mutex amount_mutex;

public:
    void record_transaction(double amount) {
        transaction_count.fetch_add(1, std::memory_order_relaxed);
        std::lock_guard<std::mutex> lock(amount_mutex);
        total_amount += amount;
    }

    int get_total_transactions() const {
        return transaction_count.load(std::memory_order_relaxed);
    }

    double get_total_amount() const {
        std::lock_guard<std::mutex> lock(amount_mutex);
        return total_amount;
    }
};

// Функція для роботи касира (БЕЗПЕЧНА)
void safe_cashier_work(SafeBankAccount& account, SafeBankStatistics& stats, int operations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> op_type(0, 1);
    std::uniform_real_distribution<> deposit_amount(50, 500);
    std::uniform_real_distribution<> withdraw_amount(10, 200);

    for (int i = 0; i < operations; ++i) {
        if (op_type(gen) == 0) {
            double amount = deposit_amount(gen);
            account.deposit(amount);
            stats.record_transaction(amount);
        }
        else {
            double amount = withdraw_amount(gen);
            if (account.withdraw(amount)) {
                stats.record_transaction(amount);
            }
        }
    }
}

void demo_safe_version() {
    std::cout << "=== SAFE VERSION DEMO ===\n";

    SafeBankAccount account;
    SafeBankStatistics stats;

    std::cout << "Initial balance: " << account.get_balance() << "\n";

    std::vector<std::thread> cashiers;
    for (int i = 0; i < 5; ++i) {
        cashiers.emplace_back(safe_cashier_work, std::ref(account), std::ref(stats), 100);
    }

    for (auto& t : cashiers) t.join();

    std::cout << "Final balance: " << account.get_balance() << "\n";
    std::cout << "Total transactions: " << stats.get_total_transactions() << "\n";
    std::cout << "Total amount: " << stats.get_total_amount() << "\n";
    std::cout << "All cashiers completed work safely!\n\n";
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    demo_unsafe_version(); // Показуємо проблеми
    demo_safe_version();   // Показуємо правильну реалізацію
    return 0;
}
