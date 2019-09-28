# Using Custon allocators: 

Consider following class,

```cpp

class Person
{
  int age;
  string name;
  
  
  public:
   
   Person();
   Person(int age);
   Person(int age, name);
}

```

Now to use custom allocators you need to make sure you are including **fmemory.h** in your file.

Now We'll consider different scenarios and how to use fnew in there.

## Allocating a single variable:

If you want to creat a variable of our Test class you can use fnew / fdelete as follows,

```cpp
//Creating objects using different constructors

//Default constructor
Person* ptr = fmemory::fnew<Person>();

//Constructor with params
Person* ptr = fmemory::fnew<Person>(10);
Person* ptr = fmemory::fnew<Person>(7,"CR");


//Deleting the object
fdelete<Person>(ptr);
```

As you can see fnew internally takes care of calling appropriate constuctor for the object.


##  Allocating an Array:

To create an array of objects, you will use **fnew_arr** and make sure that **first paramter is the count** of the array elements.
Deletion function remains the same. Please go through following example.

```cpp
//Creating an array

//Default constructor. This will create an array of 10 elements with the default values.
Person* ptr = fmemory::fnew_arr<Person>(10); 


//Constructor with params. This will create an array of 10 elements and initialize them to the parameters passed by 
//calling relevent constructor.
Person* ptr = fmemory::fnew<Person>(10,15);
Person* ptr = fmemory::fnew<Person>(10,7,"CR");


//Deleting the object
fdelete<Person>(ptr);

```

**Even though array creating is possible, I'd recommend using STL whereever possible as it'd be cleaner code to read and doesn't
have any performance overheads.**


## Custom allocators with the STL:

STL always take a second parameter in intialization. For example declaration of vector is,
```cpp
template<
    class T,
    class Allocator = std::allocator<T>
> class vector;
```

Now the allocator paramater is the one which tell vector to which allocator to use. So just by replacing that one we can make it use our 
custom allocators.

For more details you can checkout **STLInterface.h** . 

Example to create a vector of persons,

```cpp
vector<Person, fmemory::STLAllocator<Person>> vPerson; 
```
You don't need to take care of clearing or deleting anythin as STLAllocator will handle that for you. 







