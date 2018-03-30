#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

//A structure to hold information for each city
struct city{

    string cityName;
    string message;
    city *next;

    //Default constructor
    city();

    //Cities can be initialized with a name, message, and
    city(string initName, city *initNext, string initMessage){
        cityName = initName;
        message = initMessage;
        next = initNext;
    }

};

/*
A function to build the network from a pre-determined list of cities
Pre-conditions: None
Post-conditions: A linked list of cities is created and each node/city is printed to the screen.
                 The head of the list is returned.
*/
city * buildNetwork();

/*
A function to print the path of the network
Pre-conditions: buildNetwork has been run
Post-conditions: prints a path of the cities in the network
*/
void printPath(city * head);

/*
A function to transmit a message from city to city, where only once city can hold a message at a time.
The message will be received by reading a text file word by word.
Pre-conditions: buildNetwork has been run, the text file exists.
Post-condtions: Each city the message is transmitted to outputs the message to the console.
                If the list has not yet been built, prints "Empty list"
*/
void transmitMsg(city * head);

/*
A function to add a city to the current list of cities, between *previous and the city following *previous
Pre-conditions: buildNetwork has been run
Post-conditions: a new city with name = cityName is created and inserted into the linked list. The head of the linked list is returned.
*/
city * addCity(city * head, city * previous, string cityName);

/*
A function to search for a city currently in the network
Pre-conditions: buildNetwork has been run
Post-conditions: returns the city that has been searched for if it's in the list, NULL if not in the list/
*/
city * searchCity(city * head, string searchForName);

int main(){

    bool continueNetwork = true;

    //A city to act as the head of the network
    city * networkHead = new city("", NULL, "");

    //Run the 'main menu' until the user selects to quit
    while(continueNetwork){
        //Always start by printing all the menu options
        cout<<"======Main Menu======"<<endl;
        cout<<"1. Build Network"<<endl;
        cout<<"2. Print Network Path"<<endl;
        cout<<"3. Transmit Message Coast-To-Coast"<<endl;
        cout<<"4. Add City"<<endl;
        cout<<"5. Quit"<<endl;

        //Get the user input on the action they want to take
        char userChoice;
        cin>>userChoice;

        //CHOICE 1: build the network from the pre-determined list
        if(userChoice == '1'){
            networkHead = buildNetwork();

        }

        //CHOICE 2: print the current network path
        else if(userChoice == '2'){
            printPath(networkHead);

        }

        //CHOICE 3: transmit a message from a text file
        else if(userChoice == '3'){
            transmitMsg(networkHead);

        }

        //CHOICE 4: Add a city to the network
        else if(userChoice == '4'){
            string cityNew = "";
            string cityPrevious = "";

            //Clears the stream so that entering the new city name isn't skipped over.
            cin.ignore();

            cout<<"Enter a City name:"<<endl;
            getline(cin, cityNew);

            cout<<"Enter a previous City name:"<<endl;
            getline(cin, cityPrevious);

            //Use the search function to find the pointer with the name of cityPrevious
            //This way it can be passed through the addCity function
            city *prevCityPointer = new city("" , NULL , "");

            if(cityPrevious == "First"){
                //In addCity, cityName will be checked to replace the head in this case
                networkHead -> cityName = "toBeChanged";
            }
            else{
                prevCityPointer = searchCity(networkHead, cityPrevious);
            }

            //Add the city to the linked list, making sure to set networkHead equal to what is returned in case the head changes
            networkHead = addCity(networkHead, prevCityPointer, cityNew);
        }

        //CHOOICE 5: Quit
        else if(userChoice == '5'){
            continueNetwork = false;
            cout<<"Goodbye!"<<endl;
        }

        else{
            cout<<"Invalid selection"<<endl;
            continueNetwork = true;
        }
    }

}

city * buildNetwork(){

    //The pre-determined city list
    string listOfCities[10] =
    {"Los Angeles" , "Phoenix" , "Denver",
     "Dallas", "St. Louis", "Chicago",
     "Atlanta", "Washington, D.C.", "New York",
     "Boston"
    };

    //set the head and the temporary city
    city * head = new city(listOfCities[0], NULL, "");
    city * temp = head;

    //initialize the rest of the cities with the remaining names in the list
    for(int i = 1; i < 10; i++){

        city * newCity = new city(listOfCities[i], NULL, "");
        temp -> next = newCity;
        temp = newCity;

    }

    //print the initialized path and return to menu
    return head;

}

void printPath(city * head){

    //Set a temporary node and loop through it, printing the name of the city each time
    city * temp = head;

    cout<<"===CURRENT PATH==="<<endl;

    while(temp != NULL){
        cout<<temp -> cityName<<" -> ";
        temp = temp -> next;
    }

    cout<<" NULL"<<endl;

    cout<<"=================="<<endl;

}

void transmitMsg(city * head){

    //Checks to see whether or not the list has been built yet.
    if(head -> next == NULL){
        cout<<"Empty list"<<endl;
        return;
    }

    //Create stream to get the words and check to make sure it opens properly
    ifstream in_stream("messageIn.txt");

    if(in_stream.is_open()){

        string currentWord = "";

        //Get each word, delimiting by space
        while(in_stream >> currentWord){

            city * temp = head;

            //For each word, go through every city in the network and print the received message
            while(temp != NULL){

                temp -> message = currentWord;
                cout<< temp -> cityName << " received "<< temp -> message<<endl;
                temp = temp-> next;

            }

        }

    }

    //If the file doesn't open, tell the user there was an error.
    else{
        cout<<"Error opening message file"<<endl;
    }

}

city * addCity(city * head, city * previous, string cityName){

    city *newCity = new city(cityName, NULL, "");

    //If no list has been built yet, the city the user entered becomes the first item
    if(head == NULL){
        head = newCity;
    }
    //If the previous city doesn't exist, the new city becomes the head
    else if(previous == NULL){
        newCity -> next = head;
        head = newCity;
    }
    //The user typed first, so replace the head
    else if(head -> cityName == "toBeChanged"){
        newCity -> next = head -> next;
        head = newCity;
    }
    //Otherwise, we insert the newCity in after previous
    else{
        newCity -> next = previous -> next;
        previous -> next = newCity;
    }

    return head;
}

city * searchCity(city * head, string searchForName){

    //If the user doesn't enter a previous city, the city they want to add is made to be the new tail.
    if(searchForName == ""){
        city * tail = head;
        while(tail -> next != NULL){
            tail = tail -> next;
        }

        return tail;
    }

    //set a temporary city and loop through every node in the array
    //if the node is found, return the node. Otherwise, return NULL
    city *temp = head;

    while(temp != NULL){
        if(temp -> cityName == searchForName){
            return temp;
        }

        temp = temp -> next;
    }

    return NULL;
}
