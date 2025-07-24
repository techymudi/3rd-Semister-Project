#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// === File Definitions ===
#define HOUSE_FILE "houses.txt"
#define USER_FILE "users.txt"

// === Color Support (Optional) ===
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define RESET   "\033[0m"

// === Structures ===
typedef struct {
    int id;
    char owner[50];
    char address[100];
    float rent;
    int is_rented;
} House;

typedef struct {
    char username[30];
    char password[30];
} User;

// === Utility Functions ===
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
}

void printDivider() {
    printf("----------------------------------------------\n");
}

// === Function Declarations ===
void adminMenu();
void userMenu();
int adminLogin();
void addHouse();
void viewHouses();
void searchHouse();
void rentHouse();
void deleteHouse();
void modifyHouse();

void createAccount();
int userLogin();

// === Main Function ===
int main() {
    int choice;
    do {
        clearScreen();
        printf("===== 🏠 Welcome to House Rental System 🏠 =====\n");
        printDivider();
        printf("1. 🔐 Admin Login\n");
        printf("2. 👤 User Login\n");
        printf("3. 📝 Create User Account\n");
        printf("4. ❌ Exit\n");
        printDivider();
        printf("Enter your choice ➤ ");
        scanf("%d", &choice);
        getchar(); // flush newline

        switch(choice) {
            case 1:
                if (adminLogin()) {
                    adminMenu();
                } else {
                    printf(RED "❌ Login failed. Access denied.\n" RESET);
                    pause();
                }
                break;
            case 2:
                if (userLogin()) {
                    userMenu();
                } else {
                    pause();
                }
                break;
            case 3:
                createAccount();
                pause();
                break;
            case 4:
                printf(GREEN "👋 Exiting system. Goodbye!\n" RESET);
                break;
            default:
                printf(YELLOW "⚠️ Invalid choice. Try again.\n" RESET);
                pause();
        }
    } while (choice != 4);

    return 0;
}

// === Admin Login ===
int adminLogin() {
    char username[20], password[20];
    clearScreen();
    printf("=== 🔐 Admin Login ===\n");
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    return (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0);
}

// === Admin Menu ===
void adminMenu() {
    int choice;
    do {
        clearScreen();
        printf("===== 👮 Admin Panel =====\n");
        printDivider();
        printf("1. ➕ Add House\n");
        printf("2. 🗑️ Delete House\n");
        printf("3. ✏️ Modify House\n");
        printf("4. 📋 View All Houses\n");
        printf("5. 🔙 Logout\n");
        printDivider();
        printf("Enter your choice ➤ ");
        scanf("%d", &choice);
        getchar(); // flush newline
        switch(choice) {
            case 1: addHouse(); pause(); break;
            case 2: deleteHouse(); pause(); break;
            case 3: modifyHouse(); pause(); break;
            case 4: viewHouses(); pause(); break;
            case 5: printf("🔒 Logging out...\n"); pause(); break;
            default: printf(YELLOW "⚠️ Invalid option.\n" RESET); pause();
        }
    } while(choice != 5);
}

// === User Menu ===
void userMenu() {
    int choice;
    do {
        clearScreen();
        printf("===== 👤 User Panel =====\n");
        printDivider();
        printf("1. 📋 View All Houses\n");
        printf("2. 🔍 Search House by ID\n");
        printf("3. 🏠 Rent a House\n");
        printf("4. 🔙 Back to Main Menu\n");
        printDivider();
        printf("Enter your choice ➤ ");
        scanf("%d", &choice);
        getchar(); // flush newline
        switch(choice) {
            case 1: viewHouses(); pause(); break;
            case 2: searchHouse(); pause(); break;
            case 3: rentHouse(); pause(); break;
            case 4: printf("🔙 Returning to main menu...\n"); pause(); break;
            default: printf(YELLOW "⚠️ Invalid option.\n" RESET); pause();
        }
    } while(choice != 4);
}

// === Create Account ===
void createAccount() {
    FILE *fp = fopen(USER_FILE, "ab");
    if (!fp) {
        printf(RED "❌ Error opening user file.\n" RESET);
        return;
    }

    User newUser;
    clearScreen();
    printf("=== 📝 Create New Account ===\n");
    printf("Enter username: ");
    fgets(newUser.username, sizeof(newUser.username), stdin);
    newUser.username[strcspn(newUser.username, "\n")] = 0;

    printf("Enter password: ");
    fgets(newUser.password, sizeof(newUser.password), stdin);
    newUser.password[strcspn(newUser.password, "\n")] = 0;

    fwrite(&newUser, sizeof(User), 1, fp);
    fclose(fp);
    printf(GREEN "✔️ Account created successfully!\n" RESET);
}

