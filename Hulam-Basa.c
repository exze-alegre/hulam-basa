#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>	
#include <time.h>

#define MAX_BOOKS 100
#define MAX_USERS 50
#define MAX_BORROW_RECORDS 50

typedef struct {
    char title[100];
    char author[50];
    char genre[30];
    char isbn[20];
    int publicationYear;
    int isAvailable;  // 1 for available, 0 for borrowed
    int borrowedBy;   // User ID (if borrowed); -1 if available
    char dueDate[15]; // Due date for return
} Book;

typedef struct {
    int userId;
    char role[50]; //librarian, or member
    char username[50];   // Username for login
    char password[50];   // Password for login
    char name[50];       // Full name of the user
    char email[50];
    int booksBorrowed; // Number of books currently borrowed
} User;

typedef struct {
    int userId;
    char isbn[20];
    char borrowDate[15];
    char returnDate[15];
} BorrowRecord;

// Login function
int login(User users[], int numUsers, User *currentUser); // Aki x

// Menu functions
void librarianMenu(Book books[], int *numBooks, User users[], int *numUsers, BorrowRecord records[], int *numRecords); // Aki x
void memberMenu(Book books[], int numBooks, BorrowRecord records[], int numRecords, int userId); // Aki x

// Book management functions
void addBook(Book books[], int *numBooks); // Harry
void displayBooks(const Book books[], int numBooks, int sortOption); // Harry
void viewOverdueBooks(Book books[], int *numBooks, BorrowRecord records[], int *numRecords); // Aki
void updateBook(Book books[], int numBooks, const char *isbn); // Harry
void removeBook(Book books[], int *numBooks, const char *isbn); // Harry
void searchBook(const Book books[], int *numBooks, const char *searchTerm, int searchOption); // Aki x

// Borrow and return functions
void borrowBook(Book books[], int numBooks, User users[], int numUsers, BorrowRecord records[], int *numRecords); // Harry
void returnBook(Book books[], int numBooks, BorrowRecord records[], int numRecords, const char *isbn); // Harry
void viewBorrowHistory(const BorrowRecord records[], int numRecords, int userId, const Book books[], int numBooks); // Aki x
// Miscellaneous
void buffer(); // Aki x
int isAlphaString(const char *str); // Aki x 
void toLowerCase(const char *input, char *output); // Aki x

int main() {
    User users[MAX_USERS] = {{1, "member", "akina", "member123", "Akina Alegre", "akina.alegre@gmail.com", 0}, {-1, "librarian", "harry","librarian123","Harry Loquias", "harry.loquias@gmail.com", 0}};
    int numUsers = 2;

    // Sample Books
    Book books[MAX_BOOKS] = {
        {"The Great Gatsby", "F. Scott Fitzgerald", "Fiction", "9780743273565", 1925, 0, -1, ""},
        {"1984", "George Orwell", "Dystopian", "9780451524935", 1949, 0, -1, ""},
        {"To Kill a Mockingbird", "Harper Lee", "Fiction", "9780061120084", 1960, 1, -1, ""}
    };
    int numBooks = 3;

    // Sample Borrow Records
    BorrowRecord records[MAX_BORROW_RECORDS] = {
        {1, "9780743273565", "2024-11-01", "2024-11-15"},
        {1, "9780451524935", "2024-11-05", "2024-11-19"}
    };
    int numRecords = 2;

    User currentUser;
    
    while (1) {
        system("cls");
        printf("*WELCOME TO HULAM-BASA LIBRARY MIS*\n\n");
        printf("[1] Login\n");
        printf("[0] Exit\n");
        printf("Enter your choice: ");
        
        int choice;
        int scanResult = scanf("%d", &choice);
    
        if (scanResult != 1) {
            printf("Please enter a valid choice.\n");
            while ((getchar()) != '\n');
            buffer();
            continue;
        } 
        else if (getchar() != '\n') {
            printf("Please enter a valid choice.\n");
            while ((getchar()) != '\n');
            buffer();
            continue;
        }
        
        switch(choice) {
            case 1:
                login(users, numUsers, &currentUser);
                if (strcmp(currentUser.role, "member") == 0) {
                    memberMenu(books, numBooks, records, numRecords, currentUser.userId);
                }
                else if (strcmp(currentUser.role, "librarian") == 0) {
                    librarianMenu(books, &numBooks, users, &numUsers, records, &numRecords);
                }
                break;
            case 0:
                printf("Exiting program...");
                exit(0);
            default:
                printf("Please enter a valid choice.\n");
                buffer();
                break;
        }
    }

    return 0;
}

