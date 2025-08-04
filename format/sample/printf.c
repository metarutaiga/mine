int _fltused = 0;
int printf(const char* format, ...);

int main(int argc, const char* argv[])
{
    printf("%s", "Hello, World!\n");
    printf("%hhX %hX %X %lX %llX\n", 0x11, 0x2222, 0x33333333u, 0x44444444ul, 0x5555555555555555ull);
    printf("%hhf %hf %f %lf %llf\n", 1.0f, 2.0f, 3.0f, 4.0f, 5.0f);
    return 0;
}
