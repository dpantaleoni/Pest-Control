/***************************************
Pest Control
Author: Dominik Pantaleoni
Date Completed: 5/20/24
Description: Program that helps team leads assign
bugs to teamleads in xml format. Will be converted
from xml to html by front-end team.
***************************************/


#include <iostream>
#include <fstream>
#include "pugixml.hpp"
#include "HashTable.hpp"
#include "BinarySearchTree.hpp"
using namespace std;

class Bug {

    private:
    string m_id;
    string m_description;
    string m_impact;
    string m_date;
    bool m_assigned;

    public:

    //parameterized constructor
    Bug(string id, string description, string impact, string date)
     : m_id(id), m_description(description), m_impact(impact), m_date(date) {};

    // // Setter  **don't need setters
    // void setid(string s) {
    //   m_id = s;
    // }

    string getid()
    {
        return m_id;
    }

    string getdescription() {
        return m_description;
    }

    string getimpact()
    {
        return m_impact;
    }

    string getdate()
    {
        return m_date;
    }


    bool operator < (Bug const &rhs) {
        if (m_date < rhs.m_date) {
            return true;
        }
        
        return false;
    }

    bool operator > (Bug const &rhs) {
        if (m_date > rhs.m_date) {
            return true;
        }
        
        return false;
    }


};

// class Developer {  **developer custom data type not needed

//     private:
//     string m_id;
//     Bug m_hiBug;
//     Bug m_midBug;
//     Bug m_loBug;

//     public:
//     Developer(string id, Bug hiBug, Bug midBug, Bug loBug)
//      : m_id(id), m_hiBug(hiBug), m_midBug(midBug), m_loBug(loBug) {};

//     string getid()
//     {
//         return m_id;
//     }

//     Bug gethiBug()
//     {
//         return m_hiBug;
//     }

//     Bug getmidBug()
//     {
//         return m_midBug;
//     }

//     Bug getloBug()
//     {
//         return m_loBug;
//     }
    
// };

