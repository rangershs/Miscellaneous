#include <iostream>
#include "Trie.h"

int main(int argc, char** argv)
{
	TrieTree trie_tree;
	trie_tree.insertStr("five");
	trie_tree.insertStr("code");
	trie_tree.insertStr("cook");
	trie_tree.insertStr("cook");
	trie_tree.insertStr("fat");
	trie_tree.insertStr("hi");
	trie_tree.insertStr("file");
	trie_tree.insertStr("cod");

	trie_tree.traverseTree();
	std::cout << "-----------------" << std::endl;

	std::cout << trie_tree.searchStr("five") << std::endl;
	std::cout << trie_tree.searchStr("fat") << std::endl;
	std::cout << trie_tree.searchStr("hi") << std::endl;
	std::cout << trie_tree.searchStr("cod") << std::endl;
	std::cout << trie_tree.searchStr("hello") << std::endl;
	std::cout << trie_tree.searchStr("cook") << std::endl;
	std::cout << trie_tree.searchStr("file") << std::endl;
	std::cout << trie_tree.searchStr("code") << std::endl;
	std::cout << "-----------------" << std::endl;

	trie_tree.deleteStr("five");
	trie_tree.traverseTree();
	std::cout << "-----------------" << std::endl;

	trie_tree.deleteStr("hi");
	trie_tree.traverseTree();
	std::cout << "-----------------" << std::endl;

	trie_tree.deleteStr("file");
	trie_tree.traverseTree();
	std::cout << "-----------------" << std::endl;

	trie_tree.deleteStr("cod");
	trie_tree.traverseTree();
	std::cout << "-----------------" << std::endl;

	trie_tree.deleteStr("fat");
	trie_tree.traverseTree();
	std::cout << "-----------------" << std::endl;

	trie_tree.deleteStr("code");
	trie_tree.traverseTree();
	std::cout << "-----------------" << std::endl;

	trie_tree.deleteStr("cook");
	trie_tree.traverseTree();
	std::cout << "-----------------" << std::endl;

	getchar();
	return 0;
}