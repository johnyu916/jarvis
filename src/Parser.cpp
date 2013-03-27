namespace Jarvis{
    //return 0 if true
    int runCommand(Command command){
        //tokenize?
        list<string>tokens = command.tokens();
        //tokenize(line, tokens);

        /*
        cout <<"contents of the command: "<<endl;
        for (int a = 0; a < tokens.size(); a++){
            cout <<tokens[a]<<endl;
        }
        */

        //process instructions
        int size = tokens.size();
        if (size == 0){
            //do nothing
            return 0;
        }
        string first = tokens.front();
        tokens.pop_front();
        if (size == 1){
            if (first == "") return 0;
            else if (first == "exit"){
                return 1;
            }
            else if (first == "clear"){
                //reset device to new
                //Device *device = new Device();
                //State::instance().device(device);
                command.device()->clear();
                return 0;
            }
            else{
                return 2;
            }
        }
        else{
            if (first == "load"){
                return runLoad(command);
            }
            else if (first == "link"){
                return runLink(command);
            }
            else if (first == "label"){
                return runLabel(command);
            }
            else if (first == "save"){
                return runSave(command);
            }
            else if (first == "unlink"){
                return runUnlink(command);
            }
            else if (first == "run"){
                return runRun(command); 
            }
            else if (fisrt == "show"){

            }
            else{
                return 2;
            }
        }
    }
    int runScript(char *fileName){
        Device *device = new Device();
        runScript(fileName, device);
    }

    int runScript(char *fileName, Device *device){
        string line;
        ifstream stream(fileName);

        if (stream.is_open()){
            while (stream.good()){
                getline(stream, line);
                Command command(line, device);
                int result = runCommand(command);
                if (result !=0) return result;
            }
        }
        else{
            cerr << "Unable to open script: "<<fileName<<endl;
            return 3;
        }
        return 0;
    }
}
