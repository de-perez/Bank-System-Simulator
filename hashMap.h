#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>

//      string key, int value
template <class t1, class t2>
class hashMap
{
public:
	hashMap(std::size_t = 5);
	t2& operator[](t1);
private:
	struct keyVal
	{
		keyVal()
		{
			key = t1();
			value = t2();
		}

		t1 key;
		t2 value;
	};

	void resize(std::size_t);

	std::size_t hash1(std::string);
	std::size_t hash2(std::string);

	std::vector<keyVal> table1;
	std::vector<keyVal> table2;

	std::size_t items1;
	std::size_t items2;
};

template <class t1, class t2>
hashMap<t1, t2>::hashMap(std::size_t size)
{
	items1 = items2 = 0;
	table1.resize(size);
	table2.resize(size);
}

template <class t1, class t2>
t2& hashMap<t1, t2>::operator[](t1 key)
{
    /*
    FUNCTION_IDENTIFIER: This operator overload function will help with finding
    and inserting entries to the hashmap. It will also resize the tables if the
    load factor is above 20%.
    Return Value: table value at index
    */

	// 8:06
	// 10:38 blank entry t1()

    
    // Step 1: Calculate the Load Factors
	// (double) turns the variable temporarily into a double for artihmetic
    double table1LF = (double) items1 / table1.size(); 
    double table2LF = (double) items2 / table2.size();

    // If one or both load factors are 20% or higher, resize the tables by doubling their sizes
    if (table1LF >= 0.2 || table2LF >= 0.2)
	{
		// Resize function resizes both tables
        this->resize(table1.size() * 2);
    }

    // Step 2 Calculate the indices for the key in both tables
    std::size_t index1 = hash1(key) % table1.size();
    std::size_t index2 = hash2(key) % table2.size();

    // Step 3: Initialize collision counters
    int i1 = 0, i2 = 0;


    while (true)
    {
    // Step 4: Check table1[index1]
    // t1 is default value. If it is = to default, it was just made.
    // Therefore, it did not previously exist.
    if (table1[index1].key == t1()) 
    {
        // Create a new entry in this position
        // by setting the key and value of the entry.
        table1[index1].key = key;
        table1[index1].value = t2();
        items1++;

        return table1[index1].value;
    } 
    else if (table1[index1].key == key)
    {
        // This entry has a matching key.
        // This means an entry already existed.
        return table1[index1].value;
    }

    // Step 4 Part 2: Check table2[index2]
    if (table2[index2].key == t1()) {
        // Create a new entry in this position
        table2[index2].key = key;
        table2[index2].value = t2();
        items2++;

        return table2[index2].value;
    }
    else if (table2[index2].key == key)
    {
        // This entry has a matching key
        return table2[index2].value;
    }

    // If function has reached this point, a collision occurred.
    // Keep probing until an empty slot is found
    
        // Update both collision counters
        i1++;
        i2++;
        // index1 = (index1 + i1 * hash2(key)) % table1.size();
        // index2 = (index2 + i2 * hash1(key)) % table2.size();

        // Update index 1 and 2
        if (i1 % 2 == 0)
        {
            index1 = ( index1 + i1 * hash2 ( key )) % table1 . size (); // on even iterations
            index2 = ( index2 + i2 * hash1 ( key )) % table2 . size (); // on even iterations
        }
        else
        {
            index2 = ( index2 + i2 * hash2 ( key )) % table2 . size (); // on odd iterations
            index1 = ( index1 + i1 * hash1 ( key )) % table1 . size (); // on odd iterations
        }
    }

}

template <class t1, class t2>
void hashMap<t1, t2>::resize(std::size_t amount)
{
    /*
    FUNCTION_IDENTIFIER: This resize function will resize both
    tables 1 and 2. It will create two tables to hold the old tables.
    Then, it resizes the tables with the amount parameter. After, it will 
    iterate through each element in table 1 and 2, and if the key is not equal
    to the default value it will insert a key-value pair into the key index.
    Return Value: Void
    */

    // Resize by creating bigger tables and copying data.

    // Create new tables to hold the old ones
	std::vector<keyVal> oldTable1 = table1;
    std::vector<keyVal> oldTable2 = table2;
    size_t oldTable1Size = table1.size();
    size_t oldTable2Size = table2.size();

    // Reset # of items currently in the tables
    items1 = 0;
    items2 = 0;
    table1.resize(amount); // Resize by parameter
    table2.resize(amount); // Resize by parameter

    // Iterate through table1
    for (size_t i = 0; i < oldTable1Size; i++)
    {
        // If 
        if (oldTable1[i].key != t1())
        {
            (*this)[oldTable1[i].key] = oldTable1[i].value;
        }
    }
    // Iterate through table2
    for (std::size_t i = 0; i < oldTable2Size; i++)
    {
        if (oldTable2[i].key != t1())
        {
            (*this)[oldTable2[i].key] = oldTable2[i].value;
        }
    }
}

template <class t1, class t2>
std::size_t hashMap<t1, t2>::hash1(std::string key)
{
	size_t mySum = 0;
	for (int i = 0; i < key.length(); i++)
	{
		mySum += key[i] * pow(10, i);
	}

	return mySum;
}

template <class t1, class t2>
std::size_t hashMap<t1, t2>::hash2(std::string key)
{
	size_t mySum = 0;
	for (int i = 0; i < key.length(); i++)
	{
		mySum += key[ (key.length()-1) - i] * pow(10, i);
	}

	return mySum;
}
