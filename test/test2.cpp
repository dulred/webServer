#include <iostream>
#include <vector>
#include <sstream> // 引入 ostringstream


using namespace std;
int main (int argc, char **argv){

    std::string path = "14,2,3,4";
    std::vector<std::string> vec;
    int j = 0;
    for (size_t i = 0; i < path.length(); i++)
    {
        if (path[i] == ',')
        {
            std::string str = "";
            for (size_t k = j; k < i; k++)
            {
                str+=path[k];
            }
            j=i+1;
            vec.push_back(str);
        }
        if (i == path.length() - 1)
        {
            std::string str = "";
            for (size_t k = j; k < path.length(); k++)
            {
                str+=path[k];
            }
            vec.push_back(str);
        }
        
    }


    for (auto &&i : vec)
    {
        cout << i << endl;
    }
    
    
    return 0;   
}
