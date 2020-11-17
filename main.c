#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>


int randfile;

int random() {
    int *toReturn;
    int check = read(randfile, toReturn, sizeof(toReturn));
    if (check == -1) {
        return -1;
    } else {
        return *toReturn;
    }
}

void readArray(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("\trandom %d: %d\n", i, *(arr + i));
    }
}


int main() {

    randfile = open("/dev/random", O_RDONLY);
    if (randfile == -1) {
        printf("errno: %d - error: %s\n", errno, strerror(errno));
        return 0;
    }

    int arr[10];
    int i;
    for (i = 0; i < 10; i++) {
        int getRandom = random();
        if (getRandom == -1) {
            printf("Failed to get random %d\n", i);
            printf("errno: %d - error: %s\n", errno, strerror(errno));
            return 0;
        }
        arr[i] = getRandom;
    }

    printf("Generating random numbers:\n");
    readArray(arr, 10);

    printf("\nWriting numbers to temp.txt...\n");
    int toWrite = open("nums", O_CREAT | O_WRONLY, 0664);
    int checkWrite = write(toWrite, arr, sizeof(arr));

    if (checkWrite == -1) {
        printf("Writing failed. errno: %d - %s\n", errno, strerror(errno));
        return 0;
    }

    printf("\n");
    close(toWrite);

    printf("Reading numbers from temp.txt...\n");
    int newArr[10];
    int toRead = open("nums", O_RDONLY);

    if (toRead == -1) {
        printf("Failed to read from file. errno: %d - %s\n", errno, strerror(errno));
        return 0;
    }

    int checkRead = read(toRead, newArr, sizeof(newArr));
    
    if (checkRead == -1) {
        printf("Reading failed. errno: %d - %s\n", errno, strerror(errno));
        return 0;
    }
    printf("\n");

    printf("Verifying numbers are the same:\n");
    readArray(newArr, 10);

    close(randfile);
    close(toRead);
}

