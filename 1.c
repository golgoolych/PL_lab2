#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
char* words[7];
int cnt_words;
char* result;
char buf[512];
char letters[10];
int cnt_uniq;
int value[26];
int word_len[7];
int result_len;
int used[10];
int begin_zero[26];
int parse(char* line) {
    strcpy(buf, line);
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
    for (int i = 0; i < cnt_words; i++)
        word_len[i] = strlen(words[i]);
    result_len = strlen(result);
    return 0;
}
int word_value(char* word) {
    int val = 0;
    for (int i = 0; word[i] != '\0'; i++) {
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
/*
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
        if (word_len[i] > 1 && value[words[i][0] - 'A'] == 0) {
            return false;
        }
    }
    if (result_len > 1 && value[result[0] - 'A'] == 0) {
        return false;
    }
*/
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
int solve(int index) {
    if (index == cnt_uniq) {
        if (check()) {
            print_solution();
            return 1;
        }
        return 0;
    }
    int c = letters[index];
    for (int i = 0; i < 10; i++) {
        if (used[i]) continue;
        if (i == 0 && begin_zero[c - 'A']) continue;
        used[i] = 1;
        value[c - 'A'] = i;
        if (solve(index + 1)) return 1;
        value[c - 'A'] = -1;
        used[i] = 0;
    }
    return 0;

}
int main(void) {
    FILE* f = fopen("tests.txt", "r");
    if (!f) {
        printf("file error");
        return 1;
    }
    char line[512];
    while (fgets(line, sizeof line, f)) {
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0') continue;

        printf("%s\n", line);

        if (parse(line)) continue;
        int alphavit[26] = { 0 };
        cnt_uniq = 0;
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
      
        memset(begin_zero, 0, sizeof begin_zero);
        for (int i = 0; i < cnt_words; i++) {
            if (word_len[i] > 1) {
                begin_zero[words[i][0] - 'A'] = 1;
            }
        }
        if (result_len > 1)
            begin_zero[result[0] - 'A'] = 1;
        for (int i = 0; i < cnt_uniq - 1; i++) {
            for (int j = i + 1; j < cnt_uniq; j++) {
                int pr_1 = 100 * begin_zero[letters[i] - 'A'] + alphavit[letters[i] - 'A'];
                int pr_2 = 100 * begin_zero[letters[j] - 'A'] + alphavit[letters[j] - 'A'];
                if (pr_2 > pr_1) {
                    char t = letters[i];
                    letters[i] = letters[j];
                    letters[j] = t;
                }
            }
        }
        if (cnt_uniq > 10) {
            printf("net resheniy");
            continue;
        }   
        memset(value, -1, sizeof value);
        memset(used, 0, sizeof used);
        clock_t start = clock();
        solve(0);
        clock_t end = clock();
        double seconds = (double)(end - start) / CLOCKS_PER_SEC;
        printf("work time: %f\n", seconds);
    }
    fclose(f);
    return 0;
}