// === User Login ===
int userLogin() {
    char uname[30], pword[30];
    User u;
    int found = 0;

    FILE *fp = fopen(USER_FILE, "rb");
    if (!fp) {
        printf(RED "❌ No user data found. Please create an account first.\n" RESET);
        return 0;
    }

    clearScreen();
    printf("=== 👤 User Login ===\n");
    printf("Username: ");
    fgets(uname, sizeof(uname), stdin);
    uname[strcspn(uname, "\n")] = 0;

    printf("Password: ");
    fgets(pword, sizeof(pword), stdin);
    pword[strcspn(pword, "\n")] = 0;

    while (fread(&u, sizeof(User), 1, fp)) {
        if (strcmp(u.username, uname) == 0 && strcmp(u.password, pword) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found) {
        printf(GREEN "✔️ Login successful!\n" RESET);
        return 1;
    } else {
        printf(RED "❌ Invalid username or password.\n" RESET);
        return 0;
    }
}

// === Add House ===
void addHouse() {
    House h;
    FILE *fp = fopen(HOUSE_FILE, "ab");
    if (!fp) {
        printf(RED "❌ Error opening file.\n" RESET);
        return;
    }
    clearScreen();
    printf("=== ➕ Add New House ===\n");
    printf("Enter House ID: ");
    scanf("%d", &h.id); getchar();
    printf("Enter Owner Name: ");
    fgets(h.owner, sizeof(h.owner), stdin);
    h.owner[strcspn(h.owner, "\n")] = 0;
    printf("Enter Address: ");
    fgets(h.address, sizeof(h.address), stdin);
    h.address[strcspn(h.address, "\n")] = 0;
    printf("Enter Monthly Rent: ");
    scanf("%f", &h.rent); getchar();
    h.is_rented = 0;
    fwrite(&h, sizeof(House), 1, fp);
    fclose(fp);
    printf(GREEN "✔️ House added successfully!\n" RESET);
}

// === View Houses ===
void viewHouses() {
    House h;
    FILE *fp = fopen(HOUSE_FILE, "rb");
    if (!fp) {
        printf(YELLOW "⚠️ No house data found.\n" RESET);
        return;
    }
    clearScreen();
    printf("=== 📋 All Available Houses ===\n");
    printDivider();
    while (fread(&h, sizeof(House), 1, fp)) {
        printf("ID: %d | Owner: %s | Address: %s | Rent: %.2f | Status: %s\n",
               h.id, h.owner, h.address, h.rent,
               h.is_rented ? "Rented" : "Available");
    }
    fclose(fp);
}

// === Search House ===
void searchHouse() {
    int id, found = 0;
    House h;
    FILE *fp = fopen(HOUSE_FILE, "rb");
    if (!fp) {
        printf(RED "❌ File error.\n" RESET);
        return;
    }
    clearScreen();
    printf("Enter House ID to search: ");
    scanf("%d", &id); getchar();

    while (fread(&h, sizeof(House), 1, fp)) {
        if (h.id == id) {
            printf(GREEN "✔️ House Found:\n" RESET);
            printf("Owner: %s\nAddress: %s\nRent: %.2f\nStatus: %s\n",
                   h.owner, h.address, h.rent, h.is_rented ? "Rented" : "Available");
            found = 1;
            break;
        }
    }
    if (!found) printf(RED "❌ House not found.\n" RESET);
    fclose(fp);
}

// === Rent House ===
void rentHouse() {
    int id, found = 0;
    House h;
    FILE *fp = fopen(HOUSE_FILE, "rb+");

    if (!fp) {
        printf(RED "❌ Error opening file.\n" RESET);
        return;
    }

    clearScreen();
    printf("Enter House ID to rent: ");
    scanf("%d", &id); getchar();

    while (fread(&h, sizeof(House), 1, fp)) {
        if (h.id == id) {
            found = 1;
            if (h.is_rented) {
                printf(YELLOW "⚠️ House is already rented.\n" RESET);
            } else {
                h.is_rented = 1;
                fseek(fp, -sizeof(House), SEEK_CUR);
                fwrite(&h, sizeof(House), 1, fp);
                printf(GREEN "✔️ House rented successfully!\n" RESET);
            }
            break;
        }
    }

    if (!found) printf(RED "❌ House ID not found.\n" RESET);
    fclose(fp);
}

// === Delete House ===
void deleteHouse() {
    int id, found = 0;
    House h;
    FILE *fp = fopen(HOUSE_FILE, "rb");
    FILE *temp = fopen("temp.txt", "wb");

    if (!fp || !temp) {
        printf(RED "❌ File error.\n" RESET);
        return;
    }

    clearScreen();
    printf("Enter House ID to delete: ");
    scanf("%d", &id); getchar();

    while (fread(&h, sizeof(House), 1, fp)) {
        if (h.id == id) {
            found = 1;
            continue;
        }
        fwrite(&h, sizeof(House), 1, temp);
    }
    fclose(fp);
    fclose(temp);
    remove(HOUSE_FILE);
    rename("temp.txt", HOUSE_FILE);

    if (found)
        printf(GREEN "✔️ House deleted.\n" RESET);
    else
        printf(RED "❌ House ID not found.\n" RESET);
}

// === Modify House ===
void modifyHouse() {
    int id, found = 0;
    House h;
    FILE *fp = fopen(HOUSE_FILE, "rb+");

    if (!fp) {
        printf(RED "❌ File error.\n" RESET);
        return;
    }
    clearScreen();
    printf("Enter House ID to modify: ");
    scanf("%d", &id); getchar();

    while (fread(&h, sizeof(House), 1, fp)) {
        if (h.id == id) {
            found = 1;
            printf("Enter new Owner Name: ");
            fgets(h.owner, sizeof(h.owner), stdin);
            h.owner[strcspn(h.owner, "\n")] = 0;

            printf("Enter new Address: ");
            fgets(h.address, sizeof(h.address), stdin);
            h.address[strcspn(h.address, "\n")] = 0;

            printf("Enter new Rent: ");
            scanf("%f", &h.rent); getchar();

            fseek(fp, -sizeof(House), SEEK_CUR);
            fwrite(&h, sizeof(House), 1, fp);

            printf(GREEN "✔️ House details updated.\n" RESET);
            break;
        }
    }
    if (!found)
        printf(RED "❌ House ID not found.\n" RESET);
    fclose(fp);
}
