#include <iostream>
#include "MMPlayer.h"
#include "MMAV.h"
#include <vector>
#include "MMQueue.h"
#include <unistd.h>

using namespace std;

int main()
{
    MMPlayer player("../../test.mp4");

    int ret = player.Open();
    if(ret)
    {
        cout << "Player Open Error!!" << endl;
        return -1;
    }
    
    player.Play();
    cout << "===========1 play =================" << endl;
    
    //wait...
    std::this_thread::sleep_for(std::chrono::seconds(1));

    player.Seek(5);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    player.Pause();
    cout << "===========1 pause =================" << endl;

    //wait...
    std::this_thread::sleep_for(std::chrono::seconds(5));

    player.Play();

    cout << "===========2 play =================" << endl;

    //wait..
    std::this_thread::sleep_for(std::chrono::seconds(60 * 60));

    player.Stop();

    return 0;
}
