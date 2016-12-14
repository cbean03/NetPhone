#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
using namespace std;

//function to check if mp3 file exists yet
inline bool fileExists(const string &name)
{
    if (FILE *file = fopen(name.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}
//check if file is empty
bool fileis_empty(std::ifstream &pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

inline void delay(unsigned long ms)
{
    usleep(ms * 1000);
}

int main()
{

    cout << "starting program...\n";
    system("./testMP3Receiver > test.mp3");
    delay(5000);

    //if file doesn't exist yet, wait for a second
    if (!fileExists("test.mp3"))
    {
        delay(1000);
    }
    //now check if file exists
    if (fileExists("test.mp3"))
    {
        std::ifstream file("test.mp3");
        //check if the file has data in it
        if (fileis_empty(file))
        {
            cout << "sorry file is still empty\n";
        }
        else
        {
            cout << "We should play the file now\n";
            system("afplay test.mp3");
        }
    }
    //if file doesn't exists, sned an error saying that the file can't be created
    else
    {
        cout << "Error, file has not been created. Terminating.\n";
    }

}


