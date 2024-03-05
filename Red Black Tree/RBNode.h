#ifndef RBNODE_H_
#define RBNODE_H_

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

struct RBNode
{
    int key;
    RBNode *parent;
    RBNode *left;
    RBNode *right;
    enum color { RED, BLACK };
    color col;

    static RBNode* Nil; // sentinel leaf node

    RBNode(int k, RBNode *l = Nil, RBNode *r = Nil, RBNode *p = Nil, color c = RBNode::BLACK) :
        key(k), parent(p), left(l), right(r), col(c) { }

    string toString()
    {
        ostringstream os;
        os << key;
        return os.str();
    }

    ~RBNode()
    {
        if (this != Nil)
        {
            if (left != Nil)
                delete left;
            if (right != Nil)
                delete right;
        }
    }

    int max_depth() const
    {
        const int left_depth = left ? left->max_depth() : 0;
        const int right_depth = right ? right->max_depth() : 0;
        return (left_depth > right_depth ? left_depth : right_depth) + 1;
    }
};

RBNode* RBNode::Nil = new RBNode(0);

struct RBTree
{
    RBNode* root;

    RBTree()
    {
        root = RBNode::Nil;
    }
    ~RBTree()
    {
        delete root;
    }

    RBNode* createNode (int key)
    {
        return new RBNode(key);
    }

    bool isNil (RBNode* n)
    {
        return (n == RBNode::Nil);
    }

    RBNode* search (RBNode* w, int key)
    {
        if (isNil(w) || w->key == key)
            return w;
        return search( (key < w->key) ? w->left : w->right, key);
    }

    void LeftRotate (RBNode* x)
    {
        RBNode* y = x->right;
        x->right = y->left;
        if (y->left != RBNode::Nil)
            y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == RBNode::Nil)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void RightRotate (RBNode* y)
    {
        RBNode* x = y->left;
        y->left = x->right;
        if (x->right != RBNode::Nil)
            x->right->parent = y;
        x->parent = y->parent;
        if (y->parent == RBNode::Nil)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        x->right = y;
        y->parent = x;
    }

    RBNode* maximum (RBNode* w)
    {
        RBNode* x = w;
        while (!isNil(x->right))
            x = x->right;
        return x;
    }

    RBNode* minimum (RBNode* w)
    {
        RBNode* x = w;
        while (!isNil(x->left))
            x = x->left;
        return x;
    }

    RBNode* successor (RBNode* w)
    {
        if (isNil(w))
            return w;

        RBNode* x = w;
        if (!isNil(x->right))
            return minimum(x->right);
        RBNode* y = x->parent;
        while (!isNil(y) && x == y->right)
        {
            x = y;
            y = x->parent;
        }
        return y;
    }

    RBNode* predecessor (RBNode* w)
    {
        if (isNil(w))
            return w;

        RBNode* x = w;
        if (!isNil(x->left))
            return maximum(x->left);
        RBNode* y = x->parent;
        while (!isNil(y) && x == y->left)
        {
            x = y;
            y = x->parent;
        }
        return y;
    }

    void RBInsert (RBNode* z)
    {
        RBNode *y = RBNode::Nil;
        RBNode *x = root;
        while (!isNil(x))
        {
            y = x;
            x = (z->key < x->key) ? x->left : x->right;
        }
        z->parent = y;
        if (isNil(y))
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;
        z->left = z->right = RBNode::Nil;
        z->col = RBNode::RED;
        RBInsertFixup(z);
    }

    RBNode* del (RBNode* z)
    {
        RBNode *y = (isNil(z->left) || isNil(z->right)) ? z : successor(z);
        RBNode *x = !isNil(y->left) ? y->left : y->right;
        x->parent = y->parent;
        if (isNil(y->parent))
            root = x;
        else
        {
            if (y == y->parent->left)
                y->parent->left = x;
            else
                y->parent->right = x;
        }
        if (y != z)
            z->key = y->key;
        if (y->col == RBNode::BLACK)
            RBDeleteFixup(x);
        return y;
    }

    void inorder (RBNode* T)
    {
        if (!isNil(T))
        {
            inorder(T->left);
            cout<<T->toString()<<' ';
            inorder(T->right);
        }
    }

