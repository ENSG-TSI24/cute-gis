class ExampleClass {
public :
    
    ExampleClass();

    ~ExampleClass();

    auto get() const;

    void set();

private:

    // attributes

//So that the test classes can directly access the private attributes of the class under test.
    friend class ExampleClassTest_SUBTEST1_Test;
    friend class ExampleClassTest_SUBTEST2_Test;
};


