#include <iostream>
#include <string.h>

using namespace std;

template <typename T>
void swap(T& i, T& j)
{
    T temp = i;
    i = j;
    j = temp;
}


struct greater_than
{
    template<typename T>
    bool operator()(const T& lhs, const T& rhs) const {
        return lhs < rhs;
    }
};

/**
 * Start from the beginning and find the first i such that
 *     order(x[i],pivot) == true
 * If i == last, return i (which equals last).
 */ 
template <typename T, typename ORDER>
int find_first(const T x[], T pivot, int first, int last, ORDER order = greater_than())
{
    int i = first;
    while ( !order(x[i], pivot) && (i < last) ) {
        i++;
    }
    return i;
}

/**
 * Start from the *end* and find the first j such that
 *      order(x[j],pivot) == false.
 */ 
template<typename T, typename ORDER>
int reverse_find_first_not(const T x[], T pivot, int last, ORDER order = greater_than())
{
    int j = last;
    while ( order(x[j],pivot) ) {
        j--;
    }
    return j;
}

/**
 * When we leave partition, x is partitioned such that [first, j-1] is less
 * than the initial x[first], and [j+1, last] is greater than the initial
 * x[first]. The value that *was* in x[first] will now be in x[j].
 */
template<typename T, typename ORDER>
int partition(T x[], int first, int last, ORDER order = greater_than())
{
    int i=first;
    int j=last;
    int pivot = first;
    
    while (i<j)
    {
        i = find_first(x, x[pivot], i, j, order);
        j = reverse_find_first_not(x, x[pivot], j, order);
        if (i<j) 
        {
            ::swap(x[i], x[j]);
        }
    }
    ::swap(x[pivot], x[j]);
    
    return j;
}

template<typename T, typename ORDER>
void quicksort(T x[], int first, int last, ORDER order = greater_than())
{
     if (first<last)
     {
        int j= partition(x, first, last, order);
        quicksort(x,first,j-1, order);
        quicksort(x,j+1,last, order);
    }
}

template<typename T, typename ORDER>
void quicksort(T x[], int size, ORDER order)
{
    quicksort(x, 0, size-1, order);
}

template<typename T>
void quicksort(T x[], int size)
{
    quicksort(x, 0, size-1, greater_than());
}

template<typename T, typename Compare>
T* binary_search(T x[], int size, Compare compare)
{
    int bottom = 0;
    int top = size-1;
    while (bottom <= top)
    {
        int mid = (top + bottom) / 2;
        int result = compare(x[mid]);
        if (result == 0)
        {
            return &x[mid];
        }
        else if (result < 0)
        {
            bottom = mid + 1;
        }
        else
        {
            top = mid - 1;
        }
    }
    return 0;
}
 


struct Name 
{
public:
    Name(const char* first, const char* middle, const char* last)
    : first_(new char[strlen(first)+1]),
      middle_(new char[strlen(middle)+1]),
      last_(new char[strlen(last)+1])
    {
        strcpy(first_, first);
        strcpy(middle_, middle);
        strcpy(last_, last);
    }
    
    Name(const Name& name)
    : first_(new char[strlen(name.first_)+1]),
      middle_(new char[strlen(name.middle_)+1]),
      last_(new char[strlen(name.last_)+1])
    {
        strcpy(first_, name.first_);
        strcpy(middle_, name.middle_);
        strcpy(last_, name.last_);
    }
    
    Name& operator=(const Name& name)
    {
        first_ = new char[strlen(name.first_)+1];
        strcpy(first_, name.first_);
        middle_ = new char[strlen(name.middle_)+1];
        strcpy(middle_, name.middle_);
        last_ = new char[strlen(name.last_)+1];
        strcpy(last_, name.last_);
        return *this;
    }
    
    ~Name() 
    { 
        delete first_; 
        delete middle_; 
        delete last_; 
    }
    
    void write(ostream& out) const
    {
        out << last_ << ", " << first_ << ", " << middle_;
    }
    
    int compare(const Name& rhs) const {
        int result = strcmp(last_, rhs.last_);
        if (result != 0) {
            return result;
        } 
        result = strcmp(first_, rhs.first_);
        if (result != 0) {
            return result;
        } 
        return strcmp(middle_, rhs.middle_);
    }
    
private:
    char* first_;
    char* middle_;
    char* last_; 
};

inline ostream& operator<<(ostream& out, const Name& name)
{
    name.write(out);
    return out;
}

class Employee 
{
public:
    Employee(const char* first, const char* middle, const char* last, double salary)
    : name_(first, middle, last),
      salary_(salary)
    {
    }

    int compare(const Name& name) const {
        return name_.compare(name);
    }

    int compare(const Employee& Employee) const {
        return name_.compare(Employee.name_);
    }

    void write(ostream& out) const
    {
        out << name_ << " - " << salary_;
    }
private:    
    Name name_;
    double salary_;
};

inline ostream& operator<<(ostream& out, const Employee& employee)
{
    employee.write(out);
    return out;
}

template<typename T>
void print_array(T data[], size_t num)
{
    for (size_t i=0; i<num; ++i) {
        cout << data[i];
        cout << "\n";
    }
}

template<typename T>
void print_array(T* data[], size_t num)
{
    for (size_t i=0; i<num; ++i) {
        cout << *data[i];
        cout << "\n";
    }
}

