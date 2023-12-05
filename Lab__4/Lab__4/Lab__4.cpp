#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <memory>
#include <string>

class Product {
public:
    Product(const std::string& name, double price, int stock)
        : name(name), price(price), stock(stock) {}

    virtual ~Product() {}

    virtual void display() const {
        std::cout << "Name: " << name << ", Price: " << price
            << ", Stock: " << stock << std::endl;
    }

    virtual void update() {
        // Метод для обновления данных о продукте
    }

    virtual double calculateDiscount() const {
        return 0.0;
    }

    const std::string& getName() const {
        return name;
    }

    double getPrice() const {
        return price;
    }

    void setPrice(double newPrice) {
        price = newPrice;
    }

    int getStock() const {
        return stock;
    }

    void restock(int quantity) {
        stock += quantity;
    }

    // Добавлен оператор сравнения для удобства
    bool operator<(const Product& other) const {
        return price < other.price;
    }

protected:
    std::string name;
    double price;
    int stock;
};

class ElectronicsProduct : public Product {
public:
    ElectronicsProduct(const std::string& name, double price, int stock, int warrantyPeriod)
        : Product(name, price, stock), warrantyPeriod(warrantyPeriod) {}

    void display() const override {
        Product::display();
        std::cout << "Warranty Period: " << warrantyPeriod << " months" << std::endl;
    }

    double calculateDiscount() const override {
        // Реализация расчета скидки для электроники
        return getPrice() * 0.1;
    }

    void update() override {
        // Реализация обновления данных для электроники
        // Например, изменение гарантийного срока
        std::cout << "Enter new warranty period for " << name << ": ";
        std::cin >> warrantyPeriod;
    }

private:
    int warrantyPeriod;
};

class InventoryItem {
public:
    InventoryItem(const std::string& name, int quantity)
        : name(name), quantity(quantity) {}

    const std::string& getName() const {
        return name;
    }

    void display() const {
        std::cout << "Product: " << name << ", Quantity: " << quantity << std::endl;
    }

    int getQuantity() const {
        return quantity;
    }

    void updateQuantity(int newQuantity) {
        quantity = newQuantity;
    }

private:
    std::string name;
    int quantity;
};

template <typename T>
void sortProducts(std::vector<T>& products, bool (*compareFunction)(const T&, const T&)) {
    std::sort(products.begin(), products.end(), compareFunction);
}

bool sortByPrice(const Product& a, const Product& b) {
    return a.getPrice() < b.getPrice();  // Сортировка по цене
}

bool sortByStock(const Product& a, const Product& b) {
    return a.getStock() < b.getStock();  // Сортировка по количеству
}

bool sortByQuantity(const InventoryItem& a, const InventoryItem& b) {
    return a.getQuantity() < b.getQuantity();  // Сортировка по количеству
}

void displayProducts(const std::vector<std::unique_ptr<Product>>& products) {
    for (const auto& product : products) {
        product->display();
    }
}

void displayInventory(const std::vector<InventoryItem>& inventory) {
    for (const auto& item : inventory) {
        item.display();
    }
}

void updateProduct(std::vector<std::unique_ptr<Product>>& products) {
    std::string targetName;
    std::cout << "Enter the name of the product to update: ";
    std::cin.ignore(); // Clear the input buffer
    std::getline(std::cin, targetName);

    for (const auto& product : products) {
        if (product->getName() == targetName) {
            product->update();
            return;
        }
    }

    std::cout << "Product not found." << std::endl;
}

void saveToFile(const std::vector<std::unique_ptr<Product>>& products,
    const std::vector<InventoryItem>& inventory) {
    std::ofstream outputFile("output.txt");

    if (outputFile.is_open()) {
        for (const auto& product : products) {
            outputFile << "Product: " << product->getName() << ", Price: " << product->getPrice()
                << ", Stock: " << product->getStock() << std::endl;
        }

        outputFile << "\nInventory:\n";
        for (const auto& item : inventory) {
            outputFile << "Product: " << item.getName() << ", Quantity: " << item.getQuantity() << std::endl;
        }

        std::cout << "Data saved to output.txt." << std::endl;
    }
    else {
        std::cerr << "Unable to open output.txt for writing." << std::endl;
    }
}

int main() {
    std::vector<std::unique_ptr<Product>> products;
    std::vector<InventoryItem> inventory;

    products.push_back(std::make_unique<ElectronicsProduct>("Laptop", 1200.0, 5, 12));
    products.push_back(std::make_unique<Product>("Book", 20.0, 50));
    products.push_back(std::make_unique<ElectronicsProduct>("Smartphone", 800.0, 10, 6));

    inventory.emplace_back("Laptop", 5);
    inventory.emplace_back("Book", 50);
    inventory.emplace_back("Smartphone", 10);

    int choice;
    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. Display Products\n";
        std::cout << "2. Update Product\n";
        std::cout << "3. Save to File\n";
        std::cout << "4. Sort Products\n";
        std::cout << "5. Display Inventory\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            displayProducts(products);
            break;
        case 2:
            updateProduct(products);
            break;
        case 3:
            saveToFile(products, inventory);
            break;
        case 4:
            std::cout << "Sort by:\n";
            std::cout << "1. Price\n";
            std::cout << "2. Stock\n";
            std::cout << "Enter your choice: ";
            int sortChoice;
            std::cin >> sortChoice;
            switch (sortChoice) {
            case 1:
                sortProducts(products, sortByPrice);
                break;
            case 2:
                sortProducts(products, sortByStock);
                break;
            default:
                std::cout << "Invalid choice. Returning to main menu.\n";
                break;
            }
            break;
        case 5:
            displayInventory(inventory);
            break;
        case 0:
            std::cout << "Exiting program.\n";
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
