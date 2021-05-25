#include <iostream>
#include <string>

std::string solution(int n)
{
    std::string answer;

    int num = 0;

    while (0 < n)
    {
        num = n % 3;
        n = n / 3;

        if (0 == num)
        {
            n -= 1;
            num = 4;
        }

        answer = std::to_string(num) + answer;
    }


    return answer;
}

int main()
{

    std::cout << solution(15);

    return 0;
}

