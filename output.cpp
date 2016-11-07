/**
 Program: 	cola.cpp
 Description:	A program that simulates a 'cola' (queue) at
 some real or imaginary place (e.g. a bank).
 Author:	Adam Brownell
 Date:		March 2016
 */
/*
+-------------------------------------------------------+
| Waiting times for different Probabilities and Tellers |
+-------------------------------------------------------+
       | T = 1 | T = 2 | T = 3 | T = 4 | T = 5 |
P = 10 |13.5227|4.04545|1.13636|.636364|.409091|
P = 20 |160.235|89.6173|58.8025|42.4815|29.8765|
P = 30 |293.829|165.829|108.812|79.6410|63.4957|
P = 40 |450.150|532.275|164.908|123.379|96.1111|
P = 50 |570.801|313.054|215.780|155.774|126.613|
---------------------------------------------------------
*/
#include <iostream>	// std::cout, std::endl, std::istream
#include <fstream>	// std::ifstream
#include <string>   // std::string
#include <vector>
#include "queue.h"

using namespace Pic10B;  // A call to queue<T> is a call to Pic10B::queue<T> instead of std::queue<T>
/** Could also specify we only want Pic10B::queue like so:
 using Pic10B::queue;
 */

/**
 Definition of the 'Cliente' class.
 */

class Cliente {
public:
    inline Cliente( std::string n = "Default_client" , int a = 0 , int s = 0 )
    : name(n) , arrivalTime(a) , serviceTime(s) { }
    
    // Accessors
    inline std::string get_name() const { return name; }
    inline int get_arrival_time() const { return arrivalTime; }
    inline int get_service_time() const { return serviceTime; }
    
    friend std::istream& operator>>( std::istream& in , Cliente& rhs ) ;
    
private:
    std::string name;
    int arrivalTime;
    int serviceTime;
};

/**
 Non-member functions
 */
std::istream& operator>>( std::istream& in , Cliente& rhs ) {
    in >> rhs.name >> rhs.arrivalTime >> rhs.serviceTime;
    return in;
}

/*
 Prints the time on the screen. It assumes we are given
 the number of minutes after 9:00am.
 */
void print_time( int minsAfterNine ) {
    const int MIN_PER_HOUR = 60;
    
    int hour = 9 + minsAfterNine / MIN_PER_HOUR;
    int min = minsAfterNine % MIN_PER_HOUR;
    
    if (hour <= 12) std::cout << hour << ":";
    else std::cout << hour - 12 << ":";
    
    if (min > 9) std::cout << min;
    else std::cout << "0" << min;
    
    if (hour <= 12) std::cout << "am";
    else std::cout << "pm";
    
    return;
}

//check to see if counter is open
bool is_empty( std::vector<Cliente*> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] != NULL)
            return false;
    }
    return true;
}

/**
 main routine that simulates a 'cola' (line) at a bank
 */
int main() {
// 1.
    // Getting the Customer List:
    
    int answer;
    queue<Cliente> customerList;
    Cliente newCustomer;
    
    std::cout << "Enter Option preference:\n 1. Enter Customer List from File\n 2. Randomly Generate Customer List";
    
    //From File
    std::cin >> answer;
    if (answer == 1)
    {
        std::ifstream fin;
        fin.open("clientes.txt");
        
        while ( fin >> newCustomer )
            customerList.enter(newCustomer);
        
        fin.close();
        
        if ( customerList.is_empty() ) {
            std::cout << "ERROR: Could not read file 'clientes.txt'." << std::endl;
            return 1; // Standard practice is to NOT RETURN 0 if an error occurs.
        }
    }
    
    
    //Random Generation
    if (answer == 2)
    {
        int prob, store_hours = 1, num = 1;
        std::cout << "What is the probability that a customer will arrive per minute?";
        std::cin >> prob;
        
        while (store_hours < 360)// 360 is how many minutes are in an 8 hour day, since store open 9-6
            
        {
            if ((rand()%100+1) < prob) //if number is within prob of customer, there is a customer !
            {
                
                Cliente rando_customer ("Random Cliente #" + std::to_string(num), store_hours, (rand()%14+2));
                customerList.enter(rando_customer);
                num++;
            }
            store_hours++;
        }
        if ( customerList.is_empty() )
            std::cout << "There were no customers today" << std::endl;
    }
    
    /**
     First customer steps up to the counter immediately (i.e., no waiting occurs).
     Clock starts running at this arrival time.
     */
    std::vector<Cliente*> counter;
    std::vector<int> waitr;
    Cliente atCounter = customerList.leave();
    int currentTime = atCounter.get_arrival_time();
    int startService = atCounter.get_arrival_time();
    int tellers, total_customers_served, wait_time, line_length, max_line_length =0;
    
    
    std::cout << "How many tellers are at the bank?\n";
    std::cin >> tellers;
    for (int i = 0; i < tellers; i++)
    {
        Cliente* desk = NULL;
        counter.push_back(desk);
    }
    for (int i = 0; i <tellers; i++)
        waitr.push_back(0);

    
    print_time(currentTime);
    std::cout << "  " << atCounter.get_name() << " enters the bank." << std::endl;
    print_time(currentTime);
    std::cout << "  " << atCounter.get_name() << " steps up to counter 1" << std::endl;
    counter[0]= &atCounter;
    waitr[0] = 0;
    
    // set up our empty 'cola'
    queue<Cliente> line;
    
    // while there are customers in bank... or they are yet to arrive.
    while ( !customerList.is_empty() || !line.is_empty() ||  !is_empty(counter)){
        
        // Check if a 'Cliente' enters the 'cola'.
        if ( !customerList.is_empty() && customerList.peek().get_arrival_time() <= currentTime ){
            print_time(currentTime);
            std::cout  << "  " << customerList.peek().get_name()
            << " enters the bank." << std::endl;
            
            line.enter( customerList.leave() );
            
            line_length++;
            if (line_length > max_line_length)
                max_line_length = line_length;
        }
        
    //Check all the counters
        for (int i = 0; i < counter.size(); i++){
        // Check if a 'Cliente' leaves the counter.
            if ( counter[i] != NULL && ( counter[i] ->get_arrival_time() + counter[i]->get_service_time() + waitr[i] <= currentTime ) ){
            
            print_time(currentTime);
            std::cout << "  " << counter[i]->get_name()
            << " leaves the counter " << i+1 << std::endl;
            total_customers_served++;
            counter[i] = NULL;
        }
        
        
        // Check if counter is empty and someone is waiting.
        if ( counter[i] == NULL && !line.is_empty() ){
            atCounter = line.leave();
            counter[i] = &atCounter;
            startService = currentTime;
            
            print_time(currentTime);
            std::cout << "  " << counter[i]->get_name()
            << " steps up to the counter " << i+1 << std::endl;
            line_length--;
            wait_time+= currentTime - counter[i]->get_arrival_time();
            waitr[i] =  currentTime - counter[i] ->get_arrival_time();
        }
        }
        
        ++currentTime;
        
        // Here is where we should compute and display some queue statistics
    }
    
std::cout << "\n\nTotal Customers Served:   " << total_customers_served << " Customers" << std::endl;
std::cout << "Average Wait Time:        " << (double)(wait_time/(double)total_customers_served) <<  " Minutes" << std::endl;
std::cout << "Longest Line Length:      " << max_line_length << " People" << std::endl;

    

    return 0;
}