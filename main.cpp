#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

int line = 1;

struct variable{
    std::string type;
    std::string value;
};
std::map<std::string, variable> var_info;

std::vector<std::string> keywords {"num", "tf", "when", "otherwise", "cycle", "print","}", ";"};


void do_interpretation (std::ifstream& fin);
int  checking_keyword (std::string& first_token);
void do_decleration (std::ifstream& fin, const int type_index);
void do_condition_else(std::ifstream& fin, const int& type_index);
void do_output_print(std::ifstream& fin, const int& type_index);
void for_other_operations(std::ifstream& fin, std::string& tok);
void do_cycle (std::ifstream& fin, int& type_index);
void do_boolean(std::ifstream& fin, int& type_index);
bool is_identifier(std::string& token);
bool is_arithmetical_operator(std::string& tmp);
void error ();
bool is_symbol(const char& ch);
bool is_digit(const char& ch);
bool is_arithmetical_symbol(const char& ch);
int do_operation(std::string& expression);
double do_calculation (const double& a, const double& b, const char& ch);
void do_condition (std::ifstream& fin, const int& type_index);
bool is_operator(char& ch);
bool is_logic_operator(std::string& token);
bool check_condition(std::string& expression);
bool boolean_checking(const std::string& str1, const std::string& str2, const std::string& oper);


int main()
{
    std::ifstream file;
    std::string path = "";
    do{
        std::cout << "Enter file name: ";
        std::cin >> path;
        file.open(path + ".txt");
    }while(!file.is_open());


    if(file.is_open())
    {
        do_interpretation(file);
    }
    else
    {
        std::cout << "Something was gone wrong !" << std::endl;
    }
    file.close();
}

void do_interpretation (std::ifstream& fin)
{
    std::string token = "";
    
    while (!fin.eof())
    {
        bool check_for_condition_else = false;
        label:
        fin >> token;
        int which_keyword = checking_keyword(token);

        switch (which_keyword)
        {
            case 0:
                std::cout << std::endl << "Num is called !" << std::endl;
                do_decleration(fin, which_keyword); // num
                break;
            case 1:
                std::cout << std::endl << "TF is called !" << std::endl;
                do_boolean(fin, which_keyword); // tf
                break;
            case 2:
                std::cout << std::endl << "When is called !" << std::endl;
                do_condition(fin, which_keyword); // when
                break;
            case 3:
                //
                break;
            case 4:
                std::cout << std::endl << "Cycle is called !" << std::endl;
                do_cycle(fin, which_keyword);   // cycle
                break;
            case 5:
                std::cout << std::endl << "Print is called !" << std::endl;
                do_output_print(fin, which_keyword); //  print
                break;
            case 6:
                goto label;
                break;
            case 7:
                goto label;
                break;
            case -1:
                std::cout << std::endl << "Such variable ( " << token << " ) in the line " << line << " does not exist !" << std::endl;
                error();
                break;
            default:
                std::cout << std::endl << "Variable " << token << " is called, which exists !" << std::endl;
                for_other_operations(fin, token); // for literal that exists
                break;

        }
        line++;
    }
}
void for_other_operations(std::ifstream& fin, std::string& tok)
{
    std::string token = tok;
    fin >> token;
    
    if(token == "=")
    {
        std::string expression = "";
        std::string tmp;
        fin >> tmp;
        int index = checking_keyword(tmp);
        if(index > 7)
        {
            tmp = var_info[keywords[index]].value;
        }
        while(tmp != ";")
        {
            index = checking_keyword(tmp);
            if(index > 6)
            {
                tmp = var_info[keywords[index]].value;
            }
            expression += tmp;
            fin >> tmp;
        }
        std::cout << expression << std::endl;
        
        var_info[tok].value = std::to_string(do_operation(expression));
    }
    else
    {
        std::cout << "After known variable does not exist an arithmetical operator !" << std::endl;
        error();
    }
}



int checking_keyword (std::string& first_token)
{
    
    for (int i = 0; i < keywords.size(); i++)
    {
        if(keywords[i] == first_token)
        {
            return i;
        }
    }
    return -1;
}

