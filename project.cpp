#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

const int MAX_PRODUCTS = 100;
const int MAX_CART_ITEMS = 100;

struct Product {
    string name;
    double price;
};

struct CartItem {
    Product product;
    int quantity;
};

int loadProducts(Product products[], const string& filename) {
    ifstream file("products.txt");
    if (file.fail()) {
        cerr << "Error opening products file!" << endl;
        return 1;
    }

    int productCount = 0;
    while (file >> ws && getline(file, products[productCount].name)) {
        file >> products[productCount].price;
        productCount++;
    }
    file.close();
    return productCount;
}

void saveCart(CartItem cart[], int cartCount, const string& filename) {
    ofstream file(filename);
    if (file.fail()) {
        cerr << "Error opening cart file!" << endl;
        return;
    }

    for (int i = 0; i < cartCount; i++) {
        file << cart[i].product.name << endl;
        file << cart[i].product.price << endl;
        file << cart[i].quantity << endl;
    }
    file.close();
}

int loadCart(CartItem cart[], const string& filename, Product products[], int productCount) {
    ifstream file(filename);
    if (file.fail()) {
        return 0; // No cart file, assume empty cart
    }

    int cartCount = 0;
    string name;
    while (file >> ws && getline(file, name)) {
        double price;
        int quantity;
        file >> price >> quantity;

        cart[cartCount].product.name = name;
        cart[cartCount].product.price = price;
        cart[cartCount].quantity = quantity;
        cartCount++;
    }
    file.close();
    return cartCount;
}

void displayProducts(Product products[], int productCount) {
    cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(10) << "Price" << endl;
    cout << string(35, '-') << endl;
    for (int i = 0; i < productCount; i++) {
        cout << setw(5) << (i + 1) << setw(20) << products[i].name << "$" << fixed << setprecision(2) << products[i].price << endl;
    }
}

void displayCart(CartItem cart[], int cartCount) {
    if (cartCount == 0) {
        cout << "Your cart is empty." << endl;
        return;
    }

    double total = 0;
    cout << left << setw(20) << "Product" << setw(10) << "Quantity" << setw(10) << "Price" << setw(10) << "Total" << endl;
    cout << string(50, '-') << endl;
    for (int i = 0; i < cartCount; i++) {
        double itemTotal = cart[i].quantity * cart[i].product.price;
        cout << setw(20) << cart[i].product.name << setw(10) << cart[i].quantity << setw(10) << cart[i].product.price << setw(10) << itemTotal << endl;
        total += itemTotal;
    }
    cout << "Total Cost: $" << total << endl;
}

void addToCart(CartItem cart[], int& cartCount, Product products[], int productCount) {
    displayProducts(products, productCount);
    cout << "Enter Product ID to add to cart: ";
    int id;
    cin >> id;

    if (id < 1 || id > productCount) {
        cout << "Invalid Product ID." << endl;
        return;
    }

    cout << "Enter quantity: ";
    int quantity;
    cin >> quantity;

    if (quantity <= 0) {
        cout << "Invalid quantity." << endl;
        return;
    }

    for (int i = 0; i < cartCount; i++) {
        if (cart[i].product.name == products[id - 1].name) {
            cart[i].quantity += quantity;
            cout << "Updated quantity for " << products[id - 1].name << endl;
            return;
        }
    }

    cart[cartCount].product = products[id - 1];
    cart[cartCount].quantity = quantity;
    cartCount++;
    cout << "Added " << products[id - 1].name << " to cart." << endl;
}

void removeFromCart(CartItem cart[], int& cartCount) {
    if (cartCount == 0) {
        cout << "Your cart is empty." << endl;
        return;
    }

    displayCart(cart, cartCount);
    cout << "Enter Product ID to remove from cart: ";
    int id;
    cin >> id;

    if (id < 1 || id > cartCount) {
        cout << "Invalid ID." << endl;
        return;
    }

    for (int i = id - 1; i < cartCount - 1; i++) {
        cart[i] = cart[i + 1];
    }
    cartCount--;
    cout << "Item removed from cart." << endl;
}

void shoppingCart() {
    Product products[MAX_PRODUCTS];
    CartItem cart[MAX_CART_ITEMS];
    int productCount = loadProducts(products, "products.txt");
    int cartCount = loadCart(cart, "cart.txt", products, productCount);

    while (true) {
        cout << "\nShopping Cart Menu:\n";
        cout << "1. View Products\n";
        cout << "2. View Cart\n";
        cout << "3. Add to Cart\n";
        cout << "4. Remove from Cart\n";
        cout << "5. Checkout and Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                displayProducts(products, productCount);
                break;
            case 2:
                displayCart(cart, cartCount);
                break;
            case 3:
                addToCart(cart, cartCount, products, productCount);
                saveCart(cart, cartCount, "cart.txt");
                break;
            case 4:
                removeFromCart(cart, cartCount);
                saveCart(cart, cartCount, "cart.txt");
                break;
            case 5:
                cout << "Final Cart:\n";
                displayCart(cart, cartCount);
                saveCart(cart, cartCount, "cart.txt");
                cout << "Thank you for shopping!\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main() {
    shoppingCart();
    return 0;
}