//testing infrastructure
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

int runTest(string test){
    string bin = "./jarvis";
    string testDir = "tests/";
    string testFile = testDir + test + ".desc";
    string expectFile = testDir + test +".expect";
    string outFile = testDir + test + ".actual";
    string command = bin + " " + testFile + " > " + outFile;
    cout <<"run command: "<<command<<endl;
    int status = system(command.c_str());
    cout <<"return status: " <<status<<endl;
    if (status == 0){
        command = "diff "+expectFile+ " " + outFile;
        int diffStatus = system(command.c_str());
        if (diffStatus != 0){
            cerr << "Error expect does not match actual. diff return: "<<diffStatus<<endl;
            return -1;
        }
        else return 0;
    }
    else{
        cerr << "jarvis returned error status: "<<status<<endl;
        return status;
    }
}

int main(int argc, char *argv[]){
    //call jarvis and direct output to standard output
    string testName = "and2Test";
    if (argc == 2){
        testName = argv[1];
    }
    int status = runTest(testName);
    if (status != 0) {
        cerr << "Test failed with status: "<<status<<endl;
    }
    else{
        cout << "Test successful!" <<endl;
    }
    return 0;
}
