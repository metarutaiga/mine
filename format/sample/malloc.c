void* calloc(size_t size, size_t count);
void* malloc(size_t size);
void* realloc(void* pointer, size_t size);
void free(void* pointer);
void* _expand(void* pointer, size_t size);
size_t _msize(void* pointer);
void* _recalloc(void* pointer, size_t size, size_t count);
int printf(const char* format, ...);

int main(int argc, const char* argv[])
{
    void* ptr_1 = calloc(4, 4);
    void* ptr_2 = malloc(4);
    void* ptr_3 = realloc(ptr_2, 8);
    void* ptr_4 = _expand(ptr_3, 16);
    size_t size_5 = _msize(ptr_4);
//  void* ptr_6 = _recalloc(ptr_4, 32, 64);
    free(ptr_1);
    free(ptr_4);
    printf("%p %p %p %p %zd %p\n", ptr_1, ptr_2, ptr_3, ptr_4, size_5);

    void** array[256];
    for (int i = 0; i < 256; ++i) {
        array[i] = malloc(i);
        printf("%d:%p (%d)\n", i, array[i], _msize(array[i]));
    }
    for (int i = 0; i < 256; ++i) {
        free(array[i]);
    }
    return 0;
}
