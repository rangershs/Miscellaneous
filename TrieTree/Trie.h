#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <list>
#include <string>

/*
	实现字典树的基本操作, 支持26个英文小写字母, 如有必要, 后续可以扩展字符集
	类的方法不是线程安全的, 不支持多线程同时操作同一个对象
	现实中单词长度并不长, 采用递归方式实现, 注意递归的边界条件
*/

class TrieTree
{
public:
	struct Node
	{
		char					ch;
		bool					isWord;
		std::list<Node*>		nextNodes;	/* 涉及到数据增删操作, list优于vector; 最开始选择的是vector, 删除单词时指针操作不当导致结点误删, 如先删five, 那么file也会被删除, 而先删file则没问题 */
	};

public:
	TrieTree();
	~TrieTree();
	TrieTree(const TrieTree& oth) = delete;
	TrieTree& operator=(const TrieTree& oth) = delete;

public:
	bool	insertStr(const char* data);
	bool	searchStr(const char* data);
	bool	deleteStr(const char* data);
	void	traverseTree();

private:
	bool	strIsExist(const char* data);
	void	getNodeValue(Node* node, const std::string& data);
	void	deleteNode(Node* node, const char* data);

private:
	Node*		root;
};

#endif