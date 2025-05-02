#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// 문자열을 소문자로 변환하는 함수 (원본 문자열 변경)
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// 명시적 루프를 사용하여 "the" 개수 세기
long long search_with_loop(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    long long count = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    const char *search_term = "the";
    size_t search_len = strlen(search_term);

    // C에서는 fgets나 getline 사용이 일반적입니다. getline은 동적 할당을 관리해줍니다.
    // getline이 표준 C는 아니지만 POSIX 표준이며 많은 컴파일러(gcc, clang)에서 지원합니다.
    // 표준 C만을 사용하려면 fgets와 버퍼 재할당 로직이 필요합니다. 여기서는 getline을 사용합니다.
    while ((read = getline(&line, &len, file)) != -1) {
        // 줄 끝의 개행 문자 제거 (있다면)
        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0';
            read--;
        }

        // 현재 줄을 소문자로 변환
        char *lower_line = strdup(line); // 원본 line을 유지하기 위해 복사본 생성
        if (!lower_line) {
             fprintf(stderr, "Memory allocation failed\n");
             continue; // 다음 줄로 넘어감
        }
        to_lowercase(lower_line);

        // 현재 줄에서 "the" 찾기
        char *ptr = lower_line;
        while ((ptr = strstr(ptr, search_term)) != NULL) {
            count++;
            ptr += 1; // "the" 다음 위치부터 다시 검색 시작 (오버랩 방지)
                      // 만약 "thethe"를 2번으로 세려면 ptr += search_len;
                      // 여기서는 1칸만 이동하여 "theatre" 등도 고려될 수 있으나,
                      // 원문 의도는 단순 부분 문자열 검색으로 보이므로 ptr += 1; 사용
        }
        free(lower_line); // 복사본 메모리 해제
    }

    fclose(file);
    if (line) {
        free(line); // getline이 할당한 메모리 해제
    }

    return count;
}

int main() {
    const char *filename = "The_Adventures_of_Sherlock_Holmes.txt";
    clock_t start, end;
    double cpu_time_used;
    long long count;

    printf("Starting C benchmark...\n");

    // --- 루프 방식 실행 및 시간 측정 ---
    start = clock();
    count = search_with_loop(filename);
    end = clock();

    if (count != -1) {
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("[C Loop Version]\n");
        printf("Found '%s' %lld times.\n", "the", count);
        printf("Time taken: %f seconds\n", cpu_time_used);
    }

    return 0;
}