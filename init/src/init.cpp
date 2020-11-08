// This is the program for initializing the spacepi

// Check if the path provided points to a valid config file
bool needsUpdate(string path);

// Clears the update
void clearUpdate(string path);

// Update hostname and ip
void updateHostname(string hostname);
void updateIP(string ip);

// Proccess the XML document and make needed changes
void processXML(string path);

void processUsers(ptree& users);


int main(int argc, char ** argv){

}