int isAlphaString(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i])) {
            return 0;
        }
    }
    return 1; 
}

void toLowerCase(const char *input, char *output) {
    for (int i = 0; input[i] != '\0'; i++) {
        output[i] = tolower((unsigned char)input[i]);
    }
    output[strlen(input)] = '\0'; // Null-terminate the string
}

void buffer() {
    printf("\nPRESS ANY KEY TO CONTINUE...");
    _getch();
    system("cls");
}

int login(User users[], int numUsers, User *currentUser) {
    int valid;
    int attempts = 5;
    char inputUser[20], inputPass[20];
    
    while (attempts > 0) {
        valid = 1;
        
        system("cls");
        printf("*HULAM-BASA LIBRARY LOGIN*\n\n");
    
        printf("Enter your username: ");
        scanf("%s", inputUser);
        
        if (!isAlphaString(inputUser)) {
            printf("Invalid input, please enter only letters.");
            continue;
        }

        printf("Enter your password: ");
        scanf("%s", inputPass);
        
        for (int i = 0; i < numUsers; i++) {
            if (strcmp(users[i].username, inputUser) == 0 && strcmp(users[i].password, inputPass) == 0) {
                *currentUser = users[i];
                printf("\nSuccessfully Logged in!\n");
                buffer();
                return 1;
            }
        }
        attempts--;
        printf("Username or password incorrect. You have %d tries left.\n", attempts);
        buffer();
    } 
    
    printf("You have exceeded the maximum amount of attempts. Exiting program...");
    exit(0);
}

void librarianMenu(Book books[], int *numBooks, User users[], int *numUsers, BorrowRecord records[], int *numRecords) {
    while(1) {
        system("cls");
        printf("*HULAM-BASA LIBRARY*\n");
        printf("Welcome librarian!\n\n");
        printf("[1] Add a New Book\n");
        printf("[2] Display All Books\n");
        printf("[3] Search Book by Title/Author/ISBN\n");
        printf("[4] Update Book Details\n");
        printf("[5] Remove Books\n");
        printf("[6] View Overdue Books\n");
        printf("[8] View Member Borrowing History\n");
        printf("[0] Logout\n");
        printf("Enter your choice: ");
    
        int choice;
        int scanResult = scanf("%d", &choice);
    
        if (scanResult != 1) {
            printf("Please enter a valid choice.\n");
            while ((getchar()) != '\n');
            buffer();
            continue;
        } 
        else if (getchar() != '\n') {
            printf("Please enter a valid choice.\n");
            while ((getchar()) != '\n');
            buffer();
            continue;
        }
        
	    switch(choice) {
	        case 1:
	            // Add new book function
	            // Harry
	            break;
	        case 2:
	            // Display all books
	            // Harry
	            break;
	        case 3:	        	
	        	while (1) {
	        		system("cls");
			        printf("*HULAM-BASA LIBRARY SEARCH*\n\n");
					printf("Select a search option:\n");
					printf("[1] Title\n");
					printf("[2] Author\n");
					printf("[3] ISBN\n");
					printf("[0] Back\n");
					printf("Enter your option: ");
					
			        int searchOption;
			        int scanResult = scanf("%d", &searchOption);
			    
				    if (scanResult != 1) {
				        printf("Please enter a valid choice.\n");
				        
				        while ((getchar()) != '\n');
				        buffer();
				        continue;
				    } 
					else if (getchar() != '\n') {
			            printf("Please enter a valid choice.\n");
			
			            while ((getchar()) != '\n');
			        	buffer();
				        continue;
				    }
				    else if (searchOption == 0) {
				    	break;
					}
					printf("Enter your search term: ");
					
					char searchTerm[50];
					scanf("%s", searchTerm);
					searchBook(books, numBooks, &searchTerm, searchOption);
				}		
	            break;
	        case 4:
	            // Update book details
	            // Harry
	            break;
	        case 5:
	            // Remove books
	            // Harry
	            break;
	        case 6:
                viewOverdueBooks(books, numBooks, records, numRecords);
	            break;
	        case 7:
	            // Manage members
	            // Aki
	            break;
	        case 8:
	            // View member borrowing history
	            // Aki
	            break;
	        case 0:
	            // Logout
	            printf("Logging out...\n");
	            main();
	            break;
	        default:
	            printf("Please enter a valid choice.\n");
	            buffer();
	    }	
    }
}

