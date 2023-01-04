#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <fstream>

class Identifier
{
public:
    Identifier(const std::string& name) :
        m_name(name)
    {
    }

public:
    std::string name() const
    {
        return m_name;
    }

private:
    std::string m_name;
};

bool operator==(const Identifier& left, const Identifier& right)
{
    return left.name() == right.name();
}

size_t hash(const Identifier& id)
{
    if (id.name().length() == 1)
        return 2 * size_t(id.name()[0]);

    return size_t(id.name()[0]) + size_t(id.name()[1]);
}

class IDNotFoundException : std::exception
{
public:
    IDNotFoundException(const std::string id_name) :
        m_what(std::string("Идентификатор \'") + id_name + "\' не найден!")
    {
    }

    virtual ~IDNotFoundException() throw()
    {
    }

public:
    const char* what() const throw()
    {
        return m_what.c_str();
    }

private:
    std::string m_what;
};

class HashTable
{
public:
    static const size_t min_hash_value = int('A') + int('0');
    static const size_t max_hash_value = int('z') + int('z');
    static const size_t hash_table_size = max_hash_value - min_hash_value;

public:
    void add(const Identifier& id)
    {
        m_hash_table[hash(id) - min_hash_value].push_back(id);
    }

    Identifier& get(const std::string& id_name)
    {
        std::list<Identifier>& line = m_hash_table[hash(id_name) - min_hash_value];

        std::list<Identifier>::iterator it =
            std::find(line.begin(), line.end(), id_name);

        if (it == line.end())
            throw IDNotFoundException(id_name);

        return *it;
    }

private:
    std::list<Identifier> m_hash_table[hash_table_size];
};

void ReadFile(HashTable &ht)
{
    std::ifstream fin("File.txt");

    std::string n;

    if (!fin.good())
    {
        std::cout << "Файл File.txt не найден!\n";
    }
    else {
        while (fin >> n)
        {
            ht.add(Identifier(n));
        }
        std::cout << "Данные из File.txt получены!:\n";

    }
    fin.close();
}


int main()
{
    setlocale(LC_ALL, "rus");

    HashTable ht;

    ReadFile(ht);

    char choice;
    while (true)
    {
        std::cout << "Хотите ввести запрос для поиска? (y/n) ";
        std::cin >> choice;
        if (choice == 'y')
        {
            try
            {
                std::string search;
                std::cout << "Введите запрос для поиска : ";
                std::cin >> search;
                std::cout << "Идентификатор найден : " << ht.get(search).name() << std::endl;
            }
            catch (const IDNotFoundException& ex)
            {
                std::cerr << ex.what() << std::endl;
            }
        }
        else if (choice == 'n')
        {
            break;
        }
        else
        {
            std::cout << "Введите 'y' или 'n' ";
        }
    }



    return 0;
}