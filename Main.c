#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Member {
    char name[50];
    struct Member *left;
    struct Member *right;
    int commission;
} Member;

Member* createMember(const char name[]) {
    Member* newMember = (Member*)malloc(sizeof(Member));
    strcpy(newMember->name, name);
    newMember->left = NULL;
    newMember->right = NULL;
    newMember->commission = 0;
    return newMember;
}

Member* searchMember(Member* member, const char name[]) {
    if (member == NULL) return NULL;

    if (strcmp(member->name, name) == 0) {
        return member;
    }

    Member* foundMember = searchMember(member->left, name);
    if (foundMember != NULL) return foundMember;

    return searchMember(member->right, name);
}

void updateParentCommission(Member* member) {
    if (member == NULL) return;

    member->commission = 0;

    if (member->left != NULL) {
        member->commission += (member->left->commission * 0.10);
    }
    if (member->right != NULL) {
        member->commission += (member->right->commission * 0.10); 
    }

    if (member->left != NULL && member->right != NULL) {
        member->commission += 500; 
    }
}

void addDownline(Member* member, const char downlineName[], int position) {
    Member* existingMember = searchMember(member, downlineName);
    if (existingMember != NULL) {
        printf("Error: A member with the name %s already exists in the network.\n", downlineName);
        return;
    }

    if (position == 1) {
        if (member->left == NULL) {
            member->left = createMember(downlineName);
            printf("Downline %s added successfully under %s.\n", downlineName, member->name);
        } else {
            printf("Position already occupied by %s\n", member->left->name);
            return;
        }
    } else if (position == 2) {
        if (member->right == NULL) {
            member->right = createMember(downlineName);
            printf("Downline %s added successfully under %s.\n", downlineName, member->name);
        } else {
            printf("Position already occupied by %s\n", member->right->name);
            return;
        }
    } else {
        printf("Invalid position. Please enter 1 for left or 2 for right.\n");
        return;
    }

    updateParentCommission(member);
}

void displayDownlines(Member* member, int level) {
    if (member == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    printf("-> %s (P %d)\n", member->name, member->commission);

    displayDownlines(member->left, level + 1);
    displayDownlines(member->right, level + 1);
}

int totalCommission(Member* member) {
    if (member == NULL) return 0;
    return member->commission + totalCommission(member->left) + totalCommission(member->right);
}

int countMembers(Member* member) {
    if (member == NULL) return 0;
    return 1 + countMembers(member->left) + countMembers(member->right);
}

void displayNetwork(Member* member, int level) {
    if (member == NULL) return;

    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    printf("-> %s (P%d)\n", member->name, member->commission);
    displayNetwork(member->left, level + 1);
    displayNetwork(member->right, level + 1);
}

void freeNetwork(Member* member) {
    if (member == NULL) return;
    freeNetwork(member->left);
    freeNetwork(member->right);
    free(member);
}

void waitForUser () {
    printf("\nPress Enter to continue...\n");
    while (getchar() != '\n');
    getchar();
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    clearScreen();
    printf("\n==================================================\n");
    printf("|| Welcome to the Royale Company Network System ||\n");
    printf("==================================================\n\n");

    char recruiters[50];
    printf("Enter the name of the head of the Company: ");
    scanf("%s", recruiters);
    waitForUser ();

    Member* root = createMember(recruiters);
    int choice;
    char name[50];
    char downlineName[50];
    int position;

    while (1) {
        clearScreen();
        printf("\n==================== Royale Company Network Menu ====================\n");
        printf("1. Add New Member\n");
        printf("2. Display Downlines of a Member\n");
        printf("3. Display Entire Network Tree\n");
        printf("4. Display Total Commission of Network\n");
        printf("5. Count Total Members\n");
        printf("6. Exit\n");
        printf("=====================================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        waitForUser ();

        switch (choice) {
            case 1:
                clearScreen();
                printf("Enter the recruiter's name: ");
                scanf("%s", name);
                Member* member = searchMember(root, name);
                if (member == NULL) {
                    printf("Error: %s not found in the network.\n", name);
                } else {
                    printf("Enter the name of the new member to be recruited by %s: ", name);
                    scanf("%s", downlineName);
                    printf("Enter position (1 for left, 2 for right): ");
                    scanf("%d", &position);
                    addDownline(member, downlineName, position);
                    updateParentCommission(root); // Update head node commission
                }
                waitForUser ();
                break;

            case 2:
                clearScreen();
                printf("Enter name of the member: ");
                scanf("%s", name);
                member = searchMember(root, name);
                displayDownlines(member, 0);
                waitForUser ();
                break;

            case 3:
                clearScreen();
                printf("\n==================== Royale Company Network Tree ====================\n");
                displayNetwork(root, 0);
                printf("=====================================================================\n");
                waitForUser ();
                break;

            case 4:
                clearScreen();
                printf("Total Commission of the entire network: P%d\n", totalCommission(root));
                waitForUser ();
                break;

            case 5:
                clearScreen();
                printf("Total number of members in the network: %d\n", countMembers(root));
                waitForUser ();
                break;

            case 6:
                clearScreen();
                printf("Exiting the program. Thank you for using the Royale Company Network System!\n");
                freeNetwork(root);
                return 0;

            default:
                clearScreen();
                printf("Invalid choice. Please try again.\n");
                waitForUser ();
        }
    }
}