void do_decleration (std::ifstream& fin, const int type_index){
    std::string token;
    do{
        std::string name;
        fin >> token;
        if (is_identifier(token)) // if the variable name is repeated
        {
            std::cout << "The variable name: " << token << "- is repeated" << std::endl;
            error();
        }
        var_info[token].type = keywords[type_index];
        std::cout << "Created a variable ( " << token << " ) which is " << keywords[type_index] << " type." << std::endl;
        keywords.push_back(token);
        name = token;
        fin >> token;
        if(token == "=")
        {
           fin >> token;
           
           std::string tmp;
           fin >> tmp;
           if(tmp == "," || tmp == ";")
           {

                int index = checking_keyword(token);
                
                if (index > 7)
                {
                    std::cout << "For the name of: " << name << " is inserted " << var_info[keywords[index]].value << " value" << std::endl;
                    var_info[name].value  = var_info[keywords[index]].value;
                }
                else if(index == -1)
                {
                    std::cout << name << "'s value is iserted: " << token << std::endl;
                       var_info[name].value = token;
                }
                else
                {
                    error();
                }
                token = tmp;
           }
           else if (is_arithmetical_operator(tmp))
           {
               std::string expression = "";
               expression += token + tmp;
               while (token != "," && token != ";")
               {
                   fin >> token;
                   expression += token;
               }
               var_info[name].value = std::to_string(do_operation(expression));
               std::cout << "Therefore the " << name << "'s value is: " << var_info[name].value << "." << std::endl;
           }
           else
           {
               std::cout << "After the variable name there is NO any kind of arithmetical operator" << std::endl;
               error();
           }
        }
    }while(token != ";" && !(checking_keyword(token) + 1));
}

void do_boolean(std::ifstream& fin, int& type_index)
{
        std::string token;
    do{
        std::string name;
        fin >> token;
        if (is_identifier(token)) // if the variable name is repeated
        {
            error();
        }
        var_info[token].type = keywords[type_index];
        keywords.push_back(token);
        name = token;
        fin >> token;
        if(token == "=")
        {
           fin >> token;
           
           std::string tmp;
           fin >> tmp;
           if(tmp == "," || tmp == ";")
           {
                int index = checking_keyword(token);
                
                if (index > 7)
                {
                    if(var_info[keywords[index]].value != "0")
                    {
                        var_info[name].value  = "1";
                    }
                    else
                    {
                        var_info[name].value  = "0";
                    }
                }
                else if(index == -1)
                {
                    if(is_digit(token[0]))
                    {
                        if(token == "0")
                        {
                            var_info[name].value  = "0";
                        }
                        else
                        {
                            var_info[name].value  = "1";
                        }
                    }
                    else
                    {
                        var_info[name].value = token;
                    }
                }
                else
                {
                    error();
                }
                if(token == "true")                                     // tf(bool)  d = true ; // tf d = false;
                {
                    var_info[name].value  = "1";
                }
                else if(token == "false")
                {
                    var_info[name].value  = "0";
                }
                token = tmp;
           }
           else if (is_arithmetical_operator(tmp))
           {
               std::string expression = "";
               expression += token + tmp;
               while (token != "," && token != ";")
               {
                   fin >> token;
                   expression += token;
               }
               var_info[name].value = std::to_string(do_operation(expression));
           }
           else if(is_logic_operator(tmp) || tmp == "(")
           {
               std::string expression = "";
               expression += token + tmp;
               while (token != "," && token != ";" && token != ")")
               {
                   fin >> token;
                   expression += token;
               }
               var_info[name].value = check_condition(expression);
           }
           else
           {
               error();
           }
        }
    }while(token != ";" && !(checking_keyword(token) + 1));
}

bool is_arithmetical_operator(std::string& tmp)
{
    const std::string arr[4] = {"+", "-", "*", "/"};

    for(int i = 0; i < 4; i++)
    {
        if(tmp == arr[i])
        {
            return true;
        }
    }
    return false;
}

bool is_identifier(std::string& token)
{
    for(int i = 0; i < keywords.size(); i++)
    {
        if(token == keywords[i])
        {
            return true;
        }
    }
    return false;
}

void error ()
{
    std::cout << "You hav an E R R O R in: " << line << "th line !" << std::endl;
    exit(1);
}