int main () {
    

    //get number of developers from cin
    cout << "Enter the number of developers you wish to assign bugs: ";
    int devCount = 0;
    cin >> devCount;


    // instantiate the XML doc
    pugi::xml_document doc;

    // Load the "bugs.xml" file into the xml document variable
    // if a problem occurred while loading, show error and end program
    if (!doc.load_file("bugs.xml"))
    {
        cerr<<"Problem opening xml file \"bugs.xml\"\n";
        return 1;
    }

    // //create hash table to store luminaries   **hash table not needed
    // HashTable<Bug> BugTable;

    //create  3 BST's: 1 for each impact level
    BinarySearchTree<Bug> hiBugBST;
    BinarySearchTree<Bug> midBugBST;
    BinarySearchTree<Bug> loBugBST;

    // Instantiate an xml node object to represent the root element in the xml document. 
    // The root element in an xml document is the topmost element in the document.
    // For the "bugs.xml" file, the root element is the <bugs> element.
    pugi::xml_node root = doc.first_child();

    // Loop over all <bug> element children of the root element
    for (pugi::xml_node bugnode : root.children("bug"))
    {   
        //create bug
        string id = bugnode.attribute("id").as_string();
        string description = bugnode.child_value("description");
        string impact = bugnode.child_value("impact");
        string date = bugnode.child_value("created");
        Bug bug(id, description, impact, date);


        // //add bugs to hashtable    **hashtable not needed
        // BugTable.add(id, bug);


        //put bug into its correct BST; if no impact is assigned, goes into loBugBST
        if (bug.getimpact() == "high") {
            hiBugBST.insert(bug);
        } else if (bug.getimpact() == "medium") {
            midBugBST.insert(bug);
        } else {
            loBugBST.insert(bug);
        };

    };


    // Instantiate ofstream stream object to interact with output file
    ofstream outFile;

    // Open output file stream in "truncate" mode
    outFile.open("report.xml", ofstream::trunc);

    // Check to make sure the file was opened successfully
    if (!outFile.is_open())
    {
        cerr<<"Can't open report.xml!"<<endl;
        return 1;
    }

    // Write data to the file via insertion operator
    outFile<<"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"<<endl;

    outFile<<"<report>"<<endl;

    //now output developers and their assigned bugs, and then the unassigned bugs, in order into the outFile

    //create linked list for each BST
    LinkedList<Bug> hiBugLinkedList = hiBugBST.getSortedList();
    LinkedList<Bug> midBugLinkedList = midBugBST.getSortedList();
    LinkedList<Bug> loBugLinkedList = loBugBST.getSortedList();


     //create developers and assign bugs, then create xml for them
    for ( int i=1; i<devCount+1; i++ ) {

        string id = to_string(i);

        outFile<<"<developer id=\"" << id << "\">"<<endl;

        if (!hiBugLinkedList.empty()) {
            Bug hiBug = hiBugLinkedList.popFront();

            outFile<<"<bug id=\"" << hiBug.getid() << "\">"<<endl;
            outFile<<"<description>"<< hiBug.getdescription() <<"</description>"<<endl;
            outFile<<"<impact>"<< hiBug.getimpact() <<"</impact>"<<endl;
            outFile<<"<created>"<< hiBug.getdate() <<"</created>"<<endl;
            outFile<<"</bug>"<<endl;
        };

        if (!midBugLinkedList.empty()) {
            Bug midBug = midBugLinkedList.popFront();

            outFile<<"<bug id=\"" << midBug.getid() << "\">"<<endl;
            outFile<<"<description>"<< midBug.getdescription() <<"</description>"<<endl;
            outFile<<"<impact>"<< midBug.getimpact() <<"</impact>"<<endl;
            outFile<<"<created>"<< midBug.getdate() <<"</created>"<<endl;
            outFile<<"</bug>"<<endl;
        };

        if (!loBugLinkedList.empty()) {
            Bug loBug = loBugLinkedList.popFront();

            outFile<<"<bug id=\"" << loBug.getid() << "\">"<<endl;
            outFile<<"<description>"<< loBug.getdescription() <<"</description>"<<endl;
            outFile<<"<impact>"<< loBug.getimpact() <<"</impact>"<<endl;
            outFile<<"<created>"<< loBug.getdate() <<"</created>"<<endl;
            outFile<<"</bug>"<<endl;
        };
       
        // Developer developer(id, hiBug, midBug, loBug);  **developer custom data type not needed

        outFile<<"</developer>"<<endl;
    };

    //create xml for unassigned high impact bugs, medium impact bugs, and low impact bugs

    outFile<<"<remaining>"<<endl;

    while (!hiBugLinkedList.empty()) {
        Bug hiBug = hiBugLinkedList.popFront();

        outFile<<"<bug id=\"" << hiBug.getid() << "\">"<<endl;
        outFile<<"<description>"<< hiBug.getdescription() <<"</description>"<<endl;
        outFile<<"<impact>"<< hiBug.getimpact() <<"</impact>"<<endl;
        outFile<<"<created>"<< hiBug.getdate() <<"</created>"<<endl;
        outFile<<"</bug>"<<endl;

    };

    while (!midBugLinkedList.empty()) {
        Bug midBug = midBugLinkedList.popFront();

        outFile<<"<bug id=\"" << midBug.getid() << "\">"<<endl;
        outFile<<"<description>"<< midBug.getdescription() <<"</description>"<<endl;
        outFile<<"<impact>"<< midBug.getimpact() <<"</impact>"<<endl;
        outFile<<"<created>"<< midBug.getdate() <<"</created>"<<endl;
        outFile<<"</bug>"<<endl;

    };

    while (!loBugLinkedList.empty()) {
        Bug loBug = loBugLinkedList.popFront();

        outFile<<"<bug id=\"" << loBug.getid() << "\">"<<endl;
        outFile<<"<description>"<< loBug.getdescription() <<"</description>"<<endl;
        outFile<<"<impact>"<< loBug.getimpact() <<"</impact>"<<endl;
        outFile<<"<created>"<< loBug.getdate() <<"</created>"<<endl;
        outFile<<"</bug>"<<endl;

    };

    outFile<<"</remaining>"<<endl;

    outFile<<"</report>";
    
    // Make sure to close the file when you are done
    outFile.close();

    return 0;
};