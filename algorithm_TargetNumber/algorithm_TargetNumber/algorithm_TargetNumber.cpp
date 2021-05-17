#include <iostream>
#include <vector>

// source - programmers

using namespace std;

int answer = 0;
void dfs(vector<int> v, int t, int s, int i)
{
    if (i >= v.size())
    {
        if (s == t) ++answer;
        return;
    }


    dfs(v, t, s + v[i], i + 1);
    dfs(v, t, s - v[i], i + 1);
}

int solution(vector<int> numbers, int target)
{
    dfs(numbers, target, 0, 0);

    return answer;
}

//int solution(vector<int> numbers, int target)
//{
//    int answer = 0;
//    int size = 1 << numbers.size();
//    std::cout << size << std::endl;
//
//    for (int i = 1; i < size; i++)
//    {
//        int temp = 0;
//
//        for (int j = 0; j < numbers.size(); j++)
//        {
//            if (i & (1 << j))
//            {
//                temp -= numbers[j];
//            }
//            else temp += numbers[j];
//        }
//        if (temp == target) answer++;
//    }
//    return answer;
//}

int main()
{
    vector<int> v = { 1,1,1,1,1 };
    solution(v, 3);
}