void memberMenu(Book books[], int numBooks, BorrowRecord records[], int numRecords, int userId) {
	
	while(1) {
		system("cls");
	    printf("*HULAM-BASA LIBRARY*\n");
	    printf("Welcome user!\n\n");
	    printf("[1] Display All Books\n");
	    printf("[2] Search Book by Title/Author/ISBN\n");
	    printf("[3] Borrow A Book\n");
	    printf("[4] Return a Book\n");
	    printf("[5] View Borrowing History\n");
	    printf("[0] Logout\n");
	    printf("Enter your choice: ");
	
        int choice;
        int scanResult = scanf("%d", &choice);
    
	    if (scanResult != 1) {
	        printf("Please enter a valid choice.\n");
	        
	        while ((getchar()) != '\n');
	        buffer();
	        continue;
	    } 
		else if (getchar() != '\n') {
            printf("Please enter a valid choice.\n");

            while ((getchar()) != '\n');
        	buffer();
	        continue;
	    }
        
	    switch(choice) {
	        case 1:
	            // Display all Books
	            // Harry
	            break;
	        case 2:
	        	while (1) {
	        		system("cls");
			        printf("*HULAM-BASA LIBRARY SEARCH*\n\n");
					printf("Select a search option:\n");
					printf("[1] Title\n");
					printf("[2] Author\n");
					printf("[3] ISBN\n");
					printf("[0] Back\n");
					printf("Enter your option: ");
					
			        int searchOption;
			        int scanResult = scanf("%d", &searchOption);
			    
				    if (scanResult != 1) {
				        printf("Please enter a valid choice.\n");
				        
				        while ((getchar()) != '\n');
				        buffer();
				        continue;
				    } 
					else if (getchar() != '\n') {
			            printf("Please enter a valid choice.\n");
			
			            while ((getchar()) != '\n');
			        	buffer();
				        continue;
				    }
				    else if (searchOption == 0) {
				    	break;
					}
					printf("Enter your search term: ");
					
					char searchTerm[50];
					scanf("%s", searchTerm);
					searchBook(books, &numBooks, &searchTerm, searchOption);
				}		
	            break;
	        case 3:
	            // Borrow a book
	            // Harry
	            break;
	        case 4:
	            // Return a ook
	            // Harry
	            break;
			case 5:
			    viewBorrowHistory(records, numRecords, userId, books, numBooks);
			    break;
	        case 0:
	            // Logout
	            printf("Logging out...\n");
	            main();
	            break;
	        default:
	            printf("Please enter a valid choice.\n");
	            buffer();
	    }	
	}
}


// MENU OPTIONS

void addBook(Book books[], int *numBooks) {
	//code
} // Harry

void displayBooks(const Book books[], int numBooks, int sortOption) {
	//code
} // Harry

int isOverdue(const char *dueDate) {
    struct tm tm_dueDate = {0};
    time_t now;
    double seconds;

    // Get the current time
    time(&now);

    // Parse the dueDate string into year, month, and day
    sscanf(dueDate, "%d-%d-%d", &tm_dueDate.tm_year, &tm_dueDate.tm_mon, &tm_dueDate.tm_mday);

    // Adjust the year and month as tm_year is year since 1900, and tm_mon is 0-11
    tm_dueDate.tm_year -= 1900;
    tm_dueDate.tm_mon -= 1;

    // Set the time to the end of the day (23:59:59)
    tm_dueDate.tm_hour = 23;
    tm_dueDate.tm_min = 59;
    tm_dueDate.tm_sec = 59;

    // Convert the struct tm to time_t
    time_t due_time = mktime(&tm_dueDate);

    // Calculate the difference in seconds between now and the due date
    seconds = difftime(now, due_time);

    // If the due date is in the past, return 1 (overdue), otherwise return 0
    return seconds > 0;
} // Aki