int do_operation(std::string& expression)
{
    int size = 0;
    for(int i = 0; expression[i] != '\0'; i++)
    {
        ++size;
    }
    std::string tmp1 = "";
    std::string tmp2 = "";
    double num1 = 0;
    double num2 = 0;
    double sum = 0;
    int a = 0;
    while(is_digit(expression[a]))
    {

      if(is_digit(expression[a]))
      {
           tmp1 = tmp1 + expression[a];
      }
         ++a;
    }

    for(int i = 0; i < size; i++)
    {

        if(is_arithmetical_symbol(expression[i]))
        {
            int k = i + 1;
            char ch = expression[i];

            if(is_symbol(expression[k]))
            {
                error();
            }

            while(is_digit(expression[k]))
            {
                if(is_digit(expression[k]))
                {
                    tmp2 = tmp2 + expression[k];
                }
                ++k;
            }

            num1 = std::stod(tmp1); // converting string to double
            num2 = std::stod(tmp2);
            sum = do_calculation(num1, num2, ch);
            tmp1 = std::to_string(sum);
            expression.erase(0,k); // deleting the first part of string wich is already calculated
            tmp2.clear();
        }
    }
    std::cout << "The sum of the arithmetical operation in " << line << "'th line is: " << sum << "." << std::endl;
    return sum;
}

double do_calculation (const double& a, const double& b, const char& ch)
{
    double calculation = 0;

    switch(ch)
    {
        case '+':
                calculation = a + b;
                return calculation;
        case '-':
                calculation = a - b;
                return calculation;
        case '*':
                calculation = a * b;
                return calculation;
        case '/':
                calculation = a / b;
                return calculation;
        default :
                std::cout << "There is no such arithmetical calculation operator" << std::endl;
                error();
                break;
    }
    return 0;
}

bool is_arithmetical_symbol(const char& ch)
{
    bool check = ((ch == 37) || (ch == 42) || (ch == 47) || (ch == 45) || (ch == 43));
    return check;
}

bool is_symbol(const char& ch)
{
    bool check = ((ch <= 90 && ch >= 65) || (ch <= 122 && ch >= 97));
    return check;
}

bool is_digit(const char& ch)
{
    bool check = (ch <= 57 && ch >= 48);
    return check;
}

void do_condition (std::ifstream& fin, const int& type_index) // when(if)
{
    std::string token;
    fin >> token;
    if(token == "(")
    {
        std::string expression = "";
        std::string tmp = "";
        while(tmp != ")")
        {
            expression += tmp;
            fin >> tmp;
        }
        fin >> tmp;

        if(tmp != "{")
        {
            std::cout << "You do not entered - {" << std::endl;
            error();
        }

        if (!check_condition(expression))
        {
            std::string end;
            while(end != "}")
            {
                fin >> end;
            }
        }
    }
    else
    {
        std::cout << "You forgot this (" << std::endl;
        error();
    }
}



bool is_operator(char& ch)
{
    bool check = ((ch == '!') || (ch == '=') || (ch == '>') || (ch == '<'));
    return check;
}

bool is_logic_operator(std::string& token)
{
    std::string arr[6] = {"==", "!=", ">=", "<=", "<", ">"};
    for(int i = 0; i < 6; i++)
    {
        if(token == arr[i])
        {
            return true;
        }
    }
    return false;
}

bool check_condition(std::string& expression)
{
    int size = 0;
    for(int i = 0; expression[i] != '\0'; i++)
    {
        ++size;
    }
    std::string tmp = "";
    std::string name1 = "";
    std::string name2 = "";

    for(int i = 0; i < size; i++)
    {
        if(is_operator(expression[i]))
        {
            if(is_operator(expression[i + 1]))
            {
                tmp.push_back(expression[i]);
                tmp.push_back(expression[i + 1]);

                int a = i - 1;
                int b = i + 2;

                while(is_symbol(expression[a]) || is_symbol(expression[b]))
                {
                    if(is_symbol(expression[a]))
                    {
                        name1 = expression[a] + name1;
                        --a;
                    }
                    if(is_symbol(expression[b]))
                    {
                        name2 = name2 + expression[b];
                        ++b;
                    }
                }
                return boolean_checking(name1, name2, tmp);
            }
            if(!is_operator(expression[i + 1]))
            {
                tmp.push_back(expression[i]);
                int a = i - 1;
                int b = i + 1;
                while(is_symbol(expression[a]) || is_symbol(expression[b]))
                {
                    if(is_symbol(expression[a]))
                    {
                        name1 = expression[a] + name1;
                        --a;
                    }
                    if(is_symbol(expression[b]))
                    {
                        name2 = name2 + expression[b];
                        ++b;
                    }
                }
                return boolean_checking(name1, name2, tmp);
            }
        }
    }
    return false;
}