    void inorder()
    {
        if (isNil(root))
            cout << "empty";
        else
            inorder(root);
    }

    int depth (RBNode* x)
    {
        if (isNil(x))
            return -1;
        else
        {
            int lDepth = depth(x->left);
            int rDepth = depth(x->right);
            return (lDepth < rDepth ? rDepth : lDepth) + 1;
        }
    }

    int depth()
    {
        return depth(root);
    }

    int countBlacks (RBNode* x)
    {
        if (isNil(x))
            return 1;
        int n = countBlacks(x->left);
        return ((x->col == RBNode::RED)? n : n+1);
    }

    int bh (RBNode* x)
    {
        return (isNil(x) ? 0 : countBlacks(x->left));
    }

    int bh()
    {
        return bh(root);
    }

    int maxBlackKey (RBNode* p)
    {
        int maxi = -1000;
        while (!isNil(p))
            if (p->col == RBNode::BLACK && p->key > maxi)
                maxi = p->key;
        return maxi;
    }

    int maxBlackKey ()
    {
        return maxBlackKey(root);
    }

    int maxRedKey (RBNode* p, int &maxi)
    {
        if (!isNil(p))
        {
            int leftMax = maxRedKey(p->left, maxi);
            if (p->key > maxi)
                maxi = p->key;
            int rightMax = maxRedKey(p->right, maxi);
            return (leftMax > rightMax) ? leftMax : rightMax;
        }
        return maxi;
    }

    int maxRedKey()
    {
        int maxi = -1000;
        if (!isNil(root))
            return maxRedKey(root, maxi);
        return -1000;
    }

    void RBInsertFixup(RBNode* z)
    {
		while (z->parent->col == RBNode::RED)
			if (z->parent == z->parent->parent->left)
            {
				RBNode* y = z->parent->parent->right;
				if (y->col == RBNode::RED)
				{
					z->parent->col = RBNode::BLACK;
					y->col = RBNode::BLACK;
					z->parent->parent->col = RBNode::RED;
                    z = z->parent->parent;
				}
                else
                {
					if (z==z->parent->right)
					{
						z = z->parent;
						LeftRotate(z);
					}
					z->parent->col = RBNode::BLACK;
					z->parent->parent->col = RBNode::RED;
					RightRotate(z->parent->parent);
				}
			}
			else
            {
				RBNode* y = z->parent->parent->left;
				if (y->col == RBNode::RED)
				{
					z->parent->col = RBNode::BLACK;
					y->col = RBNode::BLACK;
					z->parent->parent->col = RBNode::RED;
                    z = z->parent->parent;
				}
                else
                {
					if (z==z->parent->left)
					{
						z = z->parent;
						RightRotate(z);
					}
					z->parent->col = RBNode::BLACK;
					z->parent->parent->col = RBNode::RED;
					LeftRotate(z->parent->parent);
				}
			}
		root->col = RBNode::BLACK;
	}

    void RBDeleteFixup (RBNode* x)
    {
		RBNode* w;
		while ((x != root) && (x->col == RBNode::BLACK))
        {
			if (x == x->parent->left)
			{
				w = x->parent->right;
				if (w->col == RBNode::RED)
				{
					w->col = RBNode::BLACK;
					x->parent->col = RBNode::RED;
					LeftRotate(x->parent);
					w = x->parent->right;
				}
				if ((w->left->col == RBNode::BLACK) && (w->right->col == RBNode::BLACK)) {
					w->col = RBNode::RED;
					x = x->parent;
				}
                else
                {
					if(w->right->col == RBNode::BLACK)
					{
						w->left->col = RBNode::BLACK;
						w->col = RBNode::RED;
						RightRotate(w);
						w = x->parent->right;
					}
					w->col = x->parent->col;
					x->parent->col = RBNode::BLACK;
					w->right->col = RBNode::BLACK;
					LeftRotate(x->parent);
					x = root;
				}
			}
			else
            {
				w = x->parent->left;
				if(w->col == RBNode::RED)
                {
					w->col = RBNode::BLACK;
					x->parent->col = RBNode::RED;
					RightRotate(x->parent);
					w = x->parent->left;
				}
				if ((w->left->col == RBNode::BLACK) && (w->right->col == RBNode::BLACK))
                {
					w->col = RBNode::RED;
					x = x->parent;
				}
                else
                {
					if(w->left->col == RBNode::BLACK)
					{
						w->right->col = RBNode::BLACK;
						w->col = RBNode::RED;
						LeftRotate(w);
						w = x->parent->left;
					}
					w->col = x->parent->col;
					x->parent->col = RBNode::BLACK;
					w->left->col = RBNode::BLACK;
					RightRotate(x->parent);
					x = root;
				}
			}
		}
		x->col = RBNode::BLACK;
	}

