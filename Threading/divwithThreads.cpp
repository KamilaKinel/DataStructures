// Name: Kamila Kinel
// Section: 1002
// Assn number: Assignment 3
// Description:  Do long addition by reading long
// ints in as strings, converting them to int vectors
// , and then using threads to do long addition.

#include <string>
#include <vector>
#include <iostream>
#include <thread>

// Compile:
//  $ g++ -pthread code.cpp -o code
// Running:
//  $ ./code 
using namespace std;
vector<int> num1;
vector<int> num2;
std::vector<thread*> threads;
std::vector<int> carryon;
std::vector<int> result;

int bigint_size = 8;

void changeToInt(vector<char> &vect, vector<int> &to) {
    for (auto& i : vect) {
        to.insert(to.begin(), i-'0');
    }
};
/**
 * @brief Create a digit string out of a vector of ints
 * 
 * @param data 
 * @return string 
 */
string vector_to_string(vector<int> data){
    string result = "";
    for(int i = 0; i < bigint_size; i++){
        result = to_string(data[i]) + result;
    }
    if (data[bigint_size] > 0){
        result = to_string(data[bigint_size]) + result;
    }
    return result;
}
/**
 * @brief Add 0's to a vector of int until the size of the 
 * vector equals the given length
 * 
 * @param numbers 
 * @param length 
 */
void pad_vector(vector<int>& numbers, int length){
    for(int i = numbers.size(); i < length; i++){
        numbers.push_back(0);
        // cout << "adding" <<endl;
    }
}
/**
 * @brief Get the highest power of 2 that do not exceed the given value n
 * 
 * @param n 
 * @return unsigned int 
 */
unsigned int get_highest_power_of2(unsigned int n){
    unsigned int result = 1;
    while ((result * 2) <= n){
        result *= 2;
    }
    return result;
}
/**
 * @brief Perform addition operation of num1 and num2 on digit 
 * positions between `start` and `end`
 * 
 * @param thread_index              index of the thread that is running this function
 * @param start 
 * @param end 
 */
void addition_operation(int thread_index, int start, int end){

    for(int i = start; i < end; i++){
        int c = 0; // here we are going to check if there was a carryon from the previous position
        if (i > start){
            c = carryon[i - 1];
            carryon[i - 1] = 0;
        }
        int r = num1[i] + num2[i] + c;
        if (r / 10 > 0){ // check to see if the result is greater than 9, if so we have a carry on.
            carryon[i] = 1;
        } else {
            carryon[i] = 0;
        }
        // carryon[i] = r / 10;
        result[i] = r % 10;     // we save the final value for the ith position after removing the carryon
    }

}

/**
 * @brief Check to see if there is a carryon ended up at the last digit location 
 * and if so carry it to a new digit position in the result
 * 
 */
void handle_most_significant_carryon(){
    if (carryon[bigint_size - 1] > 0){
        result[bigint_size] = carryon[bigint_size - 1];
    } else {
        result[bigint_size] = 0;
    }
}

int main(){
    string str1;    // num1
    string str2;   // num2
    // get user input
    cout << "Enter a number: " << endl;
    cin>> str1;
    cout << "Enter a number: " << endl;
    cin>> str2;

    // Converts num1 & num2 str into char and int vectors
    vector<char> input_1(str1.begin(), str1.end());   
    changeToInt(input_1, num1); // Converts from chars to ints
  
    vector<char> input_2(str2.begin(), str2.end());   
    changeToInt(input_2, num2);     // Converts from chars to ints

    // initializing the resultant vectors    
    if (num1.size() > num2.size()){
      bigint_size = num1.size() ;
      pad_vector(num2, bigint_size);
    } else {
      bigint_size = num2.size() ;
      pad_vector(num1, bigint_size);
    }

    pad_vector(carryon, bigint_size);
    pad_vector(result, bigint_size);

    result.push_back(0); // incase of a carryon at the most significant digit location is present we need to add another digit position to the result

    //________________________________________________________
    //Determine max num of threads
    unsigned int max_thread_count = std::thread::hardware_concurrency();
    unsigned int thread_count = get_highest_power_of2(max_thread_count);

    // assumption: digit count is divisable by the thread_count
    int no_of_elements_per_thread = bigint_size / thread_count;
    // Create threads assigning bigit blocks to each
    for(int i = 0; i < thread_count; i++){
        thread* t = new thread(addition_operation, i, i*no_of_elements_per_thread, (i + 1) * no_of_elements_per_thread);
        threads.push_back(t);
    }
    // for(int i = 0; i < bigint_size; i+=no_of_elements_per_thread){
    //     thread t(addition_operation, i, i + no_of_elements_per_thread);
    //     threads.push_back(&t);
    // }

    // wait for all the threads to finish doing their calculations
    for(int i = 0; i < thread_count; i++){
        threads[i]->join();
        delete threads[i];
    }

    // Check for carryons that was not resolved or needs to be carried on to digit set that belonged to the next threads
    // this part is the only part we cannot parallelize. 
    for(int i = no_of_elements_per_thread; i < bigint_size; i++){ // start from the most significant digit of the digit group of first thread
        int r = result[i] + carryon[i - 1];
        if (r / 10 > 0){ // check to see if the result is greater than 9, if so we have a carry on.
            carryon[i] = 1;
        }
        result[i] = r % 10;     // we save the final value for the ith position after removing the carryon
    }

    // if there is a carryon at the last digit location we'll update the final result by adding that 
    // carryon as a new digit to the most significant digit location
    handle_most_significant_carryon();

    string result_str = vector_to_string(result);
    cout << "Result = " << result_str << endl;

    return 0;
}


