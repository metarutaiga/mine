int _fltused = 0;
int printf(const char* format, ...);
int sprintf(char* buffer, const char* format, ...);
int snprintf(char* buffer, size_t length, const char* format, ...);

int main(int argc, const char* argv[])
{
    char temp[64];
    printf("%s", "Hello, World!\n");
    printf("%.*s\n", 8, "Hello, World!\n");
    printf("%hhX %hX %X %lX %llX\n", 0x11, 0x2222, 0x33333333u, 0x44444444ul, 0x5555555555555555ull);
    printf("%hhf %hf %f %lf %llf\n", 1.0f, 2.0f, 3.0f, 4.0f, 5.0f);
    printf("%d\n", sprintf(temp, "%s", "Hello, World!\n"));
    printf("%d\n", snprintf(temp, 64, "%s", "Hello, World!\n"));
    return 0;
}
