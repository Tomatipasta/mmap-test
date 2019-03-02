#include <stdio.h>
#ifdef __APPLE__
        #include <sys/uio.h>
#else
        #include <sys/io.h>
#endif
#include <fcntl.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

typedef struct node {
    char * val;
    struct node * next;
} node_t;

long getMicrotime() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);

    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

short isAnagram(char *s1, const char *s2) {
  int ha[256] = {0};

  while(*s1 && *s2) {
    ha[*s1++ + 128]++;
    ha[*s2++ + 128]--;
  }

  for(int i = 0; i < 256; ++i) {
    if(ha[i]) return 0;
  }

  return 1;
}

int main(int argc, char const *argv[])
{
    long start = getMicrotime();

    unsigned char *f;
    int size, current_word_length = 0, current_word_hash = 0;
    const char * target_word = argv[2];
    int target_word_hash = 0, target_word_length = strlen(target_word);
    struct stat s;
    const char * file_name = argv[1];
    int fd = open(argv[1], O_RDONLY);
    node_t * head = malloc(sizeof(node_t));
    node_t * current = head;
    head->val = NULL;
    head->next = NULL;

    const char * i = target_word;
    while (*i) target_word_hash += *i++;

    int status = fstat(fd, & s);
    size = s.st_size;

    f = (char *) mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    for (int i = 0; i < size; i++) {
        current_word_length += 1;
        current_word_hash += f[i];

        /* Line break */
        if (f[i] == 10) {
        if (
            target_word_length == current_word_length - 2 /* CRLF */
            && target_word_hash == current_word_hash - 23 /* LF = 10 + 13 */
        ) {
            char * word = malloc(current_word_length);
            memcpy(word, &f[i - current_word_length + 1], current_word_length - 2);
            word[current_word_length - 2] = '\0';
            if (isAnagram(word, target_word)) {
                current->val = word;
                current->next = malloc(sizeof(node_t));
                current = current->next;
                current->next = NULL;
                current->val  = NULL;
            }
        }

        current_word_length = 0;
        current_word_hash = 0;
        }
    }

    long duration = getMicrotime() - start;
    printf("%ld", duration);
    current = head;
    while (current && current->val) {
    	printf(",%s", current->val);
	    current = current->next;
    }
    printf("\n");

    return 0;
}