    struct display_nod
    {
        string valstr; // valoarea de afisat, ca string
        bool plin; // daca e ocupat sau nu
        bool rosu; // daca e rosu sau nu

        display_nod (const string& value, bool red) : valstr(value), plin(true), rosu(red) {}
        display_nod() : valstr(" "), plin(false), rosu(false) {}
    };

    using arbore = vector <vector <display_nod>>; // toate randurile din arbore

    /*
     * Functie care alcatuieste un vector cu fiecare rand (nivel) din arbore
    */
    arbore afisareRanduri (RBNode* node) const
    {
        // node = pointer la radacina
        vector <RBNode*> traversare; // vector de traversare a nodurilor
        vector <vector <RBNode*>> randuri; // vector de linii orizontale din arbore

        if (node == RBNode::Nil) // radacina e Nil
            return arbore(); // returneaza vector gol

        RBNode* p = node;
        const int max_depth = node->max_depth();
        randuri.resize(max_depth); // redimensioneaza vectorul - atatea pozitii cate nivele
        int depth = 0;

        for (;;) // bucla infinita
        {
            if (depth == max_depth - 1) // ultimul nivel
            {
                randuri[depth].push_back(p); // adauga nodul pe pozitia corespunzatoare nivelului
                if (depth == 0) // primul nivel - radacina
                    break;
                --depth; // urca un nivel
                continue;
            }

            if (traversare.size() == depth) // dimensiunea vectorului de traversare = nivelul curent
            {
                randuri[depth].push_back(p); // adauga nodul pe pozitia corespunzatoare nivelului
                traversare.push_back(p); // adauga nodul in vectorul de traversare 
                if (p)
                    p = p->left; // merge in fiul stang
                ++depth; // coboara un nivel
                continue;
            }

            if (randuri[depth + 1].size() % 2) // dimensiunea randului nivelului curent e impara
            {
                p = traversare.back(); 
                if (p)
                    p = p->right; // merge in fiul drept
                ++depth;
                continue;
            }

            if (depth == 0) // radacina
                break;

            traversare.pop_back();
            p = traversare.back();
            --depth; // urca un nivel
        }

        arbore afisareArbore; // pentru rezultatul final (vectorul de randuri)
        stringstream ss;

        for (const auto& r: randuri) // iteram prin fiecare rand din randuri
        {
            afisareArbore.emplace_back(); // adaugam un rand nou
            for (RBNode* nod: r) // iteram prin fiecare nod din rand
            {
                if (nod && nod->key != 0)
                {
                    ss << nod->key; // adaugam valoarea nodului in ss
                    afisareArbore.back().push_back (display_nod (ss.str(), nod->col == RBNode::RED));
                    ss = stringstream(); // golim stringstream-ul
                }
                else // obiect gol pentru nodurile goale
                    afisareArbore.back().push_back (display_nod());
            }
        }
        return afisareArbore;
    }

