#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class Product {
private:
    int productID;
    string name;
    double price;
    int stock;

public:
    Product(int id, string n, double p, int s) {
        productID = id;
        name = n;
        price = p;
        stock = s;
    }

    int getProductID() const { return productID; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    void updateStock(int quantity) {
        if (stock >= quantity) {
            stock -= quantity;
        } else {
            cout << "Not enough stock available!\n";
        }
    }

    void displayProduct() const {
        cout << "ID: " << productID << " | Name: " << name << " | Price: ?" << fixed << setprecision(2) << price 
             << " | Stock: " << stock << endl;
    }

    void saveToFile(ofstream &outfile) const {
        outfile << productID << "," << name << "," << price << "," << stock << endl;
    }

    static Product loadFromFile(ifstream &infile) {
        int id, stock;
        string name;
        double price;
        infile >> id;
        infile.ignore();
        getline(infile, name, ',');
        infile >> price >> stock;
        return Product(id, name, price, stock);
    }
};

// Function to check if product ID is unique
bool isProductIDUnique(vector<Product> &products, int id) {
    for (const auto &p : products) {
        if (p.getProductID() == id) {
            return false;
        }
    }
    return true;
}

// Function to find a product by ID
Product* findProduct(vector<Product> &products, int id) {
    for (auto &p : products) {
        if (p.getProductID() == id) {
            return &p;
        }
    }
    return nullptr;
}

// Save products to file
void saveProductsToFile(vector<Product> &products) {
    ofstream outfile("products.txt", ios::trunc);
    for (const auto &p : products) {
        p.saveToFile(outfile);
    }
    outfile.close();
}

// Load products from file
void loadProductsFromFile(vector<Product> &products) {
    ifstream infile("products.txt");
    if (!infile) return;

    while (!infile.eof()) {
        int id, stock;
        string name;
        double price;

        infile >> id;
        infile.ignore();
        if (getline(infile, name, ',') && infile >> price >> stock) {
            products.push_back(Product(id, name, price, stock));
        }
    }
    infile.close();
}

// Function to generate a customer bill
void generateBill(vector<Product> &products) {
    int id, quantity;
    double total = 0;
    vector<pair<Product*, int>> cart;

    cout << "\n===== Generate Bill =====\n";
    while (true) {
        cout << "Enter Product ID to buy (0 to finish): ";
        cin >> id;
        if (id == 0) break;

        Product* product = findProduct(products, id);
        if (product) {
            cout << "Enter Quantity: ";
            cin >> quantity;

            if (product->getStock() >= quantity) {
                product->updateStock(quantity);
                cart.push_back({product, quantity});
                total += product->getPrice() * quantity;
            } else {
                cout << "Not enough stock available!\n";
            }
        } else {
            cout << "Product not found!\n";
        }
    }

    if (!cart.empty()) {
        cout << "\n===== Customer Bill =====\n";
        cout << "Product Name\tQuantity\tPrice\tTotal\n";
        for (auto &item : cart) {
            cout << item.first->getName() << "\t\t" << item.second << "\t\t?" 
                 << item.first->getPrice() << "\t?" << item.first->getPrice() * item.second << endl;
        }
        cout << "--------------------------------------\n";
        cout << "Grand Total: ?" << fixed << setprecision(2) << total << endl;
    }

    saveProductsToFile(products);
}

int main() {
    vector<Product> products;
    loadProductsFromFile(products);

    int choice;
    while (true) {
        cout << "\n===== Supermarket Billing System =====\n";
        cout << "1. Add Product\n";
        cout << "2. View Available Products\n";
        cout << "3. Generate Customer Bill\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, stock;
            string name;
            double price;
            cout << "Enter Product ID: ";
            cin >> id;
            cin.ignore();

            if (!isProductIDUnique(products, id)) {
                cout << "Error! Product ID already exists.\n";
                continue;
            }

            cout << "Enter Product Name: ";
            getline(cin, name);
            cout << "Enter Price: ?";
            cin >> price;
            cout << "Enter Stock Quantity: ";
            cin >> stock;

            products.push_back(Product(id, name, price, stock));
            saveProductsToFile(products);
            cout << "Product added successfully!\n";

        } else if (choice == 2) {
            cout << "\n===== Available Products =====\n";
            for (const auto &p : products) {
                p.displayProduct();
            }

        } else if (choice == 3) {
            generateBill(products);

        } else if (choice == 4) {
            cout << "Exiting Supermarket Billing System. Thank you!\n";
            break;
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
