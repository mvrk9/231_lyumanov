#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

void replace_all(const char *pattern, const char *text, const char *replacement) {
    regex_t regex;
    regmatch_t match;

    if (regcomp(&regex, pattern, REG_EXTENDED)) {
        fprintf(stderr, "Invalid regular expression\n");
        return;
    }

    size_t result_size = 1024;
    char *result = malloc(result_size);
    if (!result) {
        perror("malloc");
        regfree(&regex);
        return;
    }

    size_t pos = 0;
    const char *current = text;

    while (!regexec(&regex, current, 1, &match, 0)) {
        size_t before_len = match.rm_so;
        size_t repl_len = strlen(replacement);


        if (pos + before_len + repl_len >= result_size) {
            result_size *= 2;
            result = realloc(result, result_size);
            if (!result) {
                perror("realloc");
                regfree(&regex);
                return;
            }
        }


        strncpy(result + pos, current, before_len);
        pos += before_len;

        strcpy(result + pos, replacement);
        pos += repl_len;

        current += match.rm_eo;
    }

    strcpy(result + pos, current);
    printf("Result: %s\n", result);

    free(result);
    regfree(&regex);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <regex> <text> <replacement>\n", argv[0]);
        return 1;
    }

    replace_all(argv[1], argv[2], argv[3]);
    return 0;
}
