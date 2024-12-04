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
    char title[50];
    char borrowDate[15];
    char returnDate[15];
} BorrowRecord;

// Login function
int login(User users[], int numUsers, User *currentUser); // Aki x

// Menu functions
void librarianMenu(Book books[], int *numBooks, User users[], int *numUsers, BorrowRecord records[], int *numRecords); // Aki x
void memberMenu(Book books[], int numBooks, User users[], int numUsers, BorrowRecord records[], int numRecords, int userId); // Aki x

// Book management functions
void addBook(Book books[], int *numBooks); // Harry
void displayBooks(const Book books[], int *numBooks); // Harry
void viewOverdueBooks(Book books[], int *numBooks, BorrowRecord records[], int *numRecords); // Aki
void updateBook(Book books[], int *numBooks, const char *title); // Harry
void removeBook(Book books[], int *numBooks, const char *title); // Harry
void searchBook(const Book books[], int *numBooks, const char *searchTerm, int searchOption); // Aki x

// Borrow and return functions
void borrowBook(Book books[], int numBooks, User users[], int numUsers, BorrowRecord records[], int *numRecords); // Harry
void returnBook(Book books[], int numBooks, BorrowRecord records[], int numRecords); // Harry
void viewBorrowHistory(const BorrowRecord records[], int numRecords, int userId, const Book books[], int numBooks); // Aki x
// Miscellaneous
void buffer(); // Aki x
int isAlphaString(const char *str); // Aki x 
void toLowerCase(const char *input, char *output); // Aki x

