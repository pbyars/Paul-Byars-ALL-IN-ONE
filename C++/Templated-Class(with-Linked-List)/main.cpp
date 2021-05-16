/*
	Computer Science III
	Assignment #5
	
	This program will create a templated stack class that uses a dynamic linked structure
        to represent the list. It is a group project with three members. It contains a series of 
	implementations to produce the desired results.	
	
	Due: Monday, February 17th, 2020 @ 7:30 AM 
	Programmed By: Ethan Hood, Khoi Vo, & Paul Byars
*/

#include <iostream>
#include "stackList.h"

using namespace std;

// FUNCTION PROTOTYPES

void printMenu();

int main()
{
	Stack<int> stack;
	char choice = ' ';
	int value = 0;
	int key = 0;
	printMenu();

	
	Stack<int> s;
	s.push(1);		
	s.push(2);		
	s.push(3);		
	s.push(4);		
	s.push(5);		
	s.push(6);

	Stack<int> s1(s);
	Stack<int> s2;

	do{
		cin >> choice;

		switch(choice){
		case '+':
			cin >> value;
			stack.push(value);
			stack.display();
			break;

		case '-':
			stack.pop();
			stack.display();
			break;

		case '?': 
			cin >> key;
			stack.search(key);
			cout << key << endl;
			stack.display();	
			break;

		case 'P':
		case 'p':
			stack.display();
			break;

		case '@':
			cout << stack.getCursorData() << endl;
			break;

		case '1':
			cout << "Here is stack s1 after copy from s:" << endl;
			s1.display();
			break;

		case '2':
			cout << "Here is stack s2 after assignment:" << endl;
			s2 = s1;
			s2.display();
			break;

		case '3':
			stack.~Stack<int>();
			cout << "Destructor called" << endl;
			break;

		case 'M':
		case 'm':
			printMenu();
			break;

		case 'Q':
		case 'q':
			cout << "Goodbye..." << endl;
			break;

		default: 
			cout << "Incorrect input of a choice from the menu. Try again..." << endl;
			break;
		}

	}while(choice != 'Q' && choice != 'q');

	return 0;
}

// Function: printMenu
// Purpose: print out the menu
// Incoming: none
// Outgoing: none
// Return: none

void printMenu()
{
	cout << "+  Insert an item" << endl
		<< "-  Remove an item" << endl
		<< "?  Search for an item" << endl
		<< "P  Print the list" << endl
		<< "@  Print the item at the cursor" << endl
		<< "1  Test copy constructor" << endl
		<< "2  Test assignment operator" << endl
		<< "3  Test destructor" << endl
		<< "M  Print this menu again" << endl
		<< "Q  End this program" << endl << endl;
}

