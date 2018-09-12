#include <memory>
#include <iostream>
using namespace std;

//***********************************************************************************************
class Dealloc
{
public:
	Dealloc() {}
	void operator() (int* p )
	{
		if( p )
		{
			//Do the custom deallocation job
			cout<<" Dealloc called to release the resource "<<p<<" whose value is "<<*p<<endl<<endl;
			delete p;
			p = NULL;
		}
	}
};
//***********************************************************************************************
class Base
{
public:
	Base() {}
	virtual void Foo() {}
};
//***********************************************************************************************
class Derived : public Base
{
public:
	Derived() {}
};
//***********************************************************************************************



int main()
{
	//1. Creating shared pointer using make_shared. make_shared internally takes care
	//of creating the pointer for the resource and the control block
	auto sp1 = make_shared<int>(10);
	cout<<*sp1<<endl<<endl;

	//2. Creating shared pointer from another shared pointer
	auto sp2( sp1);
	auto sp3 = sp2;

	//3. Creating a shared pointer from a raw pointer
	auto sp4 = shared_ptr<int>( new int(100) );

	//4. Creating shared pointer with a nullptr
//	auto sp5 = shared_ptr<int>(nullptr);
//	if( sp5 == NULL )
//	{
//		cout<<"Null pointer"<<endl<<endl;
//	}

	//5. Getting raw pointer from the shared pointer
	int* pRaw = sp2.get( ); //or sp1.get() or sp3.get()
    cout<<"Pointer value:"<<*pRaw<<endl;
	//6. Get how many shared pointers sharing the resource
	int nbReferences = sp1.use_count();
	cout<<" Number of shared pointers sharing the resource = "<<nbReferences<<endl<<endl;

	//7. Is this only shared pointer sharing the resource
	cout<<"Is sp1 is the only sharing pointer sharing the resource = "<< (sp1.unique() ? "Yes" : "No")<<endl<<endl;
	cout<<"Is sp4 is the only sharing pointer sharing the resource = "<< (sp4.unique() ? "Yes" : "No")<<endl<<endl;

	//8. Dont try to have more than shared pointer from a raw pointer. This cause
	//the twice the deletion of the resource and hence acesss violation
	int* p = new int(200);
	auto sp1FromRaw = shared_ptr<int>(p);


	//Uncomment the below lines to get the access violation.

	//auto sp2FromRaw = shared_ptr<int>(p);
	//Solution of above
//	auto sp2FromRaw = sp1FromRaw;
//	auto sp3FromRaw = sp2FromRaw;

	//Notice to get the reference count of either sp1FromRaw or sp2FromRaw. It will give the
	//count as 1 instead of 2 as they are created from the raw pointer.
//	 nbReferences = sp2FromRaw.use_count(); //or  sp1FromRaw.use_count();
//	cout<<" Reference count for raw pointer p = "<<nbReferences<<endl<<endl;

	//9.Using custom deallocator for creating the shared pointer. The deallocator is a
	//function object which takes the pointer to be deleted as an argument
	auto sp6 = shared_ptr<int>(new int(1000), Dealloc() );
	auto sp7(sp6);
    nbReferences = sp7.use_count(); //or  sp1FromRaw.use_count();
	cout<<" SP7 usecount before block = "<<nbReferences<<endl<<endl;
	//10. Check how the resource is freed up when the reference count to zero
	{
		auto sp8 = shared_ptr<int>( new int[5],[ ](int* p)
														{
															cout<<"Releasing array of objects..."<<endl;
															delete[ ] p;
														});
		{
			auto sp9(sp8);
			nbReferences = sp8.use_count();
			cout<<" reference for array object in the inner block = "<<nbReferences<<endl<<endl;
		}
		nbReferences = sp8.use_count();
		cout<<" reference for array object in the outer block after sp9 expires = "<<nbReferences<<endl<<endl;
	} //At the end of this scope ref count of sp7 goes to zero  hence calling the deallocator Dealloc1


	//11.Using dynamic_cast_pointer on shared pointer
	auto sp10 = shared_ptr<Derived>( new Derived( ) );
	shared_ptr<Base> sp11 = dynamic_pointer_cast<Base>(sp10);
	//shared_ptr<Base> sp11 = sp10;
	//if( sp11.get( ) != nullptr )
    if( sp11.get( ) != 0 )
	{
		cout<<"Dynamic casting from sp10 to sp11 succeeds...."<<endl<<endl;
	}

	auto sp12 = shared_ptr<Base>( new Base() );
	shared_ptr<Derived> sp13 = dynamic_pointer_cast<Derived>(sp12);
	if( sp13 )
	{
		cout<<"Dynamic casting from 12 to 13 succeeds...."<<endl<<endl;
	}
	else
	{
			cout<<"Dynamic casting from sp12 to sp13 failed ...."<<endl<<endl;
	}

	auto sp14 = shared_ptr<Base> (new Derived());
	auto sp15 = dynamic_cast<Derived*>(sp14.get());
	if( sp15 )
	{
		cout<<"Dynamic casting from 14 to 15 succeeds...."<<endl<<endl;
	}
	else
	{
			cout<<"Dynamic casting from sp14 to sp15 failed ...."<<endl<<endl;
	}
}
