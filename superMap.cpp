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
	// Determine load factor

	/*
	// Step 1: Check load factors and resize tables if necessary
    // Step 2: Compute indices for key in table1 and table2
    // Step 3: Initialize collision counters
    // Step 4: Check table1[index1] for key
    // Step 5: Check table2[index2] for key
    // Step 6: Insert key-value pair in table1 or table2
    */

    // Calculate the load factor of both tables
    double loadFactor1 = (double) items1 / table1.size();
    double loadFactor2 = (double) items2 / table2.size();

    // If one or both load factors are 20% or higher, resize the tables by doubling their sizes
    if (loadFactor1 >= 0.2 || loadFactor2 >= 0.2) {
        resize(table1.size() * 2);
    }

    // Calculate the indices for the key in both tables
    std::size_t index1 = hash1(key) % table1.size();
    std::size_t index2 = hash2(key) % table2.size();

    // Initialize collision counters
    int i1 = 0, i2 = 0;

    // Check table1[index1]
    if (table1[index1].key == t1()) {
        // Create a new entry in this position
        table1[index1].key = key;
        table1[index1].value = t2();
        items1++;
        return table1[index1].value;
    } else if (table1[index1].key == key) {
        // This entry has a matching key
        return table1[index1].value;
    }

    // Check table2[index2]
    if (table2[index2].key == t1()) {
        // Create a new entry in this position
        table2[index2].key = key;
        table2[index2].value = t2();
        items2++;
        return table2[index2].value;
    } else if (table2[index2].key == key) {
        // This entry has a matching key
        return table2[index2].value;
    }

    // A collision occurred, keep probing until an empty slot is found
    while (true) {
        i1++;
        i2++;
        index1 = (index1 + i1 * hash2(key)) % table1.size();
        index2 = (index2 + i2 * hash1(key)) % table2.size();

        // Check table1[index1]
        if (table1[index1].key == t1()) {
            // Create a new entry in this position
            table1[index1].key = key;
            table1[index1].value = t2();
            items1++;
            return table1[index1].value;
        } else if (table1[index1].key == key) {
            // This entry has a matching key
            return table1[index1].value;
        }

        // Check table2[index2]
        if (table2[index2].key == t1()) {
            // Create a new entry in this position
            table2[index2].key = key;
            table2[index2].value = t2();
            items2++;
            return table2[index2].value;
        } else if (table2[index2].key == key) {
            // This entry has a matching key
            return table2[index2].value;
        }
    }

}

template <class t1, class t2>
void hashMap<t1, t2>::resize(std::size_t amount)
{
	std::vector<keyVal> oldTable1 = table1;
    std::vector<keyVal> oldTable2 = table2;
    std::size_t oldSize1 = table1.size();
    std::size_t oldSize2 = table2.size();
    items1 = 0;
    items2 = 0;
    table1.resize(amount);
    table2.resize(amount);
    for (std::size_t i = 0; i < oldSize1; i++) {
        if (oldTable1[i].key != t1()) {
            (*this)[oldTable1[i].key] = oldTable1[i].value;
        }
    }
    for (std::size_t i = 0; i < oldSize2; i++) {
        if (oldTable2[i].key != t1()) {
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
