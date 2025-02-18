#include "../../Include.h"

int WordList(const char* key, int type) {
    printf("--------------Trying WordList------------\n");
    char dir[200];
    printf("\nEnter the directory for the wordlist files if available ");
    scanf("%199s", dir);  // Use a size specifier to prevent buffer overflow

    // Open the directory
    DIR *dp = opendir(dir);

    if (dp == NULL) {
        perror("Error opening directory");
        return 1;
    }

    struct dirent *entry;
    int result = 0; // Declare result outside the switch statement

    // Process each file in the directory
    while ((entry = readdir(dp))) {
        if (entry->d_type == DT_REG) { // Check if it's a regular file
            char file_path[512];
            snprintf(file_path, sizeof(file_path), "%s/%s", dir, entry->d_name);

            // Open the file
            FILE *file = fopen(file_path, "r");

            if (file == NULL) {
                perror("Error opening file");
                continue; // Move on to the next file
            }

            // Read the file's content
            printf("File: %s\n", entry->d_name);
            char line[512];

            while (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\n")] = '\0';  // Remove the trailing newline character
                switch (type) {
                    case 1:
                        result = MD5Hash(line, key);
                        break;
                    case 2:
                        result = SHA1Hash(line, key);
                        break;
                    case 3:
                        result = SHA256Hash(line, key);
                        break;
                    default:
                        printf("Invalid\n");
                }

                if (result == 1) {
                    printf("Password found in : %s\n", entry->d_name);
                    printf("Password : %s\n", line);
                    fclose(file);  // Close the file before exiting
                    closedir(dp);
                    return 0;
                }
            }

            fclose(file);
        }
    }

    closedir(dp);
    return 0;
}