struct name_compare {
    name_compare(const Name& n) : name_(n) { }
    int operator()(const Employee& employee) const {
        return employee.compare(name_);
    }
    int operator()(const Employee* employee) const {
        return employee->compare(name_);
    }
private:
    Name name_;
};

struct reverse_name_compare {
    reverse_name_compare(const Name& n) : name_(n) { }
    int operator()(const Employee& employee) const {
        return -employee.compare(name_);
    }
    int operator()(const Employee* employee) const {
        return -employee->compare(name_);
    }
private:
    Name name_;
};

struct employee_greater_than
{
    bool operator()(const Employee& lhs, const Employee& rhs) const
    {
        return lhs.compare(rhs) > 0;
    }
    bool operator()(const Employee* lhs, const Employee* rhs) const
    {
        return lhs->compare(*rhs) > 0;
    }
};

struct employee_less_than
{
    bool operator()(const Employee& lhs, const Employee& rhs) const
    {
        return lhs.compare(rhs) < 0;
    }
    bool operator()(const Employee* lhs, const Employee* rhs) const
    {
        return lhs->compare(*rhs) < 0;
    }
};

void int_example()
{
    cout << "\n***" << __func__ << endl;
    int x[] = { 10, 1, 2, 5, 3, 7, 6, 9, 4, 8, 0 };
    static const int num_elements = sizeof(x)/sizeof(x[0]);
    quicksort(x, num_elements);
    print_array(x, num_elements);
    cout << endl;
}

void double_example()
{
    cout << "\n***" << __func__ << endl;
    double y[] = { 10.5, 1.4, 2.3, 5.4, 3.5, 7.2, 6.1, 9.9, 4.6, 8.7 };
    static const int ynum_elements = sizeof(y)/sizeof(y[0]);
    quicksort(y, ynum_elements);
    print_array(y, ynum_elements);
    cout << endl;
}

void employee_example()
{
    cout << "\n***" << __func__ << endl;
    Employee employees[] = {
        Employee("M", "Night", "Shyamalan", 1.98),
        Employee("Steve", "Eliot", "Nix", 2.25),
        Employee("George", "X", "McKee", 3.15),
        Employee("James", "007", "Bond", 100.25)
    };
    static const int num_employees = sizeof(employees)/sizeof(employees[0]);
    quicksort(employees, num_employees, employee_greater_than());
    print_array(employees, num_employees);
    
    Name name("James", "007", "Bond");
    Employee* eptr = binary_search(employees, num_employees, name_compare(name));
    if (eptr == 0)
    {
        cout << "\n" << "Not found: " << name << endl;
    }
    else
    {
        cout << "\n" << *eptr << endl;
    }
    cout << endl;
}


void reverse_employee_example()
{
    cout << "\n***" << __func__ << endl;
    Employee employees[] = {
        Employee("M", "Night", "Shyamalan", 1.98),
        Employee("Steve", "Eliot", "Nix", 2.25),
        Employee("George", "X", "McKee", 3.15),
        Employee("James", "007", "Bond", 100.25)
    };
    static const int num_employees = sizeof(employees)/sizeof(employees[0]);
    quicksort(employees, num_employees, employee_less_than());
    print_array(employees, num_employees);
    
    Name name("James", "007", "Bond");
    Employee* eptr = binary_search(employees, num_employees, reverse_name_compare(name));
    if (eptr == 0)
    {
        cout << "\n" << "Not found: " << name << endl;
    }
    else
    {
        cout << "\n" << *eptr << endl;
    }
}

void employee_ptr_example()
{
    cout << "\n***" << __func__ << endl;
    Employee* employees[] = {
        new Employee("M", "Night", "Shyamalan", 1.98),
        new Employee("Steve", "Eliot", "Nix", 2.25),
        new Employee("George", "X", "McKee", 3.15),
        new Employee("James", "007", "Bond", 100.25)
    };
    static const int num_employees = sizeof(employees)/sizeof(employees[0]);
    quicksort(employees, num_employees, employee_greater_than());
    print_array(employees, num_employees);
    
    Name name("James", "007", "Bond");
    Employee** eptr = binary_search(employees, num_employees, name_compare(name));
    if (eptr == 0)
    {
        cout << "\n" << "Not found: " << name << endl;
    }
    else
    {
        cout << "\n" << **eptr << endl;
    }
    
    for (int i=0; i<num_employees; ++i) {
        delete employees[i];
    }
    cout << endl;
}

void reverse_employee_ptr_example()
{
    cout << "\n***" << __func__ << endl;
    Employee* employees[] = {
        new Employee("M", "Night", "Shyamalan", 1.98),
        new Employee("Steve", "Eliot", "Nix", 2.25),
        new Employee("George", "X", "McKee", 3.15),
        new Employee("James", "007", "Bond", 100.25)
    };
    static const int num_employees = sizeof(employees)/sizeof(employees[0]);
    quicksort(employees, num_employees, employee_greater_than());
    print_array(employees, num_employees);
    
    Name name("James", "007", "Bond");
    Employee** eptr = binary_search(employees, num_employees, reverse_name_compare(name));
    if (eptr == 0)
    {
        cout << "\n" << "Not found: " << name << endl;
    }
    else
    {
        cout << "\n" << **eptr << endl;
    }
    
    for (int i=0; i<num_employees; ++i) {
        delete employees[i];
    }
    cout << endl;
}

int main()
{
    int_example();
    double_example();
    employee_example();
    reverse_employee_example();
    employee_ptr_example();
    reverse_employee_example();
}
