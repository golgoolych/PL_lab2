#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
char* words[7];
int cnt_words;
char* result;
char buf[512];
char letters[10];
int cnt_uniq;
int value[26];
int parse(void) {
    if (!fgets(buf, sizeof buf, stdin)) return 1;
    buf[strcspn(buf, "\n")] = '\0';

    char* p = strchr(buf, '=');
    if (!p) return 1;
    *(p - 1) = '\0';

    result = p + 2;
    cnt_words = 0;
    char* element = strtok(buf, " + ");
    while (element != NULL) {
        words[cnt_words++] = element;
        element = strtok(NULL, " + ");
    }
    return 0;
}
int word_value(char* word) {
    int val = 0;
    for (int i = 0; i < strlen(word); i++) {
        val = val * 10 + value[word[i] - 'A'];
    }
    return val;
}
void print_solution(void) {
    for (int i = 0; i < cnt_words; i++) {
        printf("%d", word_value(words[i]));
        if (i < cnt_words - 1) printf(" + ");
    }
    printf(" = %d\n", word_value(result));
}
bool check() {
//dubl
    int for_check[10] = { 0 };
    for (int i = 0; i < cnt_uniq; i++) {
        int d = value[letters[i] - 'A'];
        if (for_check[d] == 1) {
            return false;
        }
        for_check[d] = 1;
    }
    // 0 ved
    for (int i = 0; i < cnt_words; i++) {
        if (strlen(words[i]) > 1 && value[words[i][0] - 'A'] == 0) {
            return false;
        }
    }
    if (strlen(result) > 1 && value[result[0] - 'A'] == 0) {
        return false;
    }
    //check znach
    int val_left = 0;
    for (int i = 0; i < cnt_words; i++) {
        val_left += word_value(words[i]);
    }
    if (val_left != word_value(result)) {
        return false;
    }
    return true;
}
void solve(int index) {
    if (index == cnt_uniq) {
        if (check()) {
            print_solution();
            exit(0);
        }
        return;
    }
    int c = letters[index];
    for (int i = 0; i < 10; i++) {
        value[c - 'A'] = i;
        solve(index + 1);
        value[c - 'A'] = -1;
    }

}
int main(void) {
    if (parse()) return 1;
    int alphavit[26] = { 0 };
    for (int i = 0; i < cnt_words; i++) {
        for (int j = 0; words[i][j] != '\0'; j++) {
            alphavit[words[i][j] - 'A'] += 1;
        }
    }
    for (int j = 0; result[j] != '\0'; j++) {
        alphavit[result[j] - 'A'] += 1;
    }
    for (int i = 0; i < 26; i++) {
        if (alphavit[i] != 0) {
            letters[cnt_uniq++] = 'A' + i;
        }
    }
    if (cnt_uniq > 10) {
        printf("net resheniy");
        return 1;
    }
    memset(value, -1, sizeof value);
    solve(0);

    return 0;
}