    /*
     * Functie care realizeaza aspectul dorit al arborelui
     * (construieste indentarea pentru fiecare rand,
     * da culori nodurilor,
     * creeaza muchiile arborelui)
    */
    vector <string> format (const arbore& afisareArbore) const
    {
        using sizeType = string::size_type;

        // aflam latimea maxima necesara unui nod
        sizeType latime = 0;
        for (const auto& rand: afisareArbore)
        {
            for (const auto& nod: rand)
                if (nod.plin && nod.valstr.length() > latime)
                    latime = nod.valstr.length();
        }

        // ne asiguram ca latimea e impara si minim 3 - pentru a putea fi centrate nodurile
        if (latime % 2 == 0)
            ++latime;

        if (latime < 3)
            latime = 3;

        vector <string> randuriFormatate; // reprezentarea finala
        sizeType nrRanduri = afisareArbore.size(); // numarul de nivele
        sizeType nrElemRand = 1 << (nrRanduri - 1); // numarul de noduri din rand = 2^(nrRanduri - 1)
        sizeType left_pad = 0; // left padding

        for (sizeType r = 0; r < nrRanduri; ++r) // iteram prin fiecare rand din arbore
        {
            const auto& rand_crt = afisareArbore[nrRanduri - r - 1]; // randul curent
            sizeType space = (sizeType(1) << r) * (latime + 1) / 2 - 1;
            string rand;

            for (sizeType e = 0; e < nrElemRand; ++e) // iteram prin rand
            {
                if (e)
                    rand += string (left_pad * 2 + 1, ' ');
                else
                    rand += string (left_pad, ' ');

                if (rand_crt[e].plin && rand_crt[e].valstr != "0") // nodul e plin si nenul
                {
                    const string& valstr = rand_crt[e].valstr; // valoarea nodului
                    sizeType long_padding = latime - valstr.length();
                    sizeType short_padding = long_padding / 2;
                    long_padding -= short_padding;

                    // padding la stanga
                    if (e % 2)
                        rand += string (short_padding, ' ');
                    else
                        rand += string (long_padding, ' ');
                    
                    if (rand_crt[e].rosu)
                        rand += "\033[1;31m"; // rosu
                    else
                        rand += "\033[1;30m"; // negru

                    rand += valstr;
                    rand += "\033[0m"; // reseteaza culoarea

                    // padding la dreapta
                    if (e % 2)
                        rand += string (long_padding, ' ');
                    else
                        rand += string (short_padding, ' ');
                }
                else
                    rand += string (latime, ' '); // adauga spatiu pentru nod gol sau nul
            }
            randuriFormatate.push_back (rand); // adauga randul in stiva de randuri

            if (nrElemRand == 1) // un singur nod in rand
                break;

            sizeType left_space = space + 1; // spatiu la stanga randului
            sizeType right_space = space - 1; // spatiu la dreapta randului

            for (sizeType s = 0; s < space; ++s)
            {
                string row;
                for (sizeType e = 0; e < nrElemRand; ++e)
                {
                    if (e % 2 == 0) // par
                    {
                        row += string (e ? left_space * 2 + 1 : left_space, ' ');
                        row += rand_crt[e].plin ? '/' : ' ';
                        row += string (right_space + 1, ' ');
                    }
                    else
                    {
                        row += string (right_space, ' ');
                        row += rand_crt[e].plin ? '\\' : ' ';
                    }
                }
                randuriFormatate.push_back (row);
                ++left_space;
                --right_space;
            }
            left_pad += space + 1;
            nrElemRand /= 2;
        }

        reverse (randuriFormatate.begin(), randuriFormatate.end());

        return randuriFormatate;
    }

    /*
     * Aliniaza arborele la stanga, eliminand spatiile inutile din stanga fiecarui rand
    */
    static void leftAlign (vector <string>& randuri)
    {
        if (!randuri.size())
            return;

        auto min_space = randuri.front().length(); // initializare cu lungimea primului rand
        for (const auto& r: randuri) // iteram prin fiecare rand
        {
            auto i = r.find_first_not_of(' '); // gasim pozitia primului caracter din r care nu e spatiu
            // daca nu exista in r caractere care nu sunt spatiu => i = string::npos
            if (i == string::npos) // daca nu exista in r caractere diferite de spatiu
                i = r.length();
            if (i == 0)
                return;
            if (i < min_space)
                min_space = i;
        }
        for (auto& r: randuri)
            r.erase(0, min_space);
    }

    /*
     * Functia afiseaza arborele, avand in vedere structura si formatarile aplicate
    */
    void display() const
    {
        const int d = get_max_depth();
        if (d == 0)
        {
            cout << " Red Black tree is empty.\n";
            return;
        }
        const auto rows = afisareRanduri(root);
        auto randuriFormatate = format(rows);
        leftAlign(randuriFormatate);
        for (const auto& rand: randuriFormatate)
            cout << ' ' << rand << '\n';
    }

    int get_max_depth() const
    {
        return root ? root->max_depth() : 0;
    }
};

#endif /* RBNODE_H_ */