int main() {
    User users[MAX_USERS] = {{1, "member", "akina", "member123", "Akina Alegre", "akina.alegre@gmail.com", 0}, {-1, "librarian", "harry","123","Harry Loquias", "harry.loquias@gmail.com", 0}};
    int numUsers = 2;

    // Sample Books
    Book books[MAX_BOOKS] = {
        {"The Great Gatsby", "F. Scott Fitzgerald", "Fiction", "9780743273565", 1925, 0, 1, ""},
        {"1984", "George Orwell", "Dystopian", "9780451524935", 1949, 0, 1, ""},
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
                    memberMenu(books, numBooks, users, numUsers, records, numRecords, currentUser.userId);
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
        printf("[0] Logout\n");
        printf("Enter your choice: ");

        int choice, choice2, choice3;  
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
                    system("cls");
    	            printf("*HULAM-BASA LIBRARY EDITS*\n\n");
                    addBook(books, numBooks);
                    // Harry
                    break;
                case 2:
                    displayBooks(books, numBooks);
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
                	choice2 = 1;
					while(choice2 != 0) {
				        system("cls");
	    	            printf("*HULAM-BASA LIBRARY EDITS*\n\n");
	    	            
	    	            printf("[1] Display Books\n");
	    	            printf("[2] Edit a book\n");
	    	            printf("[0] Go Back\n");
	    	            printf("Enter your choice: ");

				        int scanResult = scanf("%d", &choice2);
				
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
				        
				        switch (choice2) {
				        	case 1:                     
								displayBooks(books, numBooks);
					        	break;
				        	
				        	case 2:
				        		printf("Enter ISBN: ");
				        		char isbn[20];
				        		scanf("%s", isbn);
				                updateBook(books, numBooks, isbn);
				                break;
				                
				            case 0: 
				                break;
				                
				            default:
		                   		printf("Please enter a valid choice.\n");
	                    		buffer();			            	
						}
					}  	
                    // Harry
                    break;
                case 5:
                    while(choice3 != 0) {
				        system("cls");
	    	            printf("*HULAM-BASA LIBRARY EDITS*\n\n");
	    	            
	    	            printf("[1] Display Books\n");
	    	            printf("[2] Delete a book\n");
	    	            printf("[0] Go Back\n");
	    	            printf("Enter your choice: ");

				        int scanResult = scanf("%d", &choice3);
				
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
						
						switch (choice3) {
				        	case 1:                     
								displayBooks(books, numBooks);
					        	break;
				        	
				        	case 2:
				        		printf("Enter ISBN: ");
				        		
				        		char isbn[20];
				        		scanf("%s", isbn);
				                removeBook(books, numBooks, isbn);
				                break;
				                
				            case 0: 
				                break;
				                
				            default:
		                   		printf("Please enter a valid choice.\n");
	                    		buffer();			            	
						}
					}  	
                    // Harry
                    break;
                case 6:
                	viewOverdueBooks(books, numBooks, records, numRecords);
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

void memberMenu(Book books[], int numBooks, User users[], int numUsers, BorrowRecord records[], int numRecords, int userId) {

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
	
	    int choice, choice2, choice3;
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
                displayBooks(books, numBooks);
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
				while (choice2 != 0) {
				    system("cls");
				    printf("*HULAM-BASA LIBRARY - BORROW BOOKS*\n\n");
				    printf("[1] Display Available Books\n");
				    printf("[2] Borrow a Book\n");
				    printf("[0] Go Back\n");
				    printf("Enter your choice: ");
				
				    int scanResult = scanf("%d", &choice2);
				
				    if (scanResult != 1) {
				        printf("\nPlease enter a valid choice.\n");
				        while ((getchar()) != '\n'); 
				        buffer();
				        continue;
				    } else if (getchar() != '\n') {
				        printf("\nPlease enter a valid choice.\n");
				        while ((getchar()) != '\n'); 
				        buffer();
				        continue;
				    }
				
				    switch (choice2) {
				        case 1: 
				            displayBooks(books, numBooks);
				            break;
				
				        case 2: {
				            // Borrow a book
				            borrowBook(books, numBooks, users, numUsers, records, &numRecords);
				            break;
				        }
				
				        case 0: 
				            // Go back to the previous menu
				            break;
				
				        default:
				            printf("\nPlease enter a valid choice.\n");
				            buffer();
				            break;
				    }
				}
                // Harry
                break;
            case 4:
				while (choice3 != 0) {
				    system("cls");
				    printf("*HULAM-BASA LIBRARY - BORROW BOOKS*\n\n");
				    printf("[1] Display Available Books\n");
				    printf("[2] Borrow a Book\n");
				    printf("[0] Go Back\n");
				    printf("Enter your choice: ");
				
				    int scanResult = scanf("%d", &choice2);
				
				    if (scanResult != 1) {
				        printf("\nPlease enter a valid choice.\n");
				        while ((getchar()) != '\n'); 
				        buffer();
				        continue;
				    } else if (getchar() != '\n') {
				        printf("\nPlease enter a valid choice.\n");
				        while ((getchar()) != '\n'); 
				        buffer();
				        continue;
				    }
				
				    switch (choice3) {
				        case 1: 
				            displayBooks(books, numBooks);
				            break;
				
				        case 2: {
				            // Borrow a book
							returnBook(books, numBooks, records, numRecords); 
				            break;
				        }
				
				        case 0: 
				            // Go back to the previous menu
				            break;
				
				        default:
				            printf("\nPlease enter a valid choice.\n");
				            buffer();
				            break;
				    }
				}            
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
        if (*numBooks >= MAX_BOOKS) {
        printf("Book capacity reached! Cannot add more books.\n");
        buffer();
        return;
    }

    printf("\n*ADD NEW BOOK*\n");

    Book newBook;
    printf("Enter title: ");
    fgets(newBook.title, sizeof(newBook.title), stdin);
    strtok(newBook.title, "\n"); // Remove newline character

    printf("Enter author: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    strtok(newBook.author, "\n");

    printf("Enter genre: ");
    fgets(newBook.genre, sizeof(newBook.genre), stdin);
    strtok(newBook.genre, "\n");

    printf("Enter ISBN: ");
    scanf("%s", newBook.isbn);

    printf("Enter publication year: ");
    scanf("%d", &newBook.publicationYear);

    newBook.isAvailable = 1; // New books are always available
    newBook.borrowedBy = -1;
    strcpy(newBook.dueDate, "");

    books[*numBooks] = newBook;
    (*numBooks)++;

    printf("Book added successfully!\n");
    buffer();
} // Harry

void displayBooks(const Book books[], int *numBooks) {
        if (*numBooks == 0) {
        printf("No books available.\n");
        buffer();
        return;
    }

    printf("\n*BOOK LIST*\n");
    for (int i = 0; i < *numBooks; i++) {
        printf("\nBook %d:\n", i + 1);
        printf("Title: %s\n", books[i].title);
        printf("Author: %s\n", books[i].author);
        printf("Genre: %s\n", books[i].genre);
        printf("ISBN: %s\n", books[i].isbn);
        printf("Publication Year: %d\n", books[i].publicationYear);
        printf("Availability: %s\n", books[i].isAvailable ? "Available" : "Borrowed");
        if (!books[i].isAvailable) {
            printf("Borrowed By User ID: %d\n", books[i].borrowedBy);
            printf("Due Date: %s\n", books[i].dueDate);
        }
    }
    buffer();
} // Harry

int isOverdue(const char *dueDate) {
    struct tm tm_dueDate = {0};
    time_t now;
    double seconds;

    time(&now);

    sscanf(dueDate, "%d-%d-%d", &tm_dueDate.tm_year, &tm_dueDate.tm_mon, &tm_dueDate.tm_mday);

    tm_dueDate.tm_year -= 1900;
    tm_dueDate.tm_mon -= 1;

    tm_dueDate.tm_hour = 23;
    tm_dueDate.tm_min = 59;
    tm_dueDate.tm_sec = 59;

    time_t due_time = mktime(&tm_dueDate);

    seconds = difftime(now, due_time);

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


void updateBook(Book books[], int *numBooks, const char *isbn) {
    for (int i = 0; i < *numBooks; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) { // Compare strings directly
            printf("\n*UPDATE BOOK DETAILS*\n");
            printf("Current Title: %s\n", books[i].title);

            // Rest of the update logic remains the same
            printf("Enter new title (leave blank to keep current): ");
            getchar(); // Consume leftover newline from previous input
            char newTitle[100];
            fgets(newTitle, sizeof(newTitle), stdin);
            if (strcmp(newTitle, "\n") != 0) {
                strtok(newTitle, "\n"); // Remove trailing newline
                strcpy(books[i].title, newTitle);
            }

            printf("Current Author: %s\n", books[i].author);
            printf("Enter new author (leave blank to keep current): ");
            char newAuthor[50];
            fgets(newAuthor, sizeof(newAuthor), stdin);
            if (strcmp(newAuthor, "\n") != 0) {
                strtok(newAuthor, "\n");
                strcpy(books[i].author, newAuthor);
            }

            printf("Current Genre: %s\n", books[i].genre);
            printf("Enter new genre (leave blank to keep current): ");
            char newGenre[30];
            fgets(newGenre, sizeof(newGenre), stdin);
            if (strcmp(newGenre, "\n") != 0) {
                strtok(newGenre, "\n");
                strcpy(books[i].genre, newGenre);
            }

            printf("Current Publication Year: %d\n", books[i].publicationYear);
            printf("Enter new publication year (enter -1 to keep current): ");
            int newYear;
            scanf("%d", &newYear);
            if (newYear != -1) {
                books[i].publicationYear = newYear;
            }

            printf("Book updated successfully!\n");
            buffer();
            return;
        }
    }

    printf("Book with ISBN: %s not found.\n", isbn); // Use `title` directly
    buffer();
}


void removeBook(Book books[], int *numBooks, const char *isbn) {
        for (int i = 0; i < *numBooks; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) {
            for (int j = i; j < *numBooks - 1; j++) {
                books[j] = books[j + 1]; // Shift books left
            }
            (*numBooks)--;
            printf("Book removed successfully!\n");
            buffer();
            return;
        }
    }

    printf("Book with ISBN %s not found.\n", isbn);
    buffer();
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
	char isbn[20];
    int userId, bookFound = 0, userFound = 0;

    printf("\nEnter your User ID: ");
    scanf("%d", &userId);

    for (int i = 0; i < numUsers; i++) {
        if (users[i].userId == userId) {
            userFound = 1;
            if (users[i].booksBorrowed >= 5) { // Maximum 5 books allowed to be borrowed
                printf("\nYou have reached the borrowing limit.\n");
                return;
            }
            break;
        }
    }

    if (!userFound) {
        printf("\nInvalid User ID.\n");
        return;
    }

    printf("\nEnter the ISBN of the book you want to borrow: ");
    scanf("%s", isbn);

    for (int i = 0; i < numBooks; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) {
            bookFound = 1;
            if (books[i].isAvailable == 0) {
                printf("\nThe book is already borrowed.\n");
                return;
            }

            // Update book status
            books[i].isAvailable = 0;
            books[i].borrowedBy = userId;

            // Generate due date (15 days from today)
            time_t t = time(NULL);
            struct tm due = *localtime(&t);
            due.tm_mday += 15;
            mktime(&due);
            sprintf(books[i].dueDate, "%d-%02d-%02d", due.tm_year + 1900, due.tm_mon + 1, due.tm_mday);

            // Create a borrow record
            BorrowRecord newRecord;
            newRecord.userId = userId;
            strcpy(newRecord.isbn, isbn);

            // Borrow date (today)
            struct tm borrow = *localtime(&t);
            sprintf(newRecord.borrowDate, "%d-%02d-%02d", borrow.tm_year + 1900, borrow.tm_mon + 1, borrow.tm_mday);

            // Return date (same as due date)
            strcpy(newRecord.returnDate, books[i].dueDate);

            records[*numRecords] = newRecord;
            (*numRecords)++;

            // Update user data
            for (int j = 0; j < numUsers; j++) {
                if (users[j].userId == userId) {
                    users[j].booksBorrowed++;
                    break;
                }
            }

            printf("\nBook borrowed successfully! Due Date: %s\n", books[i].dueDate);
            return;
        }
    }

    if (!bookFound) {
        printf("\nBook not found.\n");
    }
} // Harry

void returnBook(Book books[], int numBooks, BorrowRecord records[], int numRecords) {
	char isbn[20];

    int bookFound = 0;
    
    printf("\nEnter the ISBN of the book you want to borrow: ");
    scanf("%s", isbn);    

    for (int i = 0; i < numBooks; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) {
            bookFound = 1;

            if (books[i].isAvailable == 1) {
                printf("\nThis book is not currently borrowed.\n");
                return;
            }

            // Mark the book as available
            books[i].isAvailable = 1;
            books[i].borrowedBy = -1;
            strcpy(books[i].dueDate, "");

            for (int j = 0; j < numRecords; j++) {
                if (strcmp(records[j].isbn, isbn) == 0 && records[j].userId == books[i].borrowedBy) {
                    for (int k = j; k < numRecords - 1; k++) {
                        records[k] = records[k + 1];
                    }
                    numRecords--;
                    break;
                }
            }

            printf("\nBook returned successfully!\n");
            return;
        }
    }

    if (!bookFound) {
        printf("\nBook not found.\n");
    }
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

