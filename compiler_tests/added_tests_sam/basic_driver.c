int f();
int y(int a, int b, int c, int d, int e);

int main()
{
    int x = f();
    return !(y(1,2,3,4,5)==15);
}
