#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


//*************************************************************************************************

class TestData
{
public:
	TestData( ) { m_p = new int(100); }
	~TestData( )
	{
		delete m_p;
		cout<<" Destructor is called...."<<endl;
	}
	int* GetData( )
	{
		return m_p;
	}
	void Print( )
	{
		cout<<" Value = "<<*m_p<<endl;
	}
private:
				int* m_p;
};

//*************************************************************************************************
void Fun1( unique_ptr<int> up )
{
	cout<<" Value of up = "<<*up<<endl;
}

//*************************************************************************************************
void Fun2( unique_ptr<int>& up )
{
	cout<<" Value of up = "<<*up<<endl;
}
//*************************************************************************************************
template <class T>
void Fun3( unique_ptr<T>& up )
{
	//After processing , say the pointer up is reset.
	//reset calls the destructor of the resource
	up.reset( );
}
//*************************************************************************************************

int main()
{
	//1. Creating an unique pointer
	//------------------------------------
			unique_ptr<int> up1 = unique_ptr<int>( new int(10) );
			cout<<" Value of up1 = "<<*up1<<endl;

	//2. Assigning the unique pointer is not allowed
	//----------------------------------------------------------------
			//unique_ptr<int> up2 = up1;

	//3. Copy construction is not allowed. So cannot pass by value to a function
	//------------------------------------------------------------------------------------------------------
			//Fun1(up1);

	//4. But can pass by reference
	//----------------------------------
			Fun2(up1);

	//5. Getting raw pointer from the unique pointer
	//--------------------------------------------------------
			int* p = up1.get( );
            cout<<" Value of p= "<<*p<<endl;
	//6. You can add the unique pointer to the vector
	//----------------------------------------------------------
			std::vector< unique_ptr<int> > vecUp;
			//vecUp.push_back( up1 );							//Does not work as copy is not allowed
			vecUp.push_back( std::move(up1) );			//Works fine as move is allowed
			vecUp.push_back( unique_ptr<int>( new int(20) ));
			vecUp.push_back( unique_ptr<int>( new int(30) ));

			//After move up1 is no more valid as it has transferred
			//the owneship to the element in the vector vecUp.
			if( up1 )
			{
				cout<<"up1 is still valid...."<<endl;
			}
			else
			{
				cout<<"up1 is no more valid...."<<endl;
			}

			for_each( vecUp.begin(), vecUp.end(),[ ](unique_ptr<int>& p)
			{
				cout<<*p<<" ";
			});


	//LEAKING ALIASES FROM UNIQUE POINTER
	//7. Be careful with constructing unique pointers from aliases
	//------------------------------------------------------------------------
			int* ip = new int(50);
			std::unique_ptr<int> up3(ip);

			//Resource of up3 is now moved to the argument to the function. So up3 is no more
			//valid. Accessing up3 further crashes the program
			Fun1( std::move(up3) );

			//At the end of Fun1, argument goes out of scope and the destructor for the resource is called.
			//The pointer is no more valid. So the below call prints garbage value
			cout<<" Value of ip = "<<*ip<<endl;

	//8. Unique Ptrs has backdoor to get the pointer. If the pointers go dangling when the unique
	//pointers goes out of scope, it causes crash.
	//-----------------------------------------------------------------------------------------------------------------
			TestData* pTD = NULL;
			{
				std::unique_ptr<TestData> up4( new TestData );
				pTD = up4.get( );
				pTD->Print( );
			}
			//The following call is going to crash as alreadh the destructor of the resource of the unique pointer
			//is called
			pTD->Print( );

	//9. Be careful while getting reference to the member of the resource held by unique pointer
	//------------------------------------------------------------------------------------------------------------
			std::unique_ptr<TestData> up5( new TestData);

			//Get the reference to the member of up5
			int* ipRes = up5->GetData( );

			//After processing up5 is released
			Fun3<TestData>(up5);

			//Accessing ipRes is no more valid as the function argument is destroyed at the end of the function
			cout<<" Value of ipRes = "<<*ipRes<<endl;

	//10. Assigning unique pointers release previously owned resource
	//--------------------------------------------------------------------------------------------
			std::unique_ptr<TestData> up6( new TestData );
			std::unique_ptr<TestData> up7( new TestData );

			//Calls the destructor for the resource owned by up6, before owning
			//the new resource from up7
			up6 = std::move( up7 );

}