bool boolean_checking(const std::string& str1, const std::string& str2, const std::string& oper)
{
    const std::string arr[6] = {"==", "!=", ">=", "<=", ">", "<"};
    bool check = false;

    double a = 0;
    double b = 0;
    for(int i = 0; i < keywords.size(); i++)
    {
        if(str1 == keywords[i])
        {
           a = std::stod(var_info[str1].value);
        }
        if(str2 == keywords[i])
        {
           b = std::stod(var_info[str2].value);
        }
    }

    for(int i = 0; i < 6; i++)
    {
        if(oper == arr[i])
        {
           switch(i)
           {
                case 0:
                check = (a == b);
                break;
                case 1:
                check = (a != b);
                break;
                case 2:
                check = (a >= b);
                break;
                case 3:
                check = (a <= b);
                break;
                case 4:
                check = (a > b);
                break;
                case 5:
                check = (a < b);
                break;
           }
        }
    }
    return check;
}

void do_cycle (std::ifstream& fin, int& type_index)
{
    std::string token;
    fin >> token;
    if(token == "(")
    {
        static std::string expression = "";
        std::string tmp;
        while(tmp != ")")
        {
            expression = expression + tmp;
            fin >> tmp;
        }
        if(check_condition(expression))
        {
            fin >> tmp;
            if(tmp != "{")
            {
                std::cout << "You forgot this { " << std::endl;
                error();
            }
            std::string end;
            std::ofstream cycle;
            fin >> end;
            cycle.open("cycle.txt");
            while(end != "}")
            {
                cycle << end;
                fin >> end;
            }
            cycle.close();
            std::ifstream cycin;
            cycin.open("cycle.txt");
            while(!cycin.eof())
            {
                do_interpretation(cycin);
            }
            
        }
        else
        {
            std::string end;
            while(end != "}")
            {
                fin >> end;
            }
            do_interpretation(fin);
        }
    }
    else
    {
        std::cout << "You forgot this (" << std::endl;
        error();
    }
}

void do_condition_else(std::ifstream& fin, const int& type_index)
{
    std::string token;
    fin >> token;
    if(token != "{")
    {
        std::cout << "You forgot this {" << std::endl;
        error();
    }
    else
    {
        do_interpretation(fin);
    }
}

void do_output_print(std::ifstream& fin, const int& type_index)
{
    std::string token;
    fin >> token;
    int index = checking_keyword(token);

    if(token == "#")
    {
        std::cout << "Print is going to print your string." << std::endl;
        std::string expr = "";
        std::string tmp;
        fin >> tmp;
        while(tmp != "#")
        {
            expr = expr + tmp + " ";
            fin >> tmp;
        }
        fin >> tmp;
        std::cout << expr;
        
        return;
    }
    else if(token == "##")
    {
        std::cout << "Print is going to print your string with endl." << std::endl;
        std::string expr = "";
        std::string tmp;
        fin >> tmp;
        while(tmp != "##")
        {
            expr = expr + tmp + " ";
            fin >> tmp;
        }
        fin >> tmp;
        std::cout << expr << std::endl;
        
        return;
    }
    
    
    if(index > 0 && index < 7)
    {
        std::cout << "You cannot do decleration after print." << std::endl;
        error();
        
        return;
    }
    else if(index > 6)
    {
        std::string num = var_info[keywords[index]].value;
        std::cout << "OK, print is going to print the value inside the  ( " << keywords[index] << " ) variable." << std::endl;
        std::cout << num << std::endl;
    }
    else if(token != "#" && token != "##" && index == -1)
    {
        error();
    }

    return;
}
