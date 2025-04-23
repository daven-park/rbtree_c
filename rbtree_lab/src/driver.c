#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int c, i;
    c = 1;
    // 레드블랙 트리 시작
    rbtree *tree = new_rbtree();
    printf("1: 레드블랙 트리에 정수 삽입\n");
    printf("2: 레드블랙 트리에서 정수 검색\n");
    printf("3: 레드블랙 트리에서 정수 삭제\n");
    printf("4: 레드블랙 트리에서 최소값 검색\n");
    printf("5: 레드블랙 트리에서 최대값 검색\n");
    printf("6: 레드블랙 트리에서 정수 배열로 변환\n");
    printf("7: 레드블랙 트리 삭제\n");
    printf("0: 종료\n");
    while (c != 0)
    {
        printf("명령어를 입력하세요: ");
        scanf("%d", &c);
        switch (c)
        {
        case 1:
            printf("삽입할 정수를 입력하세요: ");
            scanf("%d", &i);
            rbtree_insert(tree, i);
            break;
        case 2:
            printf("검색할 정수를 입력하세요: ");
            scanf("%d", &i);
            node_t *node = rbtree_find(tree, i);
            if (node != NULL)
                printf("정수 %d가 트리에 존재합니다.\n", i);
            else
                printf("정수 %d가 트리에 존재하지 않습니다.\n", i);
            break;
        case 3:
            printf("삭제할 정수를 입력하세요: ");
            scanf("%d", &i);
            node_t *node_to_delete = rbtree_find(tree, i);
            if (node_to_delete != NULL) {
                rbtree_erase(tree, node_to_delete);
                printf("정수 %d가 삭제되었습니다.\n", i);
            } else {
                printf("정수 %d가 트리에 존재하지 않습니다.\n", i);
            }
            break;
        case 4:
            node_t *min_node = rbtree_min(tree);
            if (min_node != NULL)
                printf("최소값은 %d입니다.\n", min_node->key);
            else
                printf("트리가 비어 있습니다.\n");
            break;
        case 5:
            node_t *max_node = rbtree_max(tree);
            if (max_node != NULL)
                printf("최대값은 %d입니다.\n", max_node->key);
            else
                printf("트리가 비어 있습니다.\n");
            break;
        case 6:
            // 배열 크기 설정
            size_t n = 100; // 예시로 100으로 설정
            key_t *arr = (key_t *)malloc(n * sizeof(key_t));
            int count = rbtree_to_array(tree, arr, n);
            printf("트리의 정수 배열: ");
            for (int j = 0; j < count; j++) {
                printf("%d ", arr[j]);
            }
            free(arr); // 동적 메모리 해제
            break;
        case 7:
            delete_rbtree(tree);
            tree = new_rbtree(); // 새로운 트리 생성
            printf("레드블랙 트리가 삭제되었습니다.\n");
            break;
        case 0:
            printf("종료합니다.\n");
            break;
        default:
            printf("잘못된 명령어입니다.\n");
            break;
        }
    }
}