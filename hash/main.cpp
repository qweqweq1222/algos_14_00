#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include "hash.h"
#include <vector>
#include "doctest.h"

int HASH_SIZE = 100;
int BLOCK_SIZE = 500;

struct my_struct{
    std::string first_name;
    std::string last_name;
    int age;
    int height;
};

std::ostream& operator << (std::ostream& ostrm, my_struct rhs) noexcept
{
    ostrm << "{" << rhs.first_name << " " << rhs.last_name << ": " << rhs.age << ", " << rhs.height << "} ";
    return ostrm;
}
std::vector <std::string> GenerateName()
{
    std::vector <std::string> first = {"Andrey", "Nionilla", "Anatoliy", "Vasiliy", "Tatiana", "Boris", "Irina", "Sofia"}; // # = 8
    std::vector <std::string> last = {"Dron", "Troik", "Bondareva", "Sadovskiy", "Vasilieva", "Zagrudnya"}; // # = 6
    std::vector <std::string> name  = { first[rand()%8], last[rand()%6]};
    return name;
}

unsigned int HashFunc (const my_struct* element)
{
    unsigned int hash = 0;
    int pow = 1;
    int p = 31;

    std::string for_hash = element->first_name + element->last_name;
    for(auto smb : for_hash)
    {
        hash += (smb - 'a' + 1) * pow;
        if(hash >= 100000)
            hash %= 100000;
        pow *= p;
    }
    return hash;
}
int Compare (const my_struct *pElement, const my_struct* pElement2)
{
    return pElement->age- pElement2->age;
}


TEST_CASE("add")
{
    lab618::CHash<my_struct, HashFunc, Compare> Hash(HASH_SIZE, BLOCK_SIZE);
    for(int i = 0; i < HASH_SIZE; ++i)
        Hash.add(new my_struct{GenerateName()[0], GenerateName()[1], 20 + rand()%55, 160 + rand()% 25});
    Hash.print();
    Hash.clear();

    SUBCASE("add")
    {
        for(int i = 0; i < HASH_SIZE; ++i)
        {
            my_struct* elem = new my_struct{GenerateName()[0], GenerateName()[1], 20 + rand() % 55, 160 + rand() % 25};
            Hash.add(elem);
        }

        Hash.add( new my_struct{"Im", "Poster", 150, 50});
        CHECK(Hash.add( new my_struct{"Im", "Poster", 150, 50}) == false);
    }

    SUBCASE("find")
    {
        for(int i = 0; i < HASH_SIZE; ++i)
        {
            my_struct* elem = new my_struct{GenerateName()[0], GenerateName()[1], 20 + rand() % 55, 160 + rand() % 25};
            Hash.add(elem);
        }

        my_struct* imposter = new my_struct{"Im", "Poster", 150, 50};
        Hash.add(imposter);
        Hash.find(my_struct{"Im", "Poster", 150, 50});
        CHECK(imposter == Hash.find(my_struct{"Im", "Poster", 150, 50}));
        Hash.remove(my_struct{"Im", "Poster", 150, 50});

        delete imposter;
    }

    SUBCASE("remove")
    {
        for(int i = 0; i < HASH_SIZE; ++i)
        {
            my_struct* elem = new my_struct{GenerateName()[0], GenerateName()[1], 20 + rand() % 55, 160 + rand() % 25};
            Hash.add(elem);
        }

        my_struct* imposter = new my_struct{"Im", "Poster", 150, 50};
        Hash.add(imposter);
        CHECK(Hash.remove(my_struct{"Im", "Poster", 150, 50}) == true);
        CHECK(Hash.remove(my_struct{"Im", "Poster", 150, 50}) == false);

        delete imposter;
    }
    SUBCASE("update")
    {
        for(int i = 0; i < HASH_SIZE; ++i)
        {
            my_struct* elem = new my_struct{GenerateName()[0], GenerateName()[1], 20 + rand() % 55, 160 + rand() % 25};
            Hash.add(elem);
        }

        my_struct* imposter = new my_struct{"Im", "Poster", 150, 50};
        Hash.add(imposter);
        CHECK(Hash.update(imposter) == true);
        CHECK(Hash.update(new my_struct {"Imposter", "Friend", 150, 51}) == false);
        Hash.remove(my_struct{"Im", "Poster", 150, 50});

        delete imposter;
    }
}


/*
 * int main(void)
{
    lab618::CHash<my_struct, HashFunc, Compare> Hash(HASH_SIZE, BLOCK_SIZE);
    for(int i = 0; i < HASH_SIZE; ++i)
        Hash.add(new my_struct{GenerateName()[0], GenerateName()[1], 20 + rand()%55, 160 + rand()% 25});
    my_struct* imposter = new my_struct{"Im", "Poster", 150, 50};
    Hash.add(imposter);
    Hash.find(my_struct{"Im", "Poster", 150, 50});
    if(imposter == Hash.find(my_struct{"Im", "Poster", 150, 50}))
        std::cout << "YES";
    return 0;
}
 */