void viewOverdueBooks(Book books[], int *numBooks, BorrowRecord records[], int *numRecords) {
    printf("\n*Overdue Books*\n");
    int foundOverdue = 0;

    for (int i = 0; i < *numRecords; i++) {
        for (int j = 0; j < *numBooks; j++) {
            if (strcmp(records[i].isbn, books[j].isbn) == 0) {
                if (isOverdue(records[i].returnDate)) {
                    printf("\nBook Title: %s\n", books[j].title);
                    printf("Borrowed By User ID: %d\n", records[i].userId);
                    printf("Due Date: %s\n", records[i].returnDate);
                    foundOverdue = 1;
                }
                break;
            }
        }
    }

    if (!foundOverdue) {
        printf("No overdue books found.\n");
    }
    buffer();
} // Aki


void updateBook(Book books[], int numBooks, const char *isbn) {
	// code
} // Harry

void removeBook(Book books[], int *numBooks, const char *isbn) {
	// code
} // Harry

void searchBook(const Book books[], int *numBooks, const char *searchTerm, int searchOption) {
    int found = 0; 
    char lowerSearchTerm[50]; 
    toLowerCase(searchTerm, lowerSearchTerm);
    
    printf("\nSearch Results:\n"); 

    for (int i = 0; i < *numBooks; i++) {
        int match = 0;
        char lowerField[50];

        switch (searchOption) {
            case 1: // Search by title
                toLowerCase(books[i].title, lowerField);
                if (strstr(lowerField, lowerSearchTerm)) {
                    match = 1;
                }
                break;

            case 2: // Search by author
                toLowerCase(books[i].author, lowerField);
                if (strstr(lowerField, lowerSearchTerm)) {
                    match = 1;
                }
                break;

			case 3: // Search by ISBN (exact match)
			    if (strcmp(books[i].isbn, searchTerm) == 0) {
			        match = 1;
			    }
			    break;

            default:
                printf("Invalid search option.\n");
                return;
        }       

        if (match) {
            found = 1;
            printf("Title: %s\nAuthor: %s\nGenre: %s\nISBN: %s\nPublication Year: %d\n%s\n",
               books[i].title,
               books[i].author,
               books[i].genre,
               books[i].isbn,
               books[i].publicationYear,
               books[i].isAvailable ? "Available" : "Borrowed");
               
               buffer();
        }
    }
    
    if (found == 0) {
        printf("No books found matching the search criteria.\n");
        buffer();
    }
    
} // Aki

void borrowBook(Book books[], int numBooks, User users[], int numUsers, BorrowRecord records[], int *numRecords) {
	// code
} // Harry

void returnBook(Book books[], int numBooks, BorrowRecord records[], int numRecords, const char *isbn) {
	// code
} // Harry

void viewBorrowHistory(const BorrowRecord records[], int numRecords, int userId, const Book books[], int numBooks) {
    int found = 0;
    printf("\n*BORROW HISTORY*\n");
    
    for (int i = 0; i < numRecords; i++) {
        if (records[i].userId == userId) {
            found = 1;
            printf("\nBorrowed Book:\n");
            printf("Book ISBN: %s\n", records[i].isbn);
            printf("Borrow Date: %s\n", records[i].borrowDate);
            printf("Return Date: %s\n", records[i].returnDate);

            for (int j = 0; j < numBooks; j++) {
                if (strcmp(books[j].isbn, records[i].isbn) == 0) {
                    printf("Title: %s\n", books[j].title);
                    printf("Author: %s\n", books[j].author);
                    printf("Genre: %s\n", books[j].genre);
                    printf("Publication Year: %d\n", books[j].publicationYear);
                    printf("Availability: %s\n", books[j].isAvailable ? "Available" : "Borrowed");
                    break; 
                }
            }

        }
    }

    if (!found) {
        printf("No borrow history found for this user.\n");
    }
    buffer();
} // Akina



