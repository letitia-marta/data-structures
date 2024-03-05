#include <cstdlib>
#include "RBNode.h"
#include "IODialog.h"
void startProcessing (RBTree*);

int main()
{
    RBTree* RB = new RBTree();
    startProcessing(RB);
    delete RB;
}

void perform (RBTree*, int);

void startProcessing (RBTree* RB)
{
    int o;
    while (true)
    {
        o = IODialog::getTreeOperation();
        perform(RB, o);
    }
}

void perform (RBTree* RB, int o)
{
    list <int> nodeKeys;
    RBNode* rbn;
    int skey;
    switch (o)
    {
        case 1: // Add nodes
            IODialog::getNodeKeys(nodeKeys);
            for (list <int> :: iterator it = nodeKeys.begin();
                    it != nodeKeys.end(); it++)
                RB->RBInsert(RB->createNode(*it));
            break;
        case 2: // Delete node
            skey = IODialog::getNodeKey();
            rbn = RB->search(RB->root, skey);
            if (!RB->isNil(rbn))
                RB->del(rbn);
            else
                cout << "RB: Node not found" << endl;
            break;
        case 3: // Get minimum node
            rbn = RB->minimum(RB->root);
            if (RB->isNil(rbn))
                cout << "RB: Minimum not found" << endl;
            else
                cout << "RB: Minimum is: " << rbn->toString() << endl;
            break;
        case 4: // Get maximum node
            rbn = RB->maximum(RB->root); // RB->maximum(RBNode*) must be implemented!
            if (RB->isNil(rbn))
                cout << "RB: Maximum not found" << endl;
            else
                cout << "RB: Maximum is: " << rbn->toString() << endl;
            break;
        case 5: // Get successor of specified node
            skey = IODialog::getNodeKey();
            rbn = RB->search(RB->root, skey);
            if (!RB->isNil(rbn))
            {
                rbn = RB->successor(rbn);
                if (RB->isNil(rbn))
                    cout << "RB: Node has no successor.\n";
                else
                    cout << "RB: Successor is: " << rbn->toString() << '\n';
            }
            else
                cout << "RB: Node not found.\n";
            break;
        case 6: // Get predecessor of specified node
            skey = IODialog::getNodeKey();
            rbn = RB->search(RB->root, skey);
            if (!RB->isNil(rbn))
            {
                rbn = RB->predecessor(rbn); // RB->predecessor(RBNode*) must be implemented!
                if (RB->isNil(rbn))
                    cout << "RB: Node has no predecessor.\n";
                else
                    cout << "RB: Predecessor is: " << rbn->toString() << '\n';
            }
            else
                cout << "RB: Node not found.\n";
            break;
        case 7: // Show tree RB
            RB->display();
            break;
        case 8:
            cout << "\nInorder traversal of RB tree yields" << endl << ' ';
            RB->inorder();
            cout << endl;
            break;
        case 9: // show black-height of RB
            cout << "The black-height of the RB tree is " << RB->bh() << endl;
            break;
        case 10: // show the maximum key of BLACK nodes of RB
            cout << "The max key of a black node in the RB tree is " << RB->maxBlackKey() << endl;
            break;
        case 11: // show the maximum key of RED nodes of RB
            cout << "The max key of a red node in the RB tree is " << RB->maxRedKey() << endl;
            break;
        case 12: // Compute and show tree depth of RB
            cout << "The tree depth of RB is " << RB->depth() << endl;
            break;
        case 13: // Exit
            delete RB;
            cout << "Bye!" << endl;
            exit (0);
    }
}
