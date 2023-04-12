#include <iostream>
#include "DoubleLinked.h"

using namespace std;

int main()
{
    DoubleLinked Container;
    Container.InsertAtHead(2);
    Container.InsertAtHead(1);
    Container.InsertAtTail(2);
    Container.InsertAtHead(2);
    Container.InsertAtHead(3);
    Container.InsertAtHead(2);
    Container.InsertAtHead(2);
    Container.PrintListAll();
    Container.eraseNode(2);
    Container.PrintListAll();
    Container.DeleteNode(Container.FindNode(1));
    Container.PrintListAll();
    Container.DeleteNode(Container.FindNode(2));
    Container.PrintListAll();
}
