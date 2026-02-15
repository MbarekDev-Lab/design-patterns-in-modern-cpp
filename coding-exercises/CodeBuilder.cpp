#include <string>
#include <ostream>
#include <vector>
#include <iostream>
using namespace std;

class CodeBuilder
{
    string class_name;
    vector<pair<string, string>> fields;

public:
    CodeBuilder(const string &class_name)
        : class_name(class_name)
    {
    }

    CodeBuilder &add_field(const string &name, const string &type)
    {
        fields.emplace_back(type, name);
        return *this;
    }

    friend ostream &operator<<(ostream &os, const CodeBuilder &obj)
    {
        os << "class " << obj.class_name << "\n{\n";

        for (const auto &field : obj.fields)
        {
            os << "  " << field.first << " " << field.second << ";\n";
        }

        os << "};";
        return os;
    }
};

int main()
{
    CodeBuilder cb{"Person"};
    cb.add_field("name", "string")
        .add_field("age", "int");

    std::cout << cb << std::endl;
    std::cout << std::endl;

    return 0;
}

/*

benraiss@Mbareks-MacBook-Air coding-exercises % clang++ -std=c++17 CodeBuilder.cpp -o CodeBuilder && ./CodeBuilder
class Person
{
  string name;
  int age;
};

Client code
   ↓
CodeBuilder("Person")
   ↓
.add_field()
   ↓
.add_field()
   ↓
operator<<
   ↓
Formatted class definition
*/
