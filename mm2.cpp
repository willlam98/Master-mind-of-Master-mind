#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <fstream>
void set_random_seed();
int randn(int n);

struct mm_code_maker{
    
    void init(int i_length, int i_num){
        length = i_length;
        num = i_num;
    }
    
    void generate_sequence(){
        for(int i = 0; i < length; i++){
            sequence.push_back(randn(num));
        }
        std::cout<<"the answer is: ";
        for(int i = 0; i < sequence.size(); i++){
            std::cout<<sequence[i]<<" ";
        }
        std::cout<<std::endl;
    }
    
    void give_feedback(const std::vector<int>& attempt, int& black_hits, int& white_hits){
        /// imagine this is implemented...
        for (int i = 0; i < attempt.size() ; i++){
            if( attempt[i] == sequence[i] ){
                black_hits++;
                // check black_hits first
            }
        }
        //total number of black and white hit
        int tot = 0;
        for(int i = 0; i < num ; i++){
            int numincode = 0;
            int numinguess = 0;
            for (int j = 0; j < sequence.size() ; j++){
                if( sequence[j] == i){
                    numincode++;
                }
                if( attempt[j] == i){
                    numinguess++;
                }
                
            }
            
            if( numincode <= numinguess ){
                tot = tot + numincode;
            }
            else{
                tot = tot + numinguess;
            }
        }
        white_hits = tot - black_hits;
    }
    
    std::vector<int> sequence;
    
    int length;
    int num;
};

struct mm_solver{
    
    void init(int i_length, int i_num){
        length = i_length;
        num = i_num;
    }
    
    /// this member function creates an attempt to find the right code
    /// this version generates a random attempt, then checks whether
    /// it has already been tried before and was not the right solution
    /// (if that's the case it generates another attempt etc)
    /// this is clearly still far from being a good implementation!
    void create_attempt(std::vector<int>& attempt){
        attempt = saved_situation[randn(saved_situation.size())];
    }
    
    /// this version just learns whether an attempt is correct or not
    /// if it is not, it adds it adds it to the list of unsuccessful attempts
    /// this is clearly still far from being a good implementation
    /// the information from black hits and white hits is not really used
    void learn(std::vector<int>& attempt, int black_hits, int white_hits){
        int blacks, whites;
        std::vector< std::vector<int> > replace;
        for(int i = 0; i < saved_situation.size(); i++){
            blacks = 0;
            whites = 0;
            attempt_hit_count(saved_situation[i], attempt, blacks, whites);
            if( (blacks == black_hits) && (whites == white_hits)){
                replace.push_back( saved_situation[i] );
            }
        }
        saved_situation = replace ;
    }
    
    void num_generator(int length, int num){
        
        int cases = std::pow(num, length);
        for(int n = 0; n < length; n++){
            situation.push_back(0);
        }
        for (int i = 1; i <= cases; i++) {
            for (int j = 0; j < length; j++) {
                
                if ( j == (length - 1)) {
                    situation[j] = (i-1)%num;
                }
                
                else{
                    int denominator = std::pow(num, (length - 1 - j) );
                    situation[j] = (((i-1)/denominator))%num;
                }
                
            }
            saved_situation.push_back(situation);
        }
    }
    
    void attempt_hit_count(const std::vector<int>& situation, const std::vector<int>& attempt , int& black_hits, int& white_hits){
        
        for (int i = 0; i < attempt.size() ; i++){
            if( attempt[i] == situation[i] ){
                black_hits++;
            }
        }
        
        int tot = 0;
        for(int i = 0; i < num ; i++){
            int numincode = 0;
            int numinguess = 0;
            for (int j = 0; j < attempt.size() ; j++){
                if( situation[j] == i){
                    numincode++;
                }
                if( attempt[j] == i){
                    numinguess++;
                }
            }
            if( numincode <= numinguess ){
                tot = tot + numincode;
            }
            else{
                tot = tot + numinguess;
            }
        }
        white_hits = tot - black_hits;
    }
    
    std::vector<int> situation;
    std::vector< std::vector<int> > saved_situation;
    
    int length;
    int num;
};

/// the main is the same as in the previous example

int main(){
    
    set_random_seed();
    
    int length, num;
    std::cout << "enter length of sequence and number of possible values:" << std::endl;
    std::cin >> length >> num;
    double k = 0;
    
    int maxi;
    int mini;

    for(int tot = 0; tot < 1; tot++){
    mm_solver solver;
    solver.init(length, num);
    solver.num_generator(length, num);
        int attempts;
    mm_code_maker maker;
    maker.init(length, num);
    maker.generate_sequence();
        if(tot == 1){
        maxi = attempts;
        mini = attempts;
        }
    int black_hits=0, white_hits=0;
    int attempts_limit = 5000;
    attempts = 0;
    while((black_hits < length) && (attempts < attempts_limit)){
        black_hits = 0;
        white_hits = 0;
        std::vector<int> attempt;
        if(attempts == 0){
            int s = length / 2;
            for (int i = 0; i < s ; i++) {
                attempt.push_back(0);
            }
            for (int j = attempt.size() ; j < length ; j++ ) {
                attempt.push_back(num-1);
            }
        }
        else{
        solver.create_attempt(attempt);
        }
        maker.give_feedback(attempt, black_hits, white_hits);
        std::cout << "attempt: " << std::endl;
        for(int i = 0; i < attempt.size(); i++){
            std::cout << attempt[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "black pegs: " << black_hits << " " << " white pegs: " << white_hits << std::endl;
        solver.learn(attempt, black_hits, white_hits);
        attempts++;
    }
        if (attempts>maxi) {
            maxi = attempts;
            
        }
        if (attempts< mini){
            mini = attempts;
        }
        k = k + attempts;
       
    }
   /* if(black_hits == length){
        std::cout << "the solver has found the sequence in " << attempts << " attempts" << std::endl;
    }
    else{
        std::cout << "after " << attempts << " attempts still no solution" << std::endl;
    }
    std::cout << "the sequence generated by the code maker was:" << std::endl;
    for(int i = 0; i < maker.sequence.size(); i++){
        std::cout << maker.sequence[i] << " ";
    }
    std::cout << std::endl;
    */
    
    std::cout<<"100 times: "<< k/1 << std::endl;
    std::cout<< "maximum number attempts: "<< maxi << " minimum number attempts: "<< mini << std::endl;
    return 0;
}

void set_random_seed(){
srand((unsigned)time (NULL));
    
}

int randn(int n){
    return rand()%n;
}

