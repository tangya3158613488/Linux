#include "VoiceAssistant.hpp"
using namespace std;
int main()
{
   // Robot rb;
   // string s = rb.Talk("你好");
   // cout << s << endl;
 //  string str;
  // volatile bool quit = false;
  // while(!quit)
  // {
  //     cout << "我# ";
  //     cin >> str;
  //     string s = rb.Talk(str);
  //     cout<< "Tony$ " << s <<  endl;
  // }
     VoiceAssistant *VA = new VoiceAssistant();
     if(!VA->LoadEtc())
     {
         return 1;
     }
     VA->Run();
    return 0;
}
