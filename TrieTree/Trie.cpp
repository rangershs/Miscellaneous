#include <iostream>
#include "Trie.h"

TrieTree::TrieTree()
{
	root			= new Node;
	root->ch		= '*';
	root->isWord	= false;
	root->nextNodes = std::list<Node*>();
}
//	TODO: delete all nodes
TrieTree::~TrieTree()
{
	if (root)
		delete root;
}

bool TrieTree::insertStr(const char* data)
{
	if (!data)
		return false;

	Node* curNodePtr = root;
	while (*data != '\0')
	{
		bool isExist(false);
		for (const auto& item : curNodePtr->nextNodes)
		{
			if (item->ch == *data)
			{
				isExist = true;
				curNodePtr = item;
				break;
			}
		}

		if (!isExist)
		{
			Node* node		= new Node;
			node->ch		= *data;
			node->isWord	= false;
			node->nextNodes = std::list<Node*>();
			curNodePtr->nextNodes.emplace_back(node);
			curNodePtr		= node;
		}
		data++;
	}
	curNodePtr->isWord = true;
	return true;
}

bool TrieTree::searchStr(const char* data)
{
	return strIsExist(data);
}

bool TrieTree::deleteStr(const char* data)
{
	if (!data)
		return false;

	if (!strIsExist(data))
		return false;

	deleteNode(root, data);
	return true;
}

void TrieTree::traverseTree()
{
	getNodeValue(root, "");
}

bool TrieTree::strIsExist(const char* data)
{
	if (!data)
		return false;

	Node* curNodePtr = root;
	while (*data != '\0')
	{
		bool isExist(false);
		for (const auto& item : curNodePtr->nextNodes)
		{
			if (item->ch == *data)
			{
				isExist = true;
				curNodePtr = item;
				break;
			}
		}
		if (!isExist)
			return false;
		else
			data++;
	}
	if (curNodePtr->isWord)
		return true;
	else
		return false;
}

void TrieTree::getNodeValue(Node* node, const std::string& data)
{
	std::string str;
	if (node->ch != '*')
		str = data + node->ch;
	if (node->isWord)
	{
		std::cout << str << std::endl;;
		if (node->nextNodes.empty())
			return;
	}
	for (auto& item : node->nextNodes)
	{
		getNodeValue(item, str);
	}
}

void TrieTree::deleteNode(Node* node, const char* data)
{
	if (!data)
		return;

	if (node->nextNodes.empty())
	{
		if (node->isWord)
			node->isWord = false;
		return;
	}
	else if (*data == '\0')
	{
		if (node->isWord)
			node->isWord = false;
		return;
	}

	Node* nextNodePtr(nullptr);
	for (auto& item : node->nextNodes)
	{
		if (item->ch == *data)
		{
			nextNodePtr = item;
			break;
		}
	}
	deleteNode(nextNodePtr, ++data);
	if (nextNodePtr->nextNodes.empty() && !nextNodePtr->isWord)
	{
		Node* cur_node = nextNodePtr;
		node->nextNodes.remove(nextNodePtr);
		if (cur_node)
		{
			delete cur_node;
			cur_node = nullptr;
		}
	}
}