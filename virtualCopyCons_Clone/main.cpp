//In the virtual constructor idiom we have seen the way to construct an object whose type is not determined until runtime. 
//Is it possible to create an object without knowing it’s exact class type?
//The virtual copy constructor address this question.
//Sometimes we may need to construct an object from another existing object.
//Precisely the copy constructor does the same. The initial state of new object will be based on another existing object state.
//The compiler places call to copy constructor when an object being instantiated from another object.
//However, the compiler needs concrete type information to invoke appropriate copy constructor.
//When we want to copy construct an object from another object created by virtual constructor,
//we can’t use usual copy constructor.
//We need a special cloning function that can duplicate the object at runtime.

#include <iostream>
enum ANIMALS { MAMMAL, DOG, CAT};
class Mammal
{
public:
 Mammal():itsAge(1) { std::cout << "Mammal constructor...\n"; }
 virtual ~Mammal() {
      std::cout << "Mammal destructor...\n";
 }
 Mammal (const Mammal & rhs);
 virtual void Speak() const { std::cout << "Mammal speak!\n"; }
 virtual Mammal* Clone() { return new Mammal(*this); }//virtual constructor
 int GetAge()const { return itsAge; }
 static Mammal* create(ANIMALS type);
protected:
   int itsAge;
};

Mammal::Mammal (const Mammal & rhs):itsAge(rhs.GetAge())
{
  std::cout << "Mammal Copy Constructor...\n";
}

class Dog : public Mammal
{
public:
    Dog() { std::cout << "Dog constructor...\n"; }
    /*virtual*/   ~Dog() { std::cout << "Dog destructor...\n"; }
    Dog (const Dog & rhs);
    void Speak()const { std::cout << "Woof!\n"; }
    /*virtual*/  Mammal* Clone() { return new Dog(*this); }
};
Dog::Dog(const Dog & rhs):
Mammal(rhs)
{
    std::cout << "Dog copy constructor...\n";
}

class Cat : public Mammal
{
public:
    Cat() { std::cout << "Cat constructor...\n"; }
    /*virtual*/ ~Cat() { std::cout << "Cat destructor...\n"; }
    Cat (Cat &);
    void Speak()const { std::cout << "Meow!\n"; }
    /*virtual*/   Mammal* Clone() { return new Cat(*this); }
};

Cat::Cat(Cat & rhs):
Mammal(rhs)
{
    std::cout << "Cat copy constructor...\n";
}

Mammal *Mammal::create(ANIMALS type)
{
    switch(type)
    {
    case DOG:
        return new Dog();
        break;
    case CAT:
        return new Cat();
        break;
    case MAMMAL:
        return new Mammal();
        break;
    default:
        return NULL;
        break;
    }
}


const int NumAnimalTypes = 3;
int main()
{
//    Mammal *theArray[NumAnimalTypes];
//    Mammal* ptr;
//    int choice,i;
//    for (i = 0; i<NumAnimalTypes; i++)
//    {
//        std::cout << "(1)dog (2)cat (3)Mammal: ";
//        std::cin >> choice;
//        switch (choice)
//        {
//        case DOG:
//            ptr = new Dog;
//            break;
//        case CAT:
//            ptr = new Cat;
//            break;
//        default:
//            ptr = new Mammal;
//            break;
//        }
//        theArray[i] = ptr;
//}
//Mammal *OtherArray[NumAnimalTypes];
//for (i=0;i<NumAnimalTypes;i++)
//{
//   theArray[i]->Speak();
//   std::cout<<"Object cloned in another array"<<std::endl;
//   OtherArray[i] = theArray[i]->Clone();
//   delete theArray[i];
//}
//for (i=0;i<NumAnimalTypes;i++)
//{
//    OtherArray[i]->Speak();
//    delete OtherArray[i];
//}


   //another scenario
   std::cout<<"Which object you want to create";
   std::cout << "(1)dog (2)cat (3)Mammal: ";
   int choice;
   std::cin >> choice;
   Mammal *obj=Mammal::create(static_cast<ANIMALS>(choice));
   obj->Speak();
   Dog *d=new Dog;
   Cat *c=new Cat;
   //Mammal *m = new Dog(d);
    Mammal *m = d->Clone();
    m->Speak();
   delete obj;
   delete m;
return 